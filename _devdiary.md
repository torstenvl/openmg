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

# 13 August 2024

Looking at trying to add Unicode support, we are going to have to try to
overcome the assumption, woven throughout mg, that 1 byte = 1 character =
1 grapheme = 1 column. In UTF-8, often each character is multiple bytes, and
each grapheme is multiple characters. However, sometimes, one grapheme is
multiple columns.  For example, ﷽ is 3 columns in VS Code's built-in
terminal, but 12 columns in Terminal.app (perhaps due to font fallback settings
or something?).

    - After far too much time fucking around with it, it appears that
﷽ is rendered in Geeza Pro in Terminal.app and... I don't know what
inside of VS Code terminal.

Within `display.c`, the code that matters most for this is here:

```
    curcol = 0;
    i = 0;
    while (i < curwp->w_doto) {
    	c = lgetc(lp, i++);
    	if (c == '\t') {
    		curcol = ntabstop(curcol, curwp->w_bufp->b_tabw);
    	} else if (ISCTRL(c) != FALSE)
    		curcol += 2;
    	else if (isprint(c))
    		curcol++;
    	else {
    		char bf[5];
    		snprintf(bf, sizeof(bf), "\\%o", c);
    		curcol += strlen(bf);
    	}
    }
```

Based on this, I think I can find this pattern a lot of places, by searching
for nbtabstop. Will keep looking.

By the way, some important globals:
     extern struct mgwin	*curwp;    // Window pointer (current)
     extern struct buffer	*curbp;    // Buffer pointer (current)
     extern struct mgwin	*wheadp;   // Window head-pointer
     extern struct buffer	*bheadp;   // Buffer head-pointer

And... "dot" is a term of art within the source referring to the cursor. 

Now, looking for similar code to the above, it looks like we implement it
MULTIPLE times throughout mg.  We ought to refactor this.  In `util.c` we have
the following:

    int
    getcolpos(struct mgwin *wp)
    {
        int col, i, c;
        char tmp[5];
    
        /* determine column */
        col = 0;
    
        for (i = 0; i < wp->w_doto; ++i) {
            c = lgetc(wp->w_dotp, i);
            if (c == '\t') {
                col = ntabstop(col, wp->w_bufp->b_tabw);
            } else if (ISCTRL(c) != FALSE)
                col += 2;
            else if (isprint(c)) {
                col++;
            } else {
                col += snprintf(tmp, sizeof(tmp), "\\%o", c);
            }
    
        }
        return (col);
    }

This should probably look more like this (in order to accommodate the needs of
different calls). 

    int 
    getcolposexplicit(const struct buffer *bfr, const struct line *ln, int tgt) {
        int i = 0;
        int b = 0;
        int colpos = 0;
        char octbuf[5] = {0};
        char utf8buf[5] = {0};
        static const int utf8locale = (!!(!strcmp(".UTF-8", strrchr((setlocale(LC_ALL, ""), setlocale(LC_ALL, NULL)), '.'))));
    
        for (i = 0; i < tgt; i++) {
    
            b = lgetc(ln, i);
    
            if (b == '\t') 
                colpos = ntabstop(colpos, bfr->b_tabw);
    
            else if (ISCTRL(b)) 
                colpos = colpos + 2; /* strlen("^x") */
    
            else if (isprint(b)) 
                colpos++;
    
            else 
                colpos = colpos + snprintf(octbuf, sizeof(octbuf), "\\%o", b);
    
        }
    
        return colpos;
    }

