### 
## Makefile
## INFO0030: Projet 4
## 
### 

## Variables

# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes `pkg-config --cflags --libs gtk+-2.0`
LD=gcc
LDFLAGS=`pkg-config --cflags --libs gtk+-2.0`

# Files
EXEC=demineur
MODULES=main_demineur.c modele_demineur.c vue_demineur.c controleur_demineur.c  type_opaque.c
OBJECTS=main_demineur.o modele_demineur.o vue_demineur.o controleur_demineur.o type_opaque.o 

# Documentation
DOC = main_demineur.c modele_demineur.c modele_demineur.h vue_demineur.c vue_demineur.h controleur_demineur.c controleur_demineur.h  type_opaque.c type_opaque.h


## Rules

#compilation
all: $(EXEC)

demineur: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

main_demineur.o: main_demineur.c
	$(CC) -c main_demineur.c -o main_demineur.o $(CFLAGS)

modele_demineur.o: modele_demineur.c
	$(CC) -c modele_demineur.c -o modele_demineur.o $(CFLAGS)

vue_demineur.o: vue_demineur.c
	$(CC) -c vue_demineur.c -o vue_demineur.o $(CFLAGS)

controleur_demineur.o: controleur_demineur.c
	$(CC) -c controleur_demineur.c -o controleur_demineur.o $(CFLAGS)

type_opaque.o: type_opaque.c
	$(CC) -c type_opaque.c -o type_opaque.o $(CFLAGS)



#compilation doc
doc:all_doc clean_latex

all_doc: $(DOC)
	doxygen $(DOC)
	mkdir -p doc
	mv html doc/html
	mv latex doc/latex
	mv doc ../doc

clean_latex:
	cd .. && rm -r doc/latex/

clean_doc:
	cd .. && rm -r doc

#rapport
rapport:all_rapport clean_log_aux

all_rapport:
	cd ../rapport && pdflatex rapport_demineur.tex

clean_log_aux:
	cd ../rapport && rm -f rapport_demineur.aux rapport_demineur.log

clean_rapport: clean_log_aux
	cd ../rapport && rm -f rapport_demineur.pdf

#création archive
archive:
	cd .. && tar -zcvf demineur_01.tar.gz source rapport doc

#supp fichiers objets + executable
clean:
	rm -f *.o $(EXEC)

