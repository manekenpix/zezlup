CC = clang++ -v -std=c++17
CFLAGS_RELEASE = -Llib/ -DGLEW_STATIC -lglfw3 -lGLEW -ldl -lX11 -lXrandr -Wl,-Bdynamic -lGL -lz -pthread -lfreetype
CFLAGS_DEBUG = -lGL -lGLEW -lglfw -lz -lX11 -lXrandr -lfreetype
SRCDIR = src
BUILDDIR = build
TARGET = zezlup
SRCEXT = cpp
FREETYPE = -I/usr/include/freetype2
RELEASE = -03
DEBUG = -g

SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

release: $(OBJECTS)
	$(CC) $(RELEASE) $^ -o $(TARGET) $(CFLAGS_RELEASE)

debug: $(OBJECTS)
	$(CC) $(DEBUG) $^ -o $(TARGET) $(CFLAGS_DEBUG)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(DEBUG) $(FREETYPE) -Wall -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
