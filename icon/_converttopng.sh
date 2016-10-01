cd "/home/laurent/mount/tchome/Documents/Perso/pc - dev/Qt5/uxplore/icon"
rm -f *.png
for i in *; do inkscape $i --export-png=`echo $i | sed -e 's/svg$/png/'`; done
touch *.png 
cp *.png "../bin/Debug/icon/."
rm -f *.png



