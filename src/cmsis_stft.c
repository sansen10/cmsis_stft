  //This is the starting of the code. I need to add preprocessors here to ensure the library is compatible
//with all/ majority of the STM boards

//The below are the possible combinations

// ARM_MATH_CM0	Cortex-M0	STM32F0, NXP LPC11xx
// ARM_MATH_CM0PLUS	Cortex-M0+	STM32L0, SAMD21
// ARM_MATH_CM3	Cortex-M3	STM32F1
// ARM_MATH_CM4	Cortex-M4	STM32F3, STM32F4
// ARM_MATH_CM7	Cortex-M7	STM32F7, STM32H7
// ARM_MATH_CM33	Cortex-M33	STM32U5, NRF5340
// ARM_MATH_CM55	Cortex-M55	AI/DSP optimized chips
// ARM_MATH_CM85	Cortex-M85	newest high-performance MCUs

#include "cmsis_stft.h"

stft_status stft_instance_init_q15(stft_instance_q15 *inst1, uint32_t buffer_length, uint8_t overlap_percent, WINDOW_TYPE window_type){

	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
    inst1->buffer_length = buffer_length;
    inst1->overlap_percent = overlap_percent;
    if(inst1->overlap_percent > 100 || overlap_percent < 0){
    	return STFT_ERROR_OVERLAP;
    }
    inst1->window_type = window_type;
    inst1->fft_length = (int) pow(2.0, ceil(log2(buffer_length)));
    if(inst1->fft_length > MAX_FFT_LENGTH || inst1->fft_length < MIN_FFT_LENGTH){
    	return STFT_ERROR_FFT_SIZE;
    }
    inst1->buffer_holder_ptr = (q15_t*)calloc(inst1->buffer_length,sizeof(q15_t));
    if(inst1->buffer_holder_ptr == NULL) goto alloc_fail;
    inst1->stft_input_buffer_ptr = (q15_t*)calloc(inst1->fft_length,sizeof(q15_t));
    if(inst1->stft_input_buffer_ptr == NULL) goto alloc_fail;
    inst1->hop_length = (int) ceil(buffer_length*(100-overlap_percent)/100);
    inst1-> iteration_number = (int)ceil(100 / (100 - overlap_percent))+1;
    inst1->input = calloc(inst1->buffer_length ,sizeof(q15_t));
    if(inst1->input == NULL) goto alloc_fail;
    inst1->input_copy = calloc(inst1->buffer_length ,sizeof(q15_t));
    if(inst1->input_copy == NULL) goto alloc_fail;
    inst1->stft_output = calloc(inst1->fft_length ,sizeof(q15_t));
    if(inst1->stft_output == NULL) goto alloc_fail;
    inst1->stft_output_mag = calloc(inst1->fft_length/2 ,sizeof(q15_t));
    if(inst1->stft_output_mag == NULL) goto alloc_fail;
    inst1->window_coeffecients = (q15_t*)calloc(inst1->buffer_length, sizeof(q15_t));
    if(inst1->window_coeffecients == NULL) goto alloc_fail;
    windowfinder_q15(inst1->window_coeffecients, inst1->window_type, inst1->buffer_length); //The window finder is yet to be written
    if(arm_rfft_init_q15(&inst1->ffthandler,inst1->fft_length,0,1) != ARM_MATH_SUCCESS) goto alloc_fail;
    inst1->callback = NULL;
    inst1->adc_offset =0;
    inst1->adc_bits = 12;
    return STFT_OK;
    alloc_fail:
		free_stft_instance_q15(inst1);
		return STFT_ERROR_ALLOC;
}

