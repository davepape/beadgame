CFLAGS = -O -I.
CPPFLAGS = $(CFLAGS)
LFLAGS = -O
LIBS = -lglfw -lGL -ltiff -lm

ALLTARGETS = bead

all: $(ALLTARGETS)

clean:
	rm -f $(ALLTARGETS) *.o

BEADOBJS = main.o Texture2D.o Sprite.o Rope.o Spring2.o Vector2.o
bead: $(BEADOBJS)
	c++ $(LFLAGS) -o bead $(BEADOBJS) $(LIBS)
