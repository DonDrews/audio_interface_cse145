/**
  ******************************************************************************
  * @file    audio_node.h
  * @author  MCD Application Team 
  * @brief   Define audio nodes 
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
#ifndef __AUDIO_NODE_H
#define __AUDIO_NODE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h> 

/* Exported Constants --------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------*/  
/*brief  Main audio buffer structure */
typedef struct
{
  uint8_t*                   data; /* pointer to circular buffer data */
  uint16_t                   rd_ptr;  /* circular buffer reading offset */
  uint16_t                   wr_ptr;   /* circular buffer writing offset */
  uint16_t                   size;   /* The size of buffer segment where samples may be read or written. It is equal or less than the real size of the buffer  */
}
AUDIO_CircularBuffer_t;

/*  audio node's states list */
typedef enum 
{
  AUDIO_NODE_OFF, /* node not initialized */
  AUDIO_NODE_INITIALIZED,
  AUDIO_NODE_STARTED,/* node is running */ 
  AUDIO_NODE_STOPPED,
  AUDIO_NODE_ERROR
}
 AUDIO_NodeState_t;

/*Properties of audio stream*/
typedef struct
{
  uint32_t           frequency; 
  uint8_t            channels_count;   /* Audio channels count */
  uint16_t           channels_map;    /* map of audio channel in the space , see USB Audio class for more informations*/
  uint16_t           audio_type;
  int                audio_volume_db_256;/* Volume scaled using USB audio class volume definition */
  uint8_t            audio_mute;
  uint8_t            resolution;        
}AUDIO_Description_t;

/* List of node types*/
typedef enum 
{
  AUDIO_INPUT,
  AUDIO_OUTPUT,
  AUDIO_CONTROL,
  AUDIO_PROCESSING
}
AUDIO_NodeType_t;

/* Definition of basic audio node structure */
typedef struct    AUDIO_Node
{
 
  AUDIO_NodeState_t    state; 
  AUDIO_Description_t* audio_description;
  AUDIO_NodeType_t     type;
  struct AUDIO_Session*     session_handle;  /* the main session where this node belong */
  struct AUDIO_Node*        next;
}
AUDIO_Node_t;

/* Events raised by nodes to session */
typedef enum 
{
  AUDIO_THRESHOLD_REACHED, /* The audio circular buffer threshold is reached , we have enough data to read from the circular buffer */
  AUDIO_BEGIN_OF_STREAM,   /* First packet is written to buffer */
  AUDIO_PACKET_RECEIVED,   /* Packet is received from the USB host */
  AUDIO_PACKET_PLAYED,     /*  packet is played by the speaker */
  AUDIO_OVERRUN,           /*  An overrun is accured on the circular buffer*/
  AUDIO_UNDERRUN,      /*  An underrun is accured on the circular buffer*/
  AUDIO_OVERRUN_TH_REACHED,  /*  An overrun threshold is reached , that means that overrun is soon but not yet reproduced on the circular buffer*/
  AUDIO_UNDERRUN_TH_REACHED, /*  An underrun threshold is reached , that means that underrun is soon but not yet reproduced on the circular buffer*/
  AUDIO_FREQUENCY_CHANGED     /* The host has request sampling rate change, we need to restart nodes and reset the circular buffer */
} AUDIO_SessionEvent_t;

/* List of session states */
typedef enum 
{
  AUDIO_SESSION_OFF,
  AUDIO_SESSION_INITIALIZED,
  AUDIO_SESSION_STARTED,
  AUDIO_SESSION_STOPPED,
  AUDIO_SESSION_ERROR
}
 AUDIO_SessionState_t;
/* Session main structure */
typedef struct    AUDIO_Session
{
  AUDIO_Node_t * node_list; /* List of nodes used by the session */
  AUDIO_SessionState_t state;
  int8_t  (*SessionCallback) (AUDIO_SessionEvent_t /* event*/ ,
                              AUDIO_Node_t* /*node_handle*/,
                              struct    AUDIO_Session* /*session handle*/);/* callback will called by nodes when an event is reproduced like overrun or underrun*/
}AUDIO_Session_t;

