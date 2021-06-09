CC = clang++ -v
CFLAGS_RELEASE = -Llib/ -DGLEW_STATIC -lglfw3 -lGLEW -ldl -Wl,-Bdynamic -lGL -lz -pthread
CFLAGS_DEBUG = -lGL -lGLEW -lglfw -lz
SRCDIR = src
INCLUDEDIR = src/include
BUILDDIR = build
TARGET = bin/zezlup
CCRELEASE = -std=c++17 -O3
CCDEBUG = -std=c++17 -g
SRCEXT = cpp
HDREXT = hpp
INC = -I include

SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

release: $(OBJECTS)
	$(CC) $(CCRELEASE) $^ -o $(TARGET) $(CFLAGS_RELEASE)

debug: $(OBJECTS)
	$(CC) $(CCDEBUG) $^ -o $(TARGET) $(CFLAGS_DEBUG)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CCDEBUG) -Wall -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
