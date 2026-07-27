#ifndef DSP_PORT_H
#define DSP_PORT_H

#define MAX_FFT_LENGTH 4096
#define MIN_FFT_LENGTH 32

/* Detect Cortex core */

#if defined(STM32F0xx)
    #define ARM_MATH_CM0
#elif defined(STM32L0xx)
    #define ARM_MATH_CM0PLUS
#elif defined(STM32F1xx)
    #define ARM_MATH_CM3
#elif defined(STM32F2xx)
    #define ARM_MATH_CM3
#elif defined(STM32L1xx)
    #define ARM_MATH_CM3
#elif defined(STM32F3xx) || defined(STM32F4xx)
    #define ARM_MATH_CM4
#elif defined(STM32L4xx) || defined(STM32L4Pxx)
    #define ARM_MATH_CM4          /* M4 with FPU */
#elif defined(STM32G4xx)
    #define ARM_MATH_CM4
#elif defined(STM32WBxx)
    #define ARM_MATH_CM4
#elif defined(STM32F7xx)
    #define ARM_MATH_CM7
#elif defined(STM32H7xx)
    #define ARM_MATH_CM7
#elif defined(STM32U5xx)
    #define ARM_MATH_CM33
#elif defined(STM32H5xx)
    #define ARM_MATH_CM33
#else
	#warning "Unknown STM32 target — ARM_MATH core not auto-detected. Define manually."
#endif

#if defined(__ARM_ARCH_7EM__)
    #define DSP_HAS_FPU 1
#else
    #define DSP_HAS_FPU 0
#endif

#endif

