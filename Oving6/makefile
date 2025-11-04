.PHONY: all kom dek clean check roundtrip

COMPILER ?= gcc
TARGETKOM = komprimering
TARGETDEK = dekomprimering
SOURCEKOM = komprimering.c
SOURCEDEK = dekomprimering.c
FILE ?= diverse.lyx
DEF_FILE := output.komprimert.deflate
DEC_FILE := diverse.lyx.dec  

all: $(TARGETKOM) $(TARGETDEK)

$(TARGETKOM): $(SOURCEKOM)
	$(COMPILER) $(CFLAGS) $^ -o $@

$(TARGETDEK): $(SOURCEDEK)
	$(COMPILER) $(CFLAGS) $^ -o $@

kom: $(TARGETKOM)
	./$(TARGETKOM) $(FILE)

dek: $(TARGETDEK)
	./$(TARGETDEK) $(DEF_FILE) $(DEC_FILE)

roundtrip: all
	@$(MAKE) kom FILE=$(FILE)
	@$(MAKE) dek FILE=$(FILE)
	@$(MAKE) check FILE=$(FILE)

check:
	@cmp -s $(FILE) $(DEC_FILE) && echo "OK, filene er like: $(FILE) == $(DEC_FILE)" || (echo "FEIL, filene er ulike: $(FILE) ikke lik $(DEC_FILE)" ; exit 1)

clean:
	rm -f $(TARGETKOM) $(TARGETDEK) *.dec output.komprimert.*