MEMORY {
	XIP (rx) : ORIGIN = 0x10000000, LENGTH = 8m
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 264k
}

ENTRY(reset_handler)

SECTIONS {

	.startup : {
		*(.startup)
	} > XIP

	__text_start = .;
	.text : {
		*(.text)
	} > XIP
	__text_end = .;

	.ARM.exidx : {
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > RAM AT> XIP

	__data_start = .;
	.data : {
		*(.data)
	} > RAM AT> XIP
	__data_end = .;

	__bss_start = .;
	.bss : {
		bss = .;
		*(.bss)
	} > RAM
	__bss_end = .;
}