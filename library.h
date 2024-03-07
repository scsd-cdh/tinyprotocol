#ifndef CCSDS_LIBRARY_H
#define CCSDS_LIBRARY_H

// This is for MEMCPY and MEMSET. In embedded you can usually find implementations for your architecture.
#include <string.h>
// ---- //

#include <stdint.h>
#include "config.h"

// =============== SPACE PACKET TYPE =============== //
typedef enum {
    SPACE_PACKET_TYPE_TM,
    SPACE_PACKET_TYPE_TC
} SpacePacketType;

// =============== SPACE PACKET PRIMARY HEADER =============== //
typedef struct {
    uint8_t version_number : 3;
    uint8_t type : 1;
    uint8_t secondary_header_flag : 1;
    uint16_t apid : 11;
    uint8_t sequence_flags : 2;
    uint16_t sequence_count : 14;
    uint16_t data_length : 16;
} SpacePacketPrimaryHeader;

// =============== SPACE PACKET =============== //
typedef struct {
    SpacePacketPrimaryHeader primary_header;
    uint8_t data[SPP_MAX_DATA_SIZE];
} SpacePacket;

// =============== CCSDS =============== //
typedef int32_t (*CCSDSReadFn)(uint8_t *buffer, uint16_t buffer_size);
typedef int32_t (*CCSDSWriteFn)(const uint8_t *buffer, uint16_t buffer_size);
typedef int32_t (*CCSDSAvailFn)();

typedef struct {
    CCSDSAvailFn avail;
    CCSDSReadFn read;
    CCSDSWriteFn write;
} CCSDSConfig;

int32_t ccsds_read(CCSDSConfig *config, uint8_t *data, uint16_t data_size);
int32_t ccsds_write(CCSDSConfig *config, const uint8_t *buffer, uint16_t data_size);

#endif //CCSDS_LIBRARY_H
