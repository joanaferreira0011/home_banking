all:
	+$(MAKE) -C server_src
	+$(MAKE) -C user_src

.PHONY: all clean

clean:
	+$(MAKE) -C server_src clean
	+$(MAKE) -C user_src clean

.PHONY: clean depend
