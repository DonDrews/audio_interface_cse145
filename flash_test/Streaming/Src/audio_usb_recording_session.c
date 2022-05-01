/**
  ******************************************************************************
  * @file    audio_usb_playback_session.c
  * @author  MCD Application Team 
  * @brief   usb audio recording session implementation.
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
#include "usbd_audio.h"
#include "usb_audio.h"
#include "audio_mic_node.h"
#include "audio_sessions_usb.h"
#if  USE_USB_AUDIO_RECORDING



/* Private defines -----------------------------------------------------------*/
#define AUDIO_USB_RECORDING_ALTERNATE           0x01
#define DEFAULT_VOLUME_DB_256                   0
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
#ifdef USE_USB_FS
#define USB_SOF_COUNT_PER_SECOND 1000
#endif /* USE_USB_FS */
#ifdef USE_USB_HS
#define USB_SOF_COUNT_PER_SECOND 8000
#endif /* USE_USB_HS */
#define AUDIO_SYNC_STARTED                      0x01 /* set to 1 when synchro parameters are ready to use */
#define AUDIO_SYNC_NEEDED                       0x02 /* We need to add or remove some samples */
#define AUDIO_SYNC_STABLE                       0x04 /* computed frequency has a good precision */
#define AUDIO_SYNCHRO_MIC_COUNTER_STARTED       0x08 /* Should be set first time when we call the MIC to start counting received bytes */
#define AUDIO_SYNCHRO_OVERRUN_UNDERR_SOON       0x10 /* Flag to detect if overrun or underrun is soon , then one sample is removed or added to each packet*/
#define AUDIO_SYNCHRO_DRIFT_DETECTED            0x40 /* A small drift is detected*/ 
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* Private typedef -----------------------------------------------------------*/
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
typedef struct 
{
  uint32_t current_frequency;   /* estimated mic frequency */
  float    sample_step;         /* sample portion to add or remove each ms */
  float    sample_frac_sum;     /* used to compute number of sample to add each ms*/
  int      samples;             /* number of sample to add/remove to/from next usb packet. When it is negative it means to remove samples*/
  uint32_t mic_estimated_freq;  /* estimated frequency of the mic , updated each second = (number of sample written by microphone during 1 second */
  uint32_t written_in_current_second; /* total of sample written to the main buffer(captured) in current seconde( incremented each ms)*/
  uint16_t sof_counter;         /* count of SOF packet reception */
  int      mic_usb_diff;        /* compute the difference between : total count of samples read from mic - total count of samples written to USB */
  int8_t   write_count_without_read; /* compute time in ms from last USB call (write action ) */
  uint16_t packet_size;         /* packet size */
  int      sample_per_s_th;     /* threshold to detect that a small drift is observed */
  uint16_t buffer_fill_max_th;  /* if filled bytes count is more than this threshold an overrun is soon */
  uint16_t buffer_fill_min_th;  /* if filled bytes count is less than this threshold an underrun is soon */
  uint16_t buffer_fill_moy;     /* the center value of filled bytes */
  uint8_t  status;              /* status of synchronization*/
  int8_t   sample_size;         /* size of 1 sample */
}USB_AudioRecordingSynchronizationParams_t;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Record usb session callbacks */
static int8_t  USB_AudioRecordingSessionStart(AUDIO_USBSession_t *rec_session);
static int8_t  USB_AudioRecordingSessionStop(AUDIO_USBSession_t *rec_session);
static int8_t  USB_AudioRecordingSessionDeInit(uint32_t session_handle);
static int8_t  USB_AudioRecordingSetAudioStreamingInterfaceAlternateSetting( uint8_t alternate,  uint32_t session_handle);
static int8_t  USB_AudioRecordingGetState(uint32_t session_handle);
static int8_t  USB_AudioRecordingSessionCallback(AUDIO_SessionEvent_t  event, 
                                               AUDIO_Node_t* node_handle, 
                                               struct    AUDIO_Session* session_handle);
