/**
  ******************************************************************************
  * @file    usb_audio_user_cfg.h
  * @author  MCD Application Team 
  * @brief   USB audio application configuration.
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
#ifndef __USB_AUDIO_USER_CFG_H
#define __USB_AUDIO_USER_CFG_H

#ifdef __cplusplus
 extern "C" {
#endif
/* includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usb_audio_constants.h"
/* Exported constants --------------------------------------------------------*/
/* configure project */
/* define which class is used USE_USB_AUDIO_CLASS_10 : must be defined, future release will supports USE_USB_AUDIO_CLASS_20 */
#define  USE_USB_AUDIO_CLASS_10 1
/* for playback project define USE_USB_AUDIO_RECORDING,  for recording project define USE_USB_AUDIO_RECORDING and for si
  * for simultaneous playback and recording define both flags  USE_USB_AUDIO_RECORDING and USE_USB_AUDIO_RECORDING */
#if USE_USB_AUDIO_PLAYBACK
/* define synchronization method */
#define USE_AUDIO_PLAYBACK_USB_FEEDBACK 1
/* definition of channel count and  space mapping of channels */
/* ! Please dont change channel count , other value than 0x02 aren't supported  @TODO add support of multichannel*/
#define USB_AUDIO_CONFIG_PLAY_CHANNEL_COUNT          0x02 /* stereo audio  */
#define USB_AUDIO_CONFIG_PLAY_CHANNEL_MAP            0x03 /* channels Left and right */
/* next two values define the supported resolution  currently expansion supports only 16 bit and 24 bits resolutions @TODO add other resolution support*/
#define USB_AUDIO_CONFIG_PLAY_RES_BIT                16 /* 24 bit per sample */
#define USB_AUDIO_CONFIG_PLAY_RES_BYTE               2 /* 3 bytes */   
/* definition of the list of frequencies */
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_192_K          0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_96_K           0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_48_K           1 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K         0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_32_K           0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_16_K           0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_PLAY_USE_FREQ_8_K            0 /* to set by user:  1 : to use , 0 to not support*/

#define USE_AUDIO_TIMER_VOLUME_CTRL  0   
#define  USB_AUDIO_CONFIG_PLAY_BUFFER_SIZE (1024 * 10)   
#endif /* USE_USB_AUDIO_PLAYBACK*/
 
#if USE_USB_AUDIO_RECORDING   
/* definition of channel count and space mapping of channels */
/* ! Please dont change channel count , other value than 0x02 aren't supported  @TODO add support of multichannel*/
#define USB_AUDIO_CONFIG_RECORD_CHANNEL_COUNT          0x02 /* stereo audio  */
#define USB_AUDIO_CONFIG_RECORD_CHANNEL_MAP            0x03 /* channels Left and right */
/* next two values define the supported resolution  currently expansion supports only 16 bit and 24 bits resolutions @TODO add other resolution support*/
#define USB_AUDIO_CONFIG_RECORD_RES_BIT                16 /* 16 bit per sample */
#define USB_AUDIO_CONFIG_RECORD_RES_BYTE               2 /* 2 bytes */ 
   
/* definition of the list of frequencies */
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_192_K          0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K           0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_48_K           1 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K         0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_32_K           0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_16_K           0 /* to set by user:  1 : to use , 0 to not support*/
#define USB_AUDIO_CONFIG_RECORD_USE_FREQ_8_K            0 /* to set by user:  1 : to use , 0 to not support*/

#define USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 1
#define USE_AUDIO_RECORDING_USB_NO_REMOVE 1

#define  USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE         (1024 * 2) 
#endif /* USE_USB_AUDIO_RECORDING */

/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __USB_AUDIO_USER_CFG_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
