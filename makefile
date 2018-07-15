$(VERBOSE).SILENT:

SRCDIR = src/
OBJDIR = obj/
BINDIR = bin/
BINNAME = termsweeper
GCCSUFFIX =
COMPILER = g++

SRCFILES = $(wildcard $(SRCDIR)*.cpp)
OBJFILES = $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCFILES))
BINFILE = $(BINDIR)$(BINNAME)
EXOBJECTS = $(wildcard $(OBJDIR)*.o)

default: all

help:
	echo "Makefile for Termsweeper"
	echo "by LukaCPP 2018"
	echo "====================================="
	echo "make all     - compile everything"
	echo "make dirs    - create necessary directories"
	echo "make clean   - remove object files"
	echo "make re      - recompile everything"
	echo "               (-> delete objects -> compile all)"
	echo "make install - install compiled binary to /usr/bin"
	echo "               (requires superuser!)"

all : $(BINFILE)

$(BINFILE) : $(OBJFILES)
	echo "Make: Linking binary"
	$(COMPILER) -o $(BINFILE) $(OBJFILES) $(GCCSUFFIX)
	echo "Make: Build finished!"

re : clean all

obj/%.o : src/%.cpp
	echo "Make: Compiling $<"
	$(COMPILER) -c $< -o $@ $(GCCSUFFIX)

dirs:
	mkdir obj
	mkdir bin
	echo "Make: Created bin and obj directories"

clean:
	rm -f $(EXOBJECTS)
	rm -f $(BINFILE)
	echo "Make: removed $(EXOBJECTS) $(BINFILE)"
