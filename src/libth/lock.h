#pragma once

#include <types.h>

void lock(bool* lock);
void unlock(bool* lock);
bool try_lock(bool* lock);
