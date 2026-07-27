# CMSIS_based_STFT_library_for_ARM_Cortex

CMSIS based STFT Library release

# Custom STFT Library for STM32

This library provides a lightweight, dynamic Short-Time Fourier Transform (STFT) implementation explicitly designed for STM32 microcontrollers. It leverages the ARM CMSIS-DSP library to perform efficient FFT calculations and windowing operations.

## Features

* **Multi-Type Support**: Includes dedicated STFT processing structures and functions for `float32_t`, `q15_t`, and `q31_t` data types.

* **Auto-Configuration**: Automatically detects the target STM32 Cortex core (e.g., STM32F4xx maps to `ARM_MATH_CM4`) to ensure proper CMSIS-DSP optimization.

* **FPU Detection**: Automatically checks for Floating-Point Unit (FPU) support during `float32_t` initialization.

* **Built-in Windowing**: Natively calculates and applies Hamming, Hanning, Blackman, Bartlett, and Rectangular window coefficients using `arm_cos_f32`.

* **ADC Integration**: Allows direct input of raw ADC data by configuring ADC offsets and bit depths for fixed-point (`q15` and `q31`) operations.

* **Interrupt/Callback Friendly**: Processes output magnitude arrays via user-defined callback functions.

## Dependencies

* **ARM CMSIS-DSP**: Requires `arm_math.h` for radix-2 fast Fourier transforms and matrix/math operations.

* **Standard C Libraries**: Utilizes `<stdlib.h>` for dynamic memory allocation (`calloc`, `free`).

* **Board Configuration**: Expects a `STFT_config.h` file for global architecture definitions.

---

## API Reference

### Core Enums

**`stft_status`**
Acts similarly to the CMSIS return status to indicate the execution state of the library functions:

* `STFT_OK` (0)

* `STFT_ERROR` (1)

* `STFT_ERROR_NULL_PTR` (2)

* `STFT_ERROR_ALLOC` (3) - Returned if memory allocation fails during initialization.

* `STFT_ERROR_FFT_SIZE` (4) - Triggered if the FFT length is strictly less than 32 or greater than 4096.

* `STFT_ERROR_OVERLAP` (5) - Triggered if the overlap percentage is less than 0 or greater than 100.

* `STFT_ERROR_BUSY` (6)

**`WINDOW_TYPE`**
Defines the windowing function applied before the FFT:

* `HAMMING`

* `HANNING`

* `BLACKMAN`

* `RECTANGULAR`

* `BARTLETT`

* `NO_WINDOW`

### Initialization

These functions allocate necessary memory buffers and initialize the underlying ARM CMSIS-DSP FFT instances.

* `stft_status stft_instance_init_f32(stft_instance_f32 *inst1, uint32_t buffer_length, uint32_t window_length, uint32_t fft_length, float32_t overlap_percent, WINDOW_TYPE window_type);`

* `stft_status stft_instance_init_q15(stft_instance_q15 *inst1, uint32_t buffer_length, uint8_t overlap_percent, WINDOW_TYPE window_type);`

* `stft_status stft_instance_init_q31(stft_instance_q31 *inst1, uint32_t buffer_length, uint8_t overlap_percent, WINDOW_TYPE window_type);`

### Configuration & Callbacks

Allows the user to bind callback functions to retrieve output magnitudes and configure hardware-specific ADC parameters.

* **Callbacks**:

* `stft_status stft_f32_set_callback(stft_instance_f32 *inst1, stft_callback_f32 cb);`

* `stft_status stft_q15_set_callback(stft_instance_q15 *inst1, stft_callback_q15 cb);`

* `stft_status stft_q31_set_callback(stft_instance_q31 *inst1, stft_callback_q31 cb);`

* **ADC Configuration** (Used to automatically shift input arrays based on hardware bit-depth):

* `stft_status stft_q15_set_adc_offset(stft_instance_q15 *inst1, uint16_t offset);`

* `stft_status stft_q15_set_adc_bits(stft_instance_q15 *inst1, uint8_t bits);` (Accepts 8 to 12 bits).

* `stft_status stft_q31_set_adc_offset(stft_instance_q31 *inst1, uint32_t offset);`

* `stft_status stft_q31_set_adc_bits(stft_instance_q31 *inst1, uint8_t bits);` (Accepts 8 to 24 bits).

### Execution

Passing a continuous data buffer to these functions calculates the hop length, applies the selected window, computes the real fast Fourier transform (RFFT), and extracts the complex magnitude.

* `stft_status stft_f32(stft_instance_f32 *inst1, float32_t* user_input);`

* `stft_status stft_q15(stft_instance_q15 *inst1, uint16_t* user_input);`

* `stft_status stft_q31(stft_instance_q31 *inst1, uint32_t* user_input);`

### Memory Management

Safely frees all dynamically allocated memory buffers (e.g., input arrays, window coefficients, and output magnitude arrays) attached to a specific instance.

* `void free_stft_instance_f32(stft_instance_f32 *inst1);`

* `void free_stft_instance_q15(stft_instance_q15 *inst1);`

* `void free_stft_instance_q31(stft_instance_q31 *inst1);`
