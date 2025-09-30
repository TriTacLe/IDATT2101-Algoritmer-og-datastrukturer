# Build settings
compiler = gcc
compiler_flags=

# Executable 
del1program = Oving4Del1
del2program = Oving4Del2

# Source files
del1source = Oving4Del1.c
del2source = Oving4Del2.c

.PHONY: all run1 run2 run-all clean

all: $(del1program) $(del2program)

$(del1program): $(del1source)
	$(compiler) $(del1source) -o $(del1program)

$(del2program): $(del2source)
	$(compiler) $(del2source) -o $(del2program)

run1: $(del1program)
	./$(del1program)

run2: $(del2program)
	./$(del2program)

run-all: $(del1program) $(del2program)
	./$(del1program)
	./$(del2program)

clean:
	rm -f $(del1program) $(del2program)

# make Oving4Del1 or make run1 to build/run del 1.
# make Oving4Del2 or make run2 to build/run del 2.
# make run-all to run both.
# make clean to remove binaries.