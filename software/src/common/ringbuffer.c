#include "common/ringbuffer.h"

// Internal function to calculate the next index in the circular buffer
static uint16_t ringbuffer_increment_index(uint16_t index, uint16_t size) {
    return (index + 1) % size;
}

bool ringbuffer8_is_full(ringbuffer8_t* self) {
    return ringbuffer_increment_index(self->iTail, self->numElems) == self->iHead;
}

bool ringbuffer16_is_full(ringbuffer16_t* self) {
    return ringbuffer_increment_index(self->iTail, self->numElems) == self->iHead;
}

bool ringbuffer32_is_full(ringbuffer32_t* self) {
    return ringbuffer_increment_index(self->iTail, self->numElems) == self->iHead;
}

uint16_t ringbuffer8_available(ringbuffer8_t* self) {
    return (self->iTail >= self->iHead) ? (self->iTail - self->iHead) : (self->numElems + self->iTail - self->iHead);
}

uint16_t ringbuffer16_available(ringbuffer16_t* self) {
    return (self->iTail >= self->iHead) ? (self->iTail - self->iHead) : (self->numElems + self->iTail - self->iHead);
}

uint16_t ringbuffer32_available(ringbuffer32_t* self) {
    return (self->iTail >= self->iHead) ? (self->iTail - self->iHead) : (self->numElems + self->iTail - self->iHead);
}

bool ringbuffer8_write(ringbuffer8_t* self, uint8_t data) {
    if (!ringbuffer8_is_full(self)) {
        self->buffer[self->iTail] = data;
        self->iTail = ringbuffer_increment_index(self->iTail, self->numElems);
        return true;
    }
    return false;
}

bool ringbuffer16_write(ringbuffer16_t* self, uint16_t data) {
    if (!ringbuffer16_is_full(self)) {
        self->buffer[self->iTail] = data;
        self->iTail = ringbuffer_increment_index(self->iTail, self->numElems);
        return true;
    }
    return false;
}

bool ringbuffer32_write(ringbuffer32_t* self, uint32_t data) {
    if (!ringbuffer32_is_full(self)) {
        self->buffer[self->iTail] = data;
        self->iTail = ringbuffer_increment_index(self->iTail, self->numElems);
        return true;
    }
    return false;
}

uint8_t ringbuffer8_read(ringbuffer8_t* self) {
    if (self->iHead != self->iTail) {
        uint8_t data = self->buffer[self->iHead];
        self->iHead = ringbuffer_increment_index(self->iHead, self->numElems);
        return data;
    }
    return 0;  // Return a default value for empty buffer
}

uint16_t ringbuffer16_read(ringbuffer16_t* self) {
    if (self->iHead != self->iTail) {
        uint16_t data = self->buffer[self->iHead];
        self->iHead = ringbuffer_increment_index(self->iHead, self->numElems);
        return data;
    }
    return 0;  // Return a default value for empty buffer
}

uint32_t ringbuffer32_read(ringbuffer32_t* self) {
    if (self->iHead != self->iTail) {
        uint32_t data = self->buffer[self->iHead];
        self->iHead = ringbuffer_increment_index(self->iHead, self->numElems);
        return data;
    }
    return 0;  // Return a default value for empty buffer
}
