.PHONY: all clean
TESTDIR=../../tests
all: $(TESTDIR)/x86_64/test_ioctl $(TESTDIR)/mips/test_ioctl $(TESTDIR)/mipsel/test_ioctl $(TESTDIR)/mips64/test_ioctl $(TESTDIR)/ppc/test_ioctl $(TESTDIR)/ppc64/test_ioctl

clean:
	rm -rf $(TESTDIR)/*/test_ioctl

$(TESTDIR)/x86_64/test_ioctl: ./test_ioctl.c
	gcc -o $@ $<
$(TESTDIR)/mips/test_ioctl: ./test_ioctl.c
	mips-linux-gnu-gcc -o $@ $<
$(TESTDIR)/mipsel/test_ioctl: ./test_ioctl.c
	mipsel-linux-gnu-gcc -o $@ $<
$(TESTDIR)/mips64/test_ioctl: ./test_ioctl.c
	mips64-linux-gnuabi64-gcc -o $@ $<
$(TESTDIR)/ppc/test_ioctl: ./test_ioctl.c
	powerpc-linux-gnu-gcc -o $@ $<
$(TESTDIR)/ppc64/test_ioctl: ./test_ioctl.c
	powerpc64-linux-gnu-gcc -o $@ $<
