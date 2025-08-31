CC = gcc
CFLAGS = -std=c11
TARGET = oh-my-shells
SRCDIR = src
LIBDIR = lib/tomlc17/src
SOURCES = $(wildcard $(SRCDIR)/*.c) $(LIBDIR)/tomlc17.c

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
