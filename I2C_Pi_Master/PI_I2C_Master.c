//#########################################################
//## PI_I2C_Master.c 
//## Created by Kenny from www.iotbreaks.vn, April 29, 2016.
//## Released into the public domain.
//## Tutorial: 
//#########################################################
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <wiringPi.h>
#include <errno.h>
#include <wiringPiI2C.h>

#define MOTOR_SWITCH_INPUT_PIN 21
#define LED_OUTPUT_PIN 22
#define TRANSMIT_BUTTON_INPUT_PIN 26

void transmitTrigger(void);
void wiPiTransmitTrigger(void);

int sendByteCount = 0;
uint8_t ledValue = 0;
uint8_t readLedPeriod = 10;
int realDelay=100;

unsigned char motorSwitchChar;
int main(int argc, char *argv[]) {

  	if (wiringPiSetup () < 0) {
  	    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
  	    return 1;
  	}
	printf("Start\n");	
    pinMode(LED_OUTPUT_PIN, OUTPUT);     
    pinMode(MOTOR_SWITCH_INPUT_PIN, INPUT);
    pinMode(TRANSMIT_BUTTON_INPUT_PIN, INPUT);

	while ( 1 ) {
		delay(readLedPeriod);
		ledValue = !ledValue ;
		digitalWrite(LED_OUTPUT_PIN, ledValue);
		if(digitalRead(TRANSMIT_BUTTON_INPUT_PIN)){
			printf("Button pressed\n");
			delay(100);
			transmitTrigger();
		}
	}
	return 1;
}

void transmitTrigger(void) {
	printf("Transmit Start\n");
	uint8_t motorSwitchValue = digitalRead(MOTOR_SWITCH_INPUT_PIN);
	unsigned char motorSwitchChar = motorSwitchValue?'H':'L';

    int fileDescriptor = wiringPiI2CSetup(0x08);
  	if (fileDescriptor < 0) {
  	    fprintf (stderr, "Unable to setup I2C with wiringPi: %s\n", strerror (errno));
  	    return; 
  	}
	
	int txStatus;

	//write
	printf("Send Byte = %d, Send Char = %c\n", motorSwitchChar, motorSwitchChar);
	txStatus = wiringPiI2CWrite(fileDescriptor, motorSwitchChar);
	printf("write result = %d\n", txStatus);
	
	delay(300);
	//read
	readLedPeriod = wiringPiI2CRead(fileDescriptor);	
	printf("Read Byte = %d,  Char = %c\n", readLedPeriod, readLedPeriod);
	
	close(fileDescriptor);
	printf("Transmit Finish\n");
	printf("readLedPeriod = %d\n", readLedPeriod);
	printf("sendByteCount = %d\n======================\n", ++sendByteCount);
}


