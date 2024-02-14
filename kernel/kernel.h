#ifndef __KERNEL_H
#define __KERNEL_H

/**
 * @file Kernel.h
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short Contains declarations for all user-space accessable classes, functions, variables, and macros
 * 
 * @warning This file must stay compatible with C
 * @todo Make this file compatible with C
 */


#include <stdint.h>

#include "cm0plus.h"

/** TODO:
 * Look into using these GCC compiler options:
 * -fstack-limit-register=reg
 * -fstack-limit-symbol=sym
 * -fsplit-stack
 */


/** TODO:
 * Split the electrical system into "sections" (think of better terminology for this)
 * - Power Management Controller (PMC)
 * - High Voltage Supply (Batteries, charging, etc) (RENAME THIS!!!)
 * - High Voltage Systems (Motor, etc) (RENAME THIS!!!)
 * - Low Voltage Main System (Main Board, Nav sensors, coms, safety devices, etc) (RENAME THIS!!!)
 * - Low Voltage Secondary System (Sensors, science, etc) (RENAME THIS!!!)
 * 
 * There will be a low-power MCU called the "Power Management Controller" or "PMC" (Rename??).
 * It will be hardwired into all power sources (Solar, Battery, Charging Port, Data Port) so that if any part
 * of the system is powered, the PMC will be powered. The high voltage systems will have hard disconnect switches
 * and may have a software disconnect controlled by the PMC and mosfets or relays. If any of the hard disconnects
 * are connected then the PMC will be powered.
 * 
 */

// #include "cm0plus.h" // Cortex-M0+ hardware definitions
// #include <rp2040.h>  // RP2040 definitions (Peripherals, etc)

// #include "Scheduler.h" // Include Scheduler.h
// #include "Components.h" // Include Components.h

// #define F_REF          12000000
// #define F_CPU          120000000
// #define F_PER          120000000
// #define F_RTC          (F_REF / 256)
// #define F_TICK         1000000

// #ifdef __cplusplus
// extern "C" {
// #endif


extern void init();
extern void loop();



/**===========================================================================
 * Data Types & Attributes
 * ===========================================================================*/

// Copy code to RAM to run faster
#define FASTRUN __attribute__ ((section(".text.fastcode")))

// #ifndef _SYS__STDINT_H
// // === Unsigned Types ===
// typedef unsigned long long uint64_t;
// typedef unsigned int uint32_t;
// typedef unsigned short uint16_t;
// typedef unsigned char uint8_t;
// // === Signed Types ===
// typedef long long int64_t;
// typedef int int32_t;
// typedef short int16_t;
// typedef char int8_t;
// #endif

// #ifdef ___int8_t_defined
// #ifndef _INT8_T_DECLARED
// typedef char int8_t ;
// #define _INT8_T_DECLARED
// #endif
// #ifndef _UINT8_T_DECLARED
// typedef unsigned char uint8_t ;
// #define _UINT8_T_DECLARED
// #endif
// #define __int8_t_defined 1
// #endif /* ___int8_t_defined */
// #ifdef ___int16_t_defined
// #ifndef _INT16_T_DECLARED
// typedef short int16_t ;
// #define _INT16_T_DECLARED
// #endif
// #ifndef _UINT16_T_DECLARED
// typedef unsigned short uint16_t ;
// #define _UINT16_T_DECLARED
// #endif
// #define __int16_t_defined 1
// #endif /* ___int16_t_defined */
// #ifdef ___int32_t_defined
// #ifndef _INT32_T_DECLARED
// typedef int int32_t ;
// #define _INT32_T_DECLARED
// #endif
// #ifndef _UINT32_T_DECLARED
// typedef unsigned int uint32_t ;
// #define _UINT32_T_DECLARED
// #endif
// #define __int32_t_defined 1
// #endif /* ___int32_t_defined */
// #ifdef ___int64_t_defined
// #ifndef _INT64_T_DECLARED
// typedef long long int64_t ;
// #define _INT64_T_DECLARED
// #endif
// #ifndef _UINT64_T_DECLARED
// typedef unsigned long long uint64_t ;
// #define _UINT64_T_DECLARED
// #endif
// #define __int64_t_defined 1
// #endif /* ___int64_t_defined */

// === Other Types ===
// ...

// === C Types ===

#include <stdbool.h>


/**===========================================================================
 * Clock, Timing, and Delay
 * ===========================================================================*/

// #if !defined(F_XOSC)
// #error 'F_XOSC' Not Defined By Core!
// // #warning 'F_XOSC' Not Defined By Core! Defaulting to 16MHz
// // #define F_XOSC (16*1000000) // 16 MHz Crystal Oscillator Frequency
// #endif
// #if !defined(F_CPU)
// #error 'F_CPU' Not Defined By Core!
// // #warning 'F_CPU' Not Defined By Core! Defaulting to 16MHz
// // #define F_CPU (16*1000000) // 16 MHz Core Frequency
// #endif

extern uint64_t F_CPU_CURRENT; // This is the clock frequency that the device is currently configured for. It must be set by the application code whenever the clock frequency changes.
// #define F_CPU_ACTUAL  // Measure the actually clock frequency

