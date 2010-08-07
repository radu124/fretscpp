LIBS:=`sdl-config --cflags --libs` -lGL -lGLU -lIL -lILU -lILUT -lfreetype -logg -lvorbis -lvorbisfile -I/usr/include/freetype2

all: game

include $(wildcard *.d)

run: game
	./game

clean: 
	-rm game game-r game.cpp *.d cpxprep *.tags

release: game-r

%.cpp: %.cpx cpxprep
	./cpxprep $< >$@
	echo -n "$@: ">$(patsubst %.cpp,%.cpp.d,$@)
	grep -E "^#line" $@ | sed -e 's/.* "//' -e 's/"//' | sort -u | while read A; do echo -n "$$A "; done >>$(patsubst %.cpp,%.cpp.d,$@)
	#geany -g game.cpp.tags `readlink -f $@`

game: game.cpp
	g++ -MMD -MF $@.d $< -O0 -g3 -ggdb -o $@ $(LIBS)

game-r: game.cpp
	g++ -DDISABLEMESSAGES $< -O3 -g3 -ggdb -o $@ $(LIBS)

cpxprep: cpxprep.cpp
	g++ $< -o $@

linkfrets:
	ln -s /usr/share/games/fretsonfire/data data
