all: testmidi

testmidi: *.cpx
	cpxprep testmidi.cpx >testmidi.cpp
	g++ testmidi.cpp -o testmidi

test: testmidi
	./testmidi ../songs/one/notes.mid
	./testmidi ../songs/two/notes.mid
	./testmidi ../songs/three/notes.mid
	