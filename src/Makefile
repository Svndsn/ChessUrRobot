.PHONY: all clean

CC:= clang++
BUILD_DIR := build
CFLAGS = -std=c++11 -Wno-deprecated-enum-enum-conversion -O2
CFLAGS += `pkg-config --cflags --libs libmodbus`
# LDFLAGS = `pkg-config --libs opencv4`


SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

all: main

clean:
	rm -r $(BUILD_DIR)

-include $(DEPENDS)

# Make object files
$(BUILD_DIR):
	mkdir -p $(dir $@)
	$(CC) $(WARNING) $(CFLAGS) -MMD -MP -c $< -o $@

# Linking the executable from the object files
$(BUILD_DIR): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@
	./main
