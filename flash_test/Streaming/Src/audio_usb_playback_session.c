/**
  ******************************************************************************
  * @file    audio_usb_playback_session.c
  * @author  MCD Application Team 
  * @brief   usb audio playback session.
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
#include "audio_speaker_node.h"
#include "audio_sessions_usb.h"

#if USE_USB_AUDIO_PLAYBACK
/* Private defines -----------------------------------------------------------*/
#define AUDIO_USB_PLAYBACK_ALTERNATE 0x01

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
#if USE_AUDIO_PLAYBACK_RECORDING_SHARED_CLOCK_SRC
#if  USE_USB_AUDIO_RECORDING
  extern AUDIO_USBSession_t USB_AudioRecordingSession;
#endif /* USE_USB_AUDIO_RECORDING*/
#endif /* USE_AUDIO_PLAYBACK_RECORDING_SHARED_CLOCK_SRC */
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Play usb session callbacks */
static int8_t  USB_AudioPlaybackSessionStart(AUDIO_USBSession_t* session);
static int8_t  USB_AudioPlaybackSessionStop(AUDIO_USBSession_t* session);
static int8_t  USB_AudioPlaybackSessionDeInit(uint32_t session_handle);
#if USE_AUDIO_USB_INTERRUPT
static int8_t  USB_AudioPlaybackSessionExternalControl( AUDIO_ControlCommand_t control , uint32_t val, uint32_t session_handle);
#endif /*USE_AUDIO_USB_INTERRUPT*/
static int8_t  USB_AudioPlaybackSetAudioStreamingInterfaceAlternateSetting( uint8_t alternate,  uint32_t session_handle);
static int8_t  USB_AudioPlaybackGetState(uint32_t session_handle);
static int8_t  USB_AudioPlaybackSessionCallback(AUDIO_SessionEvent_t  event, 
                                               AUDIO_Node_t* node_handle, 
                                               struct    AUDIO_Session* session_handle);
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
static uint32_t   USB_AudioPlaybackGetFeedback( uint32_t session_handle );
static void  AUDIO_USB_Session_Sof_Received(uint32_t session_handle );
#endif  /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */

/* Private variables ---------------------------------------------------------*/

