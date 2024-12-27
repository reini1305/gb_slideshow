#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#
ifndef GBDK_HOME
	GBDK_HOME = ../gbdk/
endif

LCC = $(GBDK_HOME)bin/lcc
PNG2ASSET = $(GBDK_HOME)bin/png2asset

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

# Enable autobanking, GBC support, MBC5
LCCFLAGS += -I$(OBJDIR) -Wl-yt0x1B -Wm-yc -Wl-j -Wm-ya4 -autobank -Wb-ext=.rel -Wb-v

PROJECTNAME = gb_slideshow

SRCDIR      = src
OBJDIR      = obj
RESOBJSRC   = obj/res
RESDIR      = res
MKDIRS      = $(OBJDIR) $(BINDIR) $(RESOBJSRC) # See bottom of Makefile for directory auto-creation

BINS	    = $(OBJDIR)/$(PROJECTNAME).gb

# For png2asset: converting source pngs -> .c -> .o
IMGPNGS     = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.png)))
IMGSOURCES  = $(IMGPNGS:%.png=$(RESOBJSRC)/%.c)
IMGOBJS     = $(IMGSOURCES:$(RESOBJSRC)/%.c=$(OBJDIR)/%.o)

CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS       = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

all: $(SRCDIR)/images.h $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

# Use png2asset to convert the png into C formatted metasprite data
# -keep_duplicate_tiles   : Don't remove duplicate tiles
# -map                    : Use "map style" output, not metasprite
# -tiles_only             : Only keep tiles, no map (since APA mode expects tiles in linear order)
# -bpp 2                  : Use 2bpp output
# -c ...                  : Set C output file
# Convert metasprite .pngs in res/ -> .c files in obj/<platform ext>/src/
$(RESOBJSRC)/%.c:	$(RESDIR)/%.png
	$(PNG2ASSET) $< -keep_duplicate_tiles -map -bpp2 -b 255 -tiles_only -c $@

# Compile the pngs that were converted to .c files
# .c files in obj/res/ -> .o files in obj/
$(OBJDIR)/%.o:	$(RESOBJSRC)/%.c
	$(LCC) $(LCCFLAGS) $(CFLAGS) -c -o $@ $<

# Generate the images.h file
$(SRCDIR)/images.h: $(RESDIR)/slides.json
	python3 $(SRCDIR)/generate_header.py

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Convert images first so they're available when compiling the main sources
$(OBJS):	$(IMGOBJS) $(SRCDIR)/images.h

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(IMGOBJS) $(OBJS)

clean:
	rm -f  $(OBJDIR)/*.*
	rm -f  $(RESOBJSRC)/*.*
	rm $(SRCDIR)/images.h
	rm $(SRCDIR)/slides.h

# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
$(info $(shell mkdir -p $(MKDIRS)))

