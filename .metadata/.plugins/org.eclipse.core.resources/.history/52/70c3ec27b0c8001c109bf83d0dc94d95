/**
  ******************************************************************************
  * @file    audio_mic_node.c
  * @author  MCD Application Team 
  * @brief   mic node implementation.
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "usbd_audio.h"
#include "audio_mic_node.h"
#include "usb_audio.h"

/* Private defines -----------------------------------------------------------*/
#define MEMS_VOLUME_MIC_RES_DB_256     256 /* 1 db 1 * 256 = 256*/ 
#define MEMS_VOLUME_MIC_MAX_DB_256     8192 /* 32db == 32*256 = 8192*/
#define MEMS_VOLUME_MIC_MIN_DB_256     -8192 /* -32db == -32*256 = -8192*/
#define MIC_CMD_STOP  1
#define MIC_CMD_EXIT  2
#define MIC_CMD_CHANGE_FREQUENCE  4

 /* #define DEBUG_MIC_NODE 1 define if debug required*/
#ifdef DEBUG_MIC_NODE
#define MIC_DEBUG_BUFFER_SIZE 1000
#endif /* DEBUG_MIC_NODE */

/* Private macros -------------------------------------------------------------*/
#define VOLUME_DB_256_TO_PERCENT(volume_db_256) ((uint8_t)((((int)(volume_db_256) - MEMS_VOLUME_MIC_MIN_DB_256)*100)/\
                                                          (MEMS_VOLUME_MIC_MAX_DB_256 - MEMS_VOLUME_MIC_MIN_DB_256)))

/* Private typedef -----------------------------------------------------------*/
#ifdef DEBUG_MIC_NODE
typedef struct
{
  uint32_t time;
  uint16_t write;
  uint16_t read;
} AUDIO_MicDebugStats;
#endif /* DEBUG_MIC_NODE*/

/* externals variables ---------------------------------------------------------*/
#ifdef DEBUG_MIC_NODE
extern __IO uint32_t uwTick;
#endif /* DEBUG_MIC_NODE*/

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
extern DMA_HandleTypeDef               hDmaTopLeft;
extern DMA_HandleTypeDef               hDmaTopRight;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* Private function prototypes -----------------------------------------------*/
/* list of Mic Callbacks */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle);
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle);
static int8_t  AUDIO_MicStop( uint32_t node_handle);
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle);
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute, uint32_t node_handle);
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256, uint32_t node_handle);
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle);
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
static int8_t  AUDIO_MicStartReadCount( uint32_t node_handle);
static uint16_t AUDIO_MicGetLastReadCount( uint32_t node_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
static void AUDIO_FillDataToBuffer(uint32_t pcm_offset);
/* Private variables ---------------------------------------------------------*/ 
static AUDIO_MicNode_t *AUDIO_MicHandler = 0;
#ifdef DEBUG_MIC_NODE
static AUDIO_MicDebugStats_t AUDIO_MicStatsBuffer[MIC_DEBUG_BUFFER_SIZE];
static  int AUDIO_MicStatsCount = 0;
static  int AUDIO_MicStatsCounter = 0;
#endif /* DEBUG_MIC_NODE*/

/* exported functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicInit
  *         Initializes the audio mic node 
  * @param  audio_description:   audio parameters
  * @param  session_handle:     session handle
  * @param  node_handle:        mic node handle must be allocated
  * @retval 0 if no error
  */
 int8_t   AUDIO_DFSDM_MEMS_MicInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle,
                       uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  
  mic   = (AUDIO_MicNode_t*)node_handle;
  memset(mic, 0, sizeof(AUDIO_MicNode_t));
  mic->node.type                = AUDIO_INPUT;
  mic->node.state               = AUDIO_NODE_INITIALIZED;
  mic->node.session_handle      = session_handle;
  mic->node.audio_description   = audio_description;
  mic->MicDeInit                = AUDIO_MicDeInit;
  mic->MicStart                 = AUDIO_MicStart;
  mic->MicStop                  = AUDIO_MicStop;
  mic->MicChangeFrequency       = AUDIO_MicChangeFrequency;
  mic->MicMute                  = AUDIO_MicMute;
  mic->MicSetVolume             = AUDIO_MicSetVolume;
  mic->MicGetVolumeDefaultsValues = AUDIO_MicGetVolumeDefaultsValues;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
  mic->MicStartReadCount        = AUDIO_MicStartReadCount;
  mic->MicGetReadCount          = AUDIO_MicGetLastReadCount;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
  mic->volume = VOLUME_DB_256_TO_PERCENT(audio_description->audio_volume_db_256);
  mic->packet_length = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_description);
  BSP_AUDIO_IN_Init(audio_description->frequency, audio_description->resolution, audio_description->channels_count);
  BSP_AUDIO_IN_AllocScratch (mic->specific.scratch, (AUDIO_SAMPLE_COUNT_LENGTH(USB_AUDIO_CONFIG_RECORD_FREQ_MAX))<<2);
  mic->specific.packet_sample_count = AUDIO_PACKET_SAMPLES_COUNT(audio_description->frequency);
  mic->specific.packet_sample_size = AUDIO_SAMPLE_LENGTH(audio_description);
  AUDIO_MicHandler = mic;
  BSP_AUDIO_IN_Record(0,0);
  return 0;
}
/**
  * @brief  BSP_AUDIO_IN_HalfTransfer_CallBack
            Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  if((AUDIO_MicHandler)&&(AUDIO_MicHandler->node.state==AUDIO_NODE_STARTED))
  {
      AUDIO_FillDataToBuffer(0);
  }
}

/**
  * @brief  BSP_AUDIO_IN_TransferComplete_CallBack
  *         Manages the DMA Transfer complete interrupt    
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  if(AUDIO_MicHandler)
  {
      AUDIO_FillDataToBuffer(AUDIO_MicHandler->specific.packet_sample_count);
  }
}

/* private functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicDeInit
  *         De-Initializes the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  
  mic = (AUDIO_MicNode_t*)node_handle;
  
  if(mic->node.state != AUDIO_NODE_OFF)
  {
    if(mic->node.state == AUDIO_NODE_STARTED)
    {
      AUDIO_MicStop(node_handle);
    }
    BSP_AUDIO_IN_Stop();
    BSP_AUDIO_IN_DeInit();  
    mic->node.state = AUDIO_NODE_OFF;
  }
  
    return 0;
}

/**
  * @brief  AUDIO_MicStart
  *         Start the audio mic node 
  * @param  buffer:      Audio data buffer       
  * @param  node_handle: mic node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer ,  uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  mic=(AUDIO_MicNode_t*)node_handle;

  if(mic->node.state != AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STARTED;
    mic->buf        = buffer;
  }
    return 0;
}

/**
  * @brief  AUDIO_MicStop
  *         stop the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStop( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;
  mic = (AUDIO_MicNode_t*)node_handle;

  if(mic->node.state == AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STOPPED;
  }
    return 0;
}

/**
  * @brief  AUDIO_MicChangeFrequency
  *         change mic frequency 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;

  mic = (AUDIO_MicNode_t*)node_handle;
  mic->specific.cmd|= MIC_CMD_CHANGE_FREQUENCE;
  
    return 0;
}

/**
  * @brief  AUDIO_MicMute
  *         mute  mic 
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
  /* @TODO check if really mic is muted */
	/* No action  currently not supported  call*/
  
  return 0;
}

