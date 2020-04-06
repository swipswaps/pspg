/*-------------------------------------------------------------------------
 *
 * pspg.h
 *	  themes initialization
 *
 * Portions Copyright (c) 2017-2020 Pavel Stehule
 *
 * IDENTIFICATION
 *	  src/pspg.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef PSPG_PSPG_H
#define PSPG_PSPG_H

#include "config.h"
#include "themes.h"
#include "st_menu.h"

#define LINEINFO_NONE				0
#define LINEINFO_BOOKMARK			1
#define LINEINFO_FOUNDSTR			2
#define LINEINFO_FOUNDSTR_MULTI		4
#define LINEINFO_UNKNOWN			8
#define LINEINFO_CONTINUATION		16

#define MAX_STYLE					20

typedef struct LineInfo
{
	char			mask;
	short int		start_char;
} LineInfo;

typedef struct LineBuffer
{
	int		first_row;
	int		nrows;
	char   *rows[1000];
	LineInfo	   *lineinfo;
	struct LineBuffer *next;
	struct LineBuffer *prev;
} LineBuffer;

typedef struct
{
	LineBuffer	   *lnb;
	int				lnb_row;
} MappedLine;

typedef enum
{
	INFO_UNKNOWN,
	INFO_DOUBLE,
	INFO_STRXFRM
} SortDataInfo;

typedef struct
{
	SortDataInfo		info;
	double			d;
	char		   *strxfrm;
	LineBuffer	   *lnb;
	int				lnb_row;
} SortData;

/*
 * Column range
 */
typedef struct
{
	int		xmin;
	int		xmax;
	int		name_pos;				/* column's name position */
	int		name_size;				/* size of column name in bytes */
} CRange;

/*
 * Available formats of headline chars
 *
 *  L, R   .. outer border
 *  I      .. inner border
 *  d      .. data
 */

/*
 * This structure should be immutable
 */
typedef struct
{
	int		border_top_row;			/* nrow of bottom outer border or -1 */
	int		border_head_row;		/* nrow of head outer (required) */
	int		border_bottom_row;		/* nrow of bottom outer border or -1 */
	int		border_type;			/* detected type of border: 0, 1, 2 */
	char	linestyle;				/* detected linestyle: a, u */
	bool	is_expanded_mode;		/* true when data are in expanded mode */
	bool	is_pgcli_fmt;			/* table was generated by pgcli */
	int		expanded_info_minx;		/* begin of info in \x mode .. RECORD x */
	char	title[65];				/* detected title (trimmed) or NULL */
	int		title_rows;				/* number of rows used as table title (skipped later) */
	char	filename[65];			/* filename (printed on top bar) */
	LineBuffer rows;				/* list of rows buffers */
	int		total_rows;				/* number of input rows */
	MappedLine   *order_map;		/* maps sorted lines to original lines */
	int		maxy;					/* maxy of used pad area with data */
	int		maxx;					/* maxx of used pad area with data */
	int		maxbytes;				/* max length of line in bytes */
	char   *namesline;				/* header column's names line */
	char   *headline;				/* header separator line */
	int		headline_size;			/* size of headerline in bytes */
	char   *headline_transl;		/* translated headline */
	int		headline_char_size;		/* size of headerline in chars */
	CRange *cranges;				/* pairs of start, end of columns */
	int		columns;				/* number of columns */
	int		first_data_row;			/* fist data row line (starts by zero) */
	int		last_data_row;			/* last line of data row */
	int		footer_row;				/* nrow of first footer row or -1 */
	int		alt_footer_row;			/* alternative footer row (used when border = 1) */
	int		footer_char_size;		/* width of footer */
	int		last_row;				/* last not empty row */
	int		fixed_rows;				/* number of fixed rows */
	int		fixed_columns;			/* number of fixed columns */
	int		data_rows;				/* number of data rows */
	int		footer_rows;			/* number of footer rows */
	bool	oid_name_table;			/* detected system table with first oid column */
	bool	multilines_already_tested;	/* true, when we know where are multilines */
} DataDesc;

#define		PSPG_WINDOW_COUNT		9

/*
 * This structure can be mutable - depends on displayed data
 */
