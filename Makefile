CC=gcc
CFLAGS=-I.
DEPS = SDL2/SDL.h SDL2/SDL2_gfxPrimitives.h
LIBS=-lSDL2 -lm -lSDL2_gfx

.PHONY: all clean

all: X_fenetre Pave_De_Serpents

X_fenetre: X_fenetre.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

Pave_De_Serpents: Pave_De_Serpents.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o X_fenetre Pave_De_Serpents
