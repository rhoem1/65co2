if [ ! -d tmp ]; then
	mkdir tmp
fi

for i in apple1 cbmbasic1 cbmbasic2 kbdbasic osi kb9 applesoft microtan aim65 sym1; do

echo $i
ca65 -D $i msbasic.s -o tmp/$i.o &&
ld65 -C $i.cfg tmp/$i.o -o tmp/$i.bin -Ln tmp/$i.lbl
xxd -g 1 -u tmp/$i.bin > tmp/$i.hex
xxd -g 1 -u -i tmp/$i.bin > tmp/$i.h

done

