#pragma once

#include <sycl/sycl.hpp>
#include <span>
#include <type_traits>
#include <utility>

template<typename T, size_t dim, bool pitched>
class accessor {
    std::span<T> data;
    sycl::range<dim> range;
    typename std::conditional<pitched, size_t, std::monostate>::type pitch;

public:
    template <typename buffer_type>
    requires buffer_concept<buffer_type> &&
            std::same_as<typename buffer_type::value_type, T>
    accessor(const buffer_type& buffer)
        : data(buffer.get_span()), range(buffer.get_range()), pitch(std::monostate{}) {
    }

    size_t get_pitch() const {  
        if constexpr (pitched) {
            return pitch;
        } else {
            return range[1];
        }
    }

    T& operator[](sycl::id<dim> idx) const {
        if constexpr (dim == 1) {
            return data[idx[0]];
        } else if constexpr (dim == 2) {
            return data[idx[0] * get_pitch() + idx[1]];
        } else if constexpr (dim == 3) {
            return data[idx[0] * get_pitch() * range[2] + idx[1] * get_pitch() + idx[2]];
        }
    }
};

// Deduction guides
template <typename T, size_t dim, bool pitched>
accessor(const buffer<T, dim, pitched>&) -> accessor<T, dim, pitched>;

template <typename T, size_t dim, bool pitched>
accessor(const buffer_view<T, dim, pitched>&) -> accessor<T, dim, pitched>;