/*****************************************************************************

 @(#) src/include/sys/termios.h

 -----------------------------------------------------------------------------

 Copyright (c) 2008-2015  Monavacon Limited <http://www.monavacon.com/>
 Copyright (c) 2001-2008  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2001  Brian F. G. Bidulock <bidulock@openss7.org>

 All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Affero General Public License as published by the Free
 Software Foundation; version 3 of the License.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
 details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>, or
 write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 *****************************************************************************/

#ifndef __SYS_TERMIOS_H__
#define __SYS_TERMIOS_H__

#define _POSIX_VDISABLE	    0

#define CTRL(c)	    ((c)&037)

#ifdef IBSHIFT
/* XXX: strangely new asm/termbits.h defines IBSHIFT as 8 */
#define O_IBSHIFT   16
#else
#define IBSHIFT	    16
#endif

#define NCC	    8
#define NCCS	    19

#ifndef _LINUX_TERMIOS_H
typedef unsigned long tcflag_t;
typedef unsigned char cc_t;
typedef unsigned long speed_t;

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};
#else				/* _LINUX_TERMIOS_H */
typedef unsigned long o_tcflag_t;
typedef unsigned char o_cc_t;
typedef unsigned long o_speed_t;

struct o_termios {
	o_tcflag_t c_iflag;
	o_tcflag_t c_oflag;
	o_tcflag_t c_cflag;
	o_tcflag_t c_lflag;
	o_cc_t c_cc[NCCS];
};
#endif				/* _LINUX_TERMIOS_H */

#ifndef VINTR
#define VINTR		0
#endif
#ifndef VQUIT
#define VQUIT		1
#endif
#ifndef VERASE
#define VERASE		2
#endif
#ifndef VKILL
#define VKILL		3
#endif
#ifndef VEOF
#define VEOF		4
#endif

#ifndef _LINUX_TERMIOS_H
#define VEOL		5
#define VEOL2		6
#define VMIN		4
#else				/* _LINUX_TERMIOS_H */
#define O_VEOL		5
#define O_VEOL2		6
#define O_VMIN		4
#endif				/* _LINUX_TERMIOS_H */

#ifndef VTIME
#define VTIME		5
#endif
#ifndef VSWTCH
#define VSWTCH		7
#endif
#ifndef VSTART
#define VSTART		8
#endif
#ifndef VSTOP
#define VSTOP		9
#endif
#ifndef VSUSP
#define VSUSP		10
#endif
#ifndef VDSUSP
#define VDSUSP		11
#endif
#ifndef VREPRINT
#define VREPRINT	12
#endif
#ifndef VDISCARD
#define VDISCARD	13
#endif
#ifndef VWERASE
#define VWERASE		14
#endif
#ifndef VLNEXT
#define VLNEXT		15
#endif

#define CNULL		0
#define CDEL		0177
#define CESC		'\\'
#define CINTR		0177
#define CQUIT		034
#define CERASE		'#'
#define CKILL		'@'
#define CEOT		CTRL('d')
#define CEOL		0
#define CEOL2		0
#define CEOF		CTRL('d')
#define CSTART		CTLR('q')
#define CSTOP		CTRL('s')
#define CSWTCH		CTRL('z')
#define CNSWTCH		0
#define CSUSP		CTRL('z')
#define CDSUSP		CTRL('y')
#define CRPRNT		CTRL('r')
#define CFLUSH		CTRL('o')
#define CWERASE		CTRL('w')
#define CLNEXT		CTRL('v')

#define CMIN		06
#define CTIME		01

/* Input flags - guard against kernel termbits redefinition */
#ifndef IGNBRK
#define IGNBRK		0000001
#endif
#ifndef BRKINT
#define BRKINT		0000002
#endif
#ifndef IGNPAR
#define IGNPAR		0000004
#endif
#ifndef PARMRK
#define PARMRK		0000010
#endif
#ifndef INPCK
#define INPCK		0000020
#endif
#ifndef ISTRIP
#define ISTRIP		0000040
#endif
#ifndef INLCR
#define INLCR		0000100
#endif
#ifndef IGNCR
#define IGNCR		0000200
#endif
#ifndef ICRNL
#define ICRNL		0000400
#endif
#ifndef IUCLC
#define IUCLC		0001000
#endif
#ifndef IXON
#define IXON		0002000
#endif
#ifndef IXANY
#define IXANY		0004000
#endif
#ifndef IXOFF
#define IXOFF		0010000
#endif
#ifndef IMAXBEL
#define IMAXBEL		0020000
#endif
#ifndef DOSMODE
#define DOSMODE		0100000
#endif

