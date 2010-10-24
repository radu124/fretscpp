.\mkdir down
.\wget -c -O "down\hoshi.7z" "http://game-systems.net/upl/hng.zip"
.\rm -rf unpk6
.\mkdir unpk6
.\mkdir -p ..\data\songs
echo "Extracting hoshi (7-zip)"
.\7za x -ounpk6 "down\hoshi.7z"
echo "Moving files"
.\mv -f "unpk6\hOSHI - hOSHIs new Groove" "..\data\songs\hOSHI - hOSHIs new Groove"
echo "Cleaning up"
.\rm -rf unpk6
