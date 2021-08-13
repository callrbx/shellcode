from pwn import *
import sys
import time

filename = sys.argv[1]

with open(filename, "rb") as f:
    p = remote("localhost", 8080)
    time.sleep(.1)
    data = f.read()
    p.send(data)
    p.send("Z")
    p.interactive()