#pragma once

#define DISABLE_COPY_MOVE_CTRS(ClassName)                                                          \
    ClassName(ClassName &&) = delete;                                                              \
    ClassName(const ClassName &) = delete;                                                         \
    ClassName &operator=(ClassName &&) = delete;                                                   \
    void operator=(const ClassName &) = delete
