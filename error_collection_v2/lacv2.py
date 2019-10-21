
import ctypes
from ctypes import *
import _ctypes
import sys

N = 1024
lib = ctypes.cdll.LoadLibrary("./Lacv2.so")

def main():
    counter_int = int(sys.argv[1])
    counter = ctypes.c_int(counter_int)
    lib.Lacv2(counter)

if __name__ == '__main__':
	main()
