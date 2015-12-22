/**************************************************************************
 * Copyright (c) 2015 - Gray Cat Labs - https://graycat.io
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

/**
 * @file cp2110.h
 * @author Alex Hiam - <alex@graycat.io>
 *
 * @brief A basic userspcae driver for the CP2110 HID USB-UART IC.
 *
 */

#ifndef _CP2110_H_
#define _CP2110_H_

#include <stdint.h>
#include <hidapi/hidapi.h>

#define CP2110_VID 0x10c4
#define CP2110_PID 0xea80

#define REPORT_DATA     0x01
#define REPORT_DATA_MAX 0x3F

#define REPORT_GET_SET_UART_ENABLE 0x41
#define REPORT_SET_PURGE_FIFOS     0x43
#define REPORT_GET_SET_UART_CONFIG 0x50

typedef hid_device CP2110_dev;

struct hid_device_info *CP2110_enumerate(void);
CP2110_dev *CP2110_init(void);
void CP2110_release(CP2110_dev *handle);
int CP2110_uartEnabled(CP2110_dev *handle);
int CP2110_enableUART(CP2110_dev *handle);
int CP2110_disableUART(CP2110_dev *handle);

typedef enum CP2110_fifo {
  FIFO_TX = 1,
  FIFO_RX,
  FIFO_BOTH
} CP2110_fifo;

int CP2110_purgeFIFO(CP2110_dev *handle, CP2110_fifo fifo);


typedef enum CP2110_parity {
  PARITY_NONE,
  PARITY_EVEN,
  PARITY_ODD,
  PARITY_MARK,
  PARITY_SPACE
} CP2110_parity;

typedef enum cp2110_flow_control {
  FLOW_CONTROL_DISABLED,
  FLOW_CONTROL_ENABLED
} CP2110_flow_control;

typedef enum cp2110_data_bits {
  DATA_BITS_5 = 0x05,
  DATA_BITS_6,
  DATA_BITS_7,
  DATA_BITS_8
} CP2110_data_bits;

typedef enum cp2110_stop_bits {
  STOP_BITS_SHORT,
  STOP_BITS_LONG
} CP2110_stop_bits;

int CP2110_getUARTConfig(CP2110_dev *handle, uint8_t *config);
int CP2110_setUARTConfig(CP2110_dev *handle, 
                         uint32_t baud,
                         CP2110_parity parity,
                         CP2110_flow_control flow_control,
                         CP2110_data_bits data_bits,
                         CP2110_stop_bits stop_bits);

int CP2110_write(CP2110_dev *handle, char data[], int len);
int CP2110_read(CP2110_dev *handle, char *rx_buf, int len);

#endif