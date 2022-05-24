#include "kernel.h"
#include "cm0plus.h"

#include "include/ArmUtil.h"

#include "Scheduler.h"

#include <vector>
#include <cstring>

#include <resets.h>
#include <gpio.h>
#include <uart.h>
#include <clock.h>
// #include "nvic.h"

extern uint32_t SVC_Handler_Table[256];

extern int main();
extern "C" void blink_led();
extern "C" int blink_led_fast();
extern "C" int blink_led_fast_loop();

extern "C" int blink_fast_thread();
extern "C" int blink_slow_thread();

extern "C" void SVC();
extern "C" void Sched_init();
extern "C" Kernel::TCB* CurrentTCB;

void clock_setup();
void gpio_setup();
void uart_setup();


extern Kernel::Scheduler* Sched;

extern "C" {
	bool* enabled;
}


// extern "C" __attribute__((noreturn, section(".startup"), optimize("no-tree-loop-distribute-patterns")))
extern "C" __attribute__((noreturn, section(".startup")))
void _startup(void) {

	// asm volatile("CPSID i":::"memory"); // Disable interrupts

	nvic_setpriority(PendSV_IRQn, 3);  // Set PendSV interrupt to the lowest priority
	nvic_setpriority(SVCall_IRQn, 1);  // Set SVCall interrupt to the second highest priority
	nvic_setpriority(SysTick_IRQn, 0); // Set Systick interrupt to the highest priority

	/** TODO: Remove F_CPU_CURRENT entirely??? */
	F_CPU_CURRENT = (6*1000000); // Startup at 6 MHz

	/** TODO: Move to system_init() or core_init()??? */
	clock_setup();   // Setup clock, PLL, etc.
	gpio_setup();    // Setup GPIOs
	uart_setup();    // Setup UART
	
	/** Set the number of clock cycles between each systick interrupt
	 * 120MHz = 120000000 = (120*1000000)
	 * The largest possible value you can set is (2^24)-1 (16777215) which at 120MHz is about 0.1398 seconds or 139.8ms ( 16777215/(120*1000000) = ~0.1398 )
	 * We have the reset value set at 120000 which is 1ms at 120MHz ( 120000/(120*1000000) = ~0.001 )
	 * So the systick interrupt is set to trigger every 1ms
	 */
	systick_init(); // Initialize the systick timer
	systick_set(120000);
	// systick_set(240000);

	asm("dsb"); // Data synchronization barrier
	asm("isb"); // Instruction synchronization barrier

	
	Kernel::Sched = new Kernel::Scheduler();
	enabled = &(Kernel::Sched->enabled);

	// Kernel::Scheduler::init(); // Initialize the scheduler
	Kernel::Sched->create("_MAIN",256, &main, Kernel::none);
	Kernel::Sched->create("_FAST",256, &blink_fast_thread, Kernel::moderate);
	Kernel::Sched->create("_SLOW",256, &blink_slow_thread, Kernel::high);
	
	CurrentTCB = Kernel::Sched->thread("_MAIN");

	// asm volatile("CPSIE i":::"memory"); // Enable interrupts

	gpio_init(13, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(13, GPIO_OUT); // Set pin as output

	asm("dsb"); // Data synchronization barrier
	asm("isb"); // Instruction synchronization barrier

	asm volatile("CPSIE i":::"memory"); // Enable interrupts


	blink_led();
	Sched_init();
	// Kernel::Sched->enabled = true; // Start the scheduler
	// *enabled = true; // Start the scheduler
	blink_led();
	while (1) asm volatile("WFI"); // DON'T RETURN!!!!

	
	// SVC_Handler_Table[0] = (uint32_t)(&Sched_init);
	// blink_led();
	// // asm volatile("movs r6, #2 \n msr control, r6" ::: "r6", "memory"); // set CONTROL.SPSEL to 1 (psp)
	// SVC(); // init the first thread
	// asm("dsb"); // Data synchronization barrier
	// asm("isb"); // Instruction synchronization barrier
	// while (1) asm volatile("WFI"); // DON'T RETURN!!!!
	// Kernel::Sched->enabled = true; // Start the scheduler
	// // SCB->ICSR |= (1 << 28); // Set PendSV to pending
	// blink_led();

	// main(); // Call main()
	while (1) asm volatile("WFI"); // DON'T RETURN!!!!
	// while (1) blink_led_fast(); // DON'T RETURN!!!!

} /** END: _startup() */


void gpio_setup() { // rename?? io_setup?? pads_setup??
	reset_release_wait(RESET_IO_BANK0); // Release reset for IO
	reset_release_wait(RESET_PADS_BANK0); // Release reset for Pads
}

/** TODO: Move to UART file and add setup functions */
void uart_setup() {
	reset_release_wait(RESET_UART0); // Release reset for UART0
	uart_init(0, 115200); // Baud rate = 115200
	// uart_init(0, 9600); // Baud rate = 9600
	gpio_init(0, GPIO_FUNC_UART); // RX
	gpio_init(1, GPIO_FUNC_UART); // TX
	gpio_dir(0, 1); // Set as output
}

void usb_setup() {
	reset_release_wait(RESET_PLL_USB); // Release reset for PLL_USB
	reset_release_wait(RESET_USBCTRL); // Release reset for USBCTRL
}
