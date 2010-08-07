all: game

run: game
	./game

clean: 
	-rm game game-r game.cpp game.dx cpxprep

release: game-r

game.cpp: cpxprep $(wildcard *.cpx)
	./cpxprep game.cpx >game.cpp
	#geany -g fof-my.tags game.cpp

game: game.cpp
	g++ game.cpp -O0 -g3 -ggdb -o game `sdl-config --cflags --libs` -lGL -lGLU -lIL -lILU -lILUT -lfreetype -logg -lvorbis -lvorbisfile -I/usr/include/freetype2

game-r: game.cpp
	g++ game.cpp -O3 -g3 -ggdb -o game-r `sdl-config --cflags --libs` -lGL -lGLU -lIL -lILU -lILUT -lfreetype -logg -lvorbis -lvorbisfile -I/usr/include/freetype2

cpxprep: cpxprep.cpp
	g++ $< -o $@
