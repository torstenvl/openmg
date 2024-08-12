# 27-28 January: mg in ANSI mode garbles modeline

## Exploration #1

When we resize a window, the modeline comes back the way it should be.
Let's trace that and see what's going on. 

The window change handler just changes a flag

    static void
    winchhandler(int sig)
    {
    	winch_flag = 1;
    }

Grepping for that flag leads us to its use in `ttgetc()` (teletype get
character -- appears to be the inner-most inner loop)

	do {
		ret = read(STDIN_FILENO, &c, 1);
		if (ret == -1 && errno == EINTR) {
			if (winch_flag) {
				redraw(0, 0);
				winch_flag = 0;
			}
		}
        // other code here ...
	} while (1);

`redraw()` just calls `do_redraw()` without a forcing flag


    int
    redraw(int f, int n)
    {
    	return (do_redraw(f, n, FALSE));
    }


`do_redraw()` doesn't actually redraw anything. It checks the window size,
and if it's changed, then sets a garble flag `sgarbf` to true and calls
`update(CMODE)`.  If it hasn't changed, then it just sets `sgarbf` to true
(why? not sure)

	if (nrow != oldnrow || ncol != oldncol || force) {
        // find top window pointer stack and recalc modeline bar location
        ...
		sgarbf = TRUE;
		update(CMODE);
	} else {
		sgarbf = TRUE;
    }

# 30 July 2024

I haven't touched this in half a year, and OpenBSD has a new upstream version.

Starting over, I did:

    git clone https://github.com/openbsd/src
    mv src openmg
    cd openmg
    git filter-repo --force --subdirectory-filter usr.bin/mg/

This is a great tool from https://github.com/newren/git-filter-repo that lets
you do nice filtering.  This got rid of all the other shit in src and usr.bin
and cleaned up the history and reflog, cutting out 90% of the size of the 
repo.

Then I copied my previous attempt's Makefile as GNUmakefile.

THE NEXT STEP IS KEY: Bring in ansiterm.h and remove the dependencies on
term.h or ncurses. 

SECOND STEP: Abstract out OS-specific shit into their own header files.

# 12 August 2024

We sometimes get a SEGFAULT if we don't replace all instances of term.h with
ansiterm.h.  Gotta be careful!

I think the best way to resolve this is to make a compat/ directory and put
that in our include search path with -I, and include our own ansiterm.h
as term.h there.  Then no source changes are required to use our own 
terminal routines. 

I also made a tomerge/ directory for things that are very important to merge
when updating from upstream.  One of them is removing the definition of 
STANDOUT_GLITCH, which resolves the modeline issues I was having before...
entirely. 