CC=gcc
CFLAGS=-Wall

# .PHONY = pas de fichier généré
.PHONY: clean

#target 

sokoban: main.o fonctions.o
	$(CC) $(CFLAGS) -o $@ $^

#regle générique
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f *.o *~ *#  sokoban
