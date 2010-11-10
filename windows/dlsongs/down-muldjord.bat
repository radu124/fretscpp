.\rm -rf unpk
.\mkdir unpk
.\mkdir down

.\wget -c -O "down\songs_muldjord.zip" "http://fretscpp.googlecode.com/files/songs_muldjord.zip"

echo "Extracting songs_muldjord (ZIP)"
.\7za x -ounpk "down\songs_muldjord.zip"
echo "Moving files"
.\mkdir -p ..\data\songs
.\mv -f "unpk\fretsonfire-songs-muldjord-2.dfsg\armygeddon" "..\data\songs\Muldjord - Armygeddon"
.\mv -f "unpk\fretsonfire-songs-muldjord-2.dfsg\chaos_god" "..\data\songs\Muldjord - Chaos God"
.\mv -f "unpk\fretsonfire-songs-muldjord-2.dfsg\internal_degeneration" "..\data\songs\Muldjord - Internal Degeneration"
.\mv -f "unpk\fretsonfire-songs-muldjord-2.dfsg\mutilated_mime" "..\data\songs\Muldjord - Mutilated Mime"

echo "Cleaning up"
.\rm -rf unpk
