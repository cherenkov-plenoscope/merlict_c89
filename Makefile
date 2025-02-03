#Compiler and Linker
CC          := gcc

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := $(SRCDIR)
BUILDDIR    := build/obj
TARGETDIR   := build/bin
RESDIR      := res
SRCEXT      := c
TESTSRCEXT  := test.c
MAINSRCEXT  := main.c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -std=c89 -Wall -pedantic -Wextra -Wstrict-prototypes
LIB         := -lm
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT) | grep -v $(TESTSRCEXT) | grep -v $(MAINSRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: resources ground_grid viewer

viewer: examples/mli_viewer/viewer.main.c $(OBJECTS)
	$(CC) -o $(TARGETDIR)/viewer $^ $(LIB)

ground_grid: examples/mli_corsika/ground_grid.main.c $(OBJECTS)
	$(CC) -o $(TARGETDIR)/ground_grid $^ $(LIB)

#Remake
remake: cleaner all

#Copy Resources from Resources Directory to Target Directory
resources: directories
#	@cp $(RESDIR)/* $(TARGETDIR)/

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources directories