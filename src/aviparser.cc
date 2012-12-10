/*******************************************************************
(C) 2012 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "aviparser.h"
#include "message.h"
#include "verbosity.h"
#include "utils.h"

enum aviidxflags
{
	AVIIF_LIST      = 0x0001,
	AVIIF_KEYFRAME  = 0x0010,
 	AVIIF_FIRSTPART = 0x0020,
	AVIIF_LASTPART  = 0x0040,
	AVIIF_NOTIME    = 0x0100
};

struct AVImainHeader
{
	uint32_t dwMicroSecPerFrame;   // frame display rate (or 0)
	uint32_t dwMaxBytesPerSec;     // maximum transfer rate
	uint32_t dwPaddingGranularity;
	uint32_t dwFlags;              // flags
	uint32_t dwTotalFrames;        // number of frames in file
	uint32_t dwInitialFrames;
	uint32_t dwStreams;
	uint32_t dwSuggestedBufferSize;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint32_t dwReserved[4];
	void read_from_file(binaryfile &f);
};

struct AVIstreamInfo
{
	string fccType;
	string fccHandler;
	uint32_t dwFlags;
	uint16_t wPriority;
	uint16_t wLanguage;
	uint32_t dwInitialFrames;
	uint32_t dwScale;
	uint32_t dwRate;               // dwRate / dwScale == samples/second
	uint32_t dwStart;
	uint32_t dwLength;             // In units above...
	uint32_t dwSuggestedBufferSize;
	uint32_t dwQuality;
	uint32_t dwSampleSize;
	uint32_t rcFrameLeft;
	uint32_t rcFrameTop;
	uint32_t rcFrameRight;
	uint32_t rcFrameBottom;
	string name;
	uint8_t *fmtheader;

	AVIstreamInfo():fmtheader(NULL) {;}
	~AVIstreamInfo();
	void read_header_from_file(binaryfile &f);
	void read_format_from_file(binaryfile &f, int size);
};

int avifile::open(string filename)
{
	int i;
	reset();
	if (f.openfile(filename)==-1) return 0;
	flen=f.size();
	DBG(AVIFILE, "File size is: %d\n",flen);
	if (!parse_containers("",0,flen)) return 0;
	return 1;
}

int avifile::parse_containers(string path, uint64_t offset, uint64_t end)
{
	f.seek(offset);
	while (offset<=end-8)
	{
		uint64_t effective_len;
		uint64_t paddinggran=2;
		avisubcontainer a;
		a.path=path;

		a.sig=f.readsig();
		a.len=f.readuint32();
		effective_len=a.len;
		//if (hdr) paddinggran=hdr->dwPaddingGranularity;
		if (paddinggran) effective_len=(effective_len+paddinggran-1)/paddinggran*paddinggran;
		a.offset=offset;
		a.typ=a.sig;
		if (a.sig=="") a.sig="****";
		if (a.sig=="RIFF" ||
		    a.sig=="LIST") a.typ=a.typ+"+"+f.readsig();
		a.path=path+a.typ;
		DBG(AVIFILE, "found: %s at %d\n",a.path,a.offset);
		if (a.sig=="avih")
		{
			hdr=new AVImainHeader();
			hdr->read_from_file(f);
		}
		if (a.typ=="LIST+strl")
		{
			streams.push_back(new AVIstreamInfo());
		}
		if (a.typ=="strh")
		{
			streams.back()->read_header_from_file(f);
			if (streams.back()->fccType=="vids")
			{
				width=streams.back()->rcFrameRight-streams.back()->rcFrameLeft;
				height=streams.back()->rcFrameBottom-streams.back()->rcFrameTop;
				uspf=streams.back()->dwScale;
				uspf*=1000000;
				uspf/=streams.back()->dwRate;
				INFO(AVIFILE, "Video Stream: %dx%d uspf=%d",width,height,uspf);
			}

		}
		if (a.typ=="strf")
		{
			streams.back()->read_format_from_file(f,a.len);
		}
		if (a.typ=="idx1")
		{
			read_old_style_index(a.len);
		}
		if (a.typ=="LIST+movi" && firstmovi==0)
			firstmovi=a.offset;
		if (a.sig=="RIFF" ||
//			a.typ=="LIST+movi" ||
			a.typ=="LIST+hdrl" ||
			a.typ=="LIST+strl" )
		{
			parse_containers(a.path+"/",a.offset+12,a.offset+a.len+8);
		}
		offset+=8;
		offset+=effective_len;
		f.seek(offset);
	}
	// TODO: report error when necessary
	return 0;
}

void avifile::reset()
{
	int i;
	subcs.resize(0);
	if (hdr)
	{
		delete hdr;
		hdr=NULL;
	}
	for (i=0; i<streams.size(); i++)
		delete streams[i];
	streams.resize(0);
	vidx.resize(0);
	if (buffer)
	{
		free(buffer);
		buffer=NULL;
	}
}

void AVImainHeader::read_from_file(binaryfile &f)
{
	dwMicroSecPerFrame    = f.readuint32();
	dwMaxBytesPerSec      = f.readuint32();
	dwPaddingGranularity  = f.readuint32();
	dwFlags               = f.readuint32();
	dwTotalFrames         = f.readuint32();
	dwInitialFrames       = f.readuint32();
	dwStreams             = f.readuint32();
	dwSuggestedBufferSize = f.readuint32();
	dwWidth               = f.readuint32();
	dwHeight              = f.readuint32();
	dwReserved[0]         = f.readuint32();
	dwReserved[1]         = f.readuint32();
	dwReserved[2]         = f.readuint32();
	dwReserved[3]         = f.readuint32();
	DBG(AVIFILE, "AVI header read: res=%dx%d streams=%d frames=%d rate=%2.2f\n",
		dwWidth, dwHeight, dwStreams, dwTotalFrames, 1000030.0/(dwMicroSecPerFrame+1));
}

void AVIstreamInfo::read_header_from_file(binaryfile &f)
{
	fccType               = f.readsig();
	fccHandler            = f.readsig();
	dwFlags               = f.readuint32();
	wPriority             = f.readuint16();
	wLanguage             = f.readuint16();
	dwInitialFrames       = f.readuint32();
	dwScale               = f.readuint32();
	dwRate                = f.readuint32();
	dwStart               = f.readuint32();
	dwLength              = f.readuint32();
	dwSuggestedBufferSize = f.readuint32();
	dwQuality             = f.readuint32();
	dwSampleSize          = f.readuint32();
	rcFrameLeft           = f.readuint16();
	rcFrameTop            = f.readuint16();
	rcFrameRight          = f.readuint16();
	rcFrameBottom         = f.readuint16();
	DBG(AVIFILE, "Stream header read: typ=%s:%s res=%dx%d rate=%d/%d start=%d length=%d\n",
		fccType, fccHandler,
		rcFrameRight-rcFrameLeft, rcFrameBottom-rcFrameTop,
		dwRate, dwScale,
		dwStart, dwLength);
}

void AVIstreamInfo::read_format_from_file(binaryfile &f, int size)
{
	int i;
	fmtheader=(uint8_t*) malloc(size);
	f.readbytes(fmtheader,size);
	DBG(AVIFILE, "Stream format header: ");
	for (i=0; i<size; i++) DBG(AVIFILE,"%02x",fmtheader[i]);
	DBG(AVIFILE, "\n");
}

AVIstreamInfo::~AVIstreamInfo()
{
	if (fmtheader) free(fmtheader);
}

avifile::~avifile()
{
	reset();
}

void avifile::read_old_style_index(int size)
{
	int i;
	int offsetmode=0;
	while (size>=16)
	{
		string ckid;
		idxentry a;
		ckid=f.readsig();
		a.flags=f.readuint32();
		a.offset=f.readuint32();
		a.size=f.readuint32();
		if (!offsetmode)
		{
			// detect
			if (a.offset>firstmovi) offsetmode=2;
			else offsetmode=1;
		}
		if (offsetmode==1)
		{
			a.offset+=firstmovi+12+4;
		} else {
			// just guessing, never seen one of these
			a.offset+=4;
		}
		size-=16;
		if (ckid!="00dc") continue;
		DBG(AVIFILE,"frame: %s %08x %d %d\n",ckid,a.flags,a.offset,a.size);
		if (!a.size) continue;
		vidx.push_back(a);
	}
}

int avifile::read_frame(int &frameno)
{
	int framesize=vidx[frameno].size;
	if (vidx.size()==0) return 0;
	// just to make sure we're not asked to read past the end
	frameno%=vidx.size();
	if (!buffer || buffer_allocd<framesize)
	{
		buffer_allocd=framesize+4096;
		if (buffer) free(buffer);
		buffer=(uint8_t*) malloc(buffer_allocd);
	}
	f.seek(vidx[frameno].offset);
	f.readbytes(buffer,framesize);
	frameno=(frameno+1)%vidx.size();
	return framesize;
}
