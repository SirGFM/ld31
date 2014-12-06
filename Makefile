
CC=gcc
TARGET=game
OBJS= \
     src/global.o \
     src/main.o \
     src/playstate.o

ASSETS= \
       assets/atlas.dat

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

%.dat: %.bmp
	rm -f $@

$(LIB):
	make shared --directory=./libs/GFraMe/ USE_OPENGL=yes

.PHONY: clean mostlyclean

clean:
	rm -f $(TARGET) $(OBJS)

mostlyclean:
	rm -f $(TARGET) $(OBJS)
	make clean --directory=./libs/GFraMe/

