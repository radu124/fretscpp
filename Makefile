LIBS:=`sdl-config --cflags --libs` -lGL -lGLU -lIL -lILU -lILUT -lfreetype -logg -lvorbis -lvorbisfile -I/usr/include/freetype2
#Windows libs, for now, manually uncomment
#LIBS:= -D_WINDOWS -Ic:/Dev-Cpp/include/SDL -Ic:/Dev-Cpp/include -Ic:/Dev-Cpp/include/GL -Ic:/Dev-Cpp/include/GLU -Ic:/Dev-Cpp/include/freetype2 -Dmain=SDL_main -Lc:/Dev-Cpp/lib -lmingw32 -lSDLmain -lSDL -mwindows -lfreetype -logg -lvorbisfile -lopengl32 -ldevil -lglu32 -lvorbis -lILU -lILUT

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
	-ln -s /usr/share/games/fretsonfire/data data

ubudeps: linkfrets
	sudo apt-get install fretsonfire build-essential libsdl-dev libdevil-dev libvorbis-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev
	