#if USE_AUDIO_USB_INTERRUPT
static int8_t  USB_AudioRecordingSessionExternalControl( AUDIO_ControlCommand_t control , uint32_t val, uint32_t session_handle);
#endif /*USE_AUDIO_USB_INTERRUPT*/

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
static void USB_AudioRecordingSofReceived(uint32_t session_handle );
static void USB_AudioRecordingSynchroInit(AUDIO_CircularBuffer_t *buf, uint32_t packet_length);
static void USB_AudioRecordingSynchroUpdate(int audio_buffer_filled_size );
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* Private variables ---------------------------------------------------------*/
static AUDIO_USBInputOutputNode_t RecordingUSBOutputNode;
static AUDIO_Description_t RecordingAudioDescription;
static AUDIO_USB_CF_NodeTypeDef RecordingFeatureUnitNode;
static AUDIO_MicNode_t RecordingMicrophoneNode;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
static  USB_AudioRecordingSynchronizationParams_t RecordingSynchronizationParams; /* synchro parameters*/
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* exported functions ---------------------------------------------------------*/

/**
  * @brief  AUDIO_RecordingSessionInit
  *         Initializes the USB audio recording session
  * @param  as_desc(OUT): list of Recording Audio Streaming USB interface information and callbacks , they are used by the USB Audio Class
  *                   Information are like interface number , alternate settings, IN endpoint number.USB Audio class then call callbacks to communicate with the session
  * @param  controls_desc(OUT): controls supported by session like volume set. session adds to this variable the list of controls with related callbacks
  * @param  control_count(IN/OUT): list of control count
  * @param  session_handle(IN): session handle
  * @retval  : 0 if no error
  */
 int8_t  AUDIO_RecordingSessionInit(USBD_AUDIO_AS_InterfaceTypeDef* as_desc,  USBD_AUDIO_ControlTypeDef* controls_desc,
                                     uint8_t* control_count, uint32_t session_handle)
{
  AUDIO_USBSession_t *rec_session;
  AUDIO_USBFeatureUnitDefaults_t controller_defaults;
  
  rec_session = (AUDIO_USBSession_t*)session_handle;
  memset(rec_session, 0, sizeof(AUDIO_USBSession_t));
  rec_session->interface_num = USBD_AUDIO_CONFIG_RECORD_SA_INTERFACE;
  rec_session->alternate = 0;

  rec_session->SessionDeInit = USB_AudioRecordingSessionDeInit;
#if USE_AUDIO_USB_INTERRUPT
   rec_session->ExternalControl = USB_AudioRecordingSessionExternalControl;
#endif /*USE_AUDIO_USB_INTERRUPT*/
  rec_session->session.SessionCallback = USB_AudioRecordingSessionCallback;
  
  /*set audio used option*/
  RecordingAudioDescription.resolution = USB_AUDIO_CONFIG_RECORD_RES_BYTE;
  RecordingAudioDescription.audio_type = USBD_AUDIO_FORMAT_TYPE_PCM;
  RecordingAudioDescription.channels_count = USB_AUDIO_CONFIG_RECORD_CHANNEL_COUNT;
  RecordingAudioDescription.channels_map = USB_AUDIO_CONFIG_RECORD_CHANNEL_MAP; 
  RecordingAudioDescription.frequency = USB_AUDIO_CONFIG_RECORD_DEF_FREQ;
  RecordingAudioDescription.audio_mute = 0;
  RecordingAudioDescription.audio_volume_db_256 = DEFAULT_VOLUME_DB_256;
  *control_count = 0;
  
  /* create list of node */

  /* create mic node */
  AUDIO_MicInit(&RecordingAudioDescription, &rec_session->session, (uint32_t)&RecordingMicrophoneNode);
  rec_session->session.node_list = (AUDIO_Node_t*)&RecordingMicrophoneNode;

  /* create record output */
  USB_AudioStreamingOutputInit(&as_desc->data_ep,  
                                  &RecordingAudioDescription,
                                  &rec_session->session,
                                  (uint32_t)&RecordingUSBOutputNode);
  
   /* create Feature UNIT */
  controller_defaults.audio_description = &RecordingAudioDescription;
  RecordingMicrophoneNode.MicGetVolumeDefaultsValues(&controller_defaults.max_volume,
                                       &controller_defaults.min_volume,
                                       &controller_defaults.res_volume,
                                       (uint32_t)&RecordingMicrophoneNode);

  USB_AudioStreamingFeatureUnitInit(controls_desc,  &controller_defaults,
                              USB_AUDIO_CONFIG_RECORD_UNIT_FEATURE_ID,
                              (uint32_t)&RecordingFeatureUnitNode);
 (*control_count)++;
  RecordingMicrophoneNode.node.next = (AUDIO_Node_t*)&RecordingFeatureUnitNode;
  RecordingFeatureUnitNode.node.next = (AUDIO_Node_t*)&RecordingUSBOutputNode;
  
    /* prepare circular buffer */
  rec_session->buffer.data = malloc(USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE);
  if(!rec_session->buffer.data)
  {
    Error_Handler();
  }
  /* @TODO optimize the margin value */
  USB_AudioStreamingInitializeDataBuffer(&rec_session->buffer, USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE,
                                   AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(&RecordingAudioDescription) , AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(&RecordingAudioDescription));
  /* set USB AUDIO class callbacks */
  as_desc->interface_num = rec_session->interface_num;
  as_desc->alternate = 0;
  as_desc->max_alternate = AUDIO_USB_RECORDING_ALTERNATE;
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
  as_desc->synch_enabled = 0;
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
  as_desc->private_data = session_handle;
  as_desc->SetAS_Alternate = USB_AudioRecordingSetAudioStreamingInterfaceAlternateSetting;
  as_desc->GetState = USB_AudioRecordingGetState;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
  as_desc->SofReceived = USB_AudioRecordingSofReceived;
#else /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
  as_desc->SofReceived =  0;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
  rec_session->session.state = AUDIO_SESSION_INITIALIZED;
  return 0;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  USB_AudioRecordingSessionStart
  *         Start the Recording session
  * @param  rec_session: recording session should be already initialized and not in error state
  * @retval 0 if no error
  */
static  int8_t  USB_AudioRecordingSessionStart( AUDIO_USBSession_t* rec_session)
{
  if(( rec_session->session.state == AUDIO_SESSION_INITIALIZED)
       ||(rec_session->session.state == AUDIO_SESSION_STOPPED))
  {
    AUDIO_USBFeatureUnitCommands_t commands;
    /* start feature control node */
    commands.private_data = (uint32_t)&RecordingMicrophoneNode;
    commands.SetCurrentVolume = RecordingMicrophoneNode.MicSetVolume;
    commands.SetMute = RecordingMicrophoneNode.MicMute;
    rec_session->buffer.rd_ptr = rec_session->buffer.wr_ptr = 0;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
    RecordingSynchronizationParams.status = 0;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */

    /* start the mic */
    RecordingMicrophoneNode.MicStart(&rec_session->buffer, (uint32_t)&RecordingMicrophoneNode);
    /* start the feature */
    RecordingFeatureUnitNode.CFStart(&commands, (uint32_t)&RecordingFeatureUnitNode);
    /* start output node */
    RecordingUSBOutputNode.IOStart(&rec_session->buffer, 0, (uint32_t)&RecordingUSBOutputNode);
    rec_session->session.state = AUDIO_SESSION_STARTED; 
  }
  return 0;
}

/**
  * @brief  USB_AudioRecordingSessionStop
  *         stop the recording session
  * @param  rec_session: recording session
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioRecordingSessionStop(AUDIO_USBSession_t *rec_session)
{
  
  if( rec_session->session.state == AUDIO_SESSION_STARTED)
  {
    RecordingUSBOutputNode.IOStop((uint32_t)&RecordingUSBOutputNode);
    RecordingFeatureUnitNode.CFStop((uint32_t)&RecordingFeatureUnitNode);
    RecordingMicrophoneNode.MicStop((uint32_t)&RecordingMicrophoneNode);
    rec_session->session.state = AUDIO_SESSION_STOPPED;
  }

  return 0;
}

/**
  * @brief  USB_AudioRecordingSessionDeInit
  *         De-Initialize the recording session
  * @param  session_handle: session handle
  * @retval 0 if no error
  */
static int8_t  USB_AudioRecordingSessionDeInit(uint32_t session_handle)
{
  AUDIO_USBSession_t *rec_session;
  
  rec_session = (AUDIO_USBSession_t*)session_handle;
  
  if( rec_session->session.state != AUDIO_SESSION_OFF)
  {
    if( rec_session->session.state == AUDIO_SESSION_STARTED)
    {
      USB_AudioRecordingSessionStop( rec_session);
    }
    RecordingMicrophoneNode.MicDeInit((uint32_t)&RecordingMicrophoneNode);
    RecordingUSBOutputNode.IODeInit((uint32_t)&RecordingUSBOutputNode);
    RecordingFeatureUnitNode.CFDeInit((uint32_t)&RecordingFeatureUnitNode);
    
    if( rec_session->buffer.data)
    {
      free( rec_session->buffer.data);
    }
    rec_session->session.state = AUDIO_SESSION_OFF;
  }

  return 0;
}
/**
  * @brief  USB_AudioRecordingSessionCallback
  *         recording session callback
  * @param  event: event type
  * @param  node: source of event
  * @param  session_handle: session
  * @retval  : 0 if no error
  */
static int underrun_count = 0;
static int overrun_count = 0;
static int8_t  USB_AudioRecordingSessionCallback(AUDIO_SessionEvent_t  event, 
                                               AUDIO_Node_t* node, 
                                               struct    AUDIO_Session* session_handle)
{
   AUDIO_USBSession_t *rec_session;
  
  rec_session = (AUDIO_USBSession_t*)session_handle;
  
  switch(event)
  {
     case AUDIO_FREQUENCY_CHANGED: 
    {
      /* recompute the buffer size */
      RecordingMicrophoneNode.MicChangeFrequency((uint32_t)&RecordingMicrophoneNode);
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
       USB_AudioRecordingSynchroInit(&rec_session->buffer, RecordingUSBOutputNode.packet_length);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
USB_AudioStreamingInitializeDataBuffer(&rec_session->buffer, USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE,
                                AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(&RecordingAudioDescription) ,
                                AUDIO_MS_MAX_PACKET_SIZE_FROM_AUD_DESC(&RecordingAudioDescription));
       break;
    }
    case AUDIO_UNDERRUN :
    case AUDIO_OVERRUN :
    {
      if(event == AUDIO_OVERRUN)
      {
        overrun_count++;
      }
      else
      {
        underrun_count++;
      }
          rec_session->buffer.rd_ptr = rec_session->buffer.wr_ptr = 0;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
      USB_AudioRecordingSynchroInit(&rec_session->buffer, RecordingUSBOutputNode.packet_length);
      RecordingUSBOutputNode.IORestart((uint32_t)&RecordingUSBOutputNode);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
    }
    break;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
  case AUDIO_PACKET_RECEIVED :
    if(++RecordingSynchronizationParams.write_count_without_read == 4)
    {
        /* empty the buffer */
        rec_session->buffer.rd_ptr = rec_session->buffer.wr_ptr = 0;
        RecordingSynchronizationParams.status = 0;
        RecordingUSBOutputNode.IORestart((uint32_t)&RecordingUSBOutputNode);
        RecordingSynchronizationParams.write_count_without_read = 0;
    }
    break;
  case AUDIO_PACKET_PLAYED:
    RecordingSynchronizationParams.write_count_without_read = 0;
    break;
  case AUDIO_BEGIN_OF_STREAM:
    USB_AudioRecordingSynchroInit(&rec_session->buffer, RecordingUSBOutputNode.packet_length);

    break;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
  default : 
    break;
  }
  return 0;
}

/**
  * @brief  USB_AudioRecordingSetAudioStreamingInterfaceAlternateSetting
  *        SA interface set alternate callback
  * @param  alternate:                  
  * @param  session_handle: session
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioRecordingSetAudioStreamingInterfaceAlternateSetting( uint8_t alternate, uint32_t session_handle )
{
  AUDIO_USBSession_t *rec_session;
  
  rec_session = (AUDIO_USBSession_t*)session_handle;
  if(alternate  ==  0)
  {
    if(rec_session->alternate != 0)
    {
      USB_AudioRecordingSessionStop(rec_session);
      rec_session->alternate = 0;
    }
  }
  else
  {
    if(rec_session->alternate  ==  0)
    {
      /* @ADD how to define threshold */
      
      USB_AudioRecordingSessionStart(rec_session);
      rec_session->alternate = alternate;
    }
  }
  return 0;
}

