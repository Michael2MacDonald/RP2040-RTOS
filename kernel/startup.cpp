#include "kernel.h"
#include "cm0plus.h"

#include "include/ArmUtil.h"

#include "Scheduler.h"

#include <vector>
// #include <string>
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

extern "C" int blink_led_fast_loop_switch();
extern "C" int blink_led_slow_loop_switch();

extern "C" void SVC();
extern "C" void Sched_init();
extern "C" Kernel::TCB* CurrentTCB;

void clock_setup();
void gpio_setup();
void uart_setup();


extern Kernel::Scheduler* Sched;

extern "C" {
	bool* enabled;

	Kernel::TCB* _MAIN;
	// Kernel::TCB* _FAST;

	// Kernel::TCB _MAIN_(0, "_MAIN", Kernel::none, Kernel::nonurgent, Kernel::queued, &main, 0);
	// Kernel::TCB _FAST_(1, "_FAST", Kernel::high, Kernel::urgent, Kernel::queued, &blink_led_fast_loop, 0);
}


extern "C" __attribute__((noreturn, section(".startup"), optimize("no-tree-loop-distribute-patterns")))
void _startup(void) {

	asm volatile("CPSID i":::"memory"); // Disable interrupts

	nvic_setpriority(PendSV_IRQn, 3);  // Set PendSV interrupt to the lowest priority
	nvic_setpriority(SVCall_IRQn, 1);  // Set SVCall interrupt to the second highest priority
	nvic_setpriority(SysTick_IRQn, 1); // Set Systick interrupt to the highest priority

	/** TODO: Remove F_CPU_CURRENT entirely??? */
	F_CPU_CURRENT = (6*1000000); // Startup at 6 MHz

	/** TODO: Move to system_init() or core_init() */
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

	// _MAIN = nullptr;
	// _FAST = nullptr;

	// _MAIN = new Kernel::TCB(0, "_MAIN", Kernel::none, Kernel::nonurgent, Kernel::queued, &main, 0);
	// _FAST = new Kernel::TCB(1, "_FAST", Kernel::high, Kernel::urgent, Kernel::queued, &blink_led_fast_loop, 0);

	// Kernel::TCB _MAIN_(0, "_MAIN", Kernel::none, Kernel::nonurgent, Kernel::queued, &main, 0);
	// Kernel::TCB _FAST_(1, "_FAST", Kernel::high, Kernel::urgent, Kernel::queued, &blink_led_fast_loop, 0);

	// _MAIN = &_MAIN_;
	// _FAST = &_FAST_;
	// _FAST->func = &blink_led_fast_loop;

	Kernel::Sched = new Kernel::Scheduler();
	enabled = &(Kernel::Sched->enabled);

	// Kernel::Scheduler::init(); // Initialize the scheduler
	// Kernel::Sched->threads.push_back(_MAIN);
	// Kernel::Sched->threads.push_back(_FAST);
	Kernel::Sched->create("_MAIN",256, &main, Kernel::none);
	Kernel::Sched->create("_FAST",256, &blink_led_fast_loop_switch, Kernel::moderate);
	Kernel::Sched->create("_SLOW",256, &blink_led_slow_loop_switch, Kernel::high);
	// Kernel::Scheduler::getInstance()->start(); // Start the scheduler

	
	// CurrentTCB = _MAIN;
	// CurrentTCB = _FAST;
	CurrentTCB = Kernel::Sched->thread("_MAIN");
	// _FAST = Kernel::Sched->thread("_FAST");
	// volatile register Kernel::TCB* CurrentTCB_r asm("r9");
	// CurrentTCB_r = _MAIN;


	// asm volatile("CPSIE i":::"memory"); // Enable interrupts

	gpio_init(13, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(13, GPIO_OUT); // Set pin as output

	asm("dsb"); // Data synchronization barrier
	asm("isb"); // Instruction synchronization barrier

	asm volatile("CPSIE i":::"memory"); // Enable interrupts


	// std::vector<int> test;
	// if(Kernel::Sched->thread("_FAST") != nullptr) {
	// 	blink_led();
	// }
	// if(Kernel::Sched->thread(0) != nullptr) {
	// 	blink_led();
	// }
	// if(_FAST != nullptr) {
	// 	blink_led();
	// }
	// if(_FAST == &_FAST_) {
	// 	blink_led();
	// }
	// if(Kernel::Sched->threads[1] == &_FAST_) {
	// 	blink_led();
	// }
	// if(Kernel::Sched->threads[1]->func == &blink_led_fast_loop) {
	// // if(Kernel::Sched->threads[1]->func == nullptr) {
	// 	blink_led();
	// }
	// if(Kernel::Sched->threads[1]->stack.pc == (uint32_t)&blink_led_fast_loop) {
	// 	blink_led();
	// }
	// for (int i = 0; i < Kernel::Sched->threads.size(); i++) {
	// 	// if(Kernel::Sched->threads[i]->func == &main) {
	// 	// 	blink_led();
	// 	// }
	// 	if(Kernel::Sched->threads[i]->name == "_MAIN") {
	// 		blink_led();
	// 	}
	// }
	// if (Kernel::Sched->thread("_MAIN") != nullptr) {
	// 	blink_led();
	// 	if (Kernel::Sched->thread("_MAIN")->name == "_MAIN") {
	// 		blink_led();
	// 	}
	// }
	// while (1) asm volatile("wfe");


	// blink_led();
	// Sched->init();
	// blink_led();
	// while (1) asm volatile("WFI"); // DON'T RETURN!!!!


	blink_led();
	// asm volatile(
	// 	".extern Sched_init \n"
	// 	"bl Sched_init \n"
	// 	::: "memory"
	// ); // set CONTROL.SPSEL to 1 (psp)
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
