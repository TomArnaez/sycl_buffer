#pragma once

#include <variant>

#include "buffer.hpp"

class generic_buffer {
    using buffer_variant = std::variant<
        buffer<uint16_t, 1, true>,
        buffer<uint16_t, 1, false>,
        buffer_view<uint16_t, 1, true>,
        buffer_view<uint16_t, 1, false>
    >;

    buffer_variant buffer_var;
};