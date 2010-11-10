.\mkdir down
.\wget -c -O "down\songs_sectoid.zip" "http://fretscpp.googlecode.com/files/songs_sectoid.zip"
.\rm -rf unpk
.\mkdir unpk
.\mkdir -p ..\data\songs
echo "Extracting songs_sectoid (ZIP)"
.\7za x -ounpk "down\songs_sectoid.zip"
echo "Moving files"
.\mv -f "unpk\fretsonfire-songs-sectoid-1.dfsg\Escape from chaosland" "..\data\songs\Sectoid - Escape from Chaosland"
.\mv -f "unpk\fretsonfire-songs-sectoid-1.dfsg\Feelings" "..\data\songs\Sectoid - Feelings"
.\mv -f "unpk\fretsonfire-songs-sectoid-1.dfsg\Metal madness" "..\data\songs\Sectoid - Metal Madness"
.\mv -f "unpk\fretsonfire-songs-sectoid-1.dfsg\War of freedom" "..\data\songs\Sectoid - War of Freedom"
echo "Cleaning up"
.\rm -rf unpk
