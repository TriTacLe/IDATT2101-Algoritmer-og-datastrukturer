.PHONY: all run clean 

COMPILER ?= javac
CFLAGS = 
TARGET = Main
SOURCE = Main.java

all: $(TARGET) 

$(TARGET): $(SOURCE)
	$(COMPILER) $(SOURCE)

run: $(TARGET)
	$(COMPILER) $(TARGET)

clean:
	rm -f $(TARGET) 

rmInputFiles:
	rm -f kanter.txt noder.txt interessepkt.txt