/**
  ******************************************************************************
  * @file    audio_mic_node.h
  * @author  MCD Application Team 
  * @brief   header file for the audio_mic_node.c file.
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
#ifndef __AUDIO_DEVICES_NODES_H
#define __AUDIO_DEVICES_NODES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_AUDIO_DUMMY_MIC
/* no need to include user files */
#else /* USE_AUDIO_DUMMY_MIC*/
#include "audio_user_devices.h"
#endif /* USE_AUDIO_DUMMY_MIC */
#include  "audio_node.h"
#include "usb_audio.h"

/* Exported constants --------------------------------------------------------*/
#if USE_AUDIO_DUMMY_MIC
#define  AUDIO_MicInit AUDIO_DUMMY_MicInit
#else /* USE_AUDIO_DUMMY_MIC */
#define  AUDIO_MicInit AUDIO_USER_MicInit
#endif /* USE_AUDIO_DUMMY_MIC */
/* Exported types ------------------------------------------------------------*/

/**
 * @brief next are specific variables for each microphone node. .
 */
#if USE_AUDIO_DUMMY_MIC
typedef struct
{
  int8_t dummy;  /* not used */
}AUDIO_MicrophoneSpecificParams_t;
#endif /* USE_AUDIO_DUMMY_MIC */
 /**
 * @brief The microphone node declaration
 *
 * The session will communicate with microphone node using this structure
 * When USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO is activated, the session will estimate the microphone relative frequency:
 * Two fuction handler will be provided by microphone to estimate frequency.
 * The first function reset a  counter variable. This counter will compute read samples in byte
 * The second function will return this counter value and reset it. 
 *      Thus, this function will provide the amount of data captured in byte since the last call
 * 
 */
typedef struct
{
  AUDIO_Node_t node; /*generic node structure*/
  AUDIO_CircularBuffer_t *buf;/* data buffer where the microphone will write received samples*/
  uint16_t packet_length; /* packet maximal length */
  uint8_t volume; /* microphone volume*/
  int8_t  (*MicDeInit)  (uint32_t /*node_handle*/);
  int8_t  (*MicStart)   (AUDIO_CircularBuffer_t* /*buffer*/ , uint32_t /*node handle*/);
  int8_t  (*MicStop)    ( uint32_t /*node handle*/);
  int8_t  (*MicChangeFrequency)    ( uint32_t /*node handle*/);
  int8_t  (*MicMute)     (uint16_t /*channel_number*/, uint8_t /*mute */,uint32_t /*node handle*/);
  int8_t  (*MicSetVolume)    ( uint16_t /*channel_number*/, int /*volume_db_256 */, uint32_t /*node handle*/);
  int8_t  (*MicGetVolumeDefaultsValues)    ( int* /*vol_max*/, int* /*vol_min*/,int* /*vol_res*/, uint32_t /*node handle*/);
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
  int8_t  (*MicStartReadCount)     (uint32_t /*node handle*/); /* reset the counter of read samples*/
  uint16_t  (*MicGetReadCount)    (  uint32_t /*node handle*/);/* function handler to read the number of samples since last call */
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
  AUDIO_MicrophoneSpecificParams_t specific;
}
AUDIO_MicNode_t;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#if USE_AUDIO_MEMS_MIC

#endif /* USE_AUDIO_MEMS_MIC */
/* This is the entry point to microphone node, it initializes microphone node struct members.*/
 int8_t  AUDIO_MicInit(AUDIO_Description_t* audio_description, AUDIO_Session_t* session_handle,  uint32_t node_handle);
#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_DEVICES_NODES_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