/* list of used nodes */
static AUDIO_USBInputOutputNode_t PlaybackUSBInputNode;
static AUDIO_Description_t PlaybackAudioDescription;
static AUDIO_USB_CF_NodeTypeDef PlaybackFeatureUnitNode;
static AUDIO_SpeakerNode_t PlaybackSpeakerOutputNode;
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
/* Playback synchronization : frequency estimation */
static uint8_t PlaybackSynchroFirstSofReceived = 0;
static uint32_t PlaybackSynchroEstimatedCodecFrequency = 0;
#endif  /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  AUDIO_PlaybackSessionInit
  *         Initializes the playback session.
  * @param  as_desc(OUT):  audio streaming callbacks to communicate with the USB Audio Class module 
  * @param  controls_desc(OUT): list of control , the playback session set required control to the USB Audio Class module
  * @param  control_count(IN/OUT): controls count
  * @param  session_handle(IN): session handle should be allocated.
  * @retval  : 0 if no error
  */
 int8_t  AUDIO_PlaybackSessionInit(USBD_AUDIO_AS_InterfaceTypeDef* as_desc,  
                                    USBD_AUDIO_ControlTypeDef* controls_desc,
                                    uint8_t* control_count, uint32_t session_handle)
{
  AUDIO_USBSession_t *play_session;
  AUDIO_USBFeatureUnitDefaults_t controller_defaults;
  
   play_session = (AUDIO_USBSession_t*)session_handle;
   memset( play_session, 0, sizeof(AUDIO_USBSession_t));
  
   play_session->interface_num = USBD_AUDIO_CONFIG_PLAY_SA_INTERFACE;
   play_session->alternate = 0;
   play_session->SessionDeInit = USB_AudioPlaybackSessionDeInit;
#if USE_AUDIO_USB_INTERRUPT
   play_session->ExternalControl = USB_AudioPlaybackSessionExternalControl;
#endif /*USE_AUDIO_USB_INTERRUPT*/
   play_session->session.SessionCallback = USB_AudioPlaybackSessionCallback;
   play_session->buffer.size = USB_AUDIO_CONFIG_PLAY_BUFFER_SIZE;
   play_session->buffer.data = malloc( USB_AUDIO_CONFIG_PLAY_BUFFER_SIZE); 
   if(! play_session->buffer.data)
   {
    Error_Handler();
   }
    /*set audio used option*/
  PlaybackAudioDescription.resolution = USB_AUDIO_CONFIG_PLAY_RES_BYTE;
  PlaybackAudioDescription.audio_type = USBD_AUDIO_FORMAT_TYPE_PCM; /* PCM*/
  PlaybackAudioDescription.channels_count = USB_AUDIO_CONFIG_PLAY_CHANNEL_COUNT;
  PlaybackAudioDescription.channels_map = USB_AUDIO_CONFIG_PLAY_CHANNEL_MAP; /* Left and Right */
  PlaybackAudioDescription.frequency = USB_AUDIO_CONFIG_PLAY_DEF_FREQ;
  PlaybackAudioDescription.audio_volume_db_256 = VOLUME_SPEAKER_DEFAULT_DB_256;
  PlaybackAudioDescription.audio_mute = 0;
  *control_count = 0;
 
   /* create usb input node */
  USB_AudioStreamingInputInit(&as_desc->data_ep,  &PlaybackAudioDescription,  &play_session->session,  (uint32_t)&PlaybackUSBInputNode);
   play_session->session.node_list = (AUDIO_Node_t*)&PlaybackUSBInputNode;
  /* initialize usb feature node */
  controller_defaults.audio_description = &PlaybackAudioDescription;
    /* please choose default volumes value of  speaker */
  controller_defaults.max_volume = VOLUME_SPEAKER_MAX_DB_256;
  controller_defaults.min_volume = VOLUME_SPEAKER_MIN_DB_256;
  controller_defaults.res_volume = VOLUME_SPEAKER_RES_DB_256;
  USB_AudioStreamingFeatureUnitInit( controls_desc,  &controller_defaults,  USB_AUDIO_CONFIG_PLAY_UNIT_FEATURE_ID, (uint32_t)&PlaybackFeatureUnitNode);
  (*control_count)++;
  PlaybackUSBInputNode.node.next = (AUDIO_Node_t*)&PlaybackFeatureUnitNode;
  AUDIO_SpeakerInit(&PlaybackAudioDescription, &play_session->session, (uint32_t)&PlaybackSpeakerOutputNode);
  PlaybackFeatureUnitNode.node.next = (AUDIO_Node_t*)&PlaybackSpeakerOutputNode;

/* initializes synchronization setting */
  
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
     as_desc->synch_enabled = 1;
     as_desc->synch_ep.ep_num = USB_AUDIO_CONFIG_PLAY_EP_SYNC;
     as_desc->synch_ep.GetFeedback = USB_AudioPlaybackGetFeedback;
     as_desc->synch_ep.private_data = (uint32_t) play_session;
     as_desc->SofReceived = AUDIO_USB_Session_Sof_Received;
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
  /* set USB AUDIO class callbacks */
  as_desc->interface_num =  play_session->interface_num;
  as_desc->alternate = 0;
  as_desc->max_alternate = AUDIO_USB_PLAYBACK_ALTERNATE;
  as_desc->private_data = session_handle;
  as_desc->SetAS_Alternate = USB_AudioPlaybackSetAudioStreamingInterfaceAlternateSetting;
  as_desc->GetState = USB_AudioPlaybackGetState;

  /* initialize working buffer */
  uint16_t buffer_margin = (PlaybackUSBInputNode.max_packet_length > PlaybackUSBInputNode.packet_length)?PlaybackUSBInputNode.max_packet_length:0;
  USB_AudioStreamingInitializeDataBuffer(&play_session->buffer, USB_AUDIO_CONFIG_PLAY_BUFFER_SIZE,
                                  AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(&PlaybackAudioDescription) , buffer_margin);
  play_session->session.state = AUDIO_SESSION_INITIALIZED;

  return 0;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  USB_AudioPlaybackSessionStart
  *         Starts  the playback session
  * @param  play_session(IN):session handler
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioPlaybackSessionStart(AUDIO_USBSession_t*  play_session)
{
  if(( play_session->session.state == AUDIO_SESSION_INITIALIZED)
     ||(play_session->session.state == AUDIO_SESSION_STOPPED))
  {
        AUDIO_USBFeatureUnitCommands_t commands;
    /* start input node */
    PlaybackUSBInputNode.IOStart(& play_session->buffer,   play_session->buffer.size/2,  (uint32_t)&PlaybackUSBInputNode);
    commands.private_data = (uint32_t)&PlaybackSpeakerOutputNode;
    commands.SetMute = PlaybackSpeakerOutputNode.SpeakerMute;
    commands.SetCurrentVolume = PlaybackSpeakerOutputNode.SpeakerSetVolume;
    PlaybackFeatureUnitNode.CFStart(&commands,(uint32_t)&PlaybackFeatureUnitNode);
    play_session->session.state = AUDIO_SESSION_STARTED;
  }
  
  return 0;
}

/**
  * @brief  USB_AudioPlaybackSessionStop
  *         Stop the playback session
  * @param  play_session:               
  * @retval 0 if no error
  */
static int8_t  USB_AudioPlaybackSessionStop(AUDIO_USBSession_t*  play_session)
{
  
  if( play_session->session.state == AUDIO_SESSION_STARTED)
  {
    PlaybackUSBInputNode.IOStop((uint32_t)&PlaybackUSBInputNode);
    PlaybackFeatureUnitNode.CFStop((uint32_t)&PlaybackFeatureUnitNode);
    PlaybackSpeakerOutputNode.SpeakerStop((uint32_t)&PlaybackSpeakerOutputNode);
    play_session->session.state = AUDIO_SESSION_STOPPED;
  }
  
  return 0;
}

/**
  * @brief  USB_AudioPlaybackSessionDeInit
  *         De-Initialize the play (streaming) session
  * @param  session_handle: session handle
  * @retval 0 if no error
  */
static int8_t  USB_AudioPlaybackSessionDeInit(uint32_t session_handle)
{
  AUDIO_USBSession_t* play_session;
  
  play_session = (AUDIO_USBSession_t*)session_handle;
  if( play_session->session.state != AUDIO_SESSION_OFF)
  {
    if( play_session->session.state == AUDIO_SESSION_STARTED)
    {
      USB_AudioPlaybackSessionStop( play_session);
    }
    PlaybackSpeakerOutputNode.SpeakerDeInit((uint32_t)&PlaybackSpeakerOutputNode);
    PlaybackFeatureUnitNode.CFDeInit((uint32_t)&PlaybackFeatureUnitNode);
    PlaybackUSBInputNode.IODeInit((uint32_t)&PlaybackUSBInputNode);
    if( play_session->buffer.data)
    {
      free( play_session->buffer.data);
    }
     play_session->session.state = AUDIO_SESSION_OFF;
  }
  return 0;
}

#if USE_AUDIO_USB_INTERRUPT
/**
  * @brief  USB_AudioPlaybackSessionExternalControl
  *         Mute or Unmute 
  * @param  session_handle: session
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioPlaybackSessionExternalControl( AUDIO_ControlCommand_t control , uint32_t val, uint32_t session_handle)
{
  AUDIO_USBSession_t *play_session;
  USBD_AUDIO_InterruptTypeDef interrupt;
  
   play_session = (AUDIO_USBSession_t*)session_handle;
   if( (play_session->session.state != AUDIO_SESSION_OFF)&&(play_session->session.state != AUDIO_SESSION_ERROR))
   {
      switch(control)
    {
      case USBD_AUDIO_MUTE_UNMUTE:
      {
        uint8_t mute;
        
        mute = !PlaybackAudioDescription.audio_mute;
        PlaybackFeatureUnitNode.CFSetMute(0,mute, (uint32_t) &PlaybackFeatureUnitNode);
        interrupt.type  = USBD_AUDIO_INTERRUPT_INFO_FROM_INTERFACE;
        interrupt.attr = USBD_AUDIO_INTERRUPT_ATTR_CUR;
        interrupt.cs = USBD_AUDIO_FU_MUTE_CONTROL;
        interrupt.cn_mcn = 0;
        interrupt.entity_id = USB_AUDIO_CONFIG_PLAY_UNIT_FEATURE_ID;
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
/**
  * @brief  USB_AudioPlaybackSessionCallback
  *         session callback for the audio playback, it receives events from nodes.
  * @param  event(IN):                      
  * @param  node(IN): Event emitter
  * @param  session_handle: playback session
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioPlaybackSessionCallback(AUDIO_SessionEvent_t  event, 
                                               AUDIO_Node_t* node, 
                                               struct    AUDIO_Session* session_handle)
{
  AUDIO_USBSession_t * play_session = (AUDIO_USBSession_t *)session_handle;
  
  switch(event)
  {
  case AUDIO_THRESHOLD_REACHED:  /*  the buffer fill threshold is reached, then playback starts the speaker to consume data */
    
    if(node->type  ==  AUDIO_INPUT)
    {
      PlaybackSpeakerOutputNode.SpeakerStart(& play_session->buffer, (uint32_t)&PlaybackSpeakerOutputNode);
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
	  PlaybackSynchroFirstSofReceived =0;   /* restart synchronization*/
#endif  /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
    }
    break;
  case AUDIO_PACKET_RECEIVED:
    
    break;
  case AUDIO_FREQUENCY_CHANGED: 
    {
      /* recompute the buffer size */
     PlaybackSpeakerOutputNode.SpeakerChangeFrequency((uint32_t)&PlaybackSpeakerOutputNode);
     uint16_t buffer_margin = (PlaybackUSBInputNode.max_packet_length > PlaybackUSBInputNode.packet_length)? PlaybackUSBInputNode.max_packet_length:0;
  USB_AudioStreamingInitializeDataBuffer(&play_session->buffer, USB_AUDIO_CONFIG_PLAY_BUFFER_SIZE,
                                  AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(&PlaybackAudioDescription) , buffer_margin);
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
     PlaybackSynchroFirstSofReceived =0;
     PlaybackSynchroEstimatedCodecFrequency = 0;
#endif  /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */   
    break;
    }
  case AUDIO_OVERRUN:
  case AUDIO_UNDERRUN:
    {
     /* restart input and stop output */
     PlaybackSpeakerOutputNode.SpeakerStop((uint32_t)&PlaybackSpeakerOutputNode);
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
     PlaybackSynchroFirstSofReceived =0;
     PlaybackSynchroEstimatedCodecFrequency = 0;
#endif  /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */ 
     if( play_session->session.state == AUDIO_SESSION_STARTED)
     {
       PlaybackUSBInputNode.IORestart((uint32_t)&PlaybackUSBInputNode);
     }
      break;
    }
  default :
   break;
  }
  return 0;
}


