CC = clang
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm -lSDL2_gfx

SRCS = Pave_De_Serpents.c
OBJS = $(SRCS:.c=.o)
TARGET = Pave_De_Serpents

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
