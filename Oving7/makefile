.PHONY: all kom dek clean check roundtrip

COMPILER ?= gcc
CFLAGS = 
TARGET = 
SOURCE = .c

all: $(TARGET) 

$(TARGET): $(SOURCE)
	$(COMPILER) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET) 

clean:
	rm -f $(TARGET)