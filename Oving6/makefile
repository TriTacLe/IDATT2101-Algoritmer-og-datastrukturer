.PHONY: all kom dek clean check roundtrip

COMPILER ?= gcc #clang?

TARGETKOM = komprimering
TARGETDEK = dekomprimering

SOURCEKOM = komprimering.c
SOURCEDEK = dekomprimering.c

FILE ?= diverse.lyx

DEF_FILE := output.komprimert.deflate
DEC_FILE := $(FILE).dec

all: $(TARGETKOM) $(TARGETDEK)

$(TARGETKOM): $(SOURCEKOM)
	$(COMPILER) $(CFLAGS) $^ -o $@

$(TARGETDEK): $(SOURCEDEK)
	$(COMPILER) $(CFLAGS) $^ -o $@

kom: $(TARGETKOM)
	./$(TARGETKOM) $(FILE)

dek: $(TARGETDEK)
	./$(TARGETDEK) $(DEF_FILE) $(DEC_FILE)

roundtrip: all kom dek check

check:
	@cmp -s $(FILE) $(DEC_FILE) && echo "OK: $(FILE) == $(DEC_FILE)" || (echo "FEIL: filer er ulike" ; exit 1)

clean:
	rm -f $(TARGETKOM) $(TARGETDEK) $(DEC_FILE)
