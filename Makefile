CC=gcc
CFLAGS=-I.
DEPS = SDL2/SDL.h
OBJ = X_fenetre.o
LIBS=-lSDL2

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

X_fenetre: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ) X_fenetre
