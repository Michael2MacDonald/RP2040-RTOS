MAX_HEAP_SIZE = DEFINED(MAX_HEAP_SIZE) ? MAX_HEAP_SIZE : 0x20000; /* 128K */

MEMORY {
	FLASH (rwx)  : ORIGIN = 0x10000000, LENGTH = 2048k
	RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 264k
}

ENTRY(Reset_Handler)

SECTIONS {
	. = ORIGIN(FLASH);

	.text.headers : {
		_sboot2 = .;
		KEEP(*(.boot2))    /* Second boot stage. Required by rp2040 to configure flash and ensure a bootable image using crc */
		_eboot2 = .;
		_svectors = .;
		KEEP(*(.vectors))  /* Reset vector table for arm exceptions and interupts (Core 0) */
		KEEP(*(.vectors1))  /* Reset vector table for arm exceptions and interupts (Core 1) */

		_ssyslut = .;     /* OS system look up table?? Rename??? */
		KEEP(*(.syslut))  /* Holds pointers to global variables and objects for OS operation (Including scheduler objects and configuration variables) */
		_sservices = .;
		KEEP(*(.services)) /* Service table for SVCall and PendSV service pointers */
		. = ALIGN(1024);
	} > FLASH

	_stext = .;

	.text.fastcode : {
		. = ALIGN(4);
		*(.text.fastcode*); /* FASTRUN code is copied to ram (Look into gcc compiler option "hot") */
		. = ALIGN(4);
	} > RAM AT> FLASH

	.text.code : {
		. = ALIGN(4);
		KEEP(*(.reset))   /* Initialises data and ensures the cores are in a predictable state */
		KEEP(*(.startup)) /* Initializes peripherals, starts RTOS, and runs application */

		*(.text.cold*) /* Code that is not often run. Separate this code so that other code can be closer together and sometimes faster (Look into gcc compiler option "cold") */

		/* *(.flashmem*) */     /* Code that should not be copied to ram */
		*(.text*)
		. = ALIGN(4);
	} > FLASH



	.text.progmem : { /* Program memory */
		. = ALIGN(4);
		/** Static Allocation Support */
		__init_array_start = .;
		KEEP (*(.init_array*))
		__init_array_end = .;

		. = ALIGN(4);
		*(.progmem*) /* Read-only data that should not be copied to ram */
		. = ALIGN(4);
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
		. = ALIGN(4);
		*(.rodata);
		*(.data);
		. = ALIGN(4);
	} > RAM AT> FLASH

	/* statically allocated variables */
	.bss : {
		__bss_start__ = .; /* Used by standard library to zero out bss */
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .; /* Used by standard library to zero out bss */
	} > RAM

	. = ALIGN(8);
	_end = .;
	end = _end;

	.heap (NOLOAD): {
		. = ALIGN(8);
		_heap_start = .;
		*(.heap*)
		. = . + MAX_HEAP_SIZE;
		_heap_end = .;
	} > RAM

	.trap (NOLOAD): { /* Trap heap and stack overflows */
		. = ALIGN(4);
		/* Trap size: 64 bytes (May need to be increased) */
		. = . + 64;
		/* Trap size: 4 bytes (Use this if trap uses MPU instead of periodic checking) */
		/*. = . + 4;*/
		. = ALIGN(4);
	} > RAM

}

/* _sboot2 = ADDR(.boot2);
_eboot2 = ADDR(.boot2) + SIZEOF(.boot2); */
ASSERT(_eboot2 - _sboot2 == 256, "ERROR: rp2040 second stage bootloader must be 256 bytes in size");

_sfastcode = ADDR(.text.fastcode);
_efastcode = ADDR(.text.fastcode) + SIZEOF(.text.fastcode);
_sfastcodeload = LOADADDR(.text.fastcode);

/* _stext = ADDR(.text); */
/* _etext = ADDR(.text) + SIZEOF(.text); */

_sdata = ADDR(.data);
_edata = ADDR(.data) + SIZEOF(.data);
_sdataload = LOADADDR(.data);

_sbss = ADDR(.bss);
_ebss = ADDR(.bss) + SIZEOF(.bss);

_sheap = ADDR(.heap);
_eheap = ADDR(.heap) + SIZEOF(.heap);

_strap = ADDR(.trap);
_etrap = ADDR(.trap) + SIZEOF(.trap);

_estack = ORIGIN(RAM) + LENGTH(RAM) - 28k;
_stack = ORIGIN(RAM) + LENGTH(RAM);

_estack_core1 = _etrap;
_stack_core1 = ORIGIN(RAM) + LENGTH(RAM) - 28k;
