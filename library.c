#include "library.h"

int32_t ccsds_read(CCSDSConfig *config, uint8_t *data, uint16_t data_size) {
    if (config->avail() < sizeof(SpacePacketPrimaryHeader) + data_size) {
        return -1;
    }

    uint8_t buffer[sizeof(SpacePacket)];
    int32_t read_size = config->read(buffer, sizeof(SpacePacketPrimaryHeader) + data_size);

    SpacePacket packet;
    memset(&packet, 0, sizeof(SpacePacket));
    memcpy(&packet.primary_header, buffer, sizeof(SpacePacketPrimaryHeader));

    uint16_t expected_data_length = packet.primary_header.data_length;
    if (expected_data_length > data_size || expected_data_length < data_size) {
        return -2;
    }

    memcpy(data, buffer + sizeof(SpacePacketPrimaryHeader), data_size);

    return read_size;
}

int32_t ccsds_write(CCSDSConfig *config, const uint8_t *data, uint16_t data_size) {
    if (data_size > SPP_MAX_DATA_SIZE) {
        return -1;
    }

    SpacePacket packet;
    memset(&packet, 0, sizeof(packet));

    packet.primary_header.version_number = 0b000;       // Version 1 CCSDS Packet
    packet.primary_header.type = 0;                     // Telemetry
    packet.primary_header.secondary_header_flag = 0;    // No secondary header
    packet.primary_header.apid = 0x07FF & 0;            // Set APID, mask to 11 bits
    packet.primary_header.sequence_flags = 3;           // Assuming unsegmented data
    packet.primary_header.sequence_count = 0;           // Increment sequence count for each packet
    packet.primary_header.data_length = data_size;      // Data data_size does not include the size of the primary header

    // Copy data into the packet
    memcpy(packet.data, data, data_size);

    // Convert the packet to a byte buffer for transmission
    uint8_t buffer[sizeof(SpacePacket)];
    memcpy(buffer, &packet, sizeof(SpacePacket));

    // Send the packet
    return config->write(buffer, sizeof(SpacePacketPrimaryHeader) + data_size);
}
