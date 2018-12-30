OPTIONS_DEBUG=-ggdb -g3 -Wall
OPTIONS_RELEASE=-O3 
OPTIONS=$(OPTIONS_DEBUG)
INCPATH=/home/bayashi/Coding/Include
LIBPATH=/home/bayashi/Coding/Include

all: pbmake_wget main
	
# Automatic installation of the repository PBMake in the parent folder
pbmake_wget:
	if [ ! -d ../PBMake ]; then wget https://github.com/BayashiPascal/PBMake/archive/master.zip; unzip master.zip; rm -f master.zip; sed -i '' 's@ROOT_DIR=.*@ROOT_DIR='"`pwd | gawk -F/ 'NF{NF-=1};1' | sed -e 's@ @/@g'`"'@' PBMake-master/Makefile.inc; mv PBMake-master ../PBMake; fi

main: main.o cloudgraph.o Makefile $(LIBPATH)/tgapaint.o $(LIBPATH)/gset.o $(LIBPATH)/bcurve.o $(LIBPATH)/pbmath.o
	gcc  $(OPTIONS) main.o $(LIBPATH)/tgapaint.o $(LIBPATH)/gset.o $(LIBPATH)/bcurve.o $(LIBPATH)/pbmath.o cloudgraph.o -o main -lm

main.o : main.c cloudgraph.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c main.c

cloudgraph.o : cloudgraph.c cloudgraph.h $(INCPATH)/tgapaint.h $(INCPATH)/gset.h $(INCPATH)/pbmath.h $(INCPATH)/bcurve.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c cloudgraph.c

clean : 
	rm -rf *.o main

test :
	main -file testCloud.txt -tga cloud.tga -line -nodeLabel -familyLabel

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main -tga cloud.tga -line -nodeLabel -familyLabel
	
