#include "TestcCommandGetter.h"
#include "cCommandGetter.h"
#include "myLib/cUart.h"
#include <string.h>

void TestcCommandGetterConfig() {
	cUartConfig(3);
	CommandGetterInitialize();
}
void TestcCommandGetter() {
	char cmdStr[10];
	memset(cmdStr,0,10);
	char ch = ReadUart();
	CommandGetter(ch, cmdStr);
	if (cmdStr[0] != '\0') {
		UartPrint("get cmd: %s\r\n", cmdStr);
	}
}
