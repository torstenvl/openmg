EXE = mg

HDR = *.h

SRC = *.c



CC = cc

RFLAGS = -std=c17 -Oz

# For Clang/LLVM use the following
DFLAGS = -std=c17 -O0 -gfull

# For GCC use the following instead
# DFLAGS = -std=c17 -Og -g3
  
SFLAGS = -W -Wall -Werror -Wno-unused-parameter -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Wreturn-type -Wcast-qual -Wswitch -Wshadow -Wcast-align -Wwrite-strings -Wmisleading-indentation

SSFLAGS = -Wunused-parameter -Wchar-subscripts -Winline -Wnested-externs -Wredundant-decls



all: release



release: $(SRC) $(HDR)
	$(CC) $(RFLAGS) $(SRC) -o $(EXE)
	strip $(EXE)

debug: $(SRC) $(HDR)
	$(CC) $(DFLAGS) $(SRC) -o $(EXE)



strict: $(SRC) $(HDR)
	$(CC) $(SFLAGS) $(RFLAGS) $(SRC) -o $(EXE)
	strip $(EXE)

strict-debug: $(SRC) $(HDR)
	$(CC) $(SFLAGS) $(DFLAGS) $(SRC) -o $(EXE)



superstrict: $(SRC) $(HDR)
	$(CC) $(SFLAGS) $(SSFLAGS) $(RFLAGS) $(SRC) -o $(EXE)
	strip $(EXE)

superstrict-debug: $(SRC) $(HDR)
	$(CC) $(SFLAGS) $(SSFLAGS) $(DFLAGS) $(SRC) -o $(EXE)



clean:
	rm -Rf core *.o *~ $(EXE) $(EXE).dSYM/
