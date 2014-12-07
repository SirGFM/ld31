
CC=gcc
TARGET=game
OBJS= \
     src/clouds.o \
     src/global.o \
     src/main.o \
     src/particles.o \
     src/playstate.o \
     src/text.o \
     src/upper_text.o

ASSETS= \
       assets/atlas.dat \
       assets/song

CFLAGS=-I./libs/GFraMe/include/ -m64
ifneq ($(RELEASE), yes)
  CFLAGS += -g -O0 -DDEBUG
endif
LDFLAGS=-L./libs/GFraMe/bin/Linux/ -lGFraMe -lm -lSDL2main -lSDL2
LIB=./libs/GFraMe/bin/Linux/libGFraMe.a

all: $(TARGET)
	date

$(TARGET): $(LIB) $(OBJS) $(ASSETS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

assets/atlas.dat: assets/atlas.bmp
	rm -f $@

assets/song: assets/song.wav
	rm -f $@

$(LIB):
	make static --directory=./libs/GFraMe/

.PHONY: clean mostlyclean

clean:
	rm -f $(TARGET) $(OBJS)

mostlyclean:
	rm -f $(TARGET) $(OBJS)
	make clean --directory=./libs/GFraMe/