/* Output flags */
#ifndef OPOST
#define OPOST		0000001
#endif
#ifndef OLCUC
#define OLCUC		0000002
#endif
#ifndef ONLCR
#define ONLCR		0000004
#endif
#ifndef OCRNL
#define OCRNL		0000010
#endif
#ifndef ONOCR
#define ONOCR		0000020
#endif
#ifndef ONLRET
#define ONLRET		0000040
#endif
#ifndef OFILL
#define OFILL		0000100
#endif
#ifndef OFDEL
#define OFDEL		0000200
#endif

#ifndef NLDLY
#define NLDLY		0000400
#endif
#ifndef NL0
#define NL0		0000000
#endif
#ifndef NL1
#define NL1		0000400
#endif

#ifndef CRDLY
#define CRDLY		0003000
#endif
#ifndef CR0
#define CR0		0000000
#endif
#ifndef CR1
#define CR1		0001000
#endif
#ifndef CR2
#define CR2		0002000
#endif
#ifndef CR3
#define CR3		0003000
#endif

#ifndef TABDLY
#define TABDLY		0014000
#endif
#ifndef TAB0
#define TAB0		0000000
#endif
#ifndef TAB1
#define TAB1		0004000
#endif
#ifndef TAB2
#define TAB2		0010000
#endif
#ifndef TAB3
#define TAB3		0014000
#endif
#ifndef XTABS
#define XTABS		0014000
#endif

#ifndef BSDLY
#define BSDLY		0020000
#endif
#ifndef BS0
#define BS0		0000000
#endif
#ifndef BS1
#define BS1		0020000
#endif

#ifndef VTDLY
#define VTDLY		0040000
#endif
#ifndef VT0
#define VT0		0000000
#endif
#ifndef VT1
#define VT1		0040000
#endif

#ifndef FFDLY
#define FFDLY		0100000
#endif
#ifndef FF0
#define FF0		0000000
#endif
#ifndef FF1
#define FF1		0100000
#endif

#ifndef PAGEOUT
#define PAGEOUT		0200000
#endif
#ifndef WRAP
#define WRAP		0400000
#endif

#ifndef _LINUX_TERMIOS_H
#define CBAUD		0000017
#else				/* _LINUX_TERMIOS_H */
#define O_CBAUD		0000017
#endif				/* _LINUX_TERMIOS_H */
#ifndef CSIZE
#define CSIZE		0000060
#endif
#ifndef CS5
#define CS5		0000000
#endif
#ifndef CS6
#define CS6		0000020
#endif
#ifndef CS7
#define CS7		0000040
#endif
#ifndef CS8
#define CS8		0000060
#endif
#ifndef CSTOPB
#define CSTOPB		0000100
#endif
#ifndef CREAD
#define CREAD		0000200
#endif
#ifndef PARENB
#define PARENB		0000400
#endif
#ifndef PARODD
#define PARODD		0001000
#endif
#ifndef HUPCL
#define HUPCL		0002000
#endif
#ifndef CLOCAL
#define CLOCAL		0004000
#endif
#ifndef RCV1EN
#define RCV1EN		0010000
#endif
#ifndef XMT1EN
#define XMT1EN		0020000
#endif
#ifndef LOBLK
#define LOBLK		0040000
#endif
#ifndef XCLUDE
#define XCLUDE		0100000
#endif
#ifndef _LINUX_TERMIOS_H
#define CIBAUD		03600000
#else				/* _LINUX_TERMIOS_H */
#define O_CIBAUD	03600000
#endif				/* _LINUX_TERMIOS_H */
#ifndef PAREXT
#define PAREXT		04000000
#endif

