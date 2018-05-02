/* industrial-dual-0-20ma-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * mcp3423.c: MCP3423 driver
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

#include "mcp3423.h"

#include "bricklib2/logging/logging.h"
#include "configs/config_mcp3423.h"

#include "communication.h"

MCP3423_t mcp3423;

static void do_read(void) {
	XMC_USIC_CH_RXFIFO_Flush(mcp3423.i2c_fifo.i2c);
	XMC_USIC_CH_TXFIFO_Flush(mcp3423.i2c_fifo.i2c);

	if(mcp3423.cfg_sps == (uint8_t)MCP3423_CONF_MSK_SPS4) {
		i2c_fifo_read_direct(&mcp3423.i2c_fifo, 4, false);
	}
	else {
		i2c_fifo_read_direct(&mcp3423.i2c_fifo, 3, false);
	}
}

void mcp3423_init(void) {
	logd("[+] mcp3423_init()\n\r");

	// Initial config
	mcp3423.ch0_current = 0;
	mcp3423.ch1_current = 0;
	mcp3423.cfg_update = false;
	mcp3423.cfg_sps = (uint8_t)MCP3423_CONF_MSK_SPS4;
	mcp3423.cfg_gain = (uint8_t)MCP3423_CONF_MSK_Gx1;

	// Setup I2C FIFO
	mcp3423.i2c_fifo.baudrate = MCP3423_I2C_BAUDRATE;
	mcp3423.i2c_fifo.i2c = MCP3423_I2C;
	mcp3423.i2c_fifo.scl_port = MCP3423_SCL_PORT;
	mcp3423.i2c_fifo.scl_pin = MCP3423_SCL_PIN;
	mcp3423.i2c_fifo.scl_mode = MCP3423_SCL_PIN_MODE;
	mcp3423.i2c_fifo.scl_input = MCP3423_SCL_INPUT;
	mcp3423.i2c_fifo.scl_source = MCP3423_SCL_SOURCE;
	mcp3423.i2c_fifo.scl_fifo_size = MCP3423_SCL_FIFO_SIZE;
	mcp3423.i2c_fifo.scl_fifo_pointer = MCP3423_SCL_FIFO_POINTER;

	mcp3423.i2c_fifo.sda_port = MCP3423_SDA_PORT;
	mcp3423.i2c_fifo.sda_pin = MCP3423_SDA_PIN;
	mcp3423.i2c_fifo.sda_mode = MCP3423_SDA_PIN_MODE;
	mcp3423.i2c_fifo.sda_input = MCP3423_SDA_INPUT;
	mcp3423.i2c_fifo.sda_source = MCP3423_SDA_SOURCE;
	mcp3423.i2c_fifo.sda_fifo_size = MCP3423_SDA_FIFO_SIZE;
	mcp3423.i2c_fifo.sda_fifo_pointer = MCP3423_SDA_FIFO_POINTER;

	// Set slave address
	mcp3423.i2c_fifo.address = (uint8_t)MCP3423_I2C_ADDRESS;

	i2c_fifo_init(&mcp3423.i2c_fifo);

	// Start reading channel 0
	mcp3423.i2c_fifo_buf[0] = \
		(MCP3423_CONF_MSK_WR_DEFAULT | MCP3423_CONF_MSK_CH0 | mcp3423.cfg_sps | mcp3423.cfg_gain);

	i2c_fifo_write_direct(&mcp3423.i2c_fifo, 1, &mcp3423.i2c_fifo_buf[0], true);

	mcp3423.sm = S_READ_CH_0_W;
}

void mcp3423_tick(void) {
	uint8_t config = 0;
	uint32_t raw_current = 0;

	I2CFifoState fifo_state = i2c_fifo_next_state(&mcp3423.i2c_fifo);

	if(fifo_state & I2C_FIFO_STATE_ERROR) {
		loge("[+] mcp3423_tick(): I2C HW Status = %d, I2C FIFO Status = %d\n\r",
		     (uint32_t)mcp3423.i2c_fifo.i2c_status,
		     fifo_state);

		mcp3423_init();

		return;
	}

	switch(mcp3423.sm) {
		case S_READ_CH_0_W: {
			if(fifo_state == I2C_FIFO_STATE_WRITE_DIRECT_READY) {
				do_read();

				mcp3423.sm = S_READ_CH_0_R;
			}

			break;
		}

		case S_READ_CH_0_R: {
			if(fifo_state == I2C_FIFO_STATE_READ_DIRECT_READY) {
				if(mcp3423.cfg_sps == (uint8_t)MCP3423_CONF_MSK_SPS4) {
					i2c_fifo_read_fifo(&mcp3423.i2c_fifo,
					                   &mcp3423.i2c_fifo_buf[0],
					                   4);

					config = mcp3423.i2c_fifo_buf[3];
				}
				else {
					i2c_fifo_read_fifo(&mcp3423.i2c_fifo,
					                   &mcp3423.i2c_fifo_buf[0],
					                   3);

					config = mcp3423.i2c_fifo_buf[2];
				}

				if(!(config & 0x80)) {
					logd("[+] mcp3423_tick(): READ CH0: i2c_fifo_buf[0] = %d\n\r", mcp3423.i2c_fifo_buf[0]);
					logd("[+] mcp3423_tick(): READ CH0: i2c_fifo_buf[1] = %d\n\r", mcp3423.i2c_fifo_buf[1]);
					logd("[+] mcp3423_tick(): READ CH0: i2c_fifo_buf[2] = %d\n\r", mcp3423.i2c_fifo_buf[2]);
					logd("[+] mcp3423_tick(): READ CH0: i2c_fifo_buf[3] = %d\n\r\n\r", mcp3423.i2c_fifo_buf[3]);

					// Channel 0 conversion is ready

					// TODO: Calculate and store the value

					// Start reading channel 1
					XMC_USIC_CH_RXFIFO_Flush(mcp3423.i2c_fifo.i2c);
					XMC_USIC_CH_TXFIFO_Flush(mcp3423.i2c_fifo.i2c);

					mcp3423.i2c_fifo_buf[0] = \
						(MCP3423_CONF_MSK_WR_DEFAULT | MCP3423_CONF_MSK_CH1 | mcp3423.cfg_sps | mcp3423.cfg_gain);

					i2c_fifo_write_direct(&mcp3423.i2c_fifo, 1, &mcp3423.i2c_fifo_buf[0], true);

					mcp3423.sm = S_READ_CH_1_W;
				}
				else {
					// Channel 0 conversion is not ready, keep reading
					do_read();
				}
			}

			break;
		}

		case S_READ_CH_1_W: {
			if(fifo_state == I2C_FIFO_STATE_WRITE_DIRECT_READY) {
				do_read();

				mcp3423.sm = S_READ_CH_1_R;
			}

			break;
		}

		case S_READ_CH_1_R: {
			if(fifo_state == I2C_FIFO_STATE_READ_DIRECT_READY) {
				if(mcp3423.cfg_sps == (uint8_t)MCP3423_CONF_MSK_SPS4) {
					i2c_fifo_read_fifo(&mcp3423.i2c_fifo,
					                   &mcp3423.i2c_fifo_buf[0],
					                   4);

					config = mcp3423.i2c_fifo_buf[3];
				}
				else {
					i2c_fifo_read_fifo(&mcp3423.i2c_fifo,
					                   &mcp3423.i2c_fifo_buf[0],
					                   3);

					config = mcp3423.i2c_fifo_buf[2];
				}

				XMC_USIC_CH_RXFIFO_Flush(mcp3423.i2c_fifo.i2c);
				XMC_USIC_CH_TXFIFO_Flush(mcp3423.i2c_fifo.i2c);

				if(!(config & 0x80)) {
					logd("[+] mcp3423_tick(): READ CH1: i2c_fifo_buf[0] = %d\n\r", mcp3423.i2c_fifo_buf[0]);
					logd("[+] mcp3423_tick(): READ CH1: i2c_fifo_buf[1] = %d\n\r", mcp3423.i2c_fifo_buf[1]);
					logd("[+] mcp3423_tick(): READ CH1: i2c_fifo_buf[2] = %d\n\r", mcp3423.i2c_fifo_buf[2]);
					logd("[+] mcp3423_tick(): READ CH1: i2c_fifo_buf[3] = %d\n\r\n\r", mcp3423.i2c_fifo_buf[3]);

					// Channel 1 conversion is ready

					// TODO: Calculate and store the value

					// We will start a new cycle, update config if required
					if(mcp3423.cfg_update) {
						mcp3423.cfg_sps = mcp3423.cfg_sps_new;
						mcp3423.cfg_gain = mcp3423.cfg_gain_new;
						mcp3423.cfg_update = false;
					}

					// Start reading channel 0
					XMC_USIC_CH_RXFIFO_Flush(mcp3423.i2c_fifo.i2c);
					XMC_USIC_CH_TXFIFO_Flush(mcp3423.i2c_fifo.i2c);

					mcp3423.i2c_fifo_buf[0] = \
						(MCP3423_CONF_MSK_WR_DEFAULT | MCP3423_CONF_MSK_CH0 | mcp3423.cfg_sps | mcp3423.cfg_gain);

					i2c_fifo_write_direct(&mcp3423.i2c_fifo, 1, &mcp3423.i2c_fifo_buf[0], true);

					mcp3423.sm = S_READ_CH_0_W;
				}
				else {
					// Channel 1 conversion is not ready, keep reading
					do_read();
				}
			}

			break;
		}

		default:
			break;
	}
}

int32_t mcp3423_get_current(uint8_t channel) {
	if(channel == 0) {
		return mcp3423.ch0_current;
	}
	else if (channel == 1) {
		return mcp3423.ch1_current;
	}
	else {
		return 0;
	}
}