/**
  * @brief  AUDIO_MicSetVolume
  *         set  mic volume 
  * @param  channel_number : which channel to set the volume (not used currently)
  * @param  volume_db_256  : description is missing
  * @param  node_handle      mic  node handle
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
  ((AUDIO_MicNode_t*)node_handle)->volume = VOLUME_DB_256_TO_PERCENT(volume_db_256);
	/* No action  currently not supported  call*/
  
  return 0;
}
/**
  * @brief  AUDIO_MicGetVolumeDefaultsValues
  *         get  mic volume max, min & resolution value  in db
  * @param  vol_max            
  * @param  vol_min            
  * @param  vol_res             
  * @param  node_handle         
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle)
{
  *vol_max = MEMS_VOLUME_MIC_MAX_DB_256;
  *vol_min = MEMS_VOLUME_MIC_MIN_DB_256;
  *vol_res = MEMS_VOLUME_MIC_RES_DB_256;
  return 0;
}


/**
  * @brief  AUDIO_FillDataToBuffer
   *        get received data from dfsdm
  * @param  pcm_offset        
  * @retval None
  */

static void AUDIO_FillDataToBuffer(uint32_t pcm_offset)
{
  uint16_t wr_distance ;
#ifdef DEBUG_MIC_NODE
  uint32_t counter;
  AUDIO_MicStatsBuffer[AUDIO_MicStatsCount].time = uwTick;
  counter = ++AUDIO_MicStatsCounter;
#endif /*DEBUG_MIC_NODE*/
  if(AUDIO_MicHandler->specific.cmd & MIC_CMD_CHANGE_FREQUENCE)
  {
     BSP_AUDIO_IN_Stop();
     BSP_AUDIO_IN_DeInit();
     AUDIO_MicHandler->packet_length = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(AUDIO_MicHandler->node.audio_description);
     BSP_AUDIO_IN_Init(AUDIO_MicHandler->node.audio_description->frequency, AUDIO_MicHandler->node.audio_description->resolution, AUDIO_MicHandler->node.audio_description->channels_count);
     BSP_AUDIO_IN_AllocScratch (AUDIO_MicHandler->specific.scratch, (AUDIO_SAMPLE_COUNT_LENGTH(AUDIO_MicHandler->node.audio_description->frequency))<<2);
     AUDIO_MicHandler->specific.packet_sample_count = AUDIO_PACKET_SAMPLES_COUNT(AUDIO_MicHandler->node.audio_description->frequency);
     AUDIO_MicHandler->specific.packet_sample_size = AUDIO_SAMPLE_LENGTH(AUDIO_MicHandler->node.audio_description);
     BSP_AUDIO_IN_Record(0,0); /* x2 for double buffering */
     AUDIO_MicHandler->specific.cmd &= ~MIC_CMD_CHANGE_FREQUENCE;
  }
  else
  {
    if(AUDIO_MicHandler->node.state==AUDIO_NODE_STARTED)
    {
      
    wr_distance = AUDIO_BUFFER_FREE_SIZE(AUDIO_MicHandler->buf);
    if(wr_distance<=AUDIO_MicHandler->packet_length)
    {
      AUDIO_MicHandler->node.session_handle->SessionCallback(AUDIO_OVERRUN, (AUDIO_Node_t*)AUDIO_MicHandler,
                                                        AUDIO_MicHandler->node.session_handle);
    }
  /* to change to support other frequencies */
    BSP_AUDIO_IN_Get_PcmBuffer((AUDIO_MicHandler->buf->data+AUDIO_MicHandler->buf->wr_ptr),AUDIO_MicHandler->specific.packet_sample_count,
                               pcm_offset, AUDIO_MicHandler->node.audio_description->resolution);
    /* check for overflow */
    AUDIO_MicHandler->buf->wr_ptr += AUDIO_MicHandler->packet_length;
   #if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
  AUDIO_MicHandler->node.session_handle->SessionCallback(AUDIO_PACKET_RECEIVED, (AUDIO_Node_t*)AUDIO_MicHandler,
                                                        AUDIO_MicHandler->node.session_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
    if(AUDIO_MicHandler->buf->wr_ptr == AUDIO_MicHandler->buf->size)
    {
      AUDIO_MicHandler->buf->wr_ptr = 0;
    }
#ifdef DEBUG_MIC_NODE
    if( counter !=AUDIO_MicStatsCounter)
    {
      Error_Handler();
    }

    AUDIO_MicStatsBuffer[AUDIO_MicStatsCount].read = AUDIO_MicHandler->buf->rd_ptr;
    AUDIO_MicStatsBuffer[AUDIO_MicStatsCount].write = AUDIO_MicHandler->buf->wr_ptr;
    
    if(++AUDIO_MicStatsCount == MIC_DEBUG_BUFFER_SIZE)
    {
        AUDIO_MicStatsCount = 0;
    }
#endif /*DEBUG_MIC_NODE*/
    }
  }
  
}

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
/**
  * @brief  AUDIO_MicStartReadCount
  *         Start a count to compute read bytes from mic each ms 
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStartReadCount( uint32_t node_handle)
{
    AUDIO_MicNode_t* mic;
  
    mic = (AUDIO_MicNode_t*)node_handle;
  
    if(mic->node.state == AUDIO_NODE_STARTED)
    {
           /* read remained value in dma buffer */
      mic->specific.dma_remaining =__HAL_DMA_GET_COUNTER(&hDmaTopRight);
      return 0;
    }
   return -1;     
} 
/**
  * @brief  AUDIO_MicGetLastReadCount
  *         read the number of samples that was read in the last 1ms 
  * @param  node_handle: mic node handle must be started
  * @retval number of read bytes, 0 if an error
  */    
static uint16_t  AUDIO_MicGetLastReadCount( uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  int remaining_data_count , read_samples;
  mic = (AUDIO_MicNode_t*)node_handle;
  
  if(mic->node.state == AUDIO_NODE_STARTED)
  {
         /* read remined value in dma buffer */
    remaining_data_count = __HAL_DMA_GET_COUNTER(&hDmaTopRight);
    read_samples =(remaining_data_count < mic->specific.dma_remaining)? mic->specific.dma_remaining - remaining_data_count :
                   ((mic->specific.packet_sample_count<<1) - remaining_data_count) + mic->specific.dma_remaining; 

    mic->specific.dma_remaining = remaining_data_count;
	return read_samples*AUDIO_MicHandler->specific.packet_sample_size;
  }
    return 0;
}
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
