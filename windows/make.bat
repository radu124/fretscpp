c:
cd c:\fretscpp
cpxprep.exe game.cpx >game.cpp
c:\Dev-Cpp\bin\windres resource.rc -O coff -o resource.o
c:\Dev-Cpp\bin\g++ game.cpp resource.o -o fretscpp.exe -O1 -DDISABLEMESSAGES -D_WINDOWS -Ic:/Dev-Cpp/include/SDL -Ic:/Dev-Cpp/include -Ic:/Dev-Cpp/include/GL -Ic:/Dev-Cpp/include/GLU -Ic:/Dev-Cpp/include/freetype2 -Dmain=SDL_main -Lc:/Dev-Cpp/lib -lmingw32 -lSDLmain -lSDL -mwindows -lfreetype -logg -lvorbisfile -lopengl32 -ldevil -lglu32 -lvorbis -lpng -lshell32
c:\Dev-Cpp\bin\strip fretscpp.exe
