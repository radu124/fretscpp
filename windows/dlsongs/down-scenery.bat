.\mkdir down
.\wget -c -O "down\fastenmefof.zip" "http://www.raveltree.com/fastenmefof.zip"
.\rm -rf unpk2
.\mkdir unpk2
.\mkdir -p ..\data\songs
echo "Extracting fastenmefof (ZIP)"
.\7za x -ounpk2 "down\fastenmefof.zip"
echo "Moving files"
.\mv -f "unpk2\fastenme" "..\data\songs\Scenery Channel - Fasten Me"
echo "Cleaning up"
.\rm -rf unpk2
