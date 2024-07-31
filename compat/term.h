/*
 * ansi display driver backend
 *
 * This file emulates the termcap/terminfo/ncurses backend with simple
 * ANSI escape sequences.  Since terminfo is the de facto default in
 * all other Mg clones, this file hooks into that and emulates that
 * interfaces to the best of its abilities.
 */

/* This file is in the public domain. */

#ifndef ANSI_H
#define ANSI_H
#if !defined(NCURSES_TERM_H_incl) || !defined(NCURSES_VERSION)

typedef struct {
	int   t_nrow;		/* Number of rows.           */
	int   t_ncol;		/* Number of columns         */
	int   t_num[10];	/* Terminal constants        */
	char *t_str[100];	/* Terminal control strings  */
	int   t_fd;		/* filedes from setupterm()  */
} TERMINAL;

extern TERMINAL *cur_term;

#ifndef CUR
#define CUR cur_term->
#endif

//extern int columns;
//extern int lines;
#ifndef columns
#define columns              cur_term->t_ncol
#endif

#ifndef lines
#define lines                cur_term->t_nrow
#endif

#ifndef change_scroll_region
#define change_scroll_region NULL
#endif

#ifndef magic_cookie_glitch
#define magic_cookie_glitch  cur_term->t_num[4]
#endif

#ifndef bell
#define bell                 cur_term->t_str[1]
#endif

#ifndef key_down
#define key_down             cur_term->t_str[11]
#endif

#ifndef key_home
#define key_home             cur_term->t_str[12]
#endif

#ifndef key_left
#define key_left             cur_term->t_str[14]
#endif

#ifndef key_right
#define key_right            cur_term->t_str[17]
#endif

#ifndef key_up
#define key_up               cur_term->t_str[19]
#endif

#ifndef key_ppage
#define key_ppage            cur_term->t_str[50]
#endif

#ifndef key_npage
#define key_npage            cur_term->t_str[51]
#endif

#ifndef key_beg
#define key_beg              NULL
#endif

#ifndef key_end
#define key_end              cur_term->t_str[52]
#endif

#ifndef key_ic
#define key_ic               cur_term->t_str[53]
#endif

#ifndef key_dc
#define key_dc               cur_term->t_str[54]
#endif

#ifndef scroll_reverse
#define scroll_reverse       cur_term->t_str[11]
#endif

#ifndef scroll_forward
#define scroll_forward       cur_term->t_str[10]
#endif

#ifndef keypad_local
#define keypad_local         NULL
#endif

#ifndef keypad_xmit
#define keypad_xmit          NULL
#endif

#ifndef parm_delete_line
#define parm_delete_line     cur_term->t_str[46]
#endif

#ifndef parm_insert_line
#define parm_insert_line     cur_term->t_str[47]
#endif

#ifndef parm_down_cursor
#define parm_down_cursor     cur_term->t_str[48]
#endif

#ifndef insert_line
#define insert_line          cur_term->t_str[23]
#endif

#ifndef delete_line
#define delete_line          cur_term->t_str[22]
#endif

#ifndef clr_eol
#define clr_eol              cur_term->t_str[6]
#endif

#ifndef clr_eos
#define clr_eos              cur_term->t_str[7]
#endif

#ifndef cursor_up
#define cursor_up            cur_term->t_str[19]
#endif

#ifndef cursor_address
#define cursor_address       cur_term->t_str[10]
#endif

#ifndef enter_ca_mode
#define enter_ca_mode        ""
#endif

#ifndef exit_ca_mode
#define exit_ca_mode         ""
#endif

#ifndef enter_standout_mode
#define enter_standout_mode  cur_term->t_str[35]
#endif

#ifndef exit_standout_mode
#define exit_standout_mode   cur_term->t_str[43]
#endif


int   setupterm(const char *term, int filedes, int *errret);
char *tgoto(const char *cap, int col, int row);
int   tputs(const char *str, int affcnt, int (*putc)(int));


#endif
#endif /* ANSI_H */
