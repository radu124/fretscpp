
DIR_FREETYPE2_INC=-I/usr/include/freetype2
DIR_FREETYPE2_LIB=

LIBS:=`sdl-config --cflags --libs` -lGL -lGLU -lpng -lfreetype -logg -lvorbis -lvorbisfile $(DIR_FREETYPE2_INC)
#Windows libs, for now, manually uncomment
#LIBS:= -D_WINDOWS -Ic:/Dev-Cpp/include/SDL -Ic:/Dev-Cpp/include -Ic:/Dev-Cpp/include/GL -Ic:/Dev-Cpp/include/GLU -Ic:/Dev-Cpp/include/freetype2 -Dmain=SDL_main -Lc:/Dev-Cpp/lib -lmingw32 -lSDLmain -lSDL -mwindows -lfreetype -logg -lvorbisfile -lopengl32 -ldevil -lglu32 -lvorbis -lILU -lILUT

DESTDIR?=

all: game

include $(wildcard *.d)

run: game
	./game

clean: 
	-rm game game-r game.cpp *.d cpxprep *.tags

release: fretscpp

fretscpp.cpp: game.cpx cpxprep
	./cpxprep $< >$@
	echo -n "$@: ">fretscpp.cpp.d
	grep -E "^#line" $@ | sed -e 's/.* "//' -e 's/"//' | sort -u | while read A; do echo -n "$$A "; done >>fretscpp.cpp.d
	#geany -g game.cpp.tags `readlink -f $@`

game: fretscpp.cpp
	g++ -MMD -MF $@.d $< -O0 -g3 -ggdb -o $@ $(LIBS)

fretscpp: fretscpp.cpp
	g++ -DDISABLEMESSAGES $< -O3 -g3 -ggdb -o $@ $(LIBS)

cpxprep: cpxprep.cpp
	g++ $< -o $@

ubudeps:
	sudo apt-get install build-essential libsdl-dev libvorbis-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev libpng-dev

ubusongs:
	sudo apt-get install fretsonfire-songs-sectoid

game64: fretscpp.cpp
	g++ -m64 -MMD -MF $@.d $< -O0 -g3 -ggdb -o $@ $(LIBS)

spec:
	find data -type d | while read A; do echo '%__install -m 755 -d $${RPM_BUILD_ROOT}'/usr/share/games/fretscpp/$$A; done >fretscpp.spec
	echo '%__install -m 755 fretscpp/fretscpp $${RPM_BUILD_ROOT}/usr/bin/fretscpp' >>fretscpp.spec
	find data -type f | while read A; do echo '%__install -m 0644 fretscpp/'$$A '$${RPM_BUILD_ROOT}'/usr/share/games/fretscpp/$$A; done >>fretscpp.spec

install: fretscpp
	install -m 755 -d $(DESTDIR)/usr
	install -m 755 -d $(DESTDIR)/usr/bin
	install -m 755 -d $(DESTDIR)/usr/share
	install -m 755 -d $(DESTDIR)/usr/share/games
	install -m 755 -d $(DESTDIR)/usr/share/games/fretscpp
	find data -type d | while read A; do install -m 755 -d "$(DESTDIR)/usr/share/games/fretscpp/$$A"; done
	install -m 755 fretscpp $(DESTDIR)/usr/bin/fretscpp
	find data -type f | while read A; do install -m 644 "$$A" "$(DESTDIR)/usr/share/games/fretscpp/$$A"; done

versionincrement:
	sed "s/[.]/ /" VERSION | (read A B; echo $A.$[B+1]) >VERSION.tmp
	mv VERSION VERSION.prev
	mv VERSION.tmp VERSION
	git add VERSION


.PHONY: run install all versionincrement ubudeps ubusongs spec
