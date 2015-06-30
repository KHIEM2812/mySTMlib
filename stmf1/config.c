/*
 This file is part of AutoQuad.

 AutoQuad is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AutoQuad is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with AutoQuad.  If not, see <http://www.gnu.org/licenses/>.

 Copyright © 2011, 2012, 2013  Bill Nesbitt
 */

#include "config.h"

float p[CONFIG_NUM_PARAMS] __attribute__((section(".ccm")));

const char *configParameterStrings[] = { "CONFIG_VERSION", "NOWEIGHTLV",
		"EMPTYWEIGHTLV", "FULLWEIGHTLV", "WEIGHTTOLLERANCE" };

//load the config defined in IBox.h
void configLoadDefault(void) {
	p[CONFIG_VERSION] = DEFAULT_CONFIG_VERSION;
	p[NOWEIGHTLV]  =  DEFAULT_NOWEIGHTLV;
	p[EMPTYWEIGHTLV]  =  DEFAULT_EMPTYWEIGHTLV;
	p[FULLWEIGHTLV]  =  DEFAULT_FULLWEIGHTLV;
	p[WEIGHTTOLLERANCE]  =  DEFAULT_WEIGHTTOLLERANCE;
}
//read the config from flash n save to the buffer
void configFlashRead(void) {
	memcpy(&p, (char *) flashStartAddr(), sizeof(p));
}
//write the config from the buffer n save to flash
unsigned char configFlashWrite(void) {
	return flashAddress(flashStartAddr(), (uint32_t *) &p, sizeof(p));
}

/*
 * get the default config from memory
 */
void ConfigInit(void) {
	float ver;

	// start with what's in flash
	configFlashRead();

	// get flash version
	ver = *(float *) flashStartAddr();
	if (isnan(ver))
		ver = 0.0f;
	//newer one defined in the code, old version in flash
	if (DEFAULT_CONFIG_VERSION > ver) {
		configLoadDefault(); //use newer one defined in the code
		configFlashWrite(); // save it into the flash memory
	}
}

unsigned int configParameterRead(void *data) {
	paramStruct_t *par = (paramStruct_t *) data;

	if (par->paramId + par->num > CONFIG_NUM_PARAMS)
		par->num = CONFIG_NUM_PARAMS - par->paramId;

	memcpy((char *) par->values, (char *) &p[par->paramId],
			par->num * sizeof(float));

	return par->num * sizeof(float);
}

unsigned int configParameterWrite(void *data) {
	paramStruct_t *par = (paramStruct_t *) data;

	memcpy((char *) &p[par->paramId], (char *) par->values,
			par->num * sizeof(float));

	return configParameterRead(data);
}

void configSetParamByID(int id, float value) {
	p[id] = value;
}
