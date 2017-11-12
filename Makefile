.PHONY: clean

EXE = cannon
SDL2_FLAGS = `sdl2-config --cflags --libs`
SDL2_COMPILE = $(SDL2_FLAGS) -lSDL2_ttf
SRCS_FILES := $(wildcard *.c)
OBJS_FILES := ${SRCS_FILES:.c=.o}
prog_OBJS := $(OBJS_FILES)

all: $(EXE)

$(EXE): $(prog_OBJS)
	$(LINK.o) $(OBJS_FILES) $(SDL2_COMPILE) -lm -o $(EXE)
	rm -rf *.o

clean:
	rm -rf $(EXE)