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

 Copyright © 2011, 2012  Bill Nesbitt
 */

#include "flash.h"

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t NbrOfPage = 0x00;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

// Gets the sector of a given address
uint32_t GetSector(uint32_t Address) {
	uint32_t sector = 0;

	if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)) {
		sector = FLASH_Sector_0;
	} else if ((Address < ADDR_FLASH_SECTOR_2)
			&& (Address >= ADDR_FLASH_SECTOR_1)) {
		sector = FLASH_Sector_1;
	} else if ((Address < ADDR_FLASH_SECTOR_3)
			&& (Address >= ADDR_FLASH_SECTOR_2)) {
		sector = FLASH_Sector_2;
	} else if ((Address < ADDR_FLASH_SECTOR_4)
			&& (Address >= ADDR_FLASH_SECTOR_3)) {
		sector = FLASH_Sector_3;
	} else if ((Address < ADDR_FLASH_SECTOR_5)
			&& (Address >= ADDR_FLASH_SECTOR_4)) {
		sector = FLASH_Sector_4;
	} else if ((Address < ADDR_FLASH_SECTOR_6)
			&& (Address >= ADDR_FLASH_SECTOR_5)) {
		sector = FLASH_Sector_5;
	} else if ((Address < ADDR_FLASH_SECTOR_7)
			&& (Address >= ADDR_FLASH_SECTOR_6)) {
		sector = FLASH_Sector_6;
	} else if ((Address < ADDR_FLASH_SECTOR_8)
			&& (Address >= ADDR_FLASH_SECTOR_7)) {
		sector = FLASH_Sector_7;
	} else if ((Address < ADDR_FLASH_SECTOR_9)
			&& (Address >= ADDR_FLASH_SECTOR_8)) {
		sector = FLASH_Sector_8;
	} else if ((Address < ADDR_FLASH_SECTOR_10)
			&& (Address >= ADDR_FLASH_SECTOR_9)) {
		sector = FLASH_Sector_9;
	} else if ((Address < ADDR_FLASH_SECTOR_11)
			&& (Address >= ADDR_FLASH_SECTOR_10)) {
		sector = FLASH_Sector_10;
	} else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/{
		sector = FLASH_Sector_11;
	}

	return sector;
}

int flashAddress(uint32_t startAddr, uint32_t *data, unsigned int len) {
	unsigned int retries;
	int ret;
	unsigned int i;

	FLASH_Unlock();

	NbrOfPage = (FLASH_END_ADDR - FLASH_START_ADDR) / FLASH_PAGE_SIZE;

	// Clear pending flags (if any)
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	ret = 1;

	for (EraseCounter = 0;
			(EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE);
			EraseCounter++) {
		retries = 0;
		do {
			// Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word
			FLASHStatus = FLASH_ErasePage(
					FLASH_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
			retries++;
		} while (FLASHStatus != FLASH_COMPLETE && retries < FLASH_RETRIES);

		if (retries == FLASH_RETRIES) {
			ret = 0;
			break;
		}
	}

	if (ret) {
		for (i = 0; i < len; i++) {
			retries = 0;
			do {
				FLASHStatus = FLASH_ProgramWord(startAddr + i * 4, *(data + i));
				retries++;
			} while (FLASHStatus != FLASH_COMPLETE && retries < FLASH_RETRIES);

			if (retries == FLASH_RETRIES) {
				ret = 0;
				break;
			}
		}
	}

	FLASH_Lock();

	return ret;
}

uint32_t flashStartAddr(void) {
	return FLASH_START_ADDR;
}

uint32_t flashSerno(uint8_t n) {
	return *((uint32_t *) (0x1FFF7A10) + n);
}