#ifndef ISIG
#define ISIG		0000001
#endif
#ifndef ICANON
#define ICANON		0000002
#endif
#ifndef XCASE
#define XCASE		0000004
#endif
#ifndef ECHO
#define ECHO		0000010
#endif
#ifndef ECHOE
#define ECHOE		0000020
#endif
#ifndef ECHOK
#define ECHOK		0000040
#endif
#ifndef ECHONL
#define ECHONL		0000100
#endif
#ifndef NOFLSH
#define NOFLSH		0000200
#endif
#ifndef TOSTOP
#define TOSTOP		0000400
#endif
#ifndef ECHOCTL
#define ECHOCTL		0001000
#endif
#ifndef ECHOPRT
#define ECHOPRT		0002000
#endif
#ifndef ECHOKE
#define ECHOKE		0004000
#endif
#ifndef DEFECHO
#define DEFECHO		0010000
#endif

#ifndef _LINUX_TERMIOS_H
#define FLUSHO		0020000
#else				/* _LINUX_TERMIOS_H */
#define O_FLUSHO	0020000
#endif				/* _LINUX_TERMIOS_H */

#ifndef PENDIN
#define PENDIN		0040000
#endif
#ifndef IXEXTERN
#define IXEXTERN	0100000
#endif

#define TIOC		('T'<<8)
#ifndef _LINUX_TERMIOS_H
#define TCGETA		(TIOC|1)
#define TCSETA		(TIOC|2)
#define TCSETAW		(TIOC|3)
#define TCSETAF		(TIOC|4)
#define TCSBRK		(TIOC|5)
#define TCXONC		(TIOC|6)
#define TCFLSH		(TIOC|7)

#define TIOCGWINSZ	(TIOC|104)
#define TIOCSWINSZ	(TIOC|103)

#define TCGETS		(TIOC|13)
#define TCSETS		(TIOC|14)
#define TCSETSW		(TIOC|15)
#define TCSETSF		(TIOC|16)
#else				/* _LINUX_TERMIOS_H */
#define O_TCGETA	(TIOC|1)
#define O_TCSETA	(TIOC|2)
#define O_TCSETAW	(TIOC|3)
#define O_TCSETAF	(TIOC|4)
#define O_TCSBRK	(TIOC|5)
#define O_TCXONC	(TIOC|6)
#define O_TCFLSH	(TIOC|7)

#define O_TIOCGWINSZ	(TIOC|104)
#define O_TIOCSWINSZ	(TIOC|103)

#define O_TCGETS	(TIOC|13)
#define O_TCSETS	(TIOC|14)
#define O_TCSETSW	(TIOC|15)
#define O_TCSETSF	(TIOC|16)
#endif				/* _LINUX_TERMIOS_H */

#define tIOC		('t'<<8)

#define N_TIOCREMOTE	(TIOC|105)

#define TIOCREMOTE	(tIOC|30)
#define TIOCSIGNAL	(tIOC|31)

#ifndef TCIFLUSH
#define TCIFLUSH	0
#endif
#ifndef TCOFLUSH
#define TCOFLUSH	1
#endif
#ifndef TCIOFLUSH
#define TCIOFLUSH	2
#endif

#ifndef TCOOFF
#define TCOOFF		0
#endif
#ifndef TCOON
#define TCOON		1
#endif
#ifndef TCIOFF
#define TCIOFF		2
#endif
#ifndef TCION
#define TCION		3
#endif

#ifndef B0
#define B0		0000000
#endif
#ifndef B50
#define B50		0000001
#endif
#ifndef B75
#define B75		0000002
#endif
#ifndef B110
#define B110		0000003
#endif
#ifndef B134
#define B134		0000004
#endif
#ifndef B150
#define B150		0000005
#endif
#ifndef B200
#define B200		0000006
#endif
#ifndef B300
#define B300		0000007
#endif
#ifndef B600
#define B600		0000010
#endif
#ifndef B1200
#define B1200		0000011
#endif
#ifndef B1800
#define B1800		0000012
#endif
#ifndef B2400
#define B2400		0000013
#endif
#ifndef B4800
#define B4800		0000014
#endif
#ifndef B9600
#define B9600		0000015
#endif
#ifndef B19200
#define B19200		0000016
#endif
#ifndef B38400
#define B38400		0000017
#endif

#ifndef _LINUX_TERMIOS_H
struct winsize {
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel;
	unsigned short ws_ypixel;
};
#else				/* _LINUX_TERMIOS_H */
struct o_winsize {
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel;
	unsigned short ws_ypixel;
};
#endif				/* _LINUX_TERMIOS_H */

#endif				/* __SYS_TERMIOS_H__ */
