all:
	cpxprep game.cpx >game.cpp
	g++ game.cpp -O0 -g3 -ggdb -o game `sdl-config --cflags --libs` -lGL -lGLU -lIL -lILU -lILUT -lfreetype -logg -lvorbis -lvorbisfile -I/usr/include/freetype2
release:
	cpxprep game.cpx >game.cpp
	g++ game.cpp -O3 -g3 -ggdb -o game-r `sdl-config --cflags --libs` -lGL -lGLU -lIL -lILU -lILUT -lfreetype -logg -lvorbis -lvorbisfile -I/usr/include/freetype2