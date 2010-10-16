function dlone()
{
	if [ -e "down/$2" ]; then
		echo "$2 already exists"
	else
		./wget "$1" -O "down/$2"
	fi
}

function dload()
{
	./mkdir down
	dlone 'http://downloads.sourceforge.net/fretsonfire/FretsOnFire-1.3.110-win32.zip?use_mirror=osdn' fof.zip
	dlone 'http://archive.ubuntu.com/ubuntu/pool/universe/f/fretsonfire-songs-sectoid/fretsonfire-songs-sectoid_1.dfsg.orig.tar.gz'  ubu_sectoid.tgz
	dlone 'http://archive.ubuntu.com/ubuntu/pool/universe/f/fretsonfire-songs-muldjord/fretsonfire-songs-muldjord_2.dfsg.orig.tar.gz'  ubu_muldjord.tgz
	dlone 'http://www.raveltree.com/fastenmefof.zip'  fastenmefof.zip
	dlone 'http://www2.rustyjazmotorfuck.com/files/AmericanRadio_rjmf_FoF.zip'  americanradio.zip
	dlone 'http://www.mhalo.com/mhalo/music/files/When_Im_Ready.zip'  whenready.zip
	dlone 'http://game-systems.net/upl/hng.zip'  hoshi.7z
	dlone 'http://100ftsnowman.com/fretsonfire/Euphoriac_Intrancis.rar' euphoriac.rar
	dlone 'http://100ftsnowman.com/fretsonfire/Head_on_a_Tripod.zip'  hontripod.zip
	if ! [ -e 7za.exe ]; then
		dlone "http://downloads.sourceforge.net/sevenzip/7za465.zip" 7unzip.zip
		./unzip -q "down/7unzip.zip" 7za.exe
	fi
}

function extract_fof()
{
	./mv -f "unpk/Frets on Fire/data/songs/bangbang" "../data/songs/Mary Jo feat. Tommi Inkila - Mistery Man"
	./mv -f "unpk/Frets on Fire/data/songs/defy" "../data/songs/Tommi Inkila - Defy the Machine"
	./mv -f "unpk/Frets on Fire/data/songs/twibmpg" "../data/songs/Tommi Inkila - This Week I've Been Mostly Playing Guitar"
}

function extract_ubu_sectoid()
{
	./mv -f "unpk/fretsonfire-songs-sectoid-1.dfsg/Escape from chaosland" "../data/songs/Sectoid - Escape from Chaosland"
	./mv -f "unpk/fretsonfire-songs-sectoid-1.dfsg/Feelings" "../data/songs/Sectoid - Feelings"
	./mv -f "unpk/fretsonfire-songs-sectoid-1.dfsg/Metal madness" "../data/songs/Sectoid - Metal Madness"
	./mv -f "unpk/fretsonfire-songs-sectoid-1.dfsg/War of freedom" "../data/songs/Sectoid - War of Freedom"
}

function extract_ubu_muldjord()
{
	./mv -f "unpk/fretsonfire-songs-muldjord-2.dfsg/armygeddon" "../data/songs/Muldjord - Armygeddon"
	./mv -f "unpk/fretsonfire-songs-muldjord-2.dfsg/chaos_god" "../data/songs/Muldjord - Chaos God"
	./mv -f "unpk/fretsonfire-songs-muldjord-2.dfsg/internal_degeneration" "../data/songs/Muldjord - Internal Degeneration"
	./mv -f "unpk/fretsonfire-songs-muldjord-2.dfsg/mutilated_mime" "../data/songs/Muldjord - Mutilated Mime"
}

function extract_fastenmefof()
{
	./mv -f "unpk/fastenme" "../data/songs/Scenery Channel - Fasten Me"
}


function extract_americanradio()
{
	./mv -f "unpk/AmericanRadio_rjmf_FoF/Rusty Jaz MotorFuck - American Radio" "../data/songs/Rusty Jaz MotorFuck - American Radio"
	./mv -f "unpk/AmericanRadio_rjmf_FoF/readme.txt" "../data/songs/Rusty Jaz MotorFuck - American Radio/"
	./mv -f "unpk/AmericanRadio_rjmf_FoF/Rusty Jaz MotoFuck Official Website.url" "../data/songs/Rusty Jaz MotorFuck - American Radio/"	
}

function extract_whenready()
{
	./mv -f "unpk/When_Im_Ready" "../data/songs/M-Halo - When I'm Ready"
}

function extract_hoshi()
{
	./mv -f "unpk/hOSHI - hOSHIs new Groove" "../data/songs/hOSHI - hOSHIs new Groove"
}

function extract_euphoriac()
{
	./mv -f "unpk/100Ft Snowman - Euphoriac Intrances" "../data/songs/100Ft Snowman - Euphoriac Intrances"
}
#	./mv -f "unpk/" "../data/songs/"

function extract_hontripod()
{
	./mv -f "unpk/100Ft Snowman - Head on a Tripod" "../data/songs/100Ft Snowman - Head on a Tripod"
}

function unpack_zip
{
	echo "Extracting $1 (ZIP)"
	./unzip -q "down/$1.zip" -d unpk
}

function unpack_tgz
{
	echo "Extracting $1 (TAR.GZ)"
	./cp "down/$1.tgz" unpk/archive.tar.gz
	./gzip -d "unpk/archive.tar.gz"
	./tar -xf unpk/archive.tar -C unpk
}

function unpack_rar
{
	echo "Extracting $1 (RAR)"
	./unrar x "down/$1.rar" 'unpk/'
}

function unpack_7z
{
	echo "Extracting $1 (7-zip)"
	./7za x -ounpk "down/$1.7z" 
}

function doextract()
{
	if ! [ -e "down/$1.$2" ]; then
		echo "Archive not found: $1.$2"
		return
	fi
	./rm -rf unpk
	./mkdir unpk
	./mkdir -p ../data/songs
	"unpack_$2" $1
	echo "Moving files"
	"extract_$1" $2
	echo "Cleaning up"
	./rm -rf unpk
}

dload
doextract fof zip
doextract ubu_sectoid tgz
doextract ubu_muldjord tgz
doextract fastenmefof zip
doextract americanradio zip
doextract whenready zip
doextract hoshi 7z
doextract euphoriac rar
doextract hontripod zip
