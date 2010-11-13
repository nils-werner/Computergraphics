# user-defined variables
EXERCISE = A3.3
LECTUREDIR = /proj/i9cg

# main-directories of the project
I9LIBDIR = /proj/i9lib/
BGSINCLUDE = $(I9LIBDIR)include/bgs/
EXERCISEDIR = $(LECTUREDIR)/bgs/$(EXERCISE)/skel/
LIBRARYDIR =  $(LECTUREDIR)/bgs/$(EXERCISE)/lib/
EXERCISEDATADIR = $(LECTUREDIR)/bgs/$(EXERCISE)/data/

STUDENTDIR   = $(shell pwd)/

# the compiler and lexer
CXX        = g++
LEX        = flex
MAKEDEPEND = g++ -MM $(CPPFLAGS)

# variable defines for targets dbg, memdbg, opt
ifeq "$(VARIANT)" "dbg" # debug defines
DEFINES  = -g -DTRACE
LDDEFINES =
endif
ifeq "$(VARIANT)" "opt" # release defines
DEFINES  = -DNDEBUG -O3
LDDEFINES =
endif
ifeq "$(VARIANT)" "prof" # profile defines
DEFINES  = -DNDEBUG -O3
LDDEFINES = -pg
endif
ifeq "$(VARIANT)" "efence" # electric-fence defines
DEFINES  = 
LDDEFINES = -lefence
endif

DEFINES += -Wall

# the common libraries to link with
LDLIBRARIES = -L/usr/X11R6/lib -L$(LIBRARYDIR) -L$(I9LIBDIR)lib -lbgs -lglut -lGL -lGLU -lXi -lXmu -lXext -lX11 -lm -lpthread

# compile- and linker-flags
CPPFLAGS = $(DEFINES) -I $(BGSINCLUDE) -I $(I9LIBDIR)include 
LDFLAGS  = $(LDLIBRARIES) $(LDDEFINES)

# the main-program is named the same as the exercise
MAIN = runme

$(MAIN) : $(EXERCISE).o
	$(CXX) -o $(MAIN) $(EXERCISE).o $(LDFLAGS)

$(MAIN).o : $(MAIN).C
	$(CXX) $(CPPFLAGS) -c $(MAIN).C

# automatically created variables:

.PHONY : all
all : $(MAINS)

dbg :
	$(MAKE) VARIANT=dbg

opt :
	$(MAKE) VARIANT=opt

prof :
	$(MAKE) VARIANT=prof

efence :
	$(MAKE) VARIANT=efence


# clear all intermediate files
.PHONY : clean
clean:
	@rm -f $(MAIN)
	@rm -f $(EXERCISE).o

