#define XIP_BASE _u(0x10000000)
#define PPB_BASE _u(0xe0000000)

// Register    : M0PLUS_VTOR
// Description : The VTOR holds the vector table offset address.
#define M0PLUS_VTOR_OFFSET _u(0x0000ed08)
#define M0PLUS_VTOR_BITS   _u(0xffffff00)
#define M0PLUS_VTOR_RESET  _u(0x00000000)

check_return:
	pop {r0}
	cmp r0, #0
	beq vector_into_flash
	bx r0
vector_into_flash:
	ldr r0, =(XIP_BASE + 0x100)
	ldr r1, =(PPB_BASE + M0PLUS_VTOR_OFFSET)
	str r0, [r1]
	ldmia r0, {r0, r1}
	msr msp, r0
	bx r1