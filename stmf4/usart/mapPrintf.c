#include "mapPrintf.h"

char *__env[1] = { 0 };
char **environ = __env;


/** Private functions ---------------------------------------------------------*/
//void Usart6Put(uint16_t ch) {
//	while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
//		;
//	USART_SendData(USART6, ch);
//
//}
//
//uint8_t Usart6Get(void) {
//	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
//		;
//	return (uint8_t) USART_ReceiveData(USART1);
//}

int _read(int file, char *ptr, int len) {
	int n;
	int num = 0;
	switch (file) {
	case STDIN_FILENO:
		for (n = 0; n < len; n++) {
			char c = (char)uart_getch();
			*ptr++ = c;
			num++;
		}
		break;
	default:
		errno = EBADF;
		return -1;
	}
	return num;
}

int _write(int file, char *ptr, int len) {
	int n;
	switch (file) {
	case STDOUT_FILENO: /*stdout*/
		for (n = 0; n < len; n++) {
			uart_putchar(*ptr++ & (uint16_t) 0x01FF);
		}
		break;
	case STDERR_FILENO: /* stderr */
		for (n = 0; n < len; n++) {
			uart_putchar(*ptr++ & (uint16_t) 0x01FF);
		}
		break;
	default:
		errno = EBADF;
		return -1;
	}
	return len;
}

void _exit(int status) {
	_write(1, "exit", 4);
	while (1) {
		;
	}
}

int _close(int file) {
	return -1;
}

/**
 execve
 Transfer control to a new process. Minimal implementation (for a system without processes):
 */
int _execve(char *name, char **argv, char **env) {
	errno = ENOMEM;
	return -1;
}

/**
 fork
 Create a new process. Minimal implementation (for a system without processes):
 */
int _fork() {
	errno = EAGAIN;
	return -1;
}

/**
 fstat
 Status of an open file. For consistency with other minimal implementations in these examples,
 all files are regarded as character special devices.
 The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
 */
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 getpid
 Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes. Minimal implementation, for a system without processes:
 */

int _getpid() {
	return 1;
}

/**
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) {
	switch (file) {
	case STDOUT_FILENO:
	case STDERR_FILENO:
	case STDIN_FILENO:
		return 1;
	default:
		//errno = ENOTTY;
		errno = EBADF;
		return 0;
	}
}

/**
 kill
 Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) {
	errno = EINVAL;
	return (-1);
}

/**
 link
 Establish a new name for an existing file. Minimal implementation:
 */
int _link(char *old, char *new) {
	errno = EMLINK;
	return -1;
}

/**
 lseek
 Set position in a file. Minimal implementation:
 */
int _lseek(int file, int ptr, int dir) {
	return 0;
}

/**
 sbrk
 Increase program data space.
 Malloc and related functions depend on this
 */
caddr_t _sbrk(int incr) {

	extern char _ebss; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &_ebss;
	}
	prev_heap_end = heap_end;

	char * stack = (char*) __get_MSP();
	if (heap_end + incr > stack) {
		_write(STDERR_FILENO, "Heap and stack collision\n", 25);
		errno = ENOMEM;
		return (caddr_t) -1;
		//abort ();
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;

}

/**
 stat
 Status of a file (by name). Minimal implementation:
 int    _EXFUN(stat,( const char *__path, struct stat *__sbuf ));
 */
int _stat(const char *filepath, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 times
 Timing information for current process. Minimal implementation:
 */
clock_t _times(struct tms *buf) {
    return -1;
}

/**
 unlink
 Remove a file's directory entry. Minimal implementation:
 */
int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

/**
 wait
 Wait for a child process. Minimal implementation:
 */
int _wait(int *status) {
    errno = ECHILD;
    return -1;
}
