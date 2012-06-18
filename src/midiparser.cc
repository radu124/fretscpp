/*******************************************************************
(C) 2010 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "midiparser.h"

#include "message.h"
#include "verbosity.h"
#include "sha1.h"

void MidiParser::debug_dump_track(int track)
{
	if (MSG_MIDIDUMP<LEV_VDBG+MSG_BIAS) return;
	if (track<0 || track>=trk_notes.size()) return;
	vnotestatus &ctrk=trk_notes[track];
	int i,j;
	for (i=0; i<ctrk.size(); i++)
	{
		DBG(MIDIDUMP,"%9d ", ctrk[i].timestamp);
		for (j=0; j<64; j++) switch (ctrk[i].val[j])
		{
			case ' ': DBG(MIDIDUMP," "); break;
			case 'O': DBG(MIDIDUMP,"O"); break;
			case 'X': DBG(MIDIDUMP,"X"); break;
			case 'B': DBG(MIDIDUMP,"B"); break;
			case '-': DBG(MIDIDUMP,"|"); break;
			default:
				DBG(MIDIDUMP,"?");
		}
		DBG(MIDIDUMP,"\n");
	}
}

void MidiParser::applydelay(int delay)
{
	int tr,j;
	delay=delay*441/10;
	for (tr=0; tr<trk_notes.size(); tr++)
	{
		for (j=0; j<trk_notes[tr].size(); j++)
		{
			int ts=trk_notes[tr][j].timestamp;
			ts+=delay;
			if (ts<0) ts=0;
			trk_notes[tr][j].timestamp=ts;
		}
	}
	lastevent+=delay;
}

int MidiParser::expect(const char *str)
{
	if (!isthere(str))
	{
		WARN(MIDI,"Midi did not contain expected string:%s\n", str);
		FAIL=1;
		return 0;
	}
	pos+=strlen(str);
	return 1;
}

int MidiParser::isthere(const char *str)
{
	int i;
	for (i=0; str[i]!=0; i++)
		if (buffer[pos+i]!=str[i]) return 0;
	return 1;
}

unsigned int MidiParser::readbyte()
{
	if (pos>=size) return 0;
	return buffer[pos++];
}

unsigned int MidiParser::readshort()
{
	unsigned int res;
	res =readbyte()*256;
	res+=readbyte();
	return res;
}

unsigned int MidiParser::readlong()
{
	unsigned int res;
	res =readshort()*65536;
	res+=readshort();
	return res;
}

unsigned int MidiParser::readvarlen()
{
	unsigned int res=0;
	unsigned char c;
	do {
		c=readbyte();
		res=res*128+(c&0x7f);
	} while (c&0x80);
	return res;
}


int MidiParser::openfile(const char *name)
{
	int fd;
	lastevent=0;
	int oflags=O_RDONLY;
#ifdef _WINDOWS
	oflags |= O_BINARY;
#endif
	fd=open(name,oflags);
	if (fd<0) return 0;
	size=lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	if (size>10000000)
	{
		WARN(MIDI,"Problem, midi file unreasonably big!!\n");
		close(fd);
		return 0;
	}
	buffer=new unsigned char[size];
	size=read(fd,buffer,size);
	INFO(READMID,"Read midi file of size %d\n", size);
	sha_buffer((char *) buffer,size,hash);
	pos=0;
	FAIL=0;
	int res=doparse();
	if (trk_difficulties.size()==0)
	{
		vector<notestatus> score;
		score.push_back(notestatus());
		score.push_back(notestatus());
		trk_notes.push_back(score);
		trk_difficulties.push_back(0);
		trk_instrument.push_back("NONE");
	}
	delete [] buffer;
	close(fd);
	return res;
}

string titleCase(string m)
{
	string res;
	int i=0;
	int noupcase=0;
	while (i<m.length() && isspace(m[i])) i++;
	for (;i<m.length(); i++)
	{
		if (noupcase && m[i]>='A' && m[i]<='Z')
		{
			res+=m[i]-'A'+'a';
			continue;
		}
		if ((!noupcase) && m[i]>='a' && m[i]<='z')
		{
			res+=m[i]-'a'+'A';
			continue;
		}
		if (isspace(m[i]))
		{
			res+=" ";
			noupcase=0;
			continue;
		}
		noupcase=1;
		res+=m[i];
	}
	return res;
}

string processInstrumentName(string m)
{
	int a=m.find("PART ");
	if (a!=string::npos) m=m.substr(a+5);
	string res=titleCase(m);
	if (res=="") return "Guitar";
	return res;
}

void MidiParser::timeincrement(int delta)
{
	// FIX: this loss of precision can potentially accumulate
	int newtimestamp=a.timestamp+(int) (44100*tickduration*delta);
	if (newtimestamp>lastevent) lastevent=newtimestamp;
	DBG(READMID,"Time increment %5d,  1000ticks=%3.3fs, time is now %7.3fs    ",
		delta, (tickduration*1000), (newtimestamp/44100.0));
	score.push_back(a);
	a.timestamp=newtimestamp;
	int i,j;
	for (i=0; i<48; i++) switch (a.val[i])
	{
		case 'O':
		case '-':
			a.val[i]='-';
			break;

		default:
			a.val[i]=' ';
	}
}

int MidiParser::doparse()
{
	if (isthere("RIFF"))
	{
		pos+=20;
		// this takes care of the most common RMI format
		// a proper parse of RIFF would be necessary only
		// if such MIDs are encoutered
	}
	expect("MThd");
	assret(!FAIL,"bad header marker, MThd expected");
	chunklen=readlong();
	warnif(chunklen!=6, "Expecting header chunk of size 6, but was %d instead", chunklen);
	savedpos=pos;
	format=readshort();
	tracks=readshort();
	unsigned int bp0,bp1;
	bp0=readbyte();
	bp1=readbyte();
	ticksperbeat=0;
	if (bp0 & 0x80)
	{
		bp0 &= 0x7f;
		tickduration=bp0;
		if ((bp0 & 0x7f)==29) tickduration=30/1001.0;
		tickduration=1/tickduration/bp1;
		INFO(READMID,"Midi Tick fps:%d ticks/frame %d 1000ticks=%2.3fs\n", bp0, bp1, (tickduration*1000));
	}
	else
	{
		ticksperbeat=bp0*256+bp1;
		tickduration=0.5f/ticksperbeat;
		// 120 bpm is standard if we get no other info
		INFO(READMID,"Midi ticks/beat %d  1000ticks=%2.3fs\n", ticksperbeat, (tickduration*1000));
	}
	pos=savedpos+chunklen;
	assret(!FAIL,"Could not read entire header, probably end-of-file");
	assret(pos<size,"Header size sent us past end-of-file");
	INFO(READMID,"Midi file has %d tracks, format is %d\n", tracks, format);
	savedpos=pos;
	trk_notes.resize(0);
	trk_difficulties.resize(0);
	trk_instrument.resize(0);
	while (tracks>0)
	{
		assret(expect("MTrk"),"Expecting track header");
		int tracksize=readlong();
		int lastcmd=0;
		INFO(READMID,"Track size is %d\n", tracksize);
		assret(tracksize+savedpos+8<=size,"Track size exceeds end-of-file");
		chunkend=pos+tracksize;
		int anynotes=0;
		int difficulties=0;
		memset(a.val,' ',sizeof(a.val));
		a.timestamp=0;
		a.val[63]=0;
		score.resize(0);
		string instrument="Guitar";
		while (pos<chunkend)
		{
			unsigned int delta=readvarlen();
			unsigned int event=readbyte();
			if (! (event & 0x80))
			{
				assret(lastcmd>0,"running with no command");
				event=lastcmd;
				pos--;
			}
			unsigned int uspqn;
			if (delta) timeincrement(delta);
			else DBG(READMID,"                                                                 ");
			if (event==0xff)
			{
				int sub=readbyte();
				int len=readbyte(); // or readvariable, not sure which
				int aftermeta=pos+len;
				string message;
				switch (sub)
				{
				case 1:
				case 6:
				case 5:
				case 7:
					message=string((const char *) buffer+pos,len);
					INFO(READMID,"MIDI Text: %s\n", message);
					break;
				case 2:
					message=string((const char *) buffer+pos,len);
					INFO(READMID,"MIDI Copyright: %s\n", message);
					break;
				case 3:
					message=string((const char *) buffer+pos,len);
					instrument=processInstrumentName(message);
					INFO(READMID,"MIDI Track name: %s instrument %s\n", message, instrument);
					break;
				case 4:
					message=string((const char *) buffer+pos,len);
					instrument=processInstrumentName(message);
					INFO(READMID,"MIDI Instrument name: %s instrument %s\n", message, instrument);
					break;
				case 47:
					INFO(READMID,"MIDI End of track\n");
					pos=chunkend;
					break;

				case 81:
					uspqn=65536*readbyte();
					uspqn+=256*readbyte();
					uspqn+=readbyte();
					tickduration=uspqn/1000000.0/ticksperbeat;
					INFO(READMID,"MIDI Set tempo us/qnote %d, ticks/beat %d  1000ticks=%2.3fs\n",
						uspqn, ticksperbeat, (tickduration*1000));
					break;
				case 84:
					INFO(READMID,"MIDI End of track\n");
					break;
				case 88:
					ts_numer=readbyte();
					ts_denom=readbyte();
					ts_metro=readbyte();
					ts_32nds=readbyte();
					INFO(READMID,"MIDI time signature %d/%d-%d-%d\n", ts_numer, ts_denom, ts_metro, ts_32nds);
					break;
				default:
					INFO(READMID,"MIDI Unknown meta event %d\n", sub);
				}
				pos=aftermeta;
				//INFO(READMID,"MIDI skipping meta %x" &pos);
				continue;
			}
			lastcmd=event;
			int evhi=(event>>4) & 0x0f;
			unsigned int notenum,velo,ournote;

			switch (evhi)
			{
			case 0x8:
			case 0x9:
			case 0xA:
			case 0xB:
			case 0xE:
				notenum=readbyte();
				velo=readbyte();
				ournote=48;
				if (notenum>=60 && notenum<=100) ournote=notenum-60;
				break;
			case 0xC:
			case 0xD:
				notenum=readbyte();
				break;
			}
			if ((event & 0xf0)==0x90 && velo==0) event &= 0x8f;
			if ((event & 0xf0)==0x90)
			{
				a.val[ournote]='O';
				DBG(READMID,"MIDI note-on:  %4d %4d %s\n", notenum, velo, a.val);
				anynotes=1;
				if (a.timestamp>40000 &&
					ournote%12<5)
					{
						difficulties |= 1<<(ournote/12);
					}
				// skip single note in the beginning
				continue;
			}
			if ((event & 0xf0)==0x80)
			{
				if (a.val[ournote]=='-' || a.val[ournote]==' ') a.val[ournote]='X';
				else a.val[ournote]='B';
				DBG(READMID,"MIDI note-off: %4d %4d %s\n", notenum, velo, a.val);
				continue;
			}
			if (evhi==0xc || evhi==0xd)
			{
				DBG(READMID,"MIDI event: %x par %x!!!\n", event, notenum);
				continue;
			}
			if (evhi==0xb || evhi==0xa || evhi==0xe)
			{
				DBG(READMID,"MIDI event: %x par %x:%x !!!\n", event, notenum, velo);
				continue;
			}
			INFO(READMID,"Event %d 0x%02x, delta %d\n", event, event, delta);

			break;
		}
		if (anynotes)
		{
			INFO(READMID,"MIDI Track had note events, time=%d\n", (a.timestamp/44100));
			timeincrement(1);
			timeincrement(100000000);
			trk_notes.push_back(score);
			debug_dump_track(trk_notes.size()-1);
			trk_difficulties.push_back(difficulties);
			trk_instrument.push_back(instrument);
		}
		pos=savedpos+tracksize+8;
		savedpos=pos;
		tracks--;
	}
	return 1;
}

/*
 * there is some overhead in doing this but at least we make our life more
 * simple in a couple of other places
 */
