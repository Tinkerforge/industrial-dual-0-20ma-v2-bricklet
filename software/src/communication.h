/* industrial-dual-0-20ma-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

#define CALLBACK_VALUE_TYPE CALLBACK_VALUE_TYPE_INT32

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define INDUSTRIAL_DUAL_0_20MA_V2_THRESHOLD_OPTION_OFF 'x'
#define INDUSTRIAL_DUAL_0_20MA_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define INDUSTRIAL_DUAL_0_20MA_V2_THRESHOLD_OPTION_INSIDE 'i'
#define INDUSTRIAL_DUAL_0_20MA_V2_THRESHOLD_OPTION_SMALLER '<'
#define INDUSTRIAL_DUAL_0_20MA_V2_THRESHOLD_OPTION_GREATER '>'

#define INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_240_SPS 0
#define INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_60_SPS 1
#define INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_15_SPS 2
#define INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_4_SPS 3

#define INDUSTRIAL_DUAL_0_20MA_V2_GAIN_1X 0
#define INDUSTRIAL_DUAL_0_20MA_V2_GAIN_2X 1
#define INDUSTRIAL_DUAL_0_20MA_V2_GAIN_4X 2
#define INDUSTRIAL_DUAL_0_20MA_V2_GAIN_8X 3

#define INDUSTRIAL_DUAL_0_20MA_V2_INFO_LED_CONFIG_OFF 0
#define INDUSTRIAL_DUAL_0_20MA_V2_INFO_LED_CONFIG_ON 1
#define INDUSTRIAL_DUAL_0_20MA_V2_INFO_LED_CONFIG_SHOW_HEARTBEAT 2
#define INDUSTRIAL_DUAL_0_20MA_V2_INFO_LED_CONFIG_SHOW_CHANNEL_STATUS 3

#define INDUSTRIAL_DUAL_0_20MA_V2_CHANNEL_LED_CONFIG_THRESHOLD 0
#define INDUSTRIAL_DUAL_0_20MA_V2_CHANNEL_LED_CONFIG_INTENSITY 1

#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_MODE_FIRMWARE 1
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_STATUS_OK 0
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define INDUSTRIAL_DUAL_0_20MA_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define INDUSTRIAL_DUAL_0_20MA_V2_STATUS_LED_CONFIG_OFF 0
#define INDUSTRIAL_DUAL_0_20MA_V2_STATUS_LED_CONFIG_ON 1
#define INDUSTRIAL_DUAL_0_20MA_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define INDUSTRIAL_DUAL_0_20MA_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_CURRENT 1
#define FID_SET_CURRENT_CALLBACK_CONFIGURATION 2
#define FID_GET_CURRENT_CALLBACK_CONFIGURATION 3
#define FID_SET_SAMPLE_RATE 5
#define FID_GET_SAMPLE_RATE 6
#define FID_SET_GAIN 7
#define FID_GET_GAIN 8
#define FID_SET_INFO_LED_CONFIG 9
#define FID_GET_INFO_LED_CONFIG 10
#define FID_SET_CHANNEL_STATUS_LED_CONFIG 11
#define FID_GET_CHANNEL_STATUS_LED_CONFIG 12

#define FID_CALLBACK_CURRENT 4

typedef struct {
	TFPMessageHeader header;
	uint8_t rate;
} __attribute__((__packed__)) SetSampleRate;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetSampleRate;

typedef struct {
	TFPMessageHeader header;
	uint8_t rate;
} __attribute__((__packed__)) GetSampleRate_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t gain;
} __attribute__((__packed__)) SetGain;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetGain;

typedef struct {
	TFPMessageHeader header;
	uint8_t gain;
} __attribute__((__packed__)) GetGain_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	uint8_t config;
} __attribute__((__packed__)) SetInfoLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetInfoLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t config;
} __attribute__((__packed__)) GetInfoLEDConfig_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	int32_t min;
	int32_t max;
	uint8_t config;
} __attribute__((__packed__)) SetChannelStatusLEDConfig;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetChannelStatusLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	int32_t min;
	int32_t max;
	uint8_t config;
} __attribute__((__packed__)) GetChannelStatusLEDConfig_Response;

// Function prototypes
BootloaderHandleMessageResponse set_sample_rate(const SetSampleRate *data);
BootloaderHandleMessageResponse get_sample_rate(const GetSampleRate *data, GetSampleRate_Response *response);
BootloaderHandleMessageResponse set_gain(const SetGain *data);
BootloaderHandleMessageResponse get_gain(const GetGain *data, GetGain_Response *response);
BootloaderHandleMessageResponse set_info_led_config(const SetInfoLEDConfig *data);
BootloaderHandleMessageResponse get_info_led_config(const GetInfoLEDConfig *data, GetInfoLEDConfig_Response *response);
BootloaderHandleMessageResponse set_channel_status_led_config(const SetChannelStatusLEDConfig *data);
BootloaderHandleMessageResponse get_channel_status_led_config(const GetChannelStatusLEDConfig *data, GetChannelStatusLEDConfig_Response *response);

// Callbacks
bool handle_current_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 1
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_current_callback, \

#endif
