#ifndef __CLOCK_H
#define __CLOCK_H

// #ifdef __cplusplus
// extern "C" {
// #endif

void xosc_init();
void xosc_disable();
void xosc_dormant();

void clock_setup();

// #ifdef __cplusplus
// }; /** END: __cplusplus */
// #endif

#endif /** END: __CLOCK_H */