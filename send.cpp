/*
 * Usage: ./send <systemCode> <unitCode> <command>
 * Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SYSTEM_CODE_LEN 5

int main(int argc, char *argv[]) {

  /*
   * output PIN is hardcoded for testing purposes
   * see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
   * for pin mapping of the raspberry pi GPIO connector
   */
  int PIN = 0;
  char     systemCode[SYSTEM_CODE_LEN + 1];
  int      unitCode;
  int      command;
  int      i;
  RCSwitch mySwitch = RCSwitch();

  if (argc <= 3) {
    printf ("usage: send <systemCode> <unitCode> <command>\n");
      goto err;
  }

  if (strlen (argv[1]) != 5) {
    printf ("Systemcode must be 5 bit binary code!\n");
    goto err;
  }

  memset (systemCode, 0, SYSTEM_CODE_LEN + 1);
  strncpy (systemCode, argv[1], SYSTEM_CODE_LEN);
  unitCode   = atoi(argv[2]);
  command    = atoi(argv[3]);

  /*iterate over systemCode and check whether it is 5 bytes of 0s and 1s */
  for (i = 0; i < SYSTEM_CODE_LEN; i++) {
    if (systemCode[i] != '0' && systemCode[i] != '1') {
      printf ("SystemCode must be 5 bit binary code!\n");
      goto err;
    }
  }

  if (wiringPiSetup () == -1) {
    printf ("wiringPiSetup failed\n");
    goto err;
  }

  piHiPri(20);
  printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
  mySwitch.setPulseLength(300);
  mySwitch.enableTransmit(PIN);

  switch(command) {
    case 0:
      mySwitch.switchOff(systemCode, unitCode);
      break;
    case 1:
      mySwitch.switchOn(systemCode, unitCode);
      break;
    case 2:
      // 00001 2 on binary coded
      mySwitch.send("010101010001000101010001");
      break;
    case 3:
      // 00001 2 on as TriState
      mySwitch.sendTriState("FFFF0F0FFF0F");
      break;
    default:
      printf("command[%i] is unsupported\n", command);
      goto err;
  }
  return 0;
err:
  return -1;
}
