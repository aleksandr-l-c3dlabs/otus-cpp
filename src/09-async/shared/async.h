#pragma once

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

void* connect(std::size_t size);

void receive(const char* buffer, std::size_t length, void* ctx);

void disconnect(void* ctx);

#ifdef __cplusplus
}
#endif