stft_status stft_instance_init_q31(stft_instance_q31 *inst1, uint32_t buffer_length, uint8_t overlap_percent, WINDOW_TYPE window_type){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
    inst1->buffer_length = buffer_length;
    inst1->overlap_percent = overlap_percent;
    if(inst1->overlap_percent > 100 || overlap_percent < 0){
        return STFT_ERROR_OVERLAP;
    }
    inst1->window_type = window_type;
    inst1->fft_length = (int) pow(2.0, ceil(log2(buffer_length)));
    if(inst1->fft_length > MAX_FFT_LENGTH || inst1->fft_length < MIN_FFT_LENGTH){
        return STFT_ERROR_FFT_SIZE;
    }
    inst1->buffer_holder_ptr = (q31_t*)calloc(inst1->buffer_length,sizeof(q31_t));
    if(inst1->buffer_holder_ptr == NULL) goto alloc_fail;
    inst1->stft_input_buffer_ptr = (q31_t*)calloc(inst1->fft_length,sizeof(q31_t));
    if(inst1->stft_input_buffer_ptr == NULL) goto alloc_fail;
    inst1->hop_length = (int) ceil(buffer_length*(100-overlap_percent)/100);
    inst1-> iteration_number = (int)ceil(100 / (100 - overlap_percent))+1;
    inst1->input = calloc(inst1->buffer_length ,sizeof(q31_t));
    if(inst1->input == NULL) goto alloc_fail;
    inst1->input_copy = calloc(inst1->buffer_length ,sizeof(q31_t));
    if(inst1->input_copy == NULL) goto alloc_fail;
    inst1->stft_output = calloc(inst1->fft_length ,sizeof(q31_t));
    if(inst1->stft_output == NULL) goto alloc_fail;
    inst1->stft_output_mag = calloc(inst1->fft_length/2 ,sizeof(q31_t));
    if(inst1->stft_output_mag == NULL) goto alloc_fail;
    inst1->window_coeffecients = (q31_t*)calloc(inst1->buffer_length, sizeof(q31_t));
    if(inst1->window_coeffecients == NULL) goto alloc_fail;
    windowfinder_q31(inst1->window_coeffecients, inst1->window_type, inst1->buffer_length); //The window finder is yet to be written
    if(arm_rfft_init_q31(&inst1->ffthandler,inst1->fft_length,0,1) != ARM_MATH_SUCCESS) goto alloc_fail;
    inst1->callback = NULL;
    inst1->adc_offset =0;
    inst1->adc_bits = 12;
    return STFT_OK;
    alloc_fail:
    	free_stft_instance_q31(inst1);
    	return STFT_ERROR_ALLOC;
}

stft_status stft_instance_init_f32(stft_instance_f32 *inst1, uint32_t buffer_length, uint32_t window_length, uint32_t fft_length, float32_t overlap_percent, WINDOW_TYPE window_type){
	if(!DSP_HAS_FPU){
		return STFT_ERROR;
	}
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
	inst1->buffer_length = buffer_length;
    inst1->window_length = window_length;
    inst1->overlap_percent = overlap_percent;
    if(inst1->overlap_percent > 100 || overlap_percent < 0){
        return STFT_ERROR_OVERLAP;
    }
    inst1->window_type = window_type;
    inst1->fft_length = fft_length;
    if(inst1->fft_length > MAX_FFT_LENGTH || inst1->fft_length < MIN_FFT_LENGTH){
        return STFT_ERROR_FFT_SIZE;
    }
    inst1->buffer_holder_ptr = (float32_t*)calloc(inst1->buffer_length,sizeof(float32_t));
    if(inst1->buffer_holder_ptr == NULL) goto alloc_fail;
    inst1->stft_input_buffer_ptr = (float32_t*)calloc(inst1->fft_length,sizeof(float32_t));
    if(inst1->stft_input_buffer_ptr == NULL) goto alloc_fail;
    inst1->hop_length = (int) ceil(window_length*(100-overlap_percent)/100); //changed
    inst1-> iteration_number = (int)floor((buffer_length - window_length)/inst1->hop_length)+1;
    inst1->input = calloc(inst1->buffer_length ,sizeof(float32_t));
    if(inst1->input == NULL) goto alloc_fail;
    inst1->input_copy = calloc(inst1->buffer_length ,sizeof(float32_t));
    if(inst1->input_copy == NULL) goto alloc_fail;
    inst1->stft_output = calloc(inst1->fft_length ,sizeof(float32_t));
    if(inst1->stft_output == NULL) goto alloc_fail;
    inst1->stft_output_mag = calloc(inst1->fft_length/2 ,sizeof(float32_t));
    if(inst1->stft_output_mag == NULL) goto alloc_fail;
    inst1->stft_output_mag_array = calloc(inst1->iteration_number ,sizeof(float32_t)*inst1->fft_length/2);
    if(inst1->stft_output_mag_array == NULL) goto alloc_fail;
    inst1->window_coeffecients = (float32_t*)calloc(inst1->window_length, sizeof(float32_t));
    if(inst1->window_coeffecients == NULL) goto alloc_fail;
    windowfinder_f32(inst1->window_coeffecients, inst1->window_type, inst1->window_length); //The window finder is yet to be written
    if(arm_rfft_fast_init_f32(&inst1->ffthandler,inst1->fft_length)!= ARM_MATH_SUCCESS) goto alloc_fail;
    inst1->callback = NULL;
    return STFT_OK;
    alloc_fail:
    	free_stft_instance_f32(inst1);
    	return STFT_ERROR_ALLOC;
}

