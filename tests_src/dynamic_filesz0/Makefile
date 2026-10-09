# ELF objects whose PT_DYNAMIC reports p_filesz == 0, which loaders have to tell apart.
#
# linked_list.debug is the separate debug-info file of tests/x86_64/linked_list: the output of
# objcopy --only-keep-debug, which is what /usr/lib/debug and -dbg packages hold and what a
# .gnu_debuglink points at. Every allocated section becomes SHT_NOBITS, so the PT_LOAD that carried
# the dynamic table keeps its p_memsz and loses its file contents, and the dynamic table has no
# contents anywhere. Built with GNU binutils 2.46; the output is byte-identical between runs.
#
# fauxware_dynamic_filesz0 is tests/x86_64/fauxware with p_filesz cleared on PT_DYNAMIC alone. Its
# dynamic table is still in the file and still behind a PT_LOAD, so it still has to be read. cle has
# parsed the dynamic table out of the loaded image for objects like this since angr/cle#321, but no
# object in this repository is shaped that way and no toolchain here emits one.

.PHONY: all clean
TESTDIR=../../tests

all: $(TESTDIR)/x86_64/linked_list.debug $(TESTDIR)/x86_64/fauxware_dynamic_filesz0

clean:
	rm -f $(TESTDIR)/x86_64/linked_list.debug $(TESTDIR)/x86_64/fauxware_dynamic_filesz0

$(TESTDIR)/x86_64/linked_list.debug: $(TESTDIR)/x86_64/linked_list
	objcopy --only-keep-debug $< $@

$(TESTDIR)/x86_64/fauxware_dynamic_filesz0: $(TESTDIR)/x86_64/fauxware ./zero_dynamic_filesz.py
	python3 ./zero_dynamic_filesz.py $< $@
