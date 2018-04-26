/* industrial-dual-0-20ma-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"

#include "mcp3423.h"

uint8_t on_channel_handle_channel = 0;
CallbackValue on_channel_cbv[ON_CHANNEL_COUNT];

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_CURRENT: {
			uint8_t channel = ((OnChannelGetCallbackValue *)message)->channel;
			return on_channel_get_callback_value(message, response, &on_channel_cbv[channel]);
		}
		case FID_SET_CURRENT_CALLBACK_CONFIGURATION: {
			uint8_t channel = ((OnChannelSetCallbackValueCallbackConfiguration *)message)->channel;
			return on_channel_set_callback_value_callback_configuration(message, &on_channel_cbv[channel]);
		}
		case FID_GET_CURRENT_CALLBACK_CONFIGURATION: {
			uint8_t channel = ((OnChannelGetCallbackValueCallbackConfiguration *)message)->channel;
			return on_channel_get_callback_value_callback_configuration(message, response, &on_channel_cbv[channel]);
		}
		case FID_SET_SAMPLE_RATE: return set_sample_rate(message);
		case FID_SET_GAIN: return set_gain(message);
		case FID_GET_GAIN: return get_gain(message, response);
		case FID_SET_INFO_LED_CONFIG: return set_info_led_config(message);
		case FID_GET_INFO_LED_CONFIG: return get_info_led_config(message, response);
		case FID_SET_CHANNEL_STATUS_LED_CONFIG: return set_channel_status_led_config(message);
		case FID_GET_CHANNEL_STATUS_LED_CONFIG: return get_channel_status_led_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_sample_rate(const SetSampleRate *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_gain(const SetGain *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gain(const GetGain *data, GetGain_Response *response) {
	response->header.length = sizeof(GetGain_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_info_led_config(const SetInfoLEDConfig *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_info_led_config(const GetInfoLEDConfig *data, GetInfoLEDConfig_Response *response) {
	response->header.length = sizeof(GetInfoLEDConfig_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_channel_status_led_config(const SetChannelStatusLEDConfig *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_channel_status_led_config(const GetChannelStatusLEDConfig *data, GetChannelStatusLEDConfig_Response *response) {
	response->header.length = sizeof(GetChannelStatusLEDConfig_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_current_callback(void) {
	bool ret = on_channel_handle_callback_value_callback(&on_channel_cbv[on_channel_handle_channel],
	                                                     FID_CALLBACK_CURRENT,
	                                                     on_channel_handle_channel);

	if(!(++on_channel_handle_channel < ON_CHANNEL_COUNT)) {
		on_channel_handle_channel = 0;
	}

	return ret;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	for(uint8_t i = 0; i < ON_CHANNEL_COUNT; i++) {
		on_channel_callback_value_init(&on_channel_cbv[i], mcp3423_get_current);
	}

	communication_callback_init();
}