stft_status stft_f32_set_callback(stft_instance_f32 *inst1, stft_callback_f32 cb){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
    inst1->callback = cb;
    return STFT_OK;
}

stft_status stft_q15_set_callback(stft_instance_q15 *inst1, stft_callback_q15 cb){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
    inst1->callback = cb;
    return STFT_OK;
}

stft_status stft_q31_set_callback(stft_instance_q31 *inst1, stft_callback_q31 cb){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
    inst1->callback = cb;
    return STFT_OK;
}


stft_status stft_q15_set_adc_offset(stft_instance_q15 *inst1, uint16_t offset){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
	inst1->adc_offset = offset;
	return STFT_OK;
}
stft_status stft_q31_set_adc_offset(stft_instance_q31 *inst1, uint32_t offset){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
	inst1->adc_offset = offset;
	return STFT_OK;
}

stft_status stft_q31_set_adc_bits(stft_instance_q31 *inst1, uint8_t bits){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
	if(bits < 8 || bits >24){
		return STFT_ERROR;
	}
	inst1->adc_bits = bits;
	return STFT_OK;
}

stft_status stft_q15_set_adc_bits(stft_instance_q15 *inst1, uint8_t bits){
	if(inst1 == NULL){
		return STFT_ERROR_NULL_PTR;
	}
	if(bits < 8 || bits >12){
		return STFT_ERROR;
	}
	inst1->adc_bits = bits;
	return STFT_OK;
}

void free_stft_instance_f32(stft_instance_f32 *inst1){
	if(inst1 == NULL) return;
	free(inst1->buffer_holder_ptr);
	inst1->buffer_holder_ptr = NULL;
	free(inst1->stft_input_buffer_ptr);
	inst1->stft_input_buffer_ptr = NULL;
	free(inst1->input);
	inst1->input = NULL;
	free(inst1->input_copy);
	inst1->input_copy = NULL;
	free(inst1->stft_output);
	inst1->stft_output = NULL;
	free(inst1->stft_output_mag);
	inst1->stft_output_mag = NULL;
	free(inst1->window_coeffecients);
	inst1->window_coeffecients = NULL;
}
void free_stft_instance_q31(stft_instance_q31 *inst1){
	if(inst1 == NULL) return;
	free(inst1->buffer_holder_ptr);
	inst1->buffer_holder_ptr = NULL;
	free(inst1->stft_input_buffer_ptr);
	inst1->stft_input_buffer_ptr = NULL;
	free(inst1->input);
	inst1->input = NULL;
	free(inst1->input_copy);
	inst1->input_copy = NULL;
	free(inst1->stft_output);
	inst1->stft_output = NULL;
	free(inst1->stft_output_mag);
	inst1->stft_output_mag = NULL;
	free(inst1->window_coeffecients);
	inst1->window_coeffecients = NULL;
}
void free_stft_instance_q15(stft_instance_q15 *inst1){
	if(inst1 == NULL) return;
	free(inst1->buffer_holder_ptr);
	inst1->buffer_holder_ptr = NULL;
	free(inst1->stft_input_buffer_ptr);
	inst1->stft_input_buffer_ptr = NULL;
	free(inst1->input);
	inst1->input = NULL;
	free(inst1->input_copy);
	inst1->input_copy = NULL;
	free(inst1->stft_output);
	inst1->stft_output = NULL;
	free(inst1->stft_output_mag);
	inst1->stft_output_mag = NULL;
	free(inst1->window_coeffecients);
	inst1->window_coeffecients = NULL;
}