/**
  * @brief  USB_AudioPlaybackSetAudioStreamingInterfaceAlternateSetting
  *         set "Audio Streaming interface alternate" setting callback to be called by the class module
  * @param  alternate(IN):                      
  * @param  session_handle(IN): session handle
  * @retval  : 0 if no error
  */
static int8_t  USB_AudioPlaybackSetAudioStreamingInterfaceAlternateSetting( uint8_t alternate , uint32_t session_handle)
{
  AUDIO_USBSession_t * play_session;
  
   play_session = (AUDIO_USBSession_t*)session_handle;
  if(alternate  ==  0)
  {
    if( play_session->alternate != 0)
    {
       USB_AudioPlaybackSessionStop(play_session);
       play_session->alternate = 0;
    }
  }
  else
  {
    if( play_session->alternate  ==  0)
    {
      USB_AudioPlaybackSessionStart(play_session);
      play_session->alternate = alternate;
    }
  }
  return 0;
}

/**
  * @brief  USB_AudioPlaybackGetState            
  *         return AS interface state
  * @param  session_handle: session
  * @retval 0 if no error
  */
static int8_t  USB_AudioPlaybackGetState(uint32_t session_handle)
{  

  return 0;
}

#if USE_AUDIO_PLAYBACK_USB_FEEDBACK

