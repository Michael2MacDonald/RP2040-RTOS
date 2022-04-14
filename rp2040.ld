MEMORY {
	FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 2048k
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 264k
}

ENTRY(_reset)

SECTIONS {
	. = ORIGIN(FLASH);

	.boot2 : {
		KEEP (*(.boot2)) /* Second boot stage. Required by rp2040 to configure flash and ensure a bootable image using crc */
	} > FLASH

	.text : {
		KEEP (*(.vectors))
		. = ALIGN(4);
		KEEP (*(.reset)) /* Initialises data and ensures the cores are in a predictable state */
		KEEP (*(.startup)) /* Initializes peripherals, starts RTOS, and runs application */
		. = ALIGN(4);

		/* *(.init) */

		. = ALIGN(4);
		*(.text*);
		. = ALIGN(4);

		/* *(.fini) */

	} > FLASH

	/* Used for stack unwinding and tracing?? Debugging?? */
	/* .ARM.extab : {
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH
	__exidx_start = .;
	.ARM.exidx : {
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
	__exidx_end = .; */

	_etext = .;

	.data : {
		*(.rodata);
		*(.data);
		. = ALIGN(4);
	} > RAM AT> FLASH

	.bss : {
		__bss_start__ = .; /* Used by standard library to zero out bss */
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .; /* Used by standard library to zero out bss */
	} > RAM

	_end = .;

	.heap (COPY): {
		*(.heap*)
		__HeapLimit = .;
	} > RAM

}



_sboot2 = ADDR(.boot2);
_eboot2 = ADDR(.boot2) + SIZEOF(.boot2);

ASSERT(_eboot2 - _sboot2 == 256, "ERROR: rp2040 second stage bootloader must be 256 bytes in size");

_stext = ADDR(.text);
/* _start_func = ADDR(.start); */

_sdata = ADDR(.data);
_edata = ADDR(.data) + SIZEOF(.data);
_sdataload = LOADADDR(.data);

_sbss = ADDR(.bss);
_ebss = ADDR(.bss) + SIZEOF(.bss);

_stack = ORIGIN(RAM) + LENGTH(RAM);