stft_status stft_f32(stft_instance_f32 *inst1, float32_t* user_input){
	if (inst1 == NULL || user_input == NULL) return STFT_ERROR_NULL_PTR;
    for(int j=0; j<inst1->buffer_length;j++){
        inst1->input[j] = (float32_t) user_input[j];
    }
    uint32_t loop_breaker = floor(100/(100-inst1->overlap_percent));
    memcpy(inst1->input_copy,inst1->input,sizeof(float32_t)*inst1->buffer_length);
    for(int i=1; i<loop_breaker; i++){
    	memcpy(inst1->stft_input_buffer_ptr,inst1->buffer_holder_ptr+(inst1->buffer_length)-(inst1->window_length)+((i)*inst1->hop_length),sizeof(float32_t)*(inst1->window_length - (i)*inst1->hop_length));
		memcpy(inst1->stft_input_buffer_ptr+(inst1->window_length-i*inst1->hop_length),inst1->input,sizeof(float32_t)*(i*inst1->hop_length));
		memset(inst1->stft_input_buffer_ptr+inst1->window_length,0,sizeof(float32_t)*(inst1->fft_length-inst1->window_length));
		if(inst1->window_type !=NO_WINDOW){
			arm_mult_f32(inst1->stft_input_buffer_ptr,inst1->window_coeffecients,inst1->stft_input_buffer_ptr,inst1->window_length);
		}
		arm_rfft_fast_f32(&inst1->ffthandler,inst1->stft_input_buffer_ptr,inst1->stft_output,0);
		arm_cmplx_mag_f32(inst1->stft_output+2,inst1->stft_output_mag+1,inst1->fft_length/2-1);
		inst1->stft_output_mag[0] = fabsf(inst1->stft_output[0]);
		memcpy(inst1->stft_output_mag_array+((i-1)*inst1->fft_length/2), inst1->stft_output_mag, sizeof(float32_t)*inst1->fft_length/2);
    }

    for(int i=0; i<inst1->iteration_number-loop_breaker+1; i++){
    	memcpy(inst1->stft_input_buffer_ptr,inst1->input+inst1->window_length - inst1->hop_length*loop_breaker + i*inst1->hop_length,sizeof(float32_t)*(inst1->window_length));
        memset(inst1->stft_input_buffer_ptr+inst1->window_length,0,sizeof(float32_t)*(inst1->fft_length-inst1->window_length));
        if(inst1->window_type !=NO_WINDOW){
            arm_mult_f32(inst1->stft_input_buffer_ptr,inst1->window_coeffecients,inst1->stft_input_buffer_ptr,inst1->window_length);
        }
        arm_rfft_fast_f32(&inst1->ffthandler,inst1->stft_input_buffer_ptr,inst1->stft_output,0);
        arm_cmplx_mag_f32(inst1->stft_output+2,inst1->stft_output_mag+1,inst1->fft_length/2-1);
        inst1->stft_output_mag[0] = fabsf(inst1->stft_output[0]);
        memcpy(inst1->stft_output_mag_array+((i+loop_breaker-1)*inst1->fft_length/2), inst1->stft_output_mag, sizeof(float32_t)*inst1->fft_length/2);
    }


    memcpy(inst1->buffer_holder_ptr,inst1->input_copy,sizeof(float32_t)*inst1->buffer_length);
    if(inst1->callback != NULL){
		inst1->callback(inst1->stft_output_mag_array, inst1->iteration_number);
	}
    return STFT_OK;
}

