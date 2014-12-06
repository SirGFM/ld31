
CC=gcc
TARGET=game
OBJS=src/main.o \
     src/global.o \
     src/playstate.o

CFLAGS=-I./libs/GFraMe/include/ -m64
ifneq ($(RELEASE), yes)
  CFLAGS += -g -O0 -DDEBUG
endif
LDFLAGS=-L./libs/GFraMe/bin/Linux/ -lGFraMe -lm -lSDL2main -lSDL2
LIB=./libs/GFraMe/bin/Linux/libGFraMe.a

all: $(TARGET)
	date

$(TARGET): $(LIB) $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIB):
	make static --directory=./lib/GFraMe/

.PHONY: clean mostlyclean

clean:
	rm -f $(TARGET) $(OBJS)

mostlyclean:
	clean
	make clean --directory=./lib/GFraMe