vector<notestatusst> MidiParser::get_lane(int instrument, int difficulty)
{
	vector<notestatus> &v=trk_notes[instrument];
	int i,j,same;
	notestatusst a;
	vector<notestatusst> res;
	// skip all empty
 	for (i=0; i<v.size(); i++)
	{
		for (j=0; j<5; j++)
			if (v[i].val[j+12*difficulty]!=' ')
				j=10;
		if (j>5) break;
	}
	// if we found nothing, copy at least one element
	if (i>=v.size()) i--;
	a.from_note_status(v[i],difficulty);
	res.push_back(a);
	i++;
	for (;i<v.size(); i++)
	{
		// it can happen that there is nothing new on this difficulty
		// because there are events on other tracks
		same=1;
		for (j=0; j<5; j++)
		{
			char ch=v[i].val[j+12*difficulty];
			if (ch!=res.back().val[j] || (ch!=' ' && ch!='-'))
			{
				same=false;
				break;
			}
		}
		if (!same)
		{
			a.from_note_status(v[i],difficulty);
			res.push_back(a);
		}
	}
	// always copy last
	a.from_note_status(v.back(),difficulty);
	res.push_back(a);
	return res;
}

void notestatusst::from_note_status(notestatus &v, int difficulty)
{
	int j;
	for (j=0; j<5; j++) val[j]=v.val[j+12*difficulty];
	timestamp=v.timestamp;
	flags=0;
	if (detect_hit()) flags |= ENS_HASHIT;
	if (detect_line()) flags |= ENS_HASLINE;
}

int notestatusst::detect_hit()
{
	int i,cnt=0;
	for (i=0; i<5; i++)
	{
		char ch=val[i];
		if (ch=='O' || ch=='B') cnt++;
	}
	return cnt;
}

int notestatusst::detect_line()
{
	int i;
	for (i=0; i<5; i++)
	{
		char ch=val[i];
		if (ch=='-') return 1;
	}
	return 0;
}

string notestatusst::str()
{
	return string()+val[0]+val[1]+val[2]+val[3]+val[4];
}

int ishit(char c)
{
	return c=='O' || c=='B';
}