stft_status stft_q15(stft_instance_q15 *inst1, uint16_t* user_input){
	if (inst1 == NULL || user_input == NULL) return STFT_ERROR_NULL_PTR;
    uint32_t shift_factor = log2(inst1->fft_length)-1;
    for(int j=0; j<inst1->buffer_length;j++){
        inst1->input[j] = (q15_t) (((int16_t)user_input[j] - inst1->adc_offset) << (16-1-inst1->adc_bits));
    }
    memcpy(inst1->input_copy,inst1->input,sizeof(q15_t)*inst1->buffer_length);

    for(int i=0; i<inst1->iteration_number; i++){
        memcpy(inst1->stft_input_buffer_ptr,inst1->buffer_holder_ptr+(i*inst1->hop_length),sizeof(q15_t)*(inst1->buffer_length-(i*inst1->hop_length)));
        memcpy(inst1->stft_input_buffer_ptr+(inst1->buffer_length-(i*inst1->hop_length)),inst1->input,sizeof(q15_t)*(i*inst1->hop_length));
        memset(inst1->stft_input_buffer_ptr+inst1->buffer_length,0,sizeof(q15_t)*(inst1->fft_length-inst1->buffer_length));
        if(inst1->window_type !=NO_WINDOW){
            arm_mult_q15(inst1->stft_input_buffer_ptr,inst1->window_coeffecients,inst1->stft_input_buffer_ptr,inst1->buffer_length);
        }
        arm_rfft_q15(&inst1->ffthandler,inst1->stft_input_buffer_ptr,inst1->stft_output);
        arm_shift_q15(inst1->stft_output, shift_factor, inst1->stft_output, inst1->fft_length);
        arm_cmplx_mag_q15(inst1->stft_output+2,inst1->stft_output_mag+1,inst1->fft_length/2-1);
        inst1->stft_output_mag[0] = (q15_t) abs(inst1->stft_output[0]);

        if(inst1->callback != NULL){
            inst1->callback(inst1->stft_output_mag);
        }
    }
    memcpy(inst1->buffer_holder_ptr,inst1->input_copy,sizeof(q15_t)*inst1->buffer_length);
    return STFT_OK;
}

stft_status stft_q31(stft_instance_q31 *inst1, uint32_t* user_input){
	if (inst1 == NULL || user_input == NULL) return STFT_ERROR_NULL_PTR;
    uint32_t shift_factor = log2(inst1->fft_length)-1;
    for(int j=0; j<inst1->buffer_length;j++){
        inst1->input[j] = (q31_t) (((int32_t)user_input[j] - inst1->adc_offset) << (32-2-inst1->adc_bits));
    }
    memcpy(inst1->input_copy,inst1->input,sizeof(q31_t)*inst1->buffer_length);

    for(int i=0; i<inst1->iteration_number; i++){
        memcpy(inst1->stft_input_buffer_ptr,inst1->buffer_holder_ptr+(i*inst1->hop_length),sizeof(q31_t)*(inst1->buffer_length-(i*inst1->hop_length)));
        memcpy(inst1->stft_input_buffer_ptr+(inst1->buffer_length-(i*inst1->hop_length)),inst1->input,sizeof(q31_t)*(i*inst1->hop_length));
        memset(inst1->stft_input_buffer_ptr+inst1->buffer_length,0,sizeof(q31_t)*(inst1->fft_length-inst1->buffer_length));
        if(inst1->window_type !=NO_WINDOW){
            arm_mult_q31(inst1->stft_input_buffer_ptr,inst1->window_coeffecients,inst1->stft_input_buffer_ptr,inst1->buffer_length);
        }
        arm_rfft_q31(&inst1->ffthandler,inst1->stft_input_buffer_ptr,inst1->stft_output);
        arm_shift_q31(inst1->stft_output, shift_factor, inst1->stft_output, inst1->fft_length);
        arm_cmplx_mag_q31(inst1->stft_output+2,inst1->stft_output_mag+1,inst1->fft_length/2-1);
        inst1->stft_output_mag[0] = (q31_t) abs(inst1->stft_output[0]);

        if(inst1->callback != NULL){
            inst1->callback(inst1->stft_output_mag);
        }
    }
    memcpy(inst1->buffer_holder_ptr,inst1->input_copy,sizeof(q31_t)*inst1->buffer_length);
    return STFT_OK;
}

