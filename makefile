# Compiler & Linker
CC := clang

# Target binary
TARGET := main

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := obj
TARGETDIR := bin
SRCEXT := c
OBJEXT := o

# Flags, Libraries, Includes
CFLAGS := -Wall -g -fsanitize=address -fsanitize-address-use-after-return=runtime -fsanitize-address-use-after-scope
CPPFLAGS :=
INC :=
LDFLAGS :=
LIBS :=

# Files
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(subst $(SRCDIR),$(BUILDDIR),$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

all: $(TARGET) clean

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(TARGETDIR)/$@

$(OBJECTS): $(SOURCES) prebuild
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $(subst $(BUILDDIR), $(SRCDIR), $(@:.$(OBJEXT)=.$(SRCEXT))) -o $@

prebuild:
	$(shell mkdir -p $(BUILDDIR))
	$(shell mkdir -p $(TARGETDIR))

clean:
	$(RM) -rf $(BUILDDIR)
	#$(RM) -rf $(TARGETDIR)
