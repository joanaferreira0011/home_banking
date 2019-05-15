all:
	+$(MAKE) -C server_src

.PHONY: all clean

clean:
	+$(MAKE) -C server_src clean

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY: clean depend
