/**
  ******************************************************************************
  * @file    filter.h
  * @author  YANDLD
  * @version V2.5
  * @date    2015.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    
  ******************************************************************************
  */

#include <math.h>

#include "filter.h"



#define M_PI 3.141592653f


/**
 * @brief  һ�ڵ�ͨ�˲���ϵ������
 * @param  time: ����ʱ�� delataT
 * @param  f_cut: ��ֹƵ��
 * @retval ����ֵ
 */
float lpf_1st_factor_cal(float time, float f_cut)
{
    return time / (time + 1 / (2 * M_PI * f_cut));
}

/**
 * @brief  һ�ڵ�ͨ�˲���
 * @param  old_data: ��һ������
 * @param  new_data: ������
 * @param  factor:   �˲�ϵ��
 * @retval ���
 */
float inline lpf_1st(float old_data, float new_data, float factor)
{
	return old_data * (1 - factor) + new_data * factor; 
}




