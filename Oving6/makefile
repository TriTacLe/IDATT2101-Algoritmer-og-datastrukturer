.PHONY: all kom dek clean

COMPILER = gcc
CFLAGS =

TARGETKOM = komprimering
TARGETDEK = dekomprimering

SOURCEKOM = komprimering.c
SOURCEDEK = dekomprimering.c

all: $(TARGETKOM) $(TARGETDEK)

$(TARGETKOM): $(SOURCEKOM)
	$(COMPILER) $^ -o $@

$(TARGETDEK): $(SOURCEDEK)
	$(COMPILER) $^ -o $@

kom:
	./$(TARGETKOM)

dek:
	./$(TARGETDEK)


clean:
	rm -f $(TARGETKOM)
	rm -f $(TARGETDEK)
