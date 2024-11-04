#include "lib.h"
#include "buffer.hpp"
#include "generic_buffer.hpp"
#include "operations.hpp"
#include <sycl/sycl.hpp>
#include <variant>

using buffer_variant = std::variant<
    buffer<uint16_t, 1, true>, buffer<uint16_t, 1, false>
>;

struct ip_buffer_t {
    buffer_t<uint16_t, 2> actual;
};

struct ip_queue_t {
    sycl::queue actual;
};

struct ip_event_t {
    sycl::event actual;
};

ip_error_t create_queue(ip_queue_t** queue) {
    if (queue == nullptr) {
        return IP_ERROR_INVALID_PARAMETER;
    }

    *queue = new ip_queue_t {
        .actual = sycl::queue()
    };

    return IP_ERROR_SUCCESS;
}

ip_error_t destroy_queue(ip_queue_t* queue) {
    if (queue != nullptr) {
        delete queue;
    }

    return IP_ERROR_SUCCESS;
}

ip_error_t create_buffer(ip_queue_t* queue, ip_buffer_t** buffer) {
    if (buffer == nullptr) {
        return IP_ERROR_INVALID_PARAMETER;
    }

    return IP_ERROR_SUCCESS;
}

ip_error_t destroy_buffer(ip_buffer_t* buffer) {
    if (buffer != nullptr) {
        delete buffer;
    }

    return IP_ERROR_SUCCESS;
}

ip_error_t ip_run_dark_correction(ip_queue_t* queue, ip_buffer_t* input, ip_buffer_t* output, ip_buffer_t* dark_map, uint16_t offset) {
    run_dark_correction(queue->actual, input->actual, output->actual, dark_map->actual, offset);
}