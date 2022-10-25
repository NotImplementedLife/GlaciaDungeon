def gen_h(symbol,w,h):
    return \
f"""#pragma once
#include "map_source.h"

extern const unsigned char {symbol}_src[];

inline const struct MapSource {symbol} = {{{w},{h}, {symbol}_src}};
"""

def gen_c(symbol,w,h):
    return \
f"""
#include "{symbol}.maps.h"

//const struct MapSource {symbol} = ;
"""

class MapSource:
    def __init__(self,w,h,s):
        self.width=w
        self.height=h
        self.src=s
        
    def set_name(self, name):
        self.name=name
        
    def write(self):
        open(self.name+".maps.s","w").write(gen_s(self.name+"_src", self.src))
        open(self.name+".maps.h","w").write(gen_h(self.name, self.width, self.height))        
        
        

import struct

def read_map(file):
    f = open(file, "rb")
    w = struct.unpack('i', f.read(4))[0]
    h = struct.unpack('i', f.read(4))[0]
    src = f.read()
    print(type(src))
    return MapSource(w,h,src) 

#def gen_c(symbol, 

def gen_s(symbol, data):    
    str_dat=".word "
    data = bytearray(data)
    if(len(data)%2==1):
        data.append(0)
    data=data.hex()
    data = [data[i:i+2] for i in range(0,len(data),2)]
    data = ["0x"+"".join(data[i:i+2][::-1]) for i in range(0,len(data),2)]
    data = "\n".join(["\t.hword "+", ".join(data[i:i+8]) for i in range(0,len(data),8)])
    #print(data)    
    return \
f"""
    .section .rodata
	.align	4
	.global {symbol}
	.hidden {symbol}
{symbol}:
{data}
"""


import sys
import re
import os

curdir = sys.argv[1]
symbol_name = sys.argv[2]

print(curdir, symbol_name)

maps_file = os.path.dirname(curdir)+"/maps/"+symbol_name+".maps"
print(maps_file)

mapsrc = read_map(maps_file)
mapsrc.set_name(symbol_name)

mapsrc.write()
print(mapsrc.width)
print(mapsrc.height)


#open(symbol_name+".maps.s","w").write(gen_s(symbol_name, open(font_file,"rb").read()))
#open(symbol_name+".maps.h","w").write(gen_h(symbol_name))


#open(fname,"w").write(f"""
#pragma once
#include "Astralbrew/text/font.hpp"

#extern const Astralbrew::Text::ReadOnlyFont {name};
#""")

#sname = name+".s"
#oname = name+".astralfont.o"

#open(sname,"w").write(".section text")