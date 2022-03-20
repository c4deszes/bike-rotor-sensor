import serial
import time

import sys, signal
def signal_handler(signal, frame):
    print("\nprogram exiting gracefully")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

with serial.Serial(port='COM3', baudrate=9600, timeout=2) as serial:
    try:
        while True:
            # val = serial.read()
            # if len(val) >= 1:
            #     print(f"{val[0]} ms")
            data = serial.read_until(size=10)
            if len(data) == 10:
                print(f"{[f'0x{d:02x}' for d in data]}")
                temp = data[0]
                speed = int.from_bytes(data[1:3], byteorder='big')
                pos = int.from_bytes(data[3:5], byteorder='big')
                dist = int.from_bytes(data[5:9], byteorder='big')
                print(f"Temp: {temp}, Speed: {speed}, Pos: {pos}, Dist: {dist}")
    except KeyboardInterrupt:
        print('End')
    except Exception as e:
        print(e)