/**
  * @brief  USB_AudioPlaybackGetFeedback
  *         get played sample counter
  * @param  session_handle: session
  * @retval  : 0 if buffer ok , 1 if overrun soon , -1 if underrun soon
  */
static uint32_t   USB_AudioPlaybackGetFeedback( uint32_t session_handle )
{
 if((PlaybackSpeakerOutputNode.node.state == AUDIO_NODE_STARTED))
  {
    if(PlaybackSynchroEstimatedCodecFrequency)
    {
      return PlaybackSynchroEstimatedCodecFrequency ;
    }
    else
    {
     AUDIO_CircularBuffer_t *buffer = &((AUDIO_USBSession_t*)session_handle)->buffer;
     uint32_t wr_distance;
     
     wr_distance=AUDIO_BUFFER_FREE_SIZE(buffer);
     if(wr_distance <= (buffer->size>>2))
     {
       return PlaybackAudioDescription.frequency - 1000;
     }
     if( wr_distance >= (buffer->size - (buffer->size>>2)))
     {
       return PlaybackAudioDescription.frequency + 1000;
     }
    }
  }
 return PlaybackAudioDescription.frequency;
}

/**
  * @brief  AUDIO_USB_Session_Sof_Received
  *         update the rate of audio
  * @param  session_handle: session
  * @retval  : 
  */

