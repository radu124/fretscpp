SHELL=/bin/bash

GOOGLECODE_USER=$(shell cat $(HOME)/.config/googlecode/user)
GOOGLECODE_PASS=$(shell cat $(HOME)/.config/googlecode/pass)

FOFMYBASE=$(shell readlink -f ..)


UNCOMMITTED:=$(shell git diff-index --quiet HEAD; echo $$?)
ifeq ($(UNCOMMITTED),1)
  THEVER:=TEMPORARY
else
  VERDATE:=$(shell TZ=UTC git log -n 1 --format="format:%cd" --date=short)
  VERMINOR:=$(shell TZ=UTC git log --since $(VERDATE):00:00:00 --format="oneline" | wc -l)
  THEVER=$(subst -,.,$(VERDATE)).$(VERMINOR)
endif


WINEBDIR:=$(HOME)/wine/build

ACHROOTB=$(shell readlink -f .)
DATADIR=$(FOFMYBASE)/data

SOURCES=$(wildcard $(FOFMYBASE)/src/*.cc) $(wildcard $(FOFMYBASE)/src/*.h)
SOURCEX=$(subst $(FOFMYBASE)/,,$(wildcard $(FOFMYBASE)/src/*.cc))
SOURCEDEPS=$(SOURCES) $(FOFMYBASE)/Makefile

# hopefully no spaces in file names
DATAFILES=$(shell find $(FOFMYBASE)/data -type f)

##
# Windows build
WINSPECIFIC=$(wildcard $(FOFMYBASE)/windows/*.rc) $(wildcard $(FOFMYBASE)/windows/*.ico)
WINDEFS=-DDISABLEMESSAGES -D_WINDOWS -D_GNU_SOURCE=1 -Dmain=SDL_main 
WINLIBS=-L/mingw/lib -lmingw32 -lSDLmain -lSDL -lSDLmain -liconv -lm -luser32 -lgdi32 -lwinmm -lopengl32 -lglu32 -lpng -lfreetype -lz c:/MinGW/lib/libogv.a -lvpx
WININCL=-Ic:/MinGW/include -Ic:/MinGW/include/freetype2 -Ic:/MinGW/include/SDL

WININSTCFG:=$(FOFMYBASE)/windows/wininstall.iss
WININSTDLEXES:=7za.exe  bash.dll  cp.exe  mkdir.exe  mv.exe  rm.exe  unrar.exe  wget.exe
WININSTDL:=$(wildcard $(FOFMYBASE)/windows/dlsongs/*.bat) $(addprefix $(FOFMYBASE)/windows/dlsongs/,$(WININSTDLEXES))

TGTDIST=$(subst build-,,$(subst /fretscpp,,$@))

all: 
#build-lucid32/fretscpp

ver:
	@echo "Version: $(THEVER)"

build-%/fretscpp: $(SOURCEDEPS)
	rm -rf build-$(TGTDIST)
	mkdir -p build-$(TGTDIST)/src
	cp $(SOURCES) build-$(TGTDIST)/src
	cp $(FOFMYBASE)/Makefile build-$(TGTDIST)
	cd $(ACHROOTB)/build-$(TGTDIST); schroot -c $(TGTDIST) make oneshot-opt
	-ln -s $(DATADIR) build-$(TGTDIST)/data

source: push/fretscpp-$(THEVER)-source.zip

.PHONY: push/fretscpp-$(THEVER)-source.zip
push/fretscpp-$(THEVER)-source.zip:
	rm -rf fretscpp-$(THEVER)
	mkdir -p push
	rm -f $@
	mkdir fretscpp-$(THEVER); cd fretscpp-$(THEVER); git clone ../.. .
	rm -rf fretscpp-$(THEVER)/.git
	zip -r push/fretscpp-$(THEVER)-source.zip fretscpp-$(THEVER)

win: build-win/fretscpp.exe
wininst: build-wininst/setup_fretscpp.exe

build-win/fretscpp.exe: $(WINSPECIFIC) $(SOURCEDEPS)
	rm -rf build-win
	rm -rf $(WINEBDIR)/drive_c/fretscpp
	mkdir -p build-win/src
	cp $(SOURCES) build-win/src
	cp $(WINSPECIFIC) build-win
	-ln -fs `pwd`/build-win $(WINEBDIR)/drive_c/fretscpp
	-ln -s $(DATADIR) build-win/data
	for i in $(SOURCEX); do echo "#include \"$$i\""; done >build-win/oneshot.cc
	cd $(WINEBDIR)/drive_c/fretscpp; WINEPREFIX=$(WINEBDIR) wine 'c:/mingw/bin/windres' resource.rc -O coff -o resource.o
	cd $(WINEBDIR)/drive_c/fretscpp; WINEPREFIX=$(WINEBDIR) wine 'c:/mingw/bin/g++' -m32 resource.o  oneshot.cc -O3 -g3 -ggdb -o fretscpp -mwindows --static $(WINLIBS) $(WININCL) $(WINDEFS)
	cd $(WINEBDIR)/drive_c/fretscpp; WINEPREFIX=$(WINEBDIR) wine 'c:/mingw/bin/strip' fretscpp.exe

build-wininst/setup_fretscpp.exe: build-win/fretscpp.exe $(DATAFILES) $(WININSTCFG) $(WININSTDL) $(FOFMYBASE)/README
	rm -rf build-wininst
	mkdir -p build-wininst/dlsongs
	rm -rf $(WINEBDIR)/drive_c/fretscpp-inst
	ln -s `pwd`/build-wininst $(WINEBDIR)/drive_c/fretscpp-inst
	cp -r $(FOFMYBASE)/data build-wininst
	cp -r $(WININSTDL) build-wininst/dlsongs
	sed "s:AppVersion=0.9:AppVersion=$(THEVER):" $(WININSTCFG) >build-wininst/wininstall.iss
	cp $< build-wininst
	cp $(FOFMYBASE)/README build-wininst/README.TXT
	cd $(WINEBDIR)/drive_c/fretscpp-inst; WINEPREFIX=$(WINEBDIR) wine "c:/program files/inno setup 5/iscc.exe" wininstall.iss
	mv build-wininst/setup.exe $@

.SECONDEXPANSION:

TESTTGT=$(subst test-,,$@)

test-%: build-%/fretscpp
	cd $(ACHROOTB)/build-$(TESTTGT); schroot -c $(TESTTGT) fretscpp

clean:
	rm -rf build-* push

push-win: build-wininst/setup_fretscpp.exe
	[ "$(THEVER)" != "TEMPORARY" ]
	mkdir -p push
	cp $< push/fretscpp-$(THEVER)-setup.exe
	googlecode_upload -s "Fretscpp $(THEVER) Windows Setup" -p fretscpp -u $(GOOGLECODE_USER) -w $(GOOGLECODE_PASS) -l Featured push/fretscpp-$(THEVER)-setup.exe

push-source: push/fretscpp-$(THEVER)-source.zip
	[ "$(THEVER)" != "TEMPORARY" ]
	googlecode_upload -s "Fretscpp $(THEVER) Source" -p fretscpp -u $(GOOGLECODE_USER) -w $(GOOGLECODE_PASS) -l Featured $<
