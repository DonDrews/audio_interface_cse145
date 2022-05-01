/**
  ******************************************************************************
  * @file    USB_Device/AUDIO_EXT_Advanced_Player_Recorder/Inc/usbd_conf.h
  * @author  MCD Application Team 
  * @brief   General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019  STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF_H
#define __USBD_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hal_usb_ex.h"
#include "usb_audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Exported constants --------------------------------------------------------*/
/* Common Config */
#define USBD_MAX_NUM_INTERFACES               2
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100
#define USBD_SUPPORT_USER_STRING              0 
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      0
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
#define USBD_SUPPORT_AUDIO_OUT_FEEDBACK 1
#endif  /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
#if USE_USB_AUDIO_CLASS_10
#if (defined USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES)||(defined USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES)
#define USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES 1
#endif /*(defined USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES)||(defined USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES) */
#endif /* USE_USB_AUDIO_CLASS_10 */
/* AUDIO Class Config */
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Memory management macros */  
#define USBD_malloc               malloc
#define USBD_free                 free
#define USBD_memset               memset
#define USBD_memcpy               memcpy
    
/* DEBUG macros */  
#if (USBD_DEBUG_LEVEL > 0)
#define  USBD_UsrLog(...)   printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_UsrLog(...)   
#endif                            
                            
#if (USBD_DEBUG_LEVEL > 1)

#define  USBD_ErrLog(...)   printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_ErrLog(...)   
#endif 
                                                        
#if (USBD_DEBUG_LEVEL > 2)                         
#define  USBD_DbgLog(...)   printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_DbgLog(...)                         
#endif

/* Exported functions ------------------------------------------------------- */
void USBD_error_handler(void);
#endif /* __USBD_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
