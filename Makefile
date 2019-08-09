# "Super-Makefile" for CNS assignment (WWW client/server) 2000
# If this Makefile does, for some reason, not work, go to each
# subdirectory manually and call configure; make; make clean
# by Christian Gosch

BASICTARGETS = 
STANDARDTARGETS = 
ADVANCEDTARGETS = source/server/server source/client/client
TARGETS = $(BASICTARGETS) $(STANDARDTARGETS) $(ADVANCEDTARGETS)

PACKAGENAME := goscy001_tonly002-$(shell date +%d%m%Y%H%M)
PKGFILES := source docs htdocs Makefile README COPYING

CONFIGDIRS := source/client source/server
MAKEDIRS   := $(CONFIGDIRS)

BASEDIR := $(shell pwd)

ALL: 
	@echo "Making all versions..."
	$(foreach DIR,$(MAKEDIRS), cd $(DIR) && $(MAKE) && $(MAKE) clean && cd $(BASEDIR); )

autoconf:
	@echo "Making configure scripts..."
	$(foreach DIR,$(MAKEDIRS), cd $(DIR) && autoconf && cd $(BASEDIR); )	

configure:
	@echo "Configuring source trees..."
	$(foreach DIR,$(CONFIGDIRS), cd $(DIR) && ./configure && cd $(BASEDIR); )

clean:
	rm -f $(TARGETS) *~
	$(foreach DIR,$(MAKEDIRS), cd $(DIR) && $(MAKE) clean && rm -f config.cache && cd $(BASEDIR); )

package:
	@echo "Making package $(PACKAGENAME).tar.gz"
	tar cf $(PACKAGENAME).tar $(PKGFILES)
	gzip $(PACKAGENAME).tar

doc:
	$(foreach DIR,$(MAKEDIRS), cd $(DIR)/include && doxygen config.doxygen && cd $(BASEDIR); )

