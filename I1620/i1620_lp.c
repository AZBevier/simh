/* i1620_lp.c: IBM 1443 line printer simulator

   Copyright (c) 2002, Robert M. Supnik

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   ROBERT M SUPNIK BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   Except as contained in this notice, the name of Robert M Supnik shall not
   be used in advertising or otherwise to promote the sale, use or other dealings
   in this Software without prior written authorization from Robert M Supnik.

   lpt		1443 line printer
*/

#include "i1620_defs.h"

#define LPT_BSIZE	197				/* buffer size */

#define K_IMM		0x10				/* control now */
#define K_LIN		0x20				/* spc lines */
#define K_CH10		0x40				/* chan 10 */
#define K_LCNT		0x03				/* line count */
#define K_CHAN		0x0F				/* channel */

extern uint8 M[MAXMEMSIZE];
extern uint8 ind[NUM_IND];
extern UNIT cpu_unit;
extern uint32 io_stop;

uint32 cct[CCT_LNT] = { 03 };				/* car ctrl tape */
int32 cct_lnt = 66, cct_ptr = 0;			/* cct len, ptr */
int32 lpt_bptr = 0;					/* lpt buf ptr */
char lpt_buf[LPT_BSIZE + 1];				/* lpt buf */
int32 lpt_savctrl = 0;					/* saved spc ctrl */

t_stat lpt_svc (UNIT *uptr);
t_stat lpt_reset (DEVICE *dptr);
t_stat lpt_attach (UNIT *uptr, char *cptr);
void lpt_buf_init (void);
t_stat lpt_num (uint32 pa, uint32 len, uint32 f1);
t_stat lpt_print (void);
t_stat lpt_space (int32 lines, int32 lflag);

#define CHP(ch,val)	((val) & (1 << (ch)))

/* LPT data structures

   lpt_dev	LPT device descriptor
   lpt_unit	LPT unit descriptor
   lpt_reg	LPT register list
*/

UNIT lpt_unit = {
	UDATA (&lpt_svc, UNIT_SEQ+UNIT_ATTABLE, 50) };

REG lpt_reg[] = {
	{ BRDATA (LBUF, lpt_buf, 8, 8, LPT_BSIZE + 1) },
	{ DRDATA (BPTR, lpt_bptr, 8) },
	{ HRDATA (PCTL, lpt_savctrl, 8) },
	{ FLDATA (PRCHK, ind[IN_PRCHK], 0) },
	{ FLDATA (PRCH9, ind[IN_PRCH9], 0) },
	{ FLDATA (PRCH12, ind[IN_PRCH12], 0) },
	{ FLDATA (PRBSY, ind[IN_PRBSY], 0) },
	{ DRDATA (POS, lpt_unit.pos, 32), PV_LEFT },
	{ BRDATA (CCT, cct, 8, 32, CCT_LNT) },
	{ DRDATA (CCTP, cct_ptr, 8), PV_LEFT },
	{ DRDATA (CCTL, cct_lnt, 8), REG_RO + PV_LEFT },
	{ NULL }  };

DEVICE lpt_dev = {
	"LPT", &lpt_unit, lpt_reg, NULL,
	1, 10, 31, 1, 8, 7,
	NULL, NULL, &lpt_reset,
	NULL, &lpt_attach, NULL };

/* Data tables */

/* Numeric (flag plus digit) to lineprinter (ASCII) */

const char num_to_lpt[32] = {
 '0', '1', '2', '3', '4', '5', '6', '7',
 '8', '9', '\'', ' ', '@', ':', ' ', 'G',
 '-', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
 'Q', 'R', 'W', ' ', '*', ' ',  -1, 'X' };

/* Alphameric (digit pair) to lineprinter (ASCII) */

