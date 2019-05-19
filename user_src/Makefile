# Project properties
PROGRAM = ../user

#source options
C_FILES := $(wildcard *.c)

#build options
BUILDDIR = build
OBJS := $(patsubst %.c, $(BUILDDIR)/%.o, $(C_FILES))
OBJS += log.o

#compiler options
CFLAGS =  -pedantic -D_REENTRANT -pthread -Wall
LDFLAGS =
LDLIBS =

all: $(PROGRAM)

$(PROGRAM): .depend $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM) $(LDLIBS)

# Dependency management

depend: .depend

.depend: cmd = gcc -MM -MF depend $(var); echo -n "$(BUILDDIR)/" >> .depend; cat depend >> .depend;
.depend:
	@echo "Generating dependencies..."
	@$(foreach var, $(C_FILES), $(cmd))
	@rm -f depend

-include .depend

$(BUILDDIR)/%.o: %.c $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -f .depend $(PROGRAM)
	rm -rf $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY: clean depend
