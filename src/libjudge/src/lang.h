#ifndef __LANG_H
#define __LANG_H

#define LANG_LENGTH	7
/* Definition for compiler names. */
const char *lang_name[] = {
	"gcc", 
	"g++", 
	"g++11", 
	"clang", 
	"clang++", 
	"clang++11", 
	"java", 
};
const char *_compile_command0[] = { "gcc", "-static", "-pipe", "-O2", "-DJUDGE", "-march=native", "-std=gnu99", "-Wall", "-Wconversion", "-Wextra", "-o", "{binary}", "{source}", NULL }; 
const char *_compile_command1[] = { "g++", "-static", "-pipe", "-O2", "-DJUDGE", "-march=native", "-Wall", "-Wconversion", "-Wextra", "-o", "{binary}", "{source}", NULL }; 
const char *_compile_command2[] = { "g++", "-static", "-pipe", "-O2", "-DJUDGE", "-march=native", "-std=gnu++11", "-Wall", "-Wconversion", "-Wextra", "-o", "{binary}", "{source}", NULL }; 
const char *_compile_command3[] = { "clang", "-static", "-pipe", "-O2", "-DJUDGE", "-march=native", "-std=c99", "-Wall", "-Wconversion", "-Wextra", "-o", "{binary}", "{source}", NULL }; 
const char *_compile_command4[] = { "clang++", "-static", "-pipe", "-O2", "-DJUDGE", "-march=native", "-Wall", "-Wconversion", "-Wextra", "-o", "{binary}", "{source}", NULL }; 
const char *_compile_command5[] = { "clang++", "-static", "-pipe", "-O2", "-DJUDGE", "-march=native", "-std=c++11", "-Wall", "-Wconversion", "-Wextra", "-o", "{binary}", "{source}", NULL }; 
const char *_compile_command6[] = { "javac", "{source}", NULL }; 
const char **compile_command[] = {
	_compile_command0,
	_compile_command1,
	_compile_command2,
	_compile_command3,
	_compile_command4,
	_compile_command5,
	_compile_command6,
};

#endif /* ifndef __LANG_H */