void windowfinder_f32(float32_t *w, WINDOW_TYPE type, uint32_t N){
    switch(type){
        case HANNING:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0.5f*(1.0f-arm_cos_f32(2.0f*PI*i/(N)));
            }
            break;
        case HAMMING:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0.54f-0.46f*arm_cos_f32(2.0f*PI*i/(N));
            }
            break;
        case BLACKMAN:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0.42f - 0.5f*arm_cos_f32(2.0f*PI*i/(N)) + 0.08f*arm_cos_f32(4.0f*PI*i/(N));
            }
            break;
        case BARTLETT:
            for(uint32_t i=0; i<N; i++){
                w[i] = (2.0f/(N-1))*(((N-1)/2)-fabsf(i-((N-1)/2)));
            }
            break;
        case RECTANGULAR:
            for(uint32_t i=0; i<N; i++){
                w[i] = 1.0f;
            }
            break;
        case NO_WINDOW:
            for(uint32_t i=0; i<N; i++){
                w[i] = 1.0f;
            }
            break;
    }
}

void windowfinder_q15(q15_t *w, WINDOW_TYPE type, uint32_t N){
    switch(type){
        case HANNING:
            for(uint32_t i=0; i<N; i++){
                w[i] = (int16_t) ((0.5f*(1.0f-arm_cos_f32(2.0f*PI*i/(N))))*32767);
            }
            break;
        case HAMMING:
            for(uint32_t i=0; i<N; i++){
                w[i] = (int16_t) ((0.54f-0.46f*arm_cos_f32(2.0f*PI*i/(N)))*32767);
            }
            break;
        case BLACKMAN:
            for(uint32_t i=0; i<N; i++){
                w[i] = (int16_t) ((0.42f - 0.5f*arm_cos_f32(2.0f*PI*i/(N)) + 0.08f*arm_cos_f32(4.0f*PI*i/(N)))*32767);
            }
            break;
        case BARTLETT:
            for(uint32_t i=0; i<N; i++){
                w[i] = (int16_t) (((2.0f/(N-1))*(((N-1)/2)-fabsf(i-((N-1)/2))))*32767);
            }
            break;
        case RECTANGULAR:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0x7FFF;
            }
            break;
        case NO_WINDOW:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0x7FFF;
            }
            break;
    }
}


void windowfinder_q31(q31_t *w, WINDOW_TYPE type, uint32_t N){
    switch(type){
        case HANNING:
            for(uint32_t i=0; i<N; i++){
                w[i] = (q31_t) ((0.5f*(1.0f-arm_cos_f32(2.0f*PI*i/(N))))*0x7FFFFFFF);
            }
            break;
        case HAMMING:
            for(uint32_t i=0; i<N; i++){
                w[i] = (q31_t) ((0.54f-0.46f*arm_cos_f32(2.0f*PI*i/(N)))*0x7FFFFFFF);
            }
            break;
        case BLACKMAN:
            for(uint32_t i=0; i<N; i++){
                w[i] = (q31_t) ((0.42f - 0.5f*arm_cos_f32(2.0f*PI*i/(N)) + 0.08f*arm_cos_f32(4.0f*PI*i/(N)))*0x7FFFFFFF);
            }
            break;
        case BARTLETT:
            for(uint32_t i=0; i<N; i++){
                w[i] = (q31_t) (((2.0f/(N-1))*(((N-1)/2)-fabsf(i-((N-1)/2))))*0x7FFFFFFF);
            }
            break;
        case RECTANGULAR:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0x7FFFFFFF;
            }
            break;
        case NO_WINDOW:
            for(uint32_t i=0; i<N; i++){
                w[i] = 0x7FFFFFFF;
            }
            break;
    }
}