/**
  * @brief  USB_AudioRecordingGetState          
  *         recording SA interface status
  * @param  session_handle: session
  * @retval 0 if no error
  */
static int8_t  USB_AudioRecordingGetState(uint32_t session_handle)
{
  return 0;
}
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
/**
  * @brief  USB_AudioRecordingSofReceived
  *         Updates the estimated  microphone frequency by computing read samples each second
  *         In addition, it computes the difference between read sample from microphone count  and written sample to USB count 
  * @param  session_handle: session handle
  * @retval None 
  */
 static void  USB_AudioRecordingSofReceived(uint32_t session_handle )
 {
    AUDIO_USBSession_t *rec_session;
    uint16_t read_bytes, audio_buffer_filled_size;
    
  rec_session = (AUDIO_USBSession_t*)session_handle;
  if(( rec_session->session.state == AUDIO_SESSION_STARTED)&&(  RecordingSynchronizationParams.status & AUDIO_SYNC_STARTED))
  {
   if(RecordingSynchronizationParams.status&AUDIO_SYNCHRO_MIC_COUNTER_STARTED)
   {

      read_bytes = RecordingMicrophoneNode.MicGetReadCount((uint32_t)&RecordingMicrophoneNode);
      RecordingSynchronizationParams.written_in_current_second += read_bytes;
       if(++RecordingSynchronizationParams.sof_counter == USB_SOF_COUNT_PER_SECOND)
      {
        RecordingSynchronizationParams.mic_estimated_freq = RecordingSynchronizationParams.written_in_current_second/RecordingSynchronizationParams.sample_size;/* the new estimated frequency */
        RecordingSynchronizationParams.sof_counter = 0;
        RecordingSynchronizationParams.written_in_current_second = 0;
      }
      
      RecordingSynchronizationParams.mic_usb_diff += read_bytes;
      if(RecordingSynchronizationParams.mic_estimated_freq)
      {
        audio_buffer_filled_size = AUDIO_BUFFER_FILLED_SIZE(&rec_session->buffer);
        USB_AudioRecordingSynchroUpdate(audio_buffer_filled_size);
      }
      else
      {
        if(RecordingSynchronizationParams.mic_usb_diff >= RecordingSynchronizationParams.packet_size*2)
        {
          RecordingSynchronizationParams.samples = RecordingSynchronizationParams.sample_size;
        }
        else
        {
          if(RecordingSynchronizationParams.mic_usb_diff + 2*RecordingSynchronizationParams.packet_size <= 0)
          {
            RecordingSynchronizationParams.samples = -RecordingSynchronizationParams.sample_size;
          }
          else
          if((RecordingSynchronizationParams.mic_usb_diff <= RecordingSynchronizationParams.packet_size)&&
             (RecordingSynchronizationParams.mic_usb_diff + RecordingSynchronizationParams.packet_size >= 0))
          {
            RecordingSynchronizationParams.samples = 0;
          }
        }
      }
   }
    else
    {
      RecordingMicrophoneNode.MicStartReadCount((uint32_t)&RecordingMicrophoneNode);
      RecordingSynchronizationParams.status |= AUDIO_SYNCHRO_MIC_COUNTER_STARTED;
    }
  }
 }


