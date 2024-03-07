#include "../library.h"
#include <assert.h>
#include <stdio.h>

// This replaces the hardware layer of abstraction
static uint8_t global_buffer[100] = {0};
static uint16_t buffer_counter = 0;

int32_t read(uint8_t *buffer, uint16_t buffer_size) {
    int ii;

    for(ii = 0; ii < buffer_size; ii ++) {
        buffer[ii] = global_buffer[ii];
    }

    buffer_counter -= ii;
    return ii;
}

int32_t write(const uint8_t *buffer, uint16_t buffer_size) {
    int ii;

    for(ii = 0; ii < buffer_size; ii ++) {
        global_buffer[ii] = buffer[ii];
    }

    buffer_counter += ii;
    return ii;
}

int32_t avail() {
    return buffer_counter;
}

// Main
int main() {
    const uint8_t test[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    uint8_t receive[8] = {0};

    CCSDSConfig config = {
        .avail = avail,
        .read = read,
        .write = write
    };

    uint32_t sent_size = ccsds_write(&config, test, 8);
    uint32_t received_size = ccsds_read(&config, receive, 8);

    assert(sent_size == received_size);

    for(int i = 0; i < 8; i++) {
        assert(test[i] == receive[i]);
    }

    printf("TEST:local_buffer:PASSED\n");
    return 0;
}

