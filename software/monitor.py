import serial
import time

import sys, signal
def signal_handler(signal, frame):
    print("\nprogram exiting gracefully")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

with serial.Serial(port='COM3', baudrate=9600, timeout=0.1) as serial:
    try:
        while True:
            # val = serial.read()
            # if len(val) >= 1:
            #     print(f"{val[0]} ms")
            data = serial.read_until(size=9)
            if len(data) == 9:
                print(f"{[f'0x{d:02x}' for d in data]}")
                speed = int.from_bytes(data[0:2], byteorder='big')
                pos = int.from_bytes(data[2:4], byteorder='big')
                dist = int.from_bytes(data[4:8], byteorder='big')
                print(f"Speed: {speed}, Pos: {pos}, Dist: {dist}")
    except KeyboardInterrupt:
        print('End')
    except Exception as e:
        print(e)