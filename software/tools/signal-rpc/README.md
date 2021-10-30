# Signal RPC

Protocol:

`void trigger()`

`0x02 0x00 0xFF`
-> Sending 2 bytes
    -> ID = 0x00
    -> Checksum = 0xFF

`void set_value(uint8_t val)`

`0x03 0x01 63 0xDF`
-> Sending 3 bytes
    -> ID = 0x01
    -> Value = 63
    -> Checksum = 0xDF
