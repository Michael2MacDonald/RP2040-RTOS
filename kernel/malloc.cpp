#include <stdlib.h>

void * operator new(size_t size)
{
	return malloc(size);
}

void * operator new[](size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	free(ptr);
}

void operator delete[](void * ptr)
{
	free(ptr);
}

void operator delete(void * ptr, size_t size)
{
	free(ptr);
}

void operator delete[](void * ptr, size_t size)
{
	free(ptr);
}


#include <errno.h>

// from the linker script
extern unsigned long _heap_start;
extern unsigned long _heap_end;

char *__brkval = (char *)&_heap_start;

void * _sbrk(int incr) {
	char *prev = __brkval;
	if (incr != 0) {
		if (prev + incr > (char *)&_heap_end) {
			errno = ENOMEM;
			return (void *)-1;
		}
		__brkval = prev + incr;
	}
	return prev;
}
