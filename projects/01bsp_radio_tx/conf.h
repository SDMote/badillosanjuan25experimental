#include <nrf.h>
#include "radio.h"

static const uint8_t packet_tx[] = {
    0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10,  //
    0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20,  //
    0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30,  //
    0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40,  //
    0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E, 0x50,  //
    0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E, 0x60,  //
    0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E, 0x70,  //
    0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E, 0x80,  //
    0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90,  //
    0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E, 0xA0,  //
    0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE, 0xB0,  //
    0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE, 0xC0,  //
    0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE, 0xD0,  //
    0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE, 0xE0,  //
    0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF0,  //
};  // 120 Bytes long

typedef struct {
    db_radio_mode_t radio_mode;              // DB_RADIO_BLE_1MBit, DB_RADIO_IEEE802154_250Kbit
    uint8_t         frequency;               // (2400 + frequency) MHz
    uint8_t         tx_power;                // RADIO_TXPOWER_TXPOWER_XdBm: X = {PosxdBm, 0dBm, NegxdBm}
    uint32_t        delay_us;                // Wait delay_us before sending
    uint32_t        tone_blocker_us;         // (0) Normal operation, (else) Use a sinusoidal blocker for the specified amount of us
    uint8_t         increase_id;             // (0) Don't increase (Blocker), (1) Increase (Main transmitter)
    uint8_t         packet_size;             // Amount of bytes of packet_tx to send
    uint8_t         increase_packet_offset;  // (0) Don't increase (always same packet), (1) Increase
} radio_config_t;

static const radio_config_t configs[] = {
    { DB_RADIO_BLE_1MBit, 25, RADIO_TXPOWER_TXPOWER_0dBm, 0, 0, 0, 120, 0 },
    { DB_RADIO_BLE_1MBit, 25, RADIO_TXPOWER_TXPOWER_0dBm, 0, 0, 0, 120, 0 },
    { DB_RADIO_IEEE802154_250Kbit, 25, RADIO_TXPOWER_TXPOWER_0dBm, 0, 0, 0, 80, 0 },
    { DB_RADIO_IEEE802154_250Kbit, 25, RADIO_TXPOWER_TXPOWER_0dBm, 0, 0, 0, 80, 0 },
    { DB_RADIO_BLE_1MBit, 25, RADIO_TXPOWER_TXPOWER_0dBm, 0, 0, 0, 120, 0 },          // same-protocol interference in time, adjacent bands
    { DB_RADIO_IEEE802154_250Kbit, 25, RADIO_TXPOWER_TXPOWER_0dBm, 0, 0, 0, 80, 0 },  // same-protocol interference in time, adjacent bands
};
