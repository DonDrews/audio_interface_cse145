/**
  ******************************************************************************
  * @file    audio_speaker_node.h
  * @author  MCD Application Team 
  * @brief   header file for the audio_speaker_node.c file.
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
#ifndef __AUDIO_SPEAKER_NODES_H
#define __AUDIO_SPEAKER_NODES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if !USE_AUDIO_SPEAKER_DUMMY
#include "audio_user_devices.h"
#endif /* USE_AUDIO_SPEAKER_DUMMY */
#include  "audio_node.h"
#include "usb_audio.h"

/* Exported constants --------------------------------------------------------*/
/*   VOLUME_SPEAKER_RES_DB_256 is the resolution of volume change, see the UAC specification for more details */
#define VOLUME_SPEAKER_RES_DB_256       128     /* 0.5 db 0.5 * 256 = 256*/ 
#define VOLUME_SPEAKER_DEFAULT_DB_256   0       /* 0 db*/ 
#define VOLUME_SPEAKER_MAX_DB_256       1536    /* 6db == 6*256 = 1536*/
#define VOLUME_SPEAKER_MIN_DB_256       -6400   /* -25db == -25*256 = -6400*/
#if USE_AUDIO_TIMER_VOLUME_CTRL 
/*  SPEAKER_CMD_CHANGE_VOLUM signals to the Timer's interrupt which responsible of volume change that a volume value change is required */
#define SPEAKER_CMD_CHANGE_VOLUME  0x10
/*  SPEAKER_CMD_MUTE_UNMUTE  signals to the Timer's interrupt, which responsible of volume change, that a mute state change is required */
#define SPEAKER_CMD_MUTE_UNMUTE    0x20
/*  SPEAKER_CMD_MUTE_FIRST  signals to the Timer's interrupt, which responsible of volume change, that it has to apply mute change then volume change */
#define SPEAKER_CMD_MUTE_FIRST     0x40
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */

#ifdef USE_AUDIO_SPEAKER_DUMMY
#define  AUDIO_SpeakerInit AUDIO_SPEAKER_DUMMY_Init
#else /* USE_AUDIO_SPEAKER_DUMMY */
#define  AUDIO_SpeakerInit AUDIO_SPEAKER_USER_Init
#endif /* USE_AUDIO_SPEAKER_DUMMY */
/* Exported types ------------------------------------------------------------*/
/* mic node */
#ifdef USE_AUDIO_SPEAKER_DUMMY
typedef struct
{
  int8_t dummy;    /* not used*/
}AUDIO_SpeakerSpecificParms_t;
#endif /* USE_AUDIO_SPEAKER_DUMMY */

/* speaker  node main structure*/
typedef struct
{
  AUDIO_Node_t              node;            /* the structure of generic node*/
  AUDIO_CircularBuffer_t*   buf;             /* the audio data buffer*/
  uint16_t               packet_length;   /* packet maximal length */
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K
  uint16_t               packet_length_max_44_1; /* packet maximal length for frequency 44_1 */
  uint8_t                injection_44_count;     /* used as count to inject 9 packets (44 samples) then 1 packet(45 samples)*/
  uint8_t                injection_45_pos;     /* used as position of  45 samples packet*/
#endif /* USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K*/
  int8_t                (*SpeakerDeInit)  (uint32_t /*node_handle*/);
  int8_t                (*SpeakerStart)   (AUDIO_CircularBuffer_t* /*buffer*/, uint32_t /*node handle*/);
  int8_t                (*SpeakerStop)    ( uint32_t /*node handle*/);
  int8_t                (*SpeakerChangeFrequency)    ( uint32_t /*node handle*/);
  int8_t                (*SpeakerMute)    (uint16_t /*channel_number*/, uint8_t /*mute */,uint32_t /*node handle*/);
  int8_t                (*SpeakerSetVolume)    ( uint16_t /*channel_number*/, int /*volume_db_256 */, uint32_t /*node handle*/);
  int8_t                (*SpeakerStartReadCount)     (uint32_t /*node handle*/);
  uint16_t              (*SpeakerGetReadCount)    (  uint32_t /*node handle*/);
  AUDIO_SpeakerSpecificParms_t specific; /*should be defined by user for user speaker */
}
AUDIO_SpeakerNode_t;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
 int8_t  AUDIO_SpeakerInit(AUDIO_Description_t* audio_description,
                           AUDIO_Session_t* session_handle,
                           uint32_t node_handle);
#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_SPEAKER_NODES_H */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
