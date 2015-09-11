/****************************************************************************
*   @(#)spec_server.h	6.2  11/04/13 CSS
*
*   "spec" Release 6
*
*   Copyright (c) 2002,2003,2004,2013
*   by Certified Scientific Software.
*   All rights reserved.
*   Copyrighted as an unpublished work.
*
****************************************************************************/

/* default port range for server */
#define SV_PORT_FIRST   6510
#define SV_PORT_LAST    6530

/* codes for svr_head.cmd */
#define SV_CLOSE                1
#define SV_ABORT                2
#define SV_CMD                  3
#define SV_CMD_WITH_RETURN      4
#define SV_RETURN               5
#define SV_REGISTER             6
#define SV_UNREGISTER           7
#define SV_EVENT                8
#define SV_FUNC                 9
#define SV_FUNC_WITH_RETURN     10
#define SV_CHAN_READ            11
#define SV_CHAN_SEND            12
#define SV_REPLY                13
#define SV_HELLO                14
#define SV_HELLO_REPLY          15

/* bits for svr_head.flags */
#define SV_DELETED              0x0001

#define SV_SPEC_MAGIC   0xFEEDFACE

#define SV_NAME_LEN     80

#define SV_VERSION      4       /* version number of svr_head */

#define SV_VER2_SIZ     44      /* first part of vers 2 header (11 integers) */
#define SV_VER3_SIZ     48      /* first part of vers 3 header (12 integers) */
#define SV_VER4_SIZ     52      /* first part of vers 4 header (13 integers) */

#if SV_VERSION == 2
# define SV_CURR_SIZ     SV_VER2_SIZ
#endif
#if SV_VERSION == 3
# define SV_CURR_SIZ     SV_VER3_SIZ
#endif
#if SV_VERSION == 4
# define SV_CURR_SIZ     SV_VER4_SIZ
#endif

struct  svr_head {
	unsigned magic; /*  1 SV_SPEC_MAGIC */
	int     vers;   /*  2 version number of this structure */
	unsigned size;  /*  3 size of this structure */
	unsigned sn;    /*  4 serial number (client's choice) */
	unsigned sec;   /*  5 time of day when sent (seconds) */
	unsigned usec;  /*  6 time of day when sent (microseconds) */
	int     cmd;    /*  7 command code */
	int     type;   /*  8 type of data:  integer, float, string, etc */
	unsigned rows;  /*  9 number of rows if array data */
	unsigned cols;  /* 10 number of cols if array data */
	unsigned len;   /* 11 bytes of data that follow */
#if SV_VERSION >= 3
	int     err;    /* 12 error code */
#endif
#if SV_VERSION >= 4
	int     flags;  /* 13 flags */
#endif
	char    name[SV_NAME_LEN];      /* name of property */
};

/* types for svr_head.type */
#define SV_DOUBLE       1
#define SV_STRING       2
#define SV_ERROR        3
#define SV_ASSOC        4
#define SV_ARR_DOUBLE   5
#define SV_ARR_FLOAT    6
#define SV_ARR_LONG     7
#define SV_ARR_ULONG    8
#define SV_ARR_SHORT    9
#define SV_ARR_USHORT   10
#define SV_ARR_CHAR     11
#define SV_ARR_UCHAR    12
#define SV_ARR_STRING   13
#define SV_ARR_LONG64   14
#define SV_ARR_ULONG64  15