typedef struct
{
	int		fix_rows_rows;			/* current number of fixed rows in window rows */
	int		fix_cols_cols;			/* current number of fixed columns in window rows */
	int		rows_rows;				/* current number of data rows */
	int		footer_rows;			/* current number of footer rows */
	int		maxy;					/* max y size of screen */
	int		maxx;					/* max x size of screen */
	int		main_maxy;				/* max y size of main place (fixed + data + footer rows) */
	int		main_maxx;				/* max x size of main place (maxx - main_start_x) */
	int		main_start_y;			/* y position of first row of main place */
	int		main_start_x;			/* x position of first row of main place */
	int		top_bar_rows;			/* 1 or 0 when top bar is not used */
	WINDOW	*wins[PSPG_WINDOW_COUNT];
	Theme	themes[PSPG_WINDOW_COUNT];
	char	searchterm[256];		/* currently active search input */
	int		searchterm_char_size;	/* size of searchterm in chars */
	int		searchterm_size;		/* size of searchterm in bytes */
	bool	has_upperchr;			/* true, when search term has upper char */
	bool	found;					/* true, when last search was successfull */
	int		found_start_x;			/* x position of found pattern */
	int		found_start_bytes;		/* bytes position of found pattern */
	int		found_row;				/* row of found pattern */
	int		first_rec_title_y;		/* y of first displayed record title in expanded mode */
	int		last_rec_title_y;		/* y of last displayed record title in expanded mode */
	char	searchcolterm[256];		/* last searched column patterm */
	int		searchcolterm_size;		/* length of searched column pattern in bytes */
	char   *fmt;					/* format string for info when refresh first is required */
	char   *par;					/* parameter for info when refresh first is required */
	bool	beep;					/* beep for info when refresh is required */
	bool	applytimeout;			/* true, when saved info should be close after timeout */
	bool	is_error;				/* true, when saved info should be displayed as error */
	bool	refresh_scr;			/* force rewrite screen */
} ScrDesc;

#define		w_luc(scrdesc)			((scrdesc)->wins[WINDOW_LUC])
#define		w_fix_rows(scrdesc)		((scrdesc)->wins[WINDOW_FIX_ROWS])
#define		w_fix_cols(scrdesc)		((scrdesc)->wins[WINDOW_FIX_COLS])
#define		w_rows(scrdesc)			((scrdesc)->wins[WINDOW_ROWS])
#define		w_footer(scrdesc)		((scrdesc)->wins[WINDOW_FOOTER])
#define		w_top_bar(scrdesc)		((scrdesc)->wins[WINDOW_TOP_BAR])
#define		w_bottom_bar(scrdesc)	((scrdesc)->wins[WINDOW_BOTTOM_BAR])
#define		w_rownum(scrdesc)		((scrdesc)->wins[WINDOW_ROWNUM])
#define		w_rownum_luc(scrdesc)	((scrdesc)->wins[WINDOW_ROWNUM_LUC])

/*
 * Used for storing not yet formatted data
 */
typedef struct
{
	int		nfields;
	char   *fields[];
} RowType;

typedef struct _rowBucketType
{
	int			nrows;
	RowType	   *rows[1000];
	bool		multilines[1000];
	bool		allocated;
	struct _rowBucketType *next_bucket;
} RowBucketType;

/*
 * Used for formatting
 */
typedef struct
{
	int		nfields;
	bool	has_header;
	char	types[1024];			/* a or d .. content in column */
	int		widths[1024];			/* column's display width */
	bool	multilines[1024];		/* true if column has multiline row */
} PrintDataDesc;

/* from print.c */
extern void window_fill(int window_identifier, int srcy, int srcx, int cursor_row, int vcursor_xmin, int vcursor_xmax, DataDesc *desc, ScrDesc *scrdesc, Options *opts);
extern void draw_data(Options *opts, ScrDesc *scrdesc, DataDesc *desc, int first_data_row, int first_row, int cursor_col, int footer_cursor_col, int fix_rows_offset);

/* from pspg.c */
extern void leave_ncurses(const char *str);
extern void leave_ncurses2(const char *fmt, const char *str);
extern void log_writeln(const char *str);

extern bool is_expanded_header(Options *opts, char *str, int *ei_minx, int *ei_maxx);
extern int min_int(int a, int b);
extern const char *nstrstr(const char *haystack, const char *needle);
extern const char *nstrstr_ignore_lower_case(const char *haystack, const char *needle);

extern const char *pspg_search(Options *opts, ScrDesc *scrdesc, const char *str);

/* from menu.c */
extern void init_menu_config(Options *opts);
extern struct ST_MENU *init_menu(struct ST_MENU *current_menu);
extern struct ST_CMDBAR *init_cmdbar(struct ST_CMDBAR *current_cmdbar, Options *opts);
extern void post_menu(Options *opts, struct ST_MENU *current_menu);

/* from sort.c */
extern void sort_column_num(SortData *sortbuf, int rows, bool desc);
extern void sort_column_text(SortData *sortbuf, int rows, bool desc);

/* from pretty-csv.c */
extern bool read_and_format(FILE *fp, Options *opts, DataDesc *desc, const char **err);

/* from pgclient.c */
extern bool pg_exec_query(Options *opts, RowBucketType *rb, PrintDataDesc *pdesc, const char **err);

/*
 * REMOVE THIS COMMENT FOR DEBUG OUTPUT
 * and modify a path.
 *
#define DEBUG_PIPE				"/home/pavel/debug"
 */

#define DEBUG_PIPE				"/home/pavel/debug"


#ifdef DEBUG_PIPE

extern FILE *debug_pipe;
extern int	debug_eventno;

#endif

#endif
