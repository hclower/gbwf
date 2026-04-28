#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = /home/hclow/win_docs/gbdk/release/gbdk/
endif

LCC = $(GBDK_HOME)/bin/lcc 
LCCFLAGS = -Wl-m -Wl-w -Wl-j -Wm-yS -Iinc
IMG_CONVERTER = png2asset
IMG_CONVERTER_OPTS = -spr8x8 -tiles_only -no_palettes -keep_duplicate_tiles

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif


# You can set the name of the .gb ROM file here
PROJECTNAME = gbwf

SRCDIR      = src
OBJDIR      = bin
RESDIR      = rsrc
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
IMGPNGS     = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.png)))
IMGSOURCES  = $(IMGPNGS:%.png=%.c) 
IMGHDRS     = $(IMGPNGS:%.png=%.h) 
IMGSOBJS    = $(IMGPNGS:%.png=%.o) 
OBJS        = $(IMGSOURCES:%.c=$(OBJDIR)/%.o) $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o) 

# Added so make won't delete intermediate files on completion- I don't want these cleared out after the build,
# so I can refer to them and reuse them.
.PRECIOUS: $(RESDIR)/%.c

LCCFLAGS += -I$(RESDIR)


all:	prepare $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

#Generate image data with png2asset
$(RESDIR)/%.c : $(RESDIR)/%.png
	$(IMG_CONVERTER) $< $(IMG_CONVERTER_OPTS)

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
	rm -f  $(OBJDIR)/*.* $(RESDIR)/*.c $(RESDIR)/*.h