// Clock cycles per unit time
#define ClocksPerMillisecond ( F_CPU / 1000UL )
#define ClocksPerMicrosecond ( F_CPU / 1000000UL )
#define ClocksPerNanosecond  ( F_CPU / 1000000000UL )
// Convert clock cycles to time
#define ClocksToMillis(cycles) ( ((cycles) * 1000UL) / ClocksPerMillisecond )
#define ClocksToMicros(cycles) ( ((cycles) * 1000UL) / ClocksPerMicrosecond )
#define ClocksToNanos(cycles)  ( ((cycles) * 1000UL) / ClocksPerNanosecond )
// Convert time to clock cycles
#define MillisToClocks(ms) ( (ms) * ClocksPerMillisecond )
#define MicrosToClocks(us) ( (us) * ClocksPerMicrosecond )
#define NanosToClocks(ns)  ( (ns) * ClocksPerNanosecond )

void delay(uint32_t msec);
void delay_nop(uint32_t count);
void sleep(uint32_t msec);

uint32_t millis(); // Return the number of milliseconds since the program started. May be reset/changed with reset_millis().
void reset_millis(uint32_t millis); // Set 'Ticks' to 'millis'


/**===========================================================================
 * System Handlers And Interrupts
 * ===========================================================================*/

extern int32_t get_current_exception(); // Get the current exception number
// extern void Default_Handler(); // Default handler for undefined interrupts

// #define SVC(svc) volatile asm("mov r4,%0"::"r" (svc)); Call_SVC()
// #define SVC(svc, arg0) volatile asm("mov r4,%0"::"r" (svc)); Call_SVC(arg0)
// #define SVC(svc, arg0, arg1) volatile asm("mov r4,%0"::"r" (svc)); Call_SVC(arg0, arg1)
// #define SVC(svc, arg0, arg1, arg2) volatile asm("mov r4,%0"::"r" (svc)); Call_SVC(arg0, arg1, arg2)
// #define SVC(svc, arg0, arg1, arg2, arg3) volatile asm("mov r4,%0"::"r" (svc)); Call_SVC(arg0, arg1, arg2, arg3)

// inline uint64_t SVC(uint64_t svc) {
// 	asm volatile ("mov r4,%0"::"r" (svc)); // Pass the svc number to SVC_Handler
// 	return Call_SVC();
// }
// inline uint64_t SVC(uint64_t svc, uint64_t arg0) {
// 	asm volatile("mov r4,%0"::"r" (svc)); // Pass the svc number to SVC_Handler
// 	return Call_SVC(arg0);
// }
// inline uint64_t SVC(uint64_t svc, uint64_t arg0, uint64_t arg1) {
// 	asm volatile("mov r4,%0"::"r" (svc)); // Pass the svc number to SVC_Handler
// 	return Call_SVC(arg0, arg1);
// }
// inline uint64_t SVC(uint64_t svc, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
// 	asm volatile("mov r4,%0"::"r" (svc)); // Pass the svc number to SVC_Handler
// 	return Call_SVC(arg0, arg1, arg2);
// }
// inline uint64_t SVC(uint64_t svc, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
// 	asm volatile("mov r4,%0"::"r" (svc)); // Pass the svc number to SVC_Handler
// 	return Call_SVC(arg0, arg1, arg2, arg3);
// }

// extern uint64_t Call_SVC();
// extern uint64_t Call_SVC(uint32_t arg0);
// extern uint64_t Call_SVC(uint32_t arg0, uint32_t arg1);
// extern uint64_t Call_SVC(uint32_t arg0, uint32_t arg1, uint32_t arg2);
// extern uint64_t Call_SVC(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
// extern uint64_t Call_SVC(uint32_t arg0 = 0, uint32_t arg1 = 0, uint32_t arg2 = 0, uint32_t arg3 = 0);

// extern uint64_t SVC(uint32_t arg0 = 0, uint32_t arg1 = 0, uint32_t arg2 = 0, uint32_t arg3 = 0);

// extern uint64_t Call_SVC(...);


// #ifdef __cplusplus
// }; /** END: __cplusplus */
// #endif


/**===========================================================================
 * Kernel Functions And Classes
 * ===========================================================================*/

// register uint32_t CurrentTCB_r;

#include "Scheduler.h"



// === Scheduler Types ===
// namespace Kernel {
// 	// Priority levels for tasks
// 	enum ThreadPriority;
// 	typedef enum ThreadPriority TPri_t;

// 	// Urgency levels for tasks (used when running two tasks of the same priority)
// 	enum ThreadUrgency;
// 	typedef enum ThreadUrgency TUrg_t;

// 	// Thread states (Active, Pending, Blocked, Etc)
// 	enum ThreadState;
// 	typedef enum ThreadState TState_t;

// 	// Thread control block
// 	struct TCB;
// } /** END: namespace Kernel */

// namespace Kernel {
// 	class Scheduler; // 
// 	class Component; // 

// 	// Allows the user to hook into the init function and run code before the scheduler starts
// 	int Init_Hook() __attribute__((weak));
// 	// This thread is run when no other threads are running. It can be useful for entering a low power mode.
// 	int Idle_Thread();

// 	void PendSV_Trigger(); // Trigger PendSV interrupt (Rename?? PendSV, Call_PendSV, PendSV_Call)

// 	// Rename to make user and system functions harder to confuse
// 	void init();
// 	// void idle(); // Rename Run()? IdleLoop()? Idle()? IdleHook()?
// 	int Init_ISRHandler();

// }; /** END: namespace Kernel */

// extern "C" void volatile PendSV_Handler();




#endif /** END: __KERNEL_H */