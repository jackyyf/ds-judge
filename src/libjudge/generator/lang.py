#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import yaml
import os.path
import sys
import shlex

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

CONFIG_FILE = BASE_DIR + '/lang.yml'
HEADER_FILE = BASE_DIR + '/autoconf/lang.h'

bytes_unit = {
    'k' : 1024,
    'm' : 1024 ** 2,
    'g' : 1024 ** 3,
    't' : 1024 ** 4,
}

seconds_unit = {
    's' : 1,
    'm' : 60,
    'h' : 3600,
    'd' : 86400,
}

default_tl = '10s'
default_ml = '128m'

def getbytes(rep):
    if rep[-1].lower() in bytes_unit:
        res = float(rep[:-1]) * bytes_unit[rep[-1].lower()]
    else:
        res = int(rep)
    res = int(res)
    if res >= 2 * (1024 ** 3): # 2G Warning
        print >>sys.stderr, 'WARNING: Memory limit %r may not work on 32-bit long platforms (>2GB)' % rep
    return res

def getseconds(rep):
    if rep[-1].lower() in seconds_unit:
        res = float(rep[:-1]) * seconds_unit[rep[-1].lower()]
    else:
        res = int(rep)
    res = int(res)
    return res

try:
    from yaml import CLoader as Loader
except ImportError:
    from yaml import Loader

with open(CONFIG_FILE, 'r') as f:
    lang_def = yaml.load(f.read())

h_file = open(HEADER_FILE, 'w')

print >>h_file, '''\
#ifndef __LANG_H
#define __LANG_H
'''

print >>h_file, '#define LANG_LENGTH    %d' % len(lang_def['compilers'])

# LANGUAGE NAME

print >>h_file, '''\
/* Definition for compiler names. */
const char *lang_name[] = {\
'''

for name in lang_def['compilers']:
    print >>h_file, '   "%s", ' % name['name'].encode('string_escape')

print >>h_file, '};'

# COMPILE COMMAND

print >>h_file, 'const char **compile_command[] = {'

for name in lang_def['compilers']:
    tokens = map(lambda x : '"' + x.encode('string_escape').replace('"', '\\"') + '"', shlex.split(name['command']))
    print >>h_file, '    { %s, NULL }, ' % ', '.join(tokens)

print >>h_file, '};'

# TIME LIMIT

print >>h_file, 'const long compile_tl[] = {'

for name in lang_def['compilers']:
    print >>h_file, '   %dL, ' % getseconds(name.get('time', default_tl))

print >>h_file, '};'

# MEMORY_LIMIT

print >>h_file, 'const long compile_ml[] = {'

for name in lang_def['compilers']:
    print >>h_file, '   %dL, ' % getbytes(name.get('memory', default_ml))

print >>h_file, '};'

print >>h_file, '''
#endif /* ifndef __LANG_H */
'''
