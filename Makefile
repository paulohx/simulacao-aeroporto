main:	main.o	util.o	tadGrafo.o
		gcc	 main.o	util.o	tadGrafo.o	-o main.exe
		rm *.o

main.o:
		gcc -c main.c

util.o:
		gcc -c	util.c

tadGrafo.o:
		gcc -c	tadGrafo.c