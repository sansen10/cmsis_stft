// This is the header file for the library and should contain the includes
#ifndef CUSTOM_STFT_H //Header Guard to see if the header file has been called already
#define CUSTOM_STFT_H
#include "arm_math.h" // Math library from ARM for various use cases
#include <stdlib.h>
#ifndef PI
    #define PI 3.14159265358979323846f // The standard value for floating point calculations
#endif
#include "stft_port.h" // Configuration file that defines the board architecture and global definitions
typedef enum{ //The window type is used to find the correct window
    HAMMING,
    HANNING,
    BLACKMAN,
    RECTANGULAR,
    NO_WINDOW,
    BARTLETT
}WINDOW_TYPE;

/* In cmsis_stft.h */
typedef enum {
    STFT_OK              = 0,
    STFT_ERROR           = 1,
    STFT_ERROR_NULL_PTR  = 2,
    STFT_ERROR_ALLOC     = 3,
    STFT_ERROR_FFT_SIZE  = 4,
    STFT_ERROR_OVERLAP   = 5,
    STFT_ERROR_BUSY      = 6
} stft_status;
// The STFT status acts similar to CMSIS return status to indicate the status of the executed function

// The user callback is a function that is assigned by the user so that they can use the STFT output
typedef void (*stft_callback_f32)(float32_t *stft_output, uint32_t slices);
typedef void (*stft_callback_q15)(q15_t *stft_output);
typedef void (*stft_callback_q31)(q31_t *stft_output);

// The STFT instance is defined for each data type float32, q15 and q31
typedef struct{
    uint32_t buffer_length; // the input length of the argument being passed
    float32_t overlap_percent; // the percentage overlap, basically defines the hop length of the sliding window
    WINDOW_TYPE window_type; // specified by the user to use the correct windowing function
    uint32_t fft_length; // next power of 2 of buffer length to make comuptation easier using radix-2

    uint32_t window_length;

    uint32_t hop_length; // calculated from overlap percent
    uint32_t iteration_number; // number of output window slices for a given input
    arm_rfft_fast_instance_f32 ffthandler; // fftinstance created to be used by cmsis arm rfft
    float32_t *stft_input_buffer_ptr;
    float32_t *buffer_holder_ptr;
    float32_t *input;
    float32_t *input_copy;
    float32_t *window_coeffecients;
    float32_t* stft_output;
    float32_t* stft_output_mag;
    float32_t* stft_output_mag_array;

    stft_callback_f32 callback;
} stft_instance_f32;

typedef struct{
    uint32_t buffer_length;
    uint8_t overlap_percent;
    WINDOW_TYPE window_type;
    uint32_t fft_length;

    uint32_t window_length;

    uint32_t hop_length;
    uint32_t iteration_number;
    arm_rfft_instance_q15 ffthandler;
    q15_t *stft_input_buffer_ptr;
    q15_t *buffer_holder_ptr;
    q15_t *input;
    q15_t *input_copy;
    q15_t *window_coeffecients;
    q15_t* stft_output;
    q15_t* stft_output_mag;
    uint16_t adc_offset;
    uint8_t adc_bits;
    stft_callback_q15 callback;
} stft_instance_q15;

typedef struct{
    uint32_t buffer_length;
    uint8_t overlap_percent;
    WINDOW_TYPE window_type;
    uint32_t fft_length;

    uint32_t window_length;

    uint32_t hop_length;
    uint32_t iteration_number;
    arm_rfft_instance_q31 ffthandler;
    q31_t *stft_input_buffer_ptr;
    q31_t *buffer_holder_ptr;
    q31_t *input;
    q31_t *input_copy;
    q31_t *window_coeffecients;
    q31_t* stft_output;
    q31_t* stft_output_mag;
    uint32_t adc_offset;
    uint8_t adc_bits;
    stft_callback_q31 callback;
} stft_instance_q31;

stft_status stft_instance_init_q15(stft_instance_q15 *inst1, uint32_t buffer_length, uint8_t overlap_percent, WINDOW_TYPE window_type);
stft_status stft_instance_init_q31(stft_instance_q31 *inst1, uint32_t buffer_length, uint8_t overlap_percent, WINDOW_TYPE window_type);
stft_status stft_instance_init_f32(stft_instance_f32 *inst1, uint32_t buffer_length, uint32_t window_length, uint32_t fft_length, float32_t overlap_percent, WINDOW_TYPE window_type);

stft_status stft_q15_set_adc_offset(stft_instance_q15 *inst1, uint16_t offset);
stft_status stft_q31_set_adc_offset(stft_instance_q31 *inst1, uint32_t offset);
stft_status stft_f32_set_callback(stft_instance_f32 *inst1, stft_callback_f32 cb);
stft_status stft_q15_set_callback(stft_instance_q15 *inst1, stft_callback_q15 cb);
stft_status stft_q31_set_callback(stft_instance_q31 *inst1, stft_callback_q31 cb);
stft_status stft_q31_set_adc_bits(stft_instance_q31 *inst1, uint8_t bits);
stft_status stft_q15_set_adc_bits(stft_instance_q15 *inst1, uint8_t bits);
stft_status stft_f32(stft_instance_f32 *inst1, float32_t* user_input);
stft_status stft_q15(stft_instance_q15 *inst1, uint16_t* user_input);
stft_status stft_q31(stft_instance_q31 *inst1, uint32_t* user_input);
void windowfinder_f32(float32_t *w, WINDOW_TYPE type, uint32_t N);
void windowfinder_q15(q15_t *w, WINDOW_TYPE type, uint32_t N);
void windowfinder_q31(q31_t *w, WINDOW_TYPE type, uint32_t N);
void free_stft_instance_f32(stft_instance_f32 *inst1);
void free_stft_instance_q31(stft_instance_q31 *inst1);
void free_stft_instance_q15(stft_instance_q15 *inst1);

#endif
