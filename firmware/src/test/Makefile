# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$

SUBDIRS  = backlight button delay lcd led serial task twi

all: $(SUBDIRS)
clean: $(SUBDIRS)
distclean: $(SUBDIRS)
rebuild: $(SUBDIRS)
lib: $(SUBDIRS)
cleanlib: $(SUBDIRS)
extcoff: $(SUBDIRS)
program: $(SUBDIRS)
debug: $(SUBDIRS)

elf: $(SUBDIRS)
hex: $(SUBDIRS)
eep: $(SUBDIRS)
lss: $(SUBDIRS)
sym: $(SUBDIRS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug elf hex eep lss sym $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

