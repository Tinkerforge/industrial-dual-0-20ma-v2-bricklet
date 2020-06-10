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

uint8_t current_callback_channel = 0;
CallbackValue_int32_t callback_values_current[CALLBACK_VALUE_CHANNEL_NUM];

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_CURRENT: return get_callback_value_int32_t(message, response, callback_values_current);
		case FID_SET_CURRENT_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_int32_t(message, callback_values_current);
		case FID_GET_CURRENT_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_int32_t(message, response, callback_values_current);
		case FID_SET_SAMPLE_RATE: return set_sample_rate(message);
		case FID_GET_SAMPLE_RATE: return get_sample_rate(message, response);
		case FID_SET_GAIN: return set_gain(message);
		case FID_GET_GAIN: return get_gain(message, response);
		case FID_SET_CHANNEL_LED_CONFIG: return set_channel_led_config(message);
		case FID_GET_CHANNEL_LED_CONFIG: return get_channel_led_config(message, response);
		case FID_SET_CHANNEL_LED_STATUS_CONFIG: return set_channel_led_status_config(message);
		case FID_GET_CHANNEL_LED_STATUS_CONFIG: return get_channel_led_status_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_sample_rate(const SetSampleRate *data) {
	switch(data->rate) {
		case INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_4_SPS:
			mcp3423.cfg_sps_new = (uint8_t)MCP3423_CONF_MSK_SPS4;
			break;

		case INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_15_SPS:
			mcp3423.cfg_sps_new = (uint8_t)MCP3423_CONF_MSK_SPS15;
			break;

		case INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_60_SPS:
			mcp3423.cfg_sps_new = (uint8_t)MCP3423_CONF_MSK_SPS60;
			break;

		case INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_240_SPS:
			mcp3423.cfg_sps_new = (uint8_t)MCP3423_CONF_MSK_SPS240;
			break;

		default:
			return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_sample_rate(const GetSampleRate *data, GetSampleRate_Response *response) {
	response->header.length = sizeof(GetSampleRate_Response);

	switch(mcp3423.cfg_sps) {
		case MCP3423_CONF_MSK_SPS4:
			response->rate = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_4_SPS;
			break;

		case MCP3423_CONF_MSK_SPS15:
			response->rate = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_15_SPS;
			break;

		case MCP3423_CONF_MSK_SPS60:
			response->rate = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_60_SPS;
			break;

		case MCP3423_CONF_MSK_SPS240:
			response->rate = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_SAMPLE_RATE_240_SPS;
			break;

		default:
			break;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gain(const SetGain *data) {
	switch(data->gain) {
		case INDUSTRIAL_DUAL_0_20MA_V2_GAIN_1X:
			mcp3423.cfg_gain_new = (uint8_t)MCP3423_CONF_MSK_Gx1;
			break;

		case INDUSTRIAL_DUAL_0_20MA_V2_GAIN_2X:
			mcp3423.cfg_gain_new = (uint8_t)MCP3423_CONF_MSK_Gx2;
			break;

		case INDUSTRIAL_DUAL_0_20MA_V2_GAIN_4X:
			mcp3423.cfg_gain_new = (uint8_t)MCP3423_CONF_MSK_Gx4;
			break;

		case INDUSTRIAL_DUAL_0_20MA_V2_GAIN_8X:
			mcp3423.cfg_gain_new = (uint8_t)MCP3423_CONF_MSK_Gx8;
			break;

		default:
			return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gain(const GetGain *data, GetGain_Response *response) {
	response->header.length = sizeof(GetGain_Response);

	switch(mcp3423.cfg_gain) {
		case MCP3423_CONF_MSK_Gx1:
			response->gain = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_GAIN_1X;
			break;

		case MCP3423_CONF_MSK_Gx2:
			response->gain = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_GAIN_2X;
			break;

		case MCP3423_CONF_MSK_Gx4:
			response->gain = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_GAIN_4X;
			break;

		case MCP3423_CONF_MSK_Gx8:
			response->gain = (uint8_t)INDUSTRIAL_DUAL_0_20MA_V2_GAIN_8X;
			break;

		default:
			break;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_channel_led_config(const SetChannelLEDConfig *data) {
	if(data->channel > CALLBACK_VALUE_CHANNEL_NUM - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	mcp3423.channel_leds[data->channel].config = data->config;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_channel_led_config(const GetChannelLEDConfig *data, GetChannelLEDConfig_Response *response) {
	response->header.length = sizeof(GetChannelLEDConfig_Response);

	if(data->channel > CALLBACK_VALUE_CHANNEL_NUM - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->config = mcp3423.channel_leds[data->channel].config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_channel_led_status_config(const SetChannelLEDStatusConfig *data) {
	if(data->channel > CALLBACK_VALUE_CHANNEL_NUM - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if (data->min > 22505322 || data->max > 22505322) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	mcp3423.channel_leds[data->channel].min = data->min;
	mcp3423.channel_leds[data->channel].max = data->max;
	mcp3423.channel_leds[data->channel].config_ch_status = data->config;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_channel_led_status_config(const GetChannelLEDStatusConfig *data, GetChannelLEDStatusConfig_Response *response) {
	response->header.length = sizeof(GetChannelLEDStatusConfig_Response);

	if(data->channel > CALLBACK_VALUE_CHANNEL_NUM - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->min = mcp3423.channel_leds[data->channel].min;
	response->max = mcp3423.channel_leds[data->channel].max;
	response->config = mcp3423.channel_leds[data->channel].config_ch_status;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_current_callback(void) {
	bool ret = handle_callback_value_callback_int32_t(callback_values_current, current_callback_channel, FID_CALLBACK_CURRENT);

	if(++current_callback_channel >= CALLBACK_VALUE_CHANNEL_NUM) {
		current_callback_channel = 0;
	}

	return ret;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_int32_t(callback_values_current, mcp3423_get_current);

	communication_callback_init();
}
