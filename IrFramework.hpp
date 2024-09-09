/*
 * This file is part of the Capibara zero (https://github.com/CapibaraZero/fw or
 * https://capibarazero.github.io/). Copyright (c) 2024 Andrea Canale.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IR_FRAMEWORK_H
#define IR_FRAMEWORK_H

#include "Arduino.h"

class IRData;

#if !defined(ARDUINO_ESP32C3_DEV) // This is due to a bug in RISC-V compiler, which requires unused function sections :-(.
#define DISABLE_CODE_FOR_RECEIVER // Disables static receiver code like receive timer ISR handler and static IRReceiver and irparams data. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not required.
#endif

// Wrapper for IRData to avoid linkage issues
typedef struct RecordedSignal {
    int protocol;
    uint16_t address;
    uint16_t command;
    uint32_t raw_len;
    uint16_t numberOfBits; ///< Number of bits received for data (address + command + parity) - to determine protocol length if different length are possible.
    uint8_t flags;          ///< IRDATA_FLAGS_IS_REPEAT, IRDATA_FLAGS_WAS_OVERFLOW etc. See IRDATA_FLAGS_* definitions above
    uint16_t extra; ///< Contains upper 16 bit of Magiquest WandID, Kaseikyo unknown vendor ID and Distance protocol (HeaderMarkTicks << 8) | HeaderSpaceTicks.e of protocol == PULSE_DISTANCE or -rather seldom- protocol == PULSE_WIDTH.
    uint64_t decodedRawData;
    uint8_t decodedRawDataArray[750]; ///< 32/64 bit decoded raw data, to be used for send function.
}RecordedSignal;

class IrFramework
{
public:
    IrFramework(size_t ir_receiver_pin, size_t ir_sender_pin);
    ~IrFramework();
    bool read_signal();
    RecordedSignal get_decoded_signal();
    void send_protocol_signal(RecordedSignal record);
    void send_lg_air_conditioner_signal(bool wall_type, char command, int param = 0);
    String enum_to_str(size_t val);
};

#endif