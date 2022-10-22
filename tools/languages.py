import configparser
import os
from os import listdir
from os.path import isfile, join
import sys



GLOBAL_DATA = {}

def is_defined(symbol, lang):
    return symbol in GLOBAL_DATA and lang in GLOBAL_DATA[symbol]

class Indexable:
    def __init__(self):
        self.dict = {}
        self.count = 0
    def add(self,name):
        if name in self.dict: return
        self.dict[name.upper()] = self.count
        self.count+=1
    def items(self) : return list(self.dict.keys())

GLOBAL_SYMBOLS = Indexable()
GLOBAL_LANGS = Indexable()

def encode_text(s):

    _encode = 0 

    def get_code(enc, id):
        nonlocal _encode
        if enc==0:
            return "\\x"+hex(id)[2:].rjust(2,'0')
        r = ""        
        if(enc!=_encode):
            _encode = enc
            r+="\\x"+hex(16+enc)[2:].rjust(2,'0')
        r+="\\x"+hex(id)[2:].rjust(2,'0')
        return r

    result = ""    
        
    for char in s:                
        code = ord(char)        
        #if(code>=128): print(char, hex(code))        
        if code < 127:
            result+=char
        elif code < 256:
            result+=get_code(1, code)
        elif code==0x0104: result += get_code(2, 161)
        elif code==0x02d8: result += get_code(2, 162)
        elif code==0x0141: result += get_code(2, 163)
        elif code==0x00a4: result += get_code(2, 164)
        elif code==0x013d: result += get_code(2, 165)
        elif code==0x015a: result += get_code(2, 166)
        elif code==0x00a7: result += get_code(2, 167)
        elif code==0x00a8: result += get_code(2, 168)
        elif code==0x0160: result += get_code(2, 169)
        elif code==0x015e: result += get_code(2, 170)
        elif code==0x0164: result += get_code(2, 171)
        elif code==0x0179: result += get_code(2, 172)
        elif code==0x00ad: result += get_code(2, 173)
        elif code==0x017d: result += get_code(2, 174)
        elif code==0x017b: result += get_code(2, 175)
        elif code==0x00b0: result += get_code(2, 176)
        elif code==0x0105: result += get_code(2, 177)
        elif code==0x02db: result += get_code(2, 178)
        elif code==0x0142: result += get_code(2, 179)
        elif code==0x00b4: result += get_code(2, 180)
        elif code==0x013e: result += get_code(2, 181)
        elif code==0x015b: result += get_code(2, 182)
        elif code==0x02c7: result += get_code(2, 183)
        elif code==0x00b8: result += get_code(2, 184)
        elif code==0x0161: result += get_code(2, 185)
        elif code==0x015f: result += get_code(2, 186)
        elif code==0x0165: result += get_code(2, 187)
        elif code==0x017a: result += get_code(2, 188)
        elif code==0x02dd: result += get_code(2, 189)
        elif code==0x017e: result += get_code(2, 190)
        elif code==0x017c: result += get_code(2, 191)
        elif code==0x0154: result += get_code(2, 192)
        elif code==0x00c1: result += get_code(2, 193)
        elif code==0x00c2: result += get_code(2, 194)
        elif code==0x0102: result += get_code(2, 195)
        elif code==0x00c4: result += get_code(2, 196)
        elif code==0x0139: result += get_code(2, 197)
        elif code==0x0106: result += get_code(2, 198)
        elif code==0x00c7: result += get_code(2, 199)
        elif code==0x010c: result += get_code(2, 200)
        elif code==0x00c9: result += get_code(2, 201)
        elif code==0x0118: result += get_code(2, 202)
        elif code==0x00cb: result += get_code(2, 203)
        elif code==0x011a: result += get_code(2, 204)
        elif code==0x00cd: result += get_code(2, 205)
        elif code==0x00ce: result += get_code(2, 206)
        elif code==0x010e: result += get_code(2, 207)
        elif code==0x0110: result += get_code(2, 208)
        elif code==0x0143: result += get_code(2, 209)
        elif code==0x0147: result += get_code(2, 210)
        elif code==0x00d3: result += get_code(2, 211)
        elif code==0x00d4: result += get_code(2, 212)
        elif code==0x0150: result += get_code(2, 213)
        elif code==0x00d6: result += get_code(2, 214)
        elif code==0x00d7: result += get_code(2, 215)
        elif code==0x0158: result += get_code(2, 216)
        elif code==0x016e: result += get_code(2, 217)
        elif code==0x00da: result += get_code(2, 218)
        elif code==0x0170: result += get_code(2, 219)
        elif code==0x00dc: result += get_code(2, 220)
        elif code==0x00dd: result += get_code(2, 221)
        elif code==0x0162: result += get_code(2, 222)
        elif code==0x00df: result += get_code(2, 223)
        elif code==0x0155: result += get_code(2, 224)
        elif code==0x00e1: result += get_code(2, 225)
        elif code==0x00e2: result += get_code(2, 226)
        elif code==0x0103: result += get_code(2, 227)
        elif code==0x00e4: result += get_code(2, 228)
        elif code==0x013a: result += get_code(2, 229)
        elif code==0x0107: result += get_code(2, 230)
        elif code==0x00e7: result += get_code(2, 231)
        elif code==0x010d: result += get_code(2, 232)
        elif code==0x00e9: result += get_code(2, 233)
        elif code==0x0119: result += get_code(2, 234)
        elif code==0x00eb: result += get_code(2, 235)
        elif code==0x011b: result += get_code(2, 236)
        elif code==0x00ed: result += get_code(2, 237)
        elif code==0x00ee: result += get_code(2, 238)
        elif code==0x010f: result += get_code(2, 239)
        elif code==0x0111: result += get_code(2, 240)
        elif code==0x0144: result += get_code(2, 241)
        elif code==0x0148: result += get_code(2, 242)
        elif code==0x00f3: result += get_code(2, 243)
        elif code==0x00f4: result += get_code(2, 244)
        elif code==0x0151: result += get_code(2, 245)
        elif code==0x00f6: result += get_code(2, 246)
        elif code==0x00f7: result += get_code(2, 247)
        elif code==0x0159: result += get_code(2, 248)
        elif code==0x016f: result += get_code(2, 249)
        elif code==0x00fa: result += get_code(2, 250)
        elif code==0x0171: result += get_code(2, 251)
        elif code==0x00fc: result += get_code(2, 252)
        elif code==0x00fd: result += get_code(2, 253)
        elif code==0x0163: result += get_code(2, 254)
        elif code==0x02d9: result += get_code(2, 255)
        
        elif code==0x0401: result += get_code(4, 161)
        elif code==0x0402: result += get_code(4, 162)
        elif code==0x0403: result += get_code(4, 163)
        elif code==0x0404: result += get_code(4, 164)
        elif code==0x0405: result += get_code(4, 165)
        elif code==0x0406: result += get_code(4, 166)
        elif code==0x0407: result += get_code(4, 167)
        elif code==0x0408: result += get_code(4, 168)
        elif code==0x0409: result += get_code(4, 169)
        elif code==0x040A: result += get_code(4, 170)
        elif code==0x040B: result += get_code(4, 171)
        elif code==0x040C: result += get_code(4, 172)
        elif code==0x040d: result += get_code(4, 173)
        elif code==0x040e: result += get_code(4, 174)
        elif code==0x040f: result += get_code(4, 175)
        elif code==0x0410: result += get_code(4, 176)
        elif code==0x0411: result += get_code(4, 177)
        elif code==0x0412: result += get_code(4, 178)
        elif code==0x0413: result += get_code(4, 179)
        elif code==0x0414: result += get_code(4, 180)
        elif code==0x0415: result += get_code(4, 181)
        elif code==0x0416: result += get_code(4, 182)
        elif code==0x0417: result += get_code(4, 183)
        elif code==0x0418: result += get_code(4, 184)
        elif code==0x0419: result += get_code(4, 185)
        elif code==0x041a: result += get_code(4, 186)
        elif code==0x041b: result += get_code(4, 187)
        elif code==0x041c: result += get_code(4, 188)
        elif code==0x041d: result += get_code(4, 189)
        elif code==0x041e: result += get_code(4, 190)
        elif code==0x041f: result += get_code(4, 191)
        elif code==0x0420: result += get_code(4, 192)
        elif code==0x0421: result += get_code(4, 193)
        elif code==0x0422: result += get_code(4, 194)
        elif code==0x0423: result += get_code(4, 195)
        elif code==0x0424: result += get_code(4, 196)
        elif code==0x0425: result += get_code(4, 197)
        elif code==0x0426: result += get_code(4, 198)
        elif code==0x0427: result += get_code(4, 199)
        elif code==0x0428: result += get_code(4, 200)
        elif code==0x0429: result += get_code(4, 201)
        elif code==0x042a: result += get_code(4, 202)
        elif code==0x042b: result += get_code(4, 203)
        elif code==0x042c: result += get_code(4, 204)
        elif code==0x042d: result += get_code(4, 205)
        elif code==0x042e: result += get_code(4, 206)
        elif code==0x042f: result += get_code(4, 207)
        elif code==0x0430: result += get_code(4, 208)
        elif code==0x0431: result += get_code(4, 209)
        elif code==0x0432: result += get_code(4, 210)
        elif code==0x0433: result += get_code(4, 211)
        elif code==0x0434: result += get_code(4, 212)
        elif code==0x0435: result += get_code(4, 213)
        elif code==0x0436: result += get_code(4, 214)
        elif code==0x0437: result += get_code(4, 215)
        elif code==0x0438: result += get_code(4, 216)
        elif code==0x0439: result += get_code(4, 217)
        elif code==0x043a: result += get_code(4, 218)
        elif code==0x043b: result += get_code(4, 219)
        elif code==0x043c: result += get_code(4, 220)
        elif code==0x043d: result += get_code(4, 221)
        elif code==0x043e: result += get_code(4, 222)
        elif code==0x043f: result += get_code(4, 223)
        elif code==0x0440: result += get_code(4, 224)
        elif code==0x0441: result += get_code(4, 225)
        elif code==0x0442: result += get_code(4, 226)
        elif code==0x0443: result += get_code(4, 227)
        elif code==0x0444: result += get_code(4, 228)
        elif code==0x0445: result += get_code(4, 229)
        elif code==0x0446: result += get_code(4, 230)
        elif code==0x0447: result += get_code(4, 231)
        elif code==0x0448: result += get_code(4, 232)
        elif code==0x0449: result += get_code(4, 233)
        elif code==0x044a: result += get_code(4, 234)
        elif code==0x044b: result += get_code(4, 235)
        elif code==0x044c: result += get_code(4, 236)
        elif code==0x044d: result += get_code(4, 237)
        elif code==0x044e: result += get_code(4, 238)
        elif code==0x044f: result += get_code(4, 239)
        elif code==0x2116: result += get_code(4, 240)
        elif code==0x0451: result += get_code(4, 241)
        elif code==0x0452: result += get_code(4, 242)
        elif code==0x0453: result += get_code(4, 243)
        elif code==0x0454: result += get_code(4, 244)
        elif code==0x0455: result += get_code(4, 245)
        elif code==0x0456: result += get_code(4, 246)
        elif code==0x0457: result += get_code(4, 247)
        elif code==0x0458: result += get_code(4, 248)
        elif code==0x0459: result += get_code(4, 249)
        elif code==0x045a: result += get_code(4, 250)
        elif code==0x045b: result += get_code(4, 251)
        elif code==0x045c: result += get_code(4, 252)
        elif code==0x00a7: result += get_code(4, 253)
        elif code==0x045e: result += get_code(4, 254)
        elif code==0x045f: result += get_code(4, 255)                
       
    
    return result
    

