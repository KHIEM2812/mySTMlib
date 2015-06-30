#include "cCommandGetter.h"
#include "myLib/cUart.h"
#include <string.h>

#define CMDSTRINGSIZE	10

typedef enum {
	RESET_CMDGETTER_STATE, GETTING_CMDGETTER_STATE, RETURN_CMDGETTER_STATE
} StateCmdGetterEnum;

char cmdString[CMDSTRINGSIZE];
int myindex;
StateCmdGetterEnum stateCmdGetter;

void ClearCmdStringBuff() {
	memset(cmdString, 0, CMDSTRINGSIZE);
	myindex = 0;
}

void CommandGetterInitialize() {
	ClearCmdStringBuff();
	stateCmdGetter = RESET_CMDGETTER_STATE;
}

int CommandGetter(char inputChar, char * outputStrCmd) {
	char ch = inputChar;
	switch (stateCmdGetter) {
	case RESET_CMDGETTER_STATE:
		myindex = 0;
		if (ch == '?') {
			ClearCmdStringBuff();
			stateCmdGetter = GETTING_CMDGETTER_STATE;
		}
		break;
	case GETTING_CMDGETTER_STATE:
		if (ch != '\0') {
			cmdString[myindex] = ch;
			myindex++;
			if (ch != '\r') {
				UartPrint("%c",ch);
			} else{
				UartPrint("\r\n",0);
			}

			if (ch == '\r') {
				cmdString[myindex-1] = '\0';//to replace '\r'
				stateCmdGetter = RETURN_CMDGETTER_STATE;
			}
		}
		break;
	case RETURN_CMDGETTER_STATE:
		memcpy(outputStrCmd, cmdString, myindex);
		ClearCmdStringBuff();
		stateCmdGetter = RESET_CMDGETTER_STATE;
		break;
	}
	return myindex;
}
