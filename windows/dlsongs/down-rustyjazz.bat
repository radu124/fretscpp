.\mkdir down
.\wget -c -O "down\americanradio.zip" "http://www2.rustyjazmotorfuck.com/files/AmericanRadio_rjmf_FoF.zip"

.\rm -rf unpk3
.\mkdir unpk3
.\mkdir -p ..\data\songs
echo "Extracting americanradio (ZIP)"
.\7za x -ounpk3 "down\americanradio.zip"
echo "Moving files"
.\mv -f "unpk3\AmericanRadio_rjmf_FoF\Rusty Jaz MotorFuck - American Radio" "..\data\songs\Rusty Jaz MotorFuck - American Radio"
.\mv -f "unpk3\AmericanRadio_rjmf_FoF\readme.txt" "..\data\songs\Rusty Jaz MotorFuck - American Radio\"
.\mv -f "unpk3\AmericanRadio_rjmf_FoF\Rusty Jaz MotoFuck Official Website.url" "..\data\songs\Rusty Jaz MotorFuck - American Radio\"
echo "Cleaning up"
.\rm -rf unpk3