const char alp_to_lpt[256] = {
 ' ',  -1, '?', '.', ')',  -1,  -1,  -1,		/* 00 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
 '+',  -1, '!', '$', '*', ' ',  -1,  -1,		/* 10 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
 '-', '/', '\'', ',', '(', -1,  -1,  -1,		/* 20 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1, '0', '=', '@', ':',  -1,  -1,		/* 30 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, 'A', 'B', 'C', 'D', 'E', 'F', 'G',		/* 40 */
 'H', 'I',  -1,  -1,  -1,  -1,  -1,  -1,
 '-', 'J', 'K', 'L', 'M', 'N', 'O', 'P',		/* 50 */
 'Q', 'R',  -1,  -1,  -1,  -1,  -1,  -1,
  -1, '/', 'S', 'T', 'U', 'V', 'W', 'X',		/* 60 */
 'Y', 'Z',  -1,  -1,  -1,  -1,  -1,  -1,
 '0', '1', '2', '3', '4', '5', '6', '7',		/* 70 */
 '8', '9',  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* 80 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* 90 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* A0 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* B0 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* C0 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* D0 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* E0 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,		/* F0 */
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 };

/* Line printer IO routine
 
   - Hard errors halt the system.
   - Invalid characters print a blank, set the WRCHK and PRCHK
     flags, and halt the system if IO stop is set.
*/

t_stat lpt (uint32 op, uint32 pa, uint32 f0, uint32 f1)
{
int8 lpc;
uint8 z, d;
t_stat r, inv = SCPE_OK;

sim_cancel (&lpt_unit);					/* "stall" until */
ind[IN_PRBSY] = 0;					/* printer free */

switch (op) {						/* decode op */
case OP_K:						/* control */
	lpt_savctrl = (f0 << 4) | f1;			/* form ctrl */
	if (lpt_savctrl & K_IMM) return lpt_print ();	/* immediate? */
	break;
case OP_DN:
	return lpt_num (pa, 20000 - (pa % 20000), f1);	/* dump numeric */
case OP_WN:
	return lpt_num (pa, 0, f1);			/* write numeric */
case OP_WA:
	for ( ; lpt_bptr < LPT_BSIZE; lpt_bptr++) {	/* only fill buf */
		d = M[pa] & DIGIT;			/* get digit */
		z = M[pa - 1] & DIGIT;			/* get zone */
		if ((d & REC_MARK) == REC_MARK) break;	/* 8-2 char? */
		lpc = alp_to_lpt[(z << 4) | d];		/* translate pair */
		if (lpc < 0) {				/* bad char? */
			ind[IN_WRCHK] = ind[IN_PRCHK] = 1;	/* wr chk */
			inv = STOP_INVCHR;  }		/* set return status */
		lpt_buf[lpt_bptr] = lpc & 0x7F;		/* fill buffer */
		pa = ADDR_A (pa, 2);  }			/* incr mem addr */
	if ((f1 & 1) == 0) {		;		/* print now? */
		r = lpt_print ();			/* print line */
		if (r != SCPE_OK) return r;  }
	CRETIOE (io_stop, inv);
default:						/* invalid function */
	return STOP_INVFNC;  }
return SCPE_OK;
}

/* Print numeric */

t_stat lpt_num (uint32 pa, uint32 len, uint32 f1)
{
uint32 end;
uint8 d;
int8 lpc;
t_stat r, inv = SCPE_OK;

end = pa + len;
for ( ; lpt_bptr < LPT_BSIZE; lpt_bptr++) {		/* only fill buf */
	d = M[pa];					/* get digit */
	if (len? (pa >= end):				/* end reached? */
		((d & REC_MARK) == REC_MARK)) break;
	lpc = num_to_lpt[d];				/* translate */
	if (lpc < 0) {					/* bad char? */
		ind[IN_WRCHK] = ind[IN_PRCHK] = 1;	/* wr chk */
		inv = STOP_INVCHR;  }			/* set return status */
	lpt_buf[lpt_bptr++] = lpc & 0x7F;		/* fill buffer */
	PP (pa);  }					/* incr mem addr */
if ((f1 & 1) == 0) {					/* print now? */
	r = lpt_print ();				/* print line */
	if (r != SCPE_OK) return r;  }
CRETIOE (io_stop, inv);
}

/* Print and space */

