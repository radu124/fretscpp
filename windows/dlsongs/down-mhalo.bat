.\mkdir down
.\wget -c -O "down\whenready.zip" "http://www.mhalo.com/mhalo/music/files/When_Im_Ready.zip"
.\rm -rf unpk5
.\mkdir unpk5
.\mkdir -p ..\data\songs
echo "Extracting whenready (ZIP)"
.\7za x -ounpk5 "down\whenready.zip"
echo "Moving files"
.\mv -f "unpk5\When_Im_Ready" "..\data\songs\M-Halo - When I'm Ready"
echo "Cleaning up"
.\rm -rf unpk5