def read_ini_file(path):
    print(path)
    config = configparser.ConfigParser()
    config.read(path,encoding='utf-8')

    sections = config.sections()

    for section in sections:
        GLOBAL_LANGS.add(section)
        for key in config[section]:
            GLOBAL_SYMBOLS.add(key)
            if not key.upper() in GLOBAL_DATA:
                GLOBAL_DATA[key.upper()] = {} 
            msg = config[section][key]
            msg = msg[1:-1]                       
            msg = str.encode(msg)
            msg = msg.decode("utf-8")
            #print(type(msg))
            #print(msg)
            msg = encode_text(msg)            
            GLOBAL_DATA[key.upper()][section.upper()] = msg

def read_in_dir(dir_path):
    print(dir_path)
    files = [f for f in listdir(dir_path) if isfile(join(dir_path, f)) \
             and f.endswith(".ini")]
    print(len(files))
    for file in files:
        read_ini_file(os.path.join(dir_path,file))


def generate_msg_list():
    result = []

    for sym in GLOBAL_SYMBOLS.items():
        for lang in GLOBAL_LANGS.items():
            if is_defined(sym, lang):
                result.append(f"{sym}_{lang}")
            else:
                result.append('""')
    print(result)
    return "{" + f'{", ".join(result)}' + "}"
    