t_stat lpt_print (void)
{
int32 i, chan, ctrl = lpt_savctrl;

if ((lpt_unit.flags & UNIT_ATT) == 0) {			/* not attached? */
	ind[IN_PRCHK] = ind[IN_WRCHK] = 1;		/* wr, pri check */
	return SCPE_UNATT;  }

ind[IN_PRBSY] = 1;					/* print busy */
sim_activate (&lpt_unit, lpt_unit.time);		/* start timer */

for (i = LPT_WIDTH; i <= LPT_BSIZE; i++)		/* clear unprintable */
	lpt_buf[i] = ' ';
while ((lpt_bptr > 0) && (lpt_buf[lpt_bptr - 1] == ' '))
	lpt_buf[--lpt_bptr] = 0;			/* trim buffer */
if (lpt_bptr) {						/* any line? */
	fputs (lpt_buf, lpt_unit.fileref);		/* print */
	lpt_unit.pos = ftell (lpt_unit.fileref);	/* update pos */
	lpt_buf_init ();				/* reinit buf */
	if (ferror (lpt_unit.fileref)) {		/* error? */
		ind[IN_PRCHK] = ind[IN_WRCHK] = 1;	/* wr, pri check */
		perror ("LPT I/O error");
		clearerr (lpt_unit.fileref);
		return SCPE_IOERR;  }  }

lpt_savctrl = 0x61;					/* reset ctrl */
if ((ctrl & K_LIN) == ((ctrl & K_IMM)? 0: K_LIN))	/* space lines? */
	return lpt_space (ctrl & K_LCNT, FALSE);
chan = lpt_savctrl & K_CHAN;				/* basic chan */
if (lpt_savctrl & K_CH10) {				/* chan 10-12? */
	if (chan == 0) chan = 10;
	else if (chan == 3) chan = 11;
	else if (chan == 4) chan = 12;
	else chan = 0;  }
if ((chan == 0) || (chan > 12)) return STOP_INVFNC;
for (i = 1; i < cct_lnt + 1; i++) {			/* sweep thru cct */
	if (CHP (chan, cct[(cct_ptr + i) % cct_lnt]))
		return lpt_space (i, TRUE);  }
return STOP_CCT;					/* runaway channel */
}

/* Space routine - space or skip n lines
   
   Inputs:
	count	=	number of lines to space or skip
	sflag	=	skip (TRUE) or space (FALSE)
*/

t_stat lpt_space (int32 count, int32 sflag)
{
int32 i;

cct_ptr = (cct_ptr + count) % cct_lnt;			/* adv cct, mod lnt */
if (sflag && CHP (0, cct[cct_ptr]))			/* skip, top of form? */
	fputs ("\n\f", lpt_unit.fileref);		/* nl, ff */
else {	for (i = 0; i < count; i++)			/* count lines */
	fputc ('\n', lpt_unit.fileref);  }
lpt_unit.pos = ftell (lpt_unit.fileref);		/* update position */
ind[IN_PRCH9] = CHP (9, cct[cct_ptr]) != 0;		/* set indicators */
ind[IN_PRCH12] = CHP (12, cct[cct_ptr]) != 0;
if (ferror (lpt_unit.fileref)) {			/* error? */
	ind[IN_PRCHK] = ind[IN_WRCHK] = 1;		/* wr, pri check */
	perror ("LPT I/O error");
	clearerr (lpt_unit.fileref);
	return SCPE_IOERR;  }
return SCPE_OK;
}

/* Unit service - clear printer busy */

t_stat lpt_svc (UNIT *uptr)
{
ind[IN_PRBSY] = 0;
return SCPE_OK;
}

/* Initialize lpt buffer */

void lpt_buf_init (void)
{
int32 i;

lpt_bptr = 0;
for (i = 0; i < LPT_WIDTH + 1; i++) lpt_buf[i] = 0;
return;
}

/* Reset routine */

t_stat lpt_reset (DEVICE *dptr)
{
lpt_buf_init ();					/* clear buffer */
cct_ptr = 0;						/* clear cct ptr */
lpt_savctrl = 0x61;					/* clear cct action */
ind[IN_PRCHK] = ind[IN_PRBSY] = 0;			/* clear indicators */
ind[IN_PRCH9] = ind[IN_PRCH12] = 0;
return SCPE_OK;
}

/* Attach routine */

t_stat lpt_attach (UNIT *uptr, char *cptr)
{
lpt_reset (&lpt_dev);
return attach_unit (uptr, cptr);
}
