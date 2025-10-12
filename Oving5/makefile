.PHONY: run clean

target = ov5
compiler = gcc

all: $(target)
	$(compiler) ov5.c -o $(target)

run: all
	./ov5 $(del)

clean:
	rm -f $(target)
