.PHONY: all kom dek clean

COMPILER = gcc
CFLAGS =

TARGETKOM = komprimering
TARGETDEK = dekomprimering

SOURCEKOM = komprimering.c
SOURCEDEK = dekomprimering.c

FILE ?= diverse.lyx

all: $(TARGETKOM) $(TARGETDEK)

$(TARGETKOM): $(SOURCEKOM)
	$(COMPILER) $^ -o $@

$(TARGETDEK): $(SOURCEDEK)
	$(COMPILER) $^ -o $@

kom: $(TARGETKOM)
	./$(TARGETKOM) $(FILE)

dek: $(TARGETDEK)
	./$(TARGETDEK) $(FILE)


clean:
	rm -f $(TARGETKOM)
	rm -f $(TARGETDEK)
