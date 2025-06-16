#pragma once

//extern const char* environ[];

int execve(const char* path, const char* const argv[], const char* const envp[]);
