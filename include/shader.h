#include "core.h"

struct Shader {
    uint32_t programId;

    void bind();
    void compile();
};