.\mkdir down
.\wget -c -O "down\euphoriac.rar" "http://100ftsnowman.com/fretsonfire/Euphoriac_Intrancis.rar"
.\wget -c -O "down\hontripod.zip" "http://100ftsnowman.com/fretsonfire/Head_on_a_Tripod.zip"
.\rm -rf unpk8
.\mkdir unpk8
.\mkdir -p ..\data\songs
echo "Extracting euphoriac (RAR)"
.\unrar x "down\euphoriac.rar" unpk8\
echo "Moving files"
.\mv -f "unpk8\100Ft Snowman - Euphoriac Intrances" "..\data\songs\100Ft Snowman - Euphoriac Intrances"
echo "Cleaning up"
.\rm -rf unpk8
.\mkdir unpk8
.\mkdir -p ..\data\songs
echo "Extracting hontripod (ZIP)"
.\7za x -ounpk8 "down\hontripod.zip"
echo "Moving files"
.\mv -f "unpk8\100Ft Snowman - Head on a Tripod" "..\data\songs\100Ft Snowman - Head on a Tripod"
echo "Cleaning up"
.\rm -rf unpk8
