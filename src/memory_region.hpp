#pragma once

#include <sycl/sycl.hpp> // for sycl::queue

#include <span>

template<typename T>
class usm_memory_region {
    std::span<T> data;
    sycl::queue q;
public:
    using value_type = T;

    usm_memory_region(sycl::queue q, size_t size)
        : q(q) {
        data = std::span(sycl::malloc<T>(q, size), size);
    }

    std::span<T> get_span() const {
        return data;
    }

    ~usm_memory_region() {
        sycl::free(data.data(), q);
    }
};