def msg_count():    
    return len(GLOBAL_SYMBOLS.items()) * len(GLOBAL_LANGS.items())    

def create_lang_symbols():
    result = []
    langs = GLOBAL_LANGS.items()
    for i in range(len(langs)):
        result.append(f"inline static const int LANG_{langs[i]} = {i};")
    return "\n".join(result)

def create_msg_symbols():
    result = []
    syms = GLOBAL_SYMBOLS.items()
    for i in range(len(syms)):
        result.append(f"inline static const int LMSG_{syms[i]} = {i};")
    return "\n".join(result)

def create_messages():
    result = []

    for sym in GLOBAL_SYMBOLS.items():
        for lang in GLOBAL_LANGS.items():
            if is_defined(sym, lang):
                name = f"{sym}_{lang}"
                value = GLOBAL_DATA[sym][lang]                
                result.append(f'inline static const char* const {name} = "{value}";')
                
    return "\n".join(result)
    

if __name__=="__main__":    
    print(sys.argv)    
    print(os.getcwd())

    read_in_dir("./" if len(sys.argv)<2 else sys.argv[1])

    if len(sys.argv)>=2: print(sys.argv[1]) 
    

    print(GLOBAL_LANGS.items())
    print(GLOBAL_SYMBOLS.items())
    
    print(GLOBAL_DATA)


    lang_symbols = create_lang_symbols()
    print(lang_symbols)

    msg_symbols = create_msg_symbols()
    print(msg_symbols)
    
    messages_list = generate_msg_list()
    print(messages_list)

    print(create_messages())

    messages = create_messages()

    header = \
f"""#pragma once

{messages}

{lang_symbols}

inline static const int LANGUAGES_COUNT = {len(GLOBAL_LANGS.items())};

{msg_symbols}


inline static const char* const MESSAGES[{msg_count()}] {messages_list};

extern int CURRENT_LANGUAGE;
void set_current_language(int LANGUAGE);
const char* const get_message(int LMSG);
"""    

    
    f = open("langs.h" if len(sys.argv)<3 else sys.argv[2], "w")
    f.write(header)
    f.close()

    print(header)

    
