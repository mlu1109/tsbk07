LAB=4
PART=4

COMMONDIR=common/
LABDIR=$(LAB)/
# Lab specific source files
ifeq ($(LAB), 1)
SOURCES_SPEC=$(LABDIR)transform.c
else ifeq ($(LAB), 2)
SOURCES_SPEC=$(LABDIR)transform.c
else ifeq ($(LAB), 3)
SOURCES_SPEC=$(LABDIR)algebra.c $(LABDIR)camera.c $(LABDIR)input.c $(LABDIR)shaders.c
else ifeq ($(LAB), 4)
SOURCES_SPEC=$(LABDIR)algebra.c $(LABDIR)camera.c $(LABDIR)input.c $(LABDIR)shaders.c $(LABDIR)object.c $(LABDIR)terrain.c
endif

SOURCES=$(LABDIR)lab$(LAB)-$(PART).c $(COMMONDIR)GL_utilities.c $(COMMONDIR)VectorUtils3.c $(COMMONDIR)loadobj.c $(COMMONDIR)LoadTGA.c $(COMMONDIR)Linux/MicroGlut.c $(SOURCES_SPEC)
CFLAGS=-Wall -o a.out -g -std=c99
INCFLAGS=-I$(COMMONDIR) -I$(COMMONDIR)Linux -DGL_GLEXT_PROTOTYPES
LIBFLAGS=-lXt -lX11 -lGL -lm

all:
	gcc $(CFLAGS) $(INCFLAGS) $(SOURCES) $(LIBFLAGS)

clean:
	rm a.out -f

run:
	@make clean
	@make all
	@echo "+---------+"
	@echo "| Lab $(LAB)-$(PART) |"
	@echo "+---------+"
	./a.out
