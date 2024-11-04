#include <sycl/sycl.hpp>

#ifdef _WIN32
    #ifdef MYLIBRARY_EXPORTS
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#elif defined(__linux__) || defined(__APPLE__)
    #define EXPORT __attribute__((visibility("default")))
#else
    #define EXPORT
#endif

typedef enum ip_error_t {
    IP_ERROR_SUCCESS,
    IP_ERROR_UNKNOWN,
    IP_ERROR_INVALID_PARAMETER
} ip_error_t;

typedef enum ip_data_type_t {
    IP_DATA_TYPE_UINT16,
    IP_DATA_TYPE_UINT32,
    IP_DATA_TYPE_FLOAT
} ip_data_type_t;

typedef struct ip_buffer_t ip_buffer_t;
typedef struct ip_event_t ip_event_t;
typedef struct ip_queue_t ip_queue_t;
typedef struct ip_external_semaphore_t ip_external_semaphore_t;
typedef struct ip_external_buffer_t ip_external_buffer_t;

typedef struct event_info {
    ip_event_t* result_event;
    ip_event_t** dependent_events;
    size_t dependent_event_count;
};

// object create/destroy
EXPORT SYCL_EXTERNAL ip_error_t ip_create_queue(ip_queue_t** queue);
EXPORT SYCL_EXTERNAL ip_error_t ip_destroy_queue(ip_queue_t* queue);
EXPORT SYCL_EXTERNAL ip_error_t ip_create_buffer(ip_queue_t* queue, ip_buffer_t** buffer);
EXPORT SYCL_EXTERNAL ip_error_t ip_destroy_buffer(ip_buffer_t* buffer);

// external
EXPORT SYCL_EXTERNAL ip_error_t ip_import_external_semaphore(void* semaphore_handle, ip_external_semaphore_t** external_semaphore);
EXPORT SYCL_EXTERNAL ip_error_t ip_release_external_semaphore(ip_external_semaphore_t* external_semaphore);
EXPORT SYCL_EXTERNAL ip_error_t ip_import_external_buffer(void* buffer_handle, ip_external_buffer_t** external_buffer);
EXPORT SYCL_EXTERNAL ip_error_t ip_release_external_semaphore(ip_external_buffer_t* external_buffer);

// general functions
EXPORT SYCL_EXTERNAL ip_error_t ip_queue_wait(ip_queue_t* queue);
EXPORT SYCL_EXTERNAL ip_error_t ip_copy(ip_queue_t* queue, ip_buffer_t* src, ip_buffer_t* dest);
EXPORT SYCL_EXTERNAL ip_error_t ip_copy2d(ip_queue_t* queue, ip_buffer_t* src, size_t src_pitch, ip_buffer_t* dest, size_t dest_pitch, size_t width, size_t height);

// operations
EXPORT SYCL_EXTERNAL ip_error_t ip_run_dark_correction(ip_buffer_t* input, ip_buffer_t* output, ip_buffer_t* dark_map, event_info* event_info);
EXPORT SYCL_EXTERNAL ip_error_t ip_run_gain_correction(ip_buffer_t* input, ip_buffer_t* output, ip_buffer_t* gain_map, event_info* event_info);