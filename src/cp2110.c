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
 * @file cp2110.c
 * @author Alex Hiam - <alex@graycat.io>
 *
 * @brief A basic userspcae driver for the CP2110 HID USB-UART IC.
 *
 */

#include <hidapi/hidapi.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "cp2110.h"

struct hid_device_info *CP2110_enumerate(void) {
  return hid_enumerate(CP2110_VID, CP2110_PID); 
}

CP2110_dev *CP2110_init(void) {
  CP2110_dev *handle;
  handle = hid_open(CP2110_VID, CP2110_PID, NULL);
  hid_set_nonblocking(handle, 1);
  return handle;
}

void CP2110_release(CP2110_dev *handle) {
    hid_close(handle);
    hid_exit();
}

int CP2110_uartEnabled(CP2110_dev *handle) {
  int ret;
  uint8_t buf[2];
  buf[0] = REPORT_GET_SET_UART_ENABLE;
  ret = hid_get_feature_report(handle, buf, sizeof(buf));
  if (ret) return buf[1];
  return -1;
}

int CP2110_enableUART(CP2110_dev *handle) {
  int ret;
  uint8_t buf[3];
  buf[0] = REPORT_GET_SET_UART_ENABLE;
  buf[1] = 1;
  ret = hid_send_feature_report(handle, buf, sizeof(buf));
  if (ret > 0) return 1;
  return ret;
}

int CP2110_disableUART(CP2110_dev *handle) {
  int ret;
  uint8_t buf[3];
  buf[0] = REPORT_GET_SET_UART_ENABLE;
  buf[1] = 0;
  ret = hid_send_feature_report(handle, buf, sizeof(buf));
  if (ret > 0) return 1;
  return ret;
}

int CP2110_purgeFIFO(CP2110_dev *handle, CP2110_fifo fifo) {
  int ret;
  uint8_t buf[2];
  buf[0] = REPORT_SET_PURGE_FIFOS;
  ret = hid_get_feature_report(handle, buf, sizeof(buf));
  if (ret) return buf[1];
  return -1;
}

int CP2110_getUARTConfig(CP2110_dev *handle, uint8_t *config) {
  int ret;
  uint8_t buf[9];
  buf[0] = REPORT_GET_SET_UART_CONFIG;
  ret = hid_get_feature_report(handle, buf, sizeof(buf));
  if (ret < 1) return ret;

  printf("baud: %d\n", (unsigned int) (buf[1]<<(8*3) | buf[2]<<(8*2) | buf[3]<<8 | buf[4]));
  printf("parity: %d\n", buf[5]);
  printf("flow control: %d\n", buf[6]);
  printf("data bits: %d\n", buf[7]);
  printf("stop bits: %d\n\n", buf[8]);

  memcpy(config, buf+1, 8);
  return 1;
}


int CP2110_setUARTConfig(CP2110_dev *handle, 
                         uint32_t baud,
                         CP2110_parity parity,
                         CP2110_flow_control flow_control,
                         CP2110_data_bits data_bits,
                         CP2110_stop_bits stop_bits) {
  int ret, i;
  uint8_t buf[9];
  buf[0] = REPORT_GET_SET_UART_CONFIG;
  
  if (baud < 300) baud = 300;
  else if (baud > 500000) baud = 500000;

  // Force LSB-first of baud (see AN434 section 2.2):
  for (i=0; i<4; i++) {
    buf[i+1] = 0xff & (baud >> (i*8));
  }

  buf[5] = parity;
  buf[6] = flow_control;
  buf[7] = data_bits;
  buf[8] = stop_bits;

  ret = hid_send_feature_report(handle, buf, sizeof(buf));
  printf("UART config ret = %d\n", ret);
  if (ret > 0) return 1;
  return ret;
}


int CP2110_write(CP2110_dev *handle, char *data, int len) {
  int ret, index, n_sent;
  uint8_t buf[REPORT_DATA_MAX];
  n_sent = 0;
  index = 0;
  while (len >= REPORT_DATA_MAX) {
    buf[0] = REPORT_DATA_MAX;
    buf[1] = data[index];
    ret = hid_write(handle, buf, sizeof(buf));
    if (ret < 0) return ret;
    n_sent += ret-1;
    if (ret < REPORT_DATA_MAX) {
      // Not all bytes were written, assume error and return
      return n_sent;
    }
    index += REPORT_DATA_MAX;
    len -= REPORT_DATA_MAX;
  }
  if (len) {
    buf[0] = len;
    buf[1] = data[index];
    ret = hid_write(handle, buf, len+1);
    if (ret < 0) return ret;
    n_sent += ret-1;
  }
  return n_sent;
}


int CP2110_read(CP2110_dev *handle, char *rx_buf, int len) {
  int ret, index, n_read;
  uint8_t buf[REPORT_DATA_MAX];
  n_read = 0;
  index = 0;

  while (len >= REPORT_DATA_MAX) {
    buf[0] = REPORT_DATA_MAX;
    ret = hid_read(handle, buf, sizeof(buf));
    if (ret < 0) return ret;
    n_read += ret ? ret-1 : 0;

    if (ret) memcpy(rx_buf+index, buf, n_read);

    if (ret < REPORT_DATA_MAX) {
      // Not all bytes were written, assume error and return
      return n_read;
    }
    index += REPORT_DATA_MAX;
    len -= REPORT_DATA_MAX;
  }
  if (len) {
    buf[0] = len;
    ret = hid_read(handle, buf, len+1);
    if (ret < 0) return ret;
    n_read += ret ? ret-1 : 0;

    if (ret) memcpy(rx_buf+index, buf, n_read);
  }
  return n_read;
}
