
TARGET=game

CFLAGS=-Wall -I../libs/GFraMe/include/
CFLAGS+=-DDEBUG -O0 -g -m64 -fPIC
LDFLAGS=-L../libs/GFraMe/bin/Linux/ -lGFraMe -lm -lSDL2main -lSDL2
LIB=../libs/GFraMe/bin/Linux/libGFraMe.a
TYPE=static

CC=gcc

OBJS = src/main.o \
       src/global.o \
       src/playstate.o \
	   src/player.o

all: $(TARGET)
	date

$(TARGET): $(LIB) $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	make $(TYPE) --directory=../libs/GFraMe

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

