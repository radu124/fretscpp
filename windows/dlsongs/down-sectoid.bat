.\mkdir down
.\wget -c -O "down\ubu_sectoid.tgz" "http://archive.ubuntu.com/ubuntu/pool/universe/f/fretsonfire-songs-sectoid/fretsonfire-songs-sectoid_1.dfsg.orig.tar.gz"
.\rm -rf unpk1
.\mkdir unpk1
.\mkdir -p ..\data\songs
echo "Extracting ubu_sectoid (TAR.GZ)"
.\7za x -ounpk1 "down\ubu_sectoid.tgz"
.\7za x -ounpk1 unpk1\ubu_sectoid.tar
echo "Moving files"
.\mv -f "unpk1\fretsonfire-songs-sectoid-1.dfsg\Escape from chaosland" "..\data\songs\Sectoid - Escape from Chaosland"
.\mv -f "unpk1\fretsonfire-songs-sectoid-1.dfsg\Feelings" "..\data\songs\Sectoid - Feelings"
.\mv -f "unpk1\fretsonfire-songs-sectoid-1.dfsg\Metal madness" "..\data\songs\Sectoid - Metal Madness"
.\mv -f "unpk1\fretsonfire-songs-sectoid-1.dfsg\War of freedom" "..\data\songs\Sectoid - War of Freedom"
echo "Cleaning up"
.\rm -rf unpk1
