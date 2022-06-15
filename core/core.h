/**
 * @file core.h
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short Provides declarations for [core/variant] functions that are required by the kernel.
 * 
 */

#ifndef __CORE_H
#define __CORE_H

/** core_startup()
 * Configure and initialize system
 * @note This function may be changed, replaced, or removed in the future.
 */
void core_startup();

/** clock_init()
 * @warning NOT YET IMPLEMENTED!!
 * Configure and initialize system clocks
 * - Use/Set Clock Defines (F_CPU, F_USB, F_TICK, etc)
 * - External Clock Inputs
 * - Internal Clocks/Ocilators
 * - PLLs
 * - System Clock Source Selection
 * - Peripheral Clock Sources
 * - Etc.
 */
void clock_init();

/** gpio_init()
 * @warning NOT YET IMPLEMENTED!!
 * Configure and initialize the GPIO pins/ports/mux
 */
void gpio_init();

#endif /** END: __CORE_H */