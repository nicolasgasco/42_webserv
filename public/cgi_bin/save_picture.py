import sys
import os

data = sys.stdin.buffer.read()

path = os.getenv("PATH_INFO")
f = open(path, "ab")
f.write(data)
f.close()
