#ifndef __MACROS_H
#define __MACROS_H

#define likely( condition ) __builtin_expect(!!(condition), 1)
#define rarely( condition ) __builtin_expect(!!(condition), 0)

/* Colors for console output */

#define CONTROL_OPEN	"\x1B["
#define CONTROL_CLOSE	"m"
#define _MAKE_CONTROL( code ) CONTROL_OPEN code CONTROL_CLOSE
#define FORE_BLACK		_MAKE_CONTROL("30")
#define FORE_RED		_MAKE_CONTROL("31")
#define FORE_GREEN		_MAKE_CONTROL("32")
#define FORE_YELLOW		_MAKE_CONTROL("33")
#define FORE_BLUE		_MAKE_CONTROL("34")
#define FORE_MAGENTA	_MAKE_CONTROL("35")
#define FORE_CYAN		_MAKE_CONTROL("36")
#define FORE_WHITE		_MAKE_CONTROL("37")
#define BG_BLACK		_MAKE_CONTROL("40")
#define BG_RED			_MAKE_CONTROL("41")
#define BG_GREEN		_MAKE_CONTROL("42")
#define BG_YELLOW		_MAKE_CONTROL("43")
#define BG_BLUE			_MAKE_CONTROL("44")
#define BG_MAGENTA		_MAKE_CONTROL("45")
#define BG_CYAN			_MAKE_CONTROL("46")
#define BG_WHITE		_MAKE_CONTROL("47")
#define STYLE_BRIGHT	_MAKE_CONTROL("1")
#define STYLE_DIM		_MAKE_CONTROL("2")
#define STYLE_NORMAL	_MAKE_CONTROL("22")
#define STYLE_RESET		_MAKE_CONTROL("0")

/* Some useful string utils */

#define startswith(src, needle) ( strncmp( (src), (needle), strlen(needle)) == 0 )

#endif /* ifndef __MACROS_H */
