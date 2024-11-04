#pragma once

#include "accessor.hpp"
#include "buffer.hpp"

template<typename buffer_type>
requires buffer_concept<buffer_type>
sycl::event run_dark_correction(
    sycl::queue q,
    const buffer_type& input,
    buffer_type& output,
    const buffer_type& dark_map,
    typename buffer_type::value_type offset
) {
    accessor input_acc(input);
    accessor output_acc(output);
    accessor dark_map_acc(dark_map);

    return q.parallel_for(input.get_range(), [=](sycl::id<buffer_type::dimensions> idx) {
        output_acc[idx] = input_acc[idx] + offset - dark_map_acc[idx];
    });
}

template<typename buffer_type>
requires buffer_concept<buffer_type>
sycl::event run_gain_correction(
    sycl::queue q,
    const buffer_type& input,
    buffer_type& output,
    const buffer_type& gain_ma
) {

}

template<typename buffer_type>
requires buffer_concept<buffer_type>
sycl::event run_defect_correction(
    sycl::queue q,
    const buffer_type& input,
    buffer_type& output,
    const buffer_type& defect_map
) {
    constexpr int dimensions = buffer_type::dimensions;

    auto input_range = input.get_range();
    auto defect_range = defect_map.get_range();
    auto output_range = output.get_range();

    for(int i = 0; i < dimensions; ++i) {
        if(input_range[i] != output_range[i] || input_range[i] != defect_range[i]) {
            throw std::runtime_error("All buffers must have the same range in each dimension.");
        }
    }

    q.copy(input.get_span().data(), output.get_span().data(), input.get_size());
}
