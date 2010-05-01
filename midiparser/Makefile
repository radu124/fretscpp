all: testmidi

testmidi: *.cpx
	cpxprep testmidi.cpx >testmidi.cpp
	g++ -Os testmidi.cpp -o testmidi

test: testmidi
	./testmidi ../songs/one/notes.mid
	@echo "============================================================================="
	./testmidi ../songs/two/notes.mid
	@echo "============================================================================="
	./testmidi ../songs/three/notes.mid
	