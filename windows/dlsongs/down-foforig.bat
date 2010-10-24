.\mkdir down
.\wget -c -O "down\fof.zip" "http://downloads.sourceforge.net/fretsonfire/FretsOnFire-1.3.110-win32.zip?use_mirror=osdn"
.\rm -rf unpk7
.\mkdir unpk7
.\mkdir -p ..\data\songs
echo "Extracting fof (ZIP)"
.\7za x -ounpk7 "down\fof.zip"
echo "Moving files"
.\mv -f "unpk7\Frets on Fire\data\songs\bangbang" "..\data\songs\Mary Jo feat. Tommi Inkila - Bang Bang, Mystery Man"
.\mv -f "unpk7\Frets on Fire\data\songs\defy" "..\data\songs\Tommi Inkila - Defy the Machine"
.\mv -f "unpk7\Frets on Fire\data\songs\twibmpg" "..\data\songs\Tommi Inkila - This Week I've Been Mostly Playing Guitar"
echo "Cleaning up"
.\rm -rf unpk7
