/**\class mapPrintf
 *
 * \ingroup KineticFarcade
 *
 * \brief Configure the hardware of usart6 + simple putchar, sendchar + ringBuffer
 *
 * \note redefine printf function and link it to usart6 class
 *
 * \author DO HOANG DUY KHIEM
 *
 * \version 2.0
 *
 * \date 6/3/2014
 *
 * Contact: khiemdo@hoptechnik.com
 */


#ifndef MAPPRINTF_H_
#define MAPPRINTF_H_

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "usart6.h"

void Usart6Put(uint16_t ch);
uint8_t Usart6Get(void);
/**
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */
int _read(int file, char *ptr, int len);
/**
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len);

void _exit(int status);
int _close(int file);
int _execve(char *name, char **argv, char **env);
int _fork();
int _fstat(int file, struct stat *st) ;
int _getpid();
int _isatty(int file);
int _kill(int pid, int sig);
int _link(char *old, char *new);
int _lseek(int file, int ptr, int dir);
caddr_t _sbrk(int incr);
int _stat(const char *filepath, struct stat *st);
clock_t _times(struct tms *buf);
int _unlink(char *name);
int _wait(int *status);

#endif /*MAPPRINTF_H_*/
