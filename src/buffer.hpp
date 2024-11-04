#pragma once

#include "memory_region.hpp"
#include <span>
#include <sycl/sycl.hpp>

template<typename buffer_type>
concept buffer_concept = requires(buffer_type a) {
    requires std::destructible<typename buffer_type::value_type>;
    
    { a.get_range() } -> std::same_as<sycl::range<buffer_type::dimensions>>;
    { a.get_size() } -> std::same_as<size_t>;
    { a.get_span() } -> std::same_as<std::span<typename buffer_type::value_type>>;
};

template <typename T, size_t dim, bool has_pitch = false>
class buffer;

template <typename T, size_t dim, bool has_pitch = false>
class buffer_view;

template <typename T, size_t dim, bool has_pitch>
class buffer_base {
protected:
    sycl::range<dim> range_;
    std::conditional_t<has_pitch, size_t, std::monostate> pitch_;
public:
    static constexpr size_t dimensions = dim;
    using value_type = T;

    template <bool H = has_pitch, typename = std::enable_if_t<H>>
    buffer_base(sycl::range<dim> r, size_t p = 0)
        : range_(r), pitch_(p) {}

    template <bool H = has_pitch, typename = std::enable_if_t<!H>>
    buffer_base(sycl::range<dim> r)
        : range_(r), pitch_() {}

    sycl::range<dim> get_range() const {
        return range_;
    }

    size_t get_pitch() const requires has_pitch {
        return pitch_;
    }

    void set_pitch(size_t p) requires has_pitch {
        pitch_ = p;
    }
};

template <typename T, size_t dim, bool has_pitch>
class buffer : public buffer_base<T, dim, has_pitch> {
protected:
    usm_memory_region<T> data_;
public:
    buffer(sycl::queue q, sycl::range<dim> r)
        : buffer_base<T, dim, has_pitch>(r),
          data_(q, r.size()) {}

    std::span<T> get_span() const {
        return data_.get_span();
    }

    size_t get_size() const {
        return data_.size();
    }

    buffer_view<T, dim, has_pitch> create_buffer_view(sycl::id<dim> offset, sycl::range<dim> view_range) const {
        for (size_t d = 0; d < dim; ++d) {
            if (offset[d] + view_range[d] > this->range_[d]) {
                throw std::out_of_range("Buffer view exceeds buffer dimensions");
            }
        }

        throw std::runtime_error("Not implemented");
    }
};

template <typename T, size_t dim, bool has_pitch>
class buffer_view : public buffer_base<T, dim, has_pitch> {
protected:
    std::span<T> data_;
    sycl::id<dim> offset_;
public:
    buffer_view(std::span<T> d, sycl::range<dim> r, sycl::id<dim> o)
        : buffer_base<T, dim, has_pitch>(r),
          data_(d), offset_(o) {}

    std::span<T> get_span() const {
        return data_.get_span();
    }

    size_t get_size() const {
        return data_.size();
    }

    sycl::id<dim> get_offset() const {
        return offset_;
    }
};

template <typename T, size_t dim>
using buffer_t = buffer<T, dim, false>;

template <typename T, size_t dim>
using pitched_buffer_t = buffer<T, dim, true>;

template <typename T, size_t dim>
using buffer_view_t = buffer_view<T, dim, false>;

template <typename T, size_t dim>
using pitched_buffer_view_t = buffer_view<T, dim, true>;