/**
  * @brief  USB_AudioRecordingSynchroInit
  *         initializes the synchronization structure.
  * @param  buf(IN): data buffer
  * @param  packet_length(IN): packet length
  * @retval None
  */
static void USB_AudioRecordingSynchroInit(AUDIO_CircularBuffer_t *buf, uint32_t packet_length)
{
  RecordingSynchronizationParams.packet_size = packet_length;
  RecordingSynchronizationParams.sample_size = AUDIO_SAMPLE_LENGTH(&RecordingAudioDescription);
  RecordingSynchronizationParams.buffer_fill_max_th = buf->size*3/4;
  RecordingSynchronizationParams.buffer_fill_min_th = buf->size/4;
  RecordingSynchronizationParams.buffer_fill_moy = buf->size>>1;
#ifdef USE_USB_HS
  RecordingSynchronizationParams.sample_per_s_th = packet_length<<2;
#else 
  RecordingSynchronizationParams.sample_per_s_th = packet_length>>1;
#endif /* USE_USB_HS */
  RecordingSynchronizationParams.current_frequency = RecordingAudioDescription.frequency;
  RecordingSynchronizationParams.mic_estimated_freq = 0;
  RecordingSynchronizationParams.write_count_without_read = 0;
  RecordingSynchronizationParams.mic_usb_diff = 0;
  RecordingSynchronizationParams.samples = 0;
  RecordingSynchronizationParams.sof_counter = 0;
  RecordingSynchronizationParams.written_in_current_second = 0;
  RecordingSynchronizationParams.status|= AUDIO_SYNC_NEEDED;
  RecordingSynchronizationParams.status = AUDIO_SYNC_STARTED;
}

/**
  * @brief  USB_AudioRecordingSynchroUpdate
  *         update synchronization parameters, when needed. This call is done within a SOF interrupt handler.
  * @param  audio_buffer_filled_size: buffer filled size
  * @retval None
  */
static void  USB_AudioRecordingSynchroUpdate(int audio_buffer_filled_size)
{
    uint8_t update_synchro = 0;
 
   if((RecordingSynchronizationParams.status&AUDIO_SYNCHRO_OVERRUN_UNDERR_SOON)== 0)
   {
     /* NO SOON OVERRUN OR UNDERRUN DETECTED*/
     if((audio_buffer_filled_size<RecordingSynchronizationParams.buffer_fill_max_th) && (audio_buffer_filled_size>RecordingSynchronizationParams.buffer_fill_min_th))
     {
       /* In this block no risk of buffer overflow or underflow*/
       if((RecordingSynchronizationParams.mic_usb_diff < RecordingSynchronizationParams.sample_per_s_th)
          &&(RecordingSynchronizationParams.mic_usb_diff > (-RecordingSynchronizationParams.sample_per_s_th)))
       {
         /* the sample rate drift is less than limits */
          if((RecordingSynchronizationParams.status&AUDIO_SYNCHRO_DRIFT_DETECTED) == 0)
          {
           if( RecordingSynchronizationParams.mic_estimated_freq != RecordingSynchronizationParams.current_frequency)
           {
             update_synchro = 1;/* frequency value is changed than compute the new frequency */
           }
          }
          else
          {
            /* in last ms a drift was detected then check if this drift is eliminated */
            if(((RecordingSynchronizationParams.mic_usb_diff<=0 )
                 &&( RecordingSynchronizationParams.mic_estimated_freq < RecordingSynchronizationParams.current_frequency))||
               ( (RecordingSynchronizationParams.mic_usb_diff>=0 )
                  &&( RecordingSynchronizationParams.mic_estimated_freq > RecordingSynchronizationParams.current_frequency)))
            {
               update_synchro = 1;
            }
          }
       }
       else
       {
        if(((RecordingSynchronizationParams.mic_usb_diff>0) && ( RecordingSynchronizationParams.mic_estimated_freq > RecordingSynchronizationParams.current_frequency))||
           ((RecordingSynchronizationParams.mic_usb_diff<0) && ( RecordingSynchronizationParams.mic_estimated_freq < RecordingSynchronizationParams.current_frequency)))
        {
           update_synchro = 1;
        }
        else
        {
          if(RecordingSynchronizationParams.mic_usb_diff > 0)
          {
            RecordingSynchronizationParams.current_frequency++;
            RecordingSynchronizationParams.sample_step += (RecordingAudioDescription.frequency < RecordingSynchronizationParams.current_frequency)? (float)RecordingSynchronizationParams.sample_size/USB_SOF_COUNT_PER_SECOND:(float)-RecordingSynchronizationParams.sample_size/USB_SOF_COUNT_PER_SECOND;
          }
           if(RecordingSynchronizationParams.mic_usb_diff < 0)
          {
            RecordingSynchronizationParams.current_frequency--;
            RecordingSynchronizationParams.sample_step += (RecordingAudioDescription.frequency < RecordingSynchronizationParams.current_frequency)? (float)-RecordingSynchronizationParams.sample_size/USB_SOF_COUNT_PER_SECOND:(float)RecordingSynchronizationParams.sample_size/USB_SOF_COUNT_PER_SECOND;
          }
          RecordingSynchronizationParams.sample_frac_sum = RecordingSynchronizationParams.sample_size;
          RecordingSynchronizationParams.status|= AUDIO_SYNCHRO_DRIFT_DETECTED;
        }
       }
     }
     else
     {
       RecordingSynchronizationParams.samples = (audio_buffer_filled_size>=RecordingSynchronizationParams.buffer_fill_max_th)? RecordingSynchronizationParams.sample_size:-RecordingSynchronizationParams.sample_size;
       RecordingSynchronizationParams.status|= AUDIO_SYNCHRO_OVERRUN_UNDERR_SOON;
     }
   }
   else
   {
     if(((RecordingSynchronizationParams.samples>0)&&(audio_buffer_filled_size>=RecordingSynchronizationParams.buffer_fill_moy))||
        ((RecordingSynchronizationParams.samples<0)&&(audio_buffer_filled_size<=RecordingSynchronizationParams.buffer_fill_moy)))
     {
       update_synchro = 1;
       RecordingSynchronizationParams.status &= ~AUDIO_SYNCHRO_OVERRUN_UNDERR_SOON;
     }
   }
   
   if((RecordingSynchronizationParams.status&AUDIO_SYNCHRO_OVERRUN_UNDERR_SOON) == 0)
   {
     if(update_synchro)
     {
       RecordingSynchronizationParams.current_frequency = RecordingSynchronizationParams.mic_estimated_freq;
       if(RecordingAudioDescription.frequency > RecordingSynchronizationParams.current_frequency)
       {
         RecordingSynchronizationParams.sample_step = (float)((RecordingAudioDescription.frequency - RecordingSynchronizationParams.current_frequency))*RecordingSynchronizationParams.sample_size/USB_SOF_COUNT_PER_SECOND;
       }
       else
       {
         RecordingSynchronizationParams.sample_step = (float)((RecordingSynchronizationParams.current_frequency- RecordingAudioDescription.frequency))*RecordingSynchronizationParams.sample_size/USB_SOF_COUNT_PER_SECOND;
       }
       RecordingSynchronizationParams.status = (AUDIO_SYNC_NEEDED|AUDIO_SYNC_STARTED|AUDIO_SYNCHRO_MIC_COUNTER_STARTED);
       RecordingSynchronizationParams.sample_frac_sum = 0;
       RecordingSynchronizationParams.samples = 0;
       RecordingSynchronizationParams.mic_usb_diff = 0;
    }
    
    if(RecordingSynchronizationParams.sample_step)
    {
       RecordingSynchronizationParams.sample_frac_sum+=RecordingSynchronizationParams.sample_step;
       if(RecordingSynchronizationParams.sample_frac_sum>RecordingSynchronizationParams.sample_size)
       {
         RecordingSynchronizationParams.samples = (RecordingAudioDescription.frequency < RecordingSynchronizationParams.current_frequency)?RecordingSynchronizationParams.sample_size:-RecordingSynchronizationParams.sample_size;
         RecordingSynchronizationParams.sample_frac_sum -= RecordingSynchronizationParams.sample_size;
       }
       else
       {
         RecordingSynchronizationParams.samples = 0;
       }
     }
   }
}

#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */

#ifdef  USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
/**
  * @brief  USB_AudioRecordingSynchronizationGetSamplesCountToAddInNextPckt
  *         get sample count to add or remove in next main circular buffer read call
  * @param  session_handle(IN): session handler
  * @retval sample to add(positive value) or remove (negative value)
  */
int8_t  USB_AudioRecordingSynchronizationGetSamplesCountToAddInNextPckt(struct  AUDIO_Session* session_handle)
{
   if(RecordingSynchronizationParams.status&AUDIO_SYNC_STARTED)
   {
     return RecordingSynchronizationParams.samples;
   }
   return 0;
}
/**
  * @brief  USB_AudioRecordingSynchronizationNotificationSamplesRead
  *         set last packet written bytes
  * @param  session_handle: session handles
  * @retval bytes : written or read
  */
 int8_t  USB_AudioRecordingSynchronizationNotificationSamplesRead(struct AUDIO_Session* session_handle, uint16_t bytes)
{
   if(RecordingSynchronizationParams.status&AUDIO_SYNC_STARTED)
   {
     RecordingSynchronizationParams.mic_usb_diff -= bytes;
   }
   return 0;
}
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
#if USE_AUDIO_USB_INTERRUPT
/**
  * @brief  USB_AudioRecordingSessionExternalControl
  *         Mute or Unmute 
  * @param  session_handle: session
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioRecordingSessionExternalControl( AUDIO_ControlCommand_t control , uint32_t val, uint32_t session_handle)
{
  AUDIO_USBSession_t *rec_session;
  USBD_AUDIO_InterruptTypeDef interrupt;
  
   rec_session = (AUDIO_USBSession_t*)session_handle;
   if( (rec_session->session.state != AUDIO_SESSION_OFF)&&(rec_session->session.state != AUDIO_SESSION_ERROR))
   {
      switch(control)
    {
      case USBD_AUDIO_MUTE_UNMUTE:
      {
        uint8_t mute;
        
        mute = !RecordingAudioDescription.audio_mute;
        RecordingFeatureUnitNode.CFSetMute(0,mute, (uint32_t) &RecordingFeatureUnitNode);
        interrupt.type  = USBD_AUDIO_INTERRUPT_INFO_FROM_INTERFACE;
        interrupt.attr = USBD_AUDIO_INTERRUPT_ATTR_CUR;
        interrupt.cs = USBD_AUDIO_FU_MUTE_CONTROL;
        interrupt.cn_mcn = 0;
        interrupt.entity_id = USB_AUDIO_CONFIG_RECORD_UNIT_FEATURE_ID;
        interrupt.ep_if_id = 0;/* Audio control interface 0*/
      interrupt.priority = USBD_AUDIO_NORMAL_PRIORITY;
      }
       break;
    default :
      break;
    }
    USBD_AUDIO_SendInterrupt  (&interrupt);
   }
   else
   {
     return -1;
   }
  return 0;
}
#endif /*USE_AUDIO_USB_INTERRUPT*/
#endif /* USE_USB_AUDIO_RECORDING*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
