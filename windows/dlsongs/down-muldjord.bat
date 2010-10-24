.\rm -rf unpk4
.\mkdir unpk4
.\mkdir down

.\wget -c -O "down\ubu_muldjord.tgz" "http://archive.ubuntu.com/ubuntu/pool/universe/f/fretsonfire-songs-muldjord/fretsonfire-songs-muldjord_2.dfsg.orig.tar.gz"

echo "Extracting ubu_muldjord (TAR.GZ)"
.\7za x -ounpk4 "down\ubu_muldjord.tgz"
.\7za x -ounpk4 unpk4\ubu_muldjord.tar
echo "Moving files"
.\mkdir -p ..\data\songs
.\mv -f "unpk4\fretsonfire-songs-muldjord-2.dfsg\armygeddon" "..\data\songs\Muldjord - Armygeddon"
.\mv -f "unpk4\fretsonfire-songs-muldjord-2.dfsg\chaos_god" "..\data\songs\Muldjord - Chaos God"
.\mv -f "unpk4\fretsonfire-songs-muldjord-2.dfsg\internal_degeneration" "..\data\songs\Muldjord - Internal Degeneration"
.\mv -f "unpk4\fretsonfire-songs-muldjord-2.dfsg\mutilated_mime" "..\data\songs\Muldjord - Mutilated Mime"

echo "Cleaning up"
.\rm -rf unpk4
