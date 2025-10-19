#if !defined(COMMON_RINGBUFFER_H_)
#define COMMON_RINGBUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t* buffer;
    uint16_t iHead ;
    uint16_t iTail ;
    uint16_t numElems;
} ringbuffer8_t;

typedef struct
{
    uint16_t* buffer;
    uint16_t iHead ;
    uint16_t iTail ;
    uint16_t numElems;
} ringbuffer16_t;

typedef struct
{
    uint32_t* buffer;
    uint16_t iHead ;
    uint16_t iTail ;
    uint16_t numElems;
} ringbuffer32_t;

bool ringbuffer8_is_full(ringbuffer8_t* self);
bool ringbuffer16_is_full(ringbuffer16_t* self);
bool ringbuffer32_is_full(ringbuffer32_t* self);

uint16_t ringbuffer8_available(ringbuffer8_t* self);
uint16_t ringbuffer16_available(ringbuffer16_t* self);
uint16_t ringbuffer32_available(ringbuffer32_t* self);

bool ringbuffer8_write(ringbuffer8_t* self, uint8_t data);
bool ringbuffer16_write(ringbuffer16_t* self, uint16_t data);
bool ringbuffer32_write(ringbuffer32_t* self, uint32_t data);

uint8_t ringbuffer8_read(ringbuffer8_t* self);
uint16_t ringbuffer16_read(ringbuffer16_t* self);
uint32_t ringbuffer32_read(ringbuffer32_t* self);

#define DECLARE_RINGBUFFER_8(bufferName) \
    extern ringbuffer8_t bufferName;

#define RINGBUFFER_8(bufferName, bufferSize) \
    uint8_t bufferName##_data[bufferSize]; \
    ringbuffer8_t bufferName = { \
        .buffer = bufferName##_data, \
        .iHead = 0, \
        .iTail = 0, \
        .numElems = bufferSize \
    }

// Macro to declare a global ring buffer for 16-bit data
#define RINGBUFFER_16(bufferName, bufferSize) \
    uint16_t bufferName##_data[bufferSize]; \
    ringbuffer16_t bufferName = { \
        .buffer = bufferName##_data, \
        .iHead = 0, \
        .iTail = 0, \
        .numElems = bufferSize \
    }

// Macro to declare a global ring buffer for 32-bit data
#define RINGBUFFER_32(bufferName, bufferSize) \
    uint32_t bufferName##_data[bufferSize]; \
    ringbuffer32_t bufferName = { \
        .buffer = bufferName##_data, \
        .iHead = 0, \
        .iTail = 0, \
        .numElems = bufferSize \
    }

#endif // COMMON_RINGBUFFER_H_
