all: testmidi

testmidi: *.cpx
	../cpxprep testmidi.cpx >testmidi.cpp
	g++ -O0 -g3 -ggdb testmidi.cpp -o testmidi

test: testmidi
	./testmidi /usr/share/games/fretsonfire/data/songs/sectoid/Feelings/notes.mid
	