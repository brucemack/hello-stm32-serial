#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/SerialDriver.h"

extern "C" {
	UART_HandleTypeDef huart1;
}

// The driver for UART1
SerialDriver sd1(&huart1);

class TestHandler : public MessageHandler {
public:
	void onMessage(const char* m) {
		printf("Got: [%s]\n", m);
		if (m[0] == '#') {
			;
		}
		else if (strlen(m) > 2 && m[0] == 'f') {
			int freq = atoi(&(m[2]));
			printf("Freq = %d\n", freq);
		}
	}
};

TestHandler handler;
static int count = 0;

extern "C" {
	// This function is called by the HAL
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		if (huart->Instance == USART1)
		{
			sd1.onReceive(huart);
		}
	}

	void CppMain_setup() {
		sd1.setMessageHandler(&handler);
		sd1.setEOM("\n");
		sd1.start();
	}

	void CppMain_loop() {
	  // Send a message
	  char msg[80];
	  sprintf(msg,"f %d\n", count++);
	  sd1.send(msg);
	  sprintf(msg,"# comment\n");
	  sd1.send(msg);
	  HAL_Delay(1000);
	}
}
