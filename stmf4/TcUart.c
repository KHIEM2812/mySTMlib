#include "myLib/TcUart.h"

void TPrinf() {
	UartPrint("hello\r\n");
}
void TPrinfFloat() {
	static float displayNum = 1000.0;
	displayNum /= 2;
	if (displayNum == 0) {
		displayNum = 1000.0;
	}
	UartPrint("current Number = %.2f", displayNum);
}
void TIsUsartTransferComplete();
void TcUartMirroInputNOutput() {
	char ret = ReadUart();
	if (ret != '\0')
		UartPrint("%c", ret);
}