/* Exported macros -----------------------------------------------------------*/ 
/*  AUDIO_BUFFER_FREE_SIZE computes the free size in the circular buffer */
#define AUDIO_BUFFER_FREE_SIZE(buff)  (((buff)->wr_ptr>=(buff)->rd_ptr)?(buff)->rd_ptr +(buff)->size -(buff)->wr_ptr : \
                                                                          (buff)->rd_ptr -(buff)->wr_ptr)
/*  AUDIO_BUFFER_FILLED_SIZE computes the filled size in the circular buffer */
#define AUDIO_BUFFER_FILLED_SIZE(buff)  (((buff)->wr_ptr>= (buff)->rd_ptr)?(buff)->wr_ptr -(buff)->rd_ptr : \
(buff)->wr_ptr +(buff)->size -(buff)->rd_ptr)

/* AUDIO_MS_PACKET_SIZE compute the nominal size(number of bytes) of an audio packet requierd for one millisecond
 * , for example for audio 48KHZ/24 bit/sterio required size is 48*3*2 , for 44.1KHZ/16bits/sterio required size is 44*2*2 */
#define AUDIO_MS_PACKET_SIZE(freq,channel_count,res_byte) (((uint32_t)((freq) /1000))* (channel_count) * (res_byte)) 
/* AUDIO_MS_MAX_PACKET_SIZE compute the maximal size(number of bytes) of an audio packet requierd for one millisecond(without computing the sample added for synchronization)
 * , for example for audio 48KHZ/24 bit/sterio required size is 48*3*2 , for 44.1KHZ/16bits/sterio required size is 45*2*2 */
#define AUDIO_MS_MAX_PACKET_SIZE(freq,channel_count,res_byte) AUDIO_MS_PACKET_SIZE(freq+999,channel_count,res_byte)

#ifdef USE_USB_HS
/* AUDIO_USB_PACKET_SIZE computes the nominal size of audio packet in HS mode(it uses the floor of frequency fractional part) */
#define AUDIO_USB_PACKET_SIZE(freq,channel_count,res_byte) (((uint32_t)((freq) /8000))* (channel_count) * (res_byte)) 
/* AUDIO_USB_MAX_PACKET_SIZE computes the nominal size of audio packet in HS mode(it uses the ceil of frequecy fractional part) */
#define AUDIO_USB_MAX_PACKET_SIZE(freq,channel_count,res_byte) AUDIO_USB_PACKET_SIZE(freq+7999,channel_count,res_byte)
#else /* USE_USB_HS */
/* AUDIO_USB_PACKET_SIZE computes the nominal size of audio packet in USB FS speed(it uses the floor of frequency fractional part) */
#define AUDIO_USB_PACKET_SIZE(freq,channel_count,res_byte) (((uint32_t)((freq) /1000))* (channel_count) * (res_byte)) 
/* AUDIO_USB_MAX_PACKET_SIZE compute the nominal size of audio packet in USB FS speed(it uses the ceil of frequency fractional part) */
#define AUDIO_USB_MAX_PACKET_SIZE(freq,channel_count,res_byte) AUDIO_USB_PACKET_SIZE(freq+999,channel_count,res_byte)
#endif /* USE_USB_HS */
/*  AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC, AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC and AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC
* are wrappers that uses AUDIO_Description_t as argument */
#define AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(audio_desc) AUDIO_USB_PACKET_SIZE((audio_desc)->frequency, (audio_desc)->channels_count, (audio_desc)->resolution)
#define AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc) AUDIO_USB_MAX_PACKET_SIZE((audio_desc)->frequency, (audio_desc)->channels_count, (audio_desc)->resolution)
#define AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_desc) AUDIO_MS_PACKET_SIZE((audio_desc)->frequency, (audio_desc)->channels_count, (audio_desc)->resolution)
#define AUDIO_MS_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc) AUDIO_MS_PACKET_SIZE((audio_desc)->frequency + 999, (audio_desc)->channels_count, (audio_desc)->resolution)
   /* AUDIO_SAMPLE_LENGTH computes 1 sample length. It uses AUDIO_Description_t as argument */
#define AUDIO_SAMPLE_LENGTH(audio_desc) ( (audio_desc)->channels_count*(audio_desc)->resolution)

#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_NODE_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
