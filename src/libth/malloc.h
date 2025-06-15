#pragma once

#include <types.h>

#define PAGE_SIZE 4096

void* malloc(usize n);
void  free(void* mem);
