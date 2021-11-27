from typing import List
import serial
import time

import sys, signal
def signal_handler(signal, frame):
    print("\nprogram exiting gracefully")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

def erase_command(section: int) -> bytearray:
    return bytearray([2, 0x33, section])

def flash_command(addr: int, data: List[int]) -> bytearray:
    return bytearray([len(data) + 3, 0x32, (addr >> 8) & 0xFF, addr & 0xFF] + data)

with serial.Serial(port='COM11', baudrate=9600, timeout=0.001) as dev:
    dev.write(erase_command(0))