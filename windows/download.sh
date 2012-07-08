#!/bin/bash

function doecho()
{
	echo 'echo "'$1'"'
}

function dlone()
{
	echo '.\wget -c -O "down\'$2'" "'$1'"'
}

function dload()
{
	echo '.\mkdir down'
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
		echo '.\unzip -q "down\7unzip.zip" 7za.exe'
	fi
}

function domove()
{
	echo '.\mv -f "'$1'" "'$2'"'
}

function extract_fof()
{
	domove "unpk\\Frets on Fire\\data\\songs\\bangbang" "..\\data\\songs\\Mary Jo feat. Tommi Inkila - Mistery Man"
	domove "unpk\\Frets on Fire\\data\\songs\\defy" "..\\data\\songs\\Tommi Inkila - Defy the Machine"
	domove "unpk\\Frets on Fire\\data\\songs\\twibmpg" "..\\data\\songs\\Tommi Inkila - This Week I've Been Mostly Playing Guitar"
}

function extract_ubu_sectoid()
{
	domove "unpk\\fretsonfire-songs-sectoid-1.dfsg\\Escape from chaosland" "..\\data\\songs\\Sectoid - Escape from Chaosland"
	domove "unpk\\fretsonfire-songs-sectoid-1.dfsg\\Feelings" "..\\data\\songs\\Sectoid - Feelings"
	domove "unpk\\fretsonfire-songs-sectoid-1.dfsg\\Metal madness" "..\\data\\songs\\Sectoid - Metal Madness"  
	domove "unpk\\fretsonfire-songs-sectoid-1.dfsg\\War of freedom" "..\\data\\songs\\Sectoid - War of Freedom"
}

function extract_ubu_muldjord()
{
	domove "unpk\\fretsonfire-songs-muldjord-2.dfsg\\armygeddon" "..\\data\\songs\\Muldjord - Armygeddon"
	domove "unpk\\fretsonfire-songs-muldjord-2.dfsg\\chaos_god" "..\\data\\songs\\Muldjord - Chaos God"
	domove "unpk\\fretsonfire-songs-muldjord-2.dfsg\\internal_degeneration" "..\\data\\songs\\Muldjord - Internal Degeneration"
	domove "unpk\\fretsonfire-songs-muldjord-2.dfsg\\mutilated_mime" "..\\data\\songs\\Muldjord - Mutilated Mime"
}

function extract_fastenmefof()
{
	domove "unpk\\fastenme" "..\\data\\songs\\Scenery Channel - Fasten Me"
}


function extract_americanradio()
{
	domove "unpk\\AmericanRadio_rjmf_FoF\\Rusty Jaz MotorFuck - American Radio" "..\\data\\songs\\Rusty Jaz MotorFuck - American Radio"
	domove "unpk\\AmericanRadio_rjmf_FoF\\readme.txt" "..\\data\\songs\\Rusty Jaz MotorFuck - American Radio\\"
	domove "unpk\\AmericanRadio_rjmf_FoF\\Rusty Jaz MotoFuck Official Website.url" "..\\data\\songs\\Rusty Jaz MotorFuck - American Radio\\"	
}

function extract_whenready()
{
	domove "unpk\\When_Im_Ready" "..\\data\\songs\\M-Halo - When I'm Ready"
}

function extract_hoshi()
{
	domove "unpk\\hOSHI - hOSHIs new Groove" "..\\data\\songs\\hOSHI - hOSHIs new Groove"
}

function extract_euphoriac()
{
	domove "unpk\\100Ft Snowman - Euphoriac Intrances" "..\\data\\songs\\100Ft Snowman - Euphoriac Intrances"
}
#	.\\mv -f "unpk\\" "..\\data\\songs\\"

function extract_hontripod()
{
	domove "unpk\\100Ft Snowman - Head on a Tripod" "..\\data\\songs\\100Ft Snowman - Head on a Tripod"
}

function unpack_zip
{
	doecho "Extracting $1 (ZIP)"
	echo '.\unzip -q "down\'$1'.zip" -d unpk'
}

function unpack_tgz
{
	doecho "Extracting $1 (TAR.GZ)"
	echo '.\cp "down\'$1'.tgz" unpk\archive.tar.gz'
	echo '.\gzip -d "unpk\\archive.tar.gz"'
	echo '.\tar -xf unpk\archive.tar -C unpk'
}

function unpack_rar
{
	doecho "Extracting $1 (RAR)"
	echo '.\unrar x "down\'$1'.rar" "unpk\\"'
}

function unpack_7z
{
	doecho "Extracting $1 (7-zip)"
	echo '.\7za x -ounpk "down\'$1'.7z"'
}

function doextract()
{
	echo '.\rm -rf unpk'
	echo '.\mkdir unpk'
	echo '.\mkdir -p ..\data\songs'
	"unpack_$2" $1
	doecho "Moving files"
	"extract_$1" $2
	doecho "Cleaning up"
	echo '.\rm -rf unpk'
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