static void  AUDIO_USB_Session_Sof_Received(uint32_t session_handle )
 {
   static uint16_t sof_counter = 0;
#ifdef USE_USB_HS
   static uint8_t micro_sof_counter = 0;
#endif /* USE_USB_HS */
   static uint32_t total_received_sub_samples = 0;
    AUDIO_USBSession_t *session;
    uint16_t read_samples_per_channel ;
    
  session = (AUDIO_USBSession_t*)session_handle;
  if( session->session.state == AUDIO_SESSION_STARTED) 
  {
   if(PlaybackSynchroFirstSofReceived)
   {
#ifdef USE_USB_HS
     if(micro_sof_counter !=7)
     {
       micro_sof_counter++;
     }
     else
     {
#endif /* USE_USB_HS */
        read_samples_per_channel = PlaybackSpeakerOutputNode.SpeakerGetReadCount((uint32_t)&PlaybackSpeakerOutputNode);
        total_received_sub_samples += read_samples_per_channel;
        if(++sof_counter == 1000)
        {
          PlaybackSynchroEstimatedCodecFrequency =((total_received_sub_samples)>>1); 
          sof_counter =0;
          total_received_sub_samples = 0;
        }
#ifdef USE_USB_HS
        micro_sof_counter = 0;
     }
#endif /* USE_USB_HS */
   }
   else
   {
       PlaybackSpeakerOutputNode.SpeakerStartReadCount((uint32_t)&PlaybackSpeakerOutputNode);
       sof_counter = 0;
#ifdef USE_USB_HS
       micro_sof_counter = 0;
#endif /* USE_USB_HS */
       total_received_sub_samples = 0;
       PlaybackSynchroFirstSofReceived = 1;
    }
  }
  else
  {
    PlaybackSynchroFirstSofReceived = 0;
  }
 }
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
  
#endif /*USE_USB_AUDIO_PLAYBACK*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
