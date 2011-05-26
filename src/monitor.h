// monitor.h -- Defines the interface for monitor.h
//              From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen.
void monitor_put(const char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(const char *c);

int printf (const char *format, ...);

void monitor_write_hex(const u32int n);

void monitor_write_dec(const u32int n);

#endif // MONITOR_H
