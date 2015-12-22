#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hidapi/hidapi.h>
#include "cp2110.h"

#define MAX_STR 255


int main() {
  int ret, i;
  struct hid_device_info *devs;
  CP2110_dev *cp2110;
  char tx_buf[MAX_STR];
  char rx_buf[MAX_STR];
  wchar_t wsrt[MAX_STR];

  devs = CP2110_enumerate();

  if (!devs) {
    puts("*no CP2110 devices found");
    exit(0);
  }
  hid_free_enumeration(devs);

  puts("CP2110 found, connecting...");

  cp2110 = CP2110_init();

  if (!cp2110) {
    puts("*could not connect");
    exit(0);
  }

  puts("Connected!");

  // Read the Manufacturer String
  ret = hid_get_manufacturer_string(cp2110, wsrt, MAX_STR);
  printf("Manufacturer String: %ls\n", wsrt);

  // Read the Product String
  ret = hid_get_product_string(cp2110, wsrt, MAX_STR);
  printf("Product String: %ls\n", wsrt);

  // Read the Serial Number String
  ret = hid_get_serial_number_string(cp2110, wsrt, MAX_STR);
  printf("Serial Number String: %ls", wsrt);
  printf("\n");

  CP2110_disableUART(cp2110);

  CP2110_purgeFIFO(cp2110, FIFO_BOTH);

  unsigned char buf[8];

  ret = CP2110_getUARTConfig(cp2110, buf);

  puts("setting UART config");
  ret = CP2110_setUARTConfig(cp2110, 
                             9600,
                             PARITY_NONE,
                             FLOW_CONTROL_DISABLED,
                             DATA_BITS_8,
                             STOP_BITS_SHORT);
  if (ret < 0) {
    puts("*Couldn't configure UART");
  }
  else puts("UART configured!");

  ret = CP2110_getUARTConfig(cp2110, buf);


  if (CP2110_uartEnabled(cp2110) < 1) {
    puts("UART not enabled, enabling...");

    if (CP2110_enableUART(cp2110) < 1) {
      puts("*Could not enable UART");
      CP2110_release(cp2110);
      exit(0);
    }
    else puts("UART enabled!");
  }
  else puts("UART already enabled");

  strcpy(tx_buf, "abc");
  printf("Sending message: %s\n", tx_buf);
  ret = CP2110_write(cp2110, tx_buf, strlen(tx_buf));
  printf("return: %d\n", ret);

  puts("reading...");
  ret = CP2110_read(cp2110, rx_buf, 10);

  printf("return: %d\n", ret);
  printf("data: ");
  for (i=0; i<ret; i++) {
    printf("%02hhx  ", rx_buf[i]);
  }
  puts("");

  puts("Releasing CP2110");
  CP2110_release(cp2110);

  return 0;
}
