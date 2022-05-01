/**
  ******************************************************************************
  * @file    usbd_audio_config_descriptors.c
  * @author  MCD Application Team 
  * @brief   List of usb audio class configuration descriptors.
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
#include "audio_node.h"

/* private defines and macro ------------------------------------------------------------------*/
#if USE_USB_AUDIO_PLAYBACK
#define PLAYBACK_AC_INTERFACE_SIZE ( USBD_AUDIO_INPUT_TERMINAL_DESC_SIZE +\
                                     USBD_AUDIO_FEATURE_UNIT_DESC_SIZE(2,1) /* Feature Unit */ + USBD_AUDIO_OUTPUT_TERMINAL_DESC_SIZE /* output terminal */)
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
#define PLAYBACK_AS_SYNCH_EP_DESC_SIZE 0x09
#else
#define PLAYBACK_AS_SYNCH_EP_DESC_SIZE 0x00
#endif

#define PLAYBACK_AS_INTERFACES_SIZE ( USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE/*AS Zero bandwidth*/+\
                                      USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE/*AS for playback*/ +\
                                      USBD_AUDIO_AS_CS_INTERFACE_DESC_SIZE /* Specific AS descriptors */ +\
                                      USBD_USBD_AUDIO_FORMAT_TYPE_I_DESC_SIZE(USB_AUDIO_CONFIG_PLAY_FREQ_COUNT) /* format type I desc */+\
                                       USBD_AUDIO_STANDARD_ENDPOINT_DESC_SIZE +\
                                       USBD_AUDIO_SPECIFIC_DATA_ENDPOINT_DESC_SIZE +\
                                       PLAYBACK_AS_SYNCH_EP_DESC_SIZE)
#define PLAYBACK_AS_INTERFACE_COUNT 1
#else /* USE_USB_AUDIO_PLAYBACK */
#define PLAYBACK_AS_INTERFACES_SIZE 0
#define PLAYBACK_AC_INTERFACE_SIZE 0
#define PLAYBACK_AS_INTERFACE_COUNT 0
#endif /* USE_USB_AUDIO_PLAYBACK */

#if  USE_USB_AUDIO_RECORDING
#define RECORDING_AC_INTERFACE_SIZE (  USBD_AUDIO_INPUT_TERMINAL_DESC_SIZE +\
                                     USBD_AUDIO_FEATURE_UNIT_DESC_SIZE(2,1) /* Feature Unit */ + USBD_AUDIO_OUTPUT_TERMINAL_DESC_SIZE /* output terminal */)


#define RECORDING_AS_INTERFACES_SIZE ( USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE/*AS Zero bandwidth*/+\
                                      USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE/*AS for RECORDING*/ +\
                                      USBD_AUDIO_AS_CS_INTERFACE_DESC_SIZE /* Specific AS descriptors */ +\
                                      USBD_USBD_AUDIO_FORMAT_TYPE_I_DESC_SIZE(USB_AUDIO_CONFIG_RECORD_FREQ_COUNT) /* format type I desc */+\
                                       USBD_AUDIO_STANDARD_ENDPOINT_DESC_SIZE +\
                                       USBD_AUDIO_SPECIFIC_DATA_ENDPOINT_DESC_SIZE)

#define RECORDING_AS_INTERFACE_COUNT 1
#else /* USE_USB_AUDIO_RECORDING */
#define RECORDING_AS_INTERFACES_SIZE 0
#define RECORDING_AC_INTERFACE_SIZE 0
#define RECORDING_AS_INTERFACE_COUNT 0
#endif /* USE_USB_AUDIO_RECORDING */

#define CONFIG_DESCRIPTOR_AS_INTERFACES_COUNT (RECORDING_AS_INTERFACE_COUNT + PLAYBACK_AS_INTERFACE_COUNT)
#define CONFIG_DESCRIPTOR_AC_TOTAL_SIZE   ( USBD_AUDIO_AC_CS_INTERFACE_DESC_SIZE(CONFIG_DESCRIPTOR_AS_INTERFACES_COUNT) /*Class-Specific AC Interface Header Descriptor */ + \
                                              PLAYBACK_AC_INTERFACE_SIZE + RECORDING_AC_INTERFACE_SIZE ) 

#define CONFIG_DESCRIPTOR_SIZE  (0x09 +\
                                 USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE+\
                                 CONFIG_DESCRIPTOR_AC_TOTAL_SIZE +\
                                 PLAYBACK_AS_INTERFACES_SIZE + \
                                 RECORDING_AS_INTERFACES_SIZE)



/* private variables ------------------------------------------------------------------*/
__ALIGN_BEGIN static uint8_t USBD_AUDIO_ConfigDescriptor[CONFIG_DESCRIPTOR_SIZE ] __ALIGN_END =
{
  /* Configuration 1 */
  0x09,                                         /* bLength */
  USB_DESC_TYPE_CONFIGURATION,                  /* bDescriptorType */
  LOBYTE(CONFIG_DESCRIPTOR_SIZE),               /* wTotalLength  */
  HIBYTE(CONFIG_DESCRIPTOR_SIZE),      
  0x01 + CONFIG_DESCRIPTOR_AS_INTERFACES_COUNT, /* bNumInterfaces */
  0x01,                                         /* bConfigurationValue */
  0x00,                                         /* iConfiguration */
  0xC0,                                         /* bmAttributes  BUS Powred*/
  0x32,                                         /* bMaxPower = 100 mA*/
  /* 09 byte*/
  
  /* Standard AC Interface Descriptor: Audio control interface*/
  USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE,                    /* bLength */
  USB_DESC_TYPE_INTERFACE,                      /* bDescriptorType */
  0x00,                                         /* bInterfaceNumber */
  0x00,                                         /* bAlternateSetting */
  0x00,                                         /* bNumEndpoints */
  USBD_AUDIO_CLASS_CODE,                       /* bInterfaceClass */
  USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOCONTROL,                  /* bInterfaceSubClass */
  USBD_AUDIO_INTERFACE_PROTOCOL_UNDEFINED,                     /* bInterfaceProtocol */
  0x00,                                         /* iInterface */
  /* 09 byte*/
  
  /* Class-Specific AC Interface Header Descriptor */
  
  USBD_AUDIO_AC_CS_INTERFACE_DESC_SIZE(CONFIG_DESCRIPTOR_AS_INTERFACES_COUNT), /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_HEADER,                         /* bDescriptorSubtype */
  LOBYTE(USBD_AUDIO_ADC_BCD),                          /* 1.00 */                     /* bcdADC */
  HIBYTE(USBD_AUDIO_ADC_BCD),
  LOBYTE(CONFIG_DESCRIPTOR_AC_TOTAL_SIZE),      /* wTotalLength*/
  HIBYTE(CONFIG_DESCRIPTOR_AC_TOTAL_SIZE),
  CONFIG_DESCRIPTOR_AS_INTERFACES_COUNT,        /*  streaming interface count */
#if USE_USB_AUDIO_PLAYBACK
  USBD_AUDIO_CONFIG_PLAY_SA_INTERFACE,          /* Audio streaming interface for  play  */
#endif /*USE_USB_AUDIO_PLAYBACK*/
#if  USE_USB_AUDIO_RECORDING
  USBD_AUDIO_CONFIG_RECORD_SA_INTERFACE,       /* Audio streaming interface for  record  */
#endif /*USE_USB_AUDIO_RECORDING*/ 
  /* 10 byte*/

#if USE_USB_AUDIO_PLAYBACK
  /* USB OUT Terminal for play session */
  /* Input Terminal Descriptor */
  USBD_AUDIO_INPUT_TERMINAL_DESC_SIZE,               /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_INPUT_TERMINAL,                 /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_PLAY_TERMINAL_INPUT_ID,      /* bTerminalID */
  LOBYTE(USBD_AUDIO_TERMINAL_IO_USB_STREAMING),         /* wTerminalType USBD_AUDIO_TERMINAL_IO_USB_STREAMING   0x0101 */
  HIBYTE(USBD_AUDIO_TERMINAL_IO_USB_STREAMING),
  0x00,                                         /* bAssocTerminal */
  USB_AUDIO_CONFIG_PLAY_CHANNEL_COUNT,         /* bNrChannels */
  LOBYTE(USB_AUDIO_CONFIG_PLAY_CHANNEL_MAP),   /* wChannelConfig*/
  HIBYTE(USB_AUDIO_CONFIG_PLAY_CHANNEL_MAP),
  0x00,                                         /* iChannelNames */
  0x00,                                         /* iTerminal */
  /* 12 byte*/
  
  /* USB Play control feature */
  /* Feature Unit Descriptor*/
  USBD_AUDIO_FEATURE_UNIT_DESC_SIZE(2,1),         /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_FEATURE_UNIT,                   /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_PLAY_UNIT_FEATURE_ID,        /* bUnitID */
  USB_AUDIO_CONFIG_PLAY_TERMINAL_INPUT_ID,      /* bSourceID: IT 02 */
  0x01,                                         /* bControlSize */
  /* @TODO add volume  controle on L/R channel */
  USBD_AUDIO_CONTROL_FEATURE_UNIT_MUTE|USBD_AUDIO_CONTROL_FEATURE_UNIT_VOLUME,      /* bmaControls(0) */
  0,                                            /* bmaControls(1) */
  0,                                            /* bmaControls(2) */
  0x00,                                         /* iTerminal */
  /* 10 byte*/
  
  /*USB Play : Speaker Terminal */
  /* Output Terminal Descriptor */
  USBD_AUDIO_OUTPUT_TERMINAL_DESC_SIZE,                                         /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_OUTPUT_TERMINAL,                /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_PLAY_TERMINAL_OUTPUT_ID,     /* bTerminalID */
  LOBYTE(USBD_AUDIO_TERMINAL_O_SPEAKER),        /* wTerminalType  0x0301*/
  HIBYTE(USBD_AUDIO_TERMINAL_O_SPEAKER),
  0x00,                                         /* bAssocTerminal */
  USB_AUDIO_CONFIG_PLAY_UNIT_FEATURE_ID,        /* bSourceID FU 06*/
  0x00,                                         /* iTerminal */
  /* 09 byte*/
#endif /*USE_USB_AUDIO_PLAYBACK*/

#if  USE_USB_AUDIO_RECORDING  
  /* USB record input : MIC */
  /* Input Terminal Descriptor */
  USBD_AUDIO_INPUT_TERMINAL_DESC_SIZE,               /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_INPUT_TERMINAL,                 /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_RECORD_TERMINAL_INPUT_ID,    /* bTerminalID */
  LOBYTE(USBD_AUDIO_TERMINAL_I_MICROPHONE),             /* wTerminalType MICROPHONE   0x0201 */
  HIBYTE(USBD_AUDIO_TERMINAL_I_MICROPHONE),
  0x00,                                         /* bAssocTerminal */
  USB_AUDIO_CONFIG_RECORD_CHANNEL_COUNT,       /* bNrChannels */
  LOBYTE(USB_AUDIO_CONFIG_RECORD_CHANNEL_MAP), /* wChannelConfig*/
  HIBYTE(USB_AUDIO_CONFIG_RECORD_CHANNEL_MAP),
  0x00,                                         /* iChannelNames */
  0x00,                                         /* iTerminal */
  /* 12 byte*/
  
  /* USB Record control feature */
  /* Feature Unit Descriptor*/
  USBD_AUDIO_FEATURE_UNIT_DESC_SIZE(2,1),         /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_FEATURE_UNIT,                   /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_RECORD_UNIT_FEATURE_ID,      /* bUnitID */
  USB_AUDIO_CONFIG_RECORD_TERMINAL_INPUT_ID,    /* bSourceID */
  0x01,                                         /* bControlSize */
  USBD_AUDIO_CONTROL_FEATURE_UNIT_MUTE|USBD_AUDIO_CONTROL_FEATURE_UNIT_VOLUME,      /* bmaControls(0) */
  0x00,                                         /* bmaControls(1) */
  0x00,                                         /* bmaControls(2) */
  0x00,                                         /* iTerminal */
  /* 10 byte*/
  
  /*USB IN: Record output*/
  /* Output Terminal Descriptor */
  USBD_AUDIO_OUTPUT_TERMINAL_DESC_SIZE,      /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_AC_SUBTYPE_OUTPUT_TERMINAL,                /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_RECORD_TERMINAL_OUTPUT_ID,   /* bTerminalID */
  LOBYTE(USBD_AUDIO_TERMINAL_IO_USB_STREAMING),         /* wTerminalType USBD_AUDIO_TERMINAL_IO_USB_STREAMING   0x0101 */
  HIBYTE(USBD_AUDIO_TERMINAL_IO_USB_STREAMING),
  0x00,                                         /* bAssocTerminal */
  USB_AUDIO_CONFIG_RECORD_UNIT_FEATURE_ID,      /* bSourceID */
  0x00,                                         /* iTerminal */
  /* 09 byte*/
#endif /* USE_USB_AUDIO_RECORDING*/  
#if USE_USB_AUDIO_PLAYBACK
  /* USB play Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
  /* Standard AS Interface Descriptor */
  USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE,                    /* bLength */
  USB_DESC_TYPE_INTERFACE,                      /* bDescriptorType */
  USBD_AUDIO_CONFIG_PLAY_SA_INTERFACE,          /* bInterfaceNumber */
  0x00,                                         /* bAlternateSetting */
  0x00,                                         /* bNumEndpoints */
  USBD_AUDIO_CLASS_CODE,                       /* bInterfaceClass */
  USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOSTREAMING,                /* bInterfaceSubClass */
  USBD_AUDIO_INTERFACE_PROTOCOL_UNDEFINED,                     /* bInterfaceProtocol */
  0x00,                                         /* iInterface */
  /* 09 byte*/
  
  /* USB play Standard AS Interface Descriptors - Audio streaming operational */
  /* Standard AS Interface Descriptor */
  USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE,                    /* bLength */
  USB_DESC_TYPE_INTERFACE,                      /* bDescriptorType */
  USBD_AUDIO_CONFIG_PLAY_SA_INTERFACE,          /* bInterfaceNumber */
  0x01,                                         /* bAlternateSetting */
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK 
  0x02,                                         /* bNumEndpoints */
#else
  0x01,                                         /* bNumEndpoints */
#endif 
  USBD_AUDIO_CLASS_CODE,                       /* bInterfaceClass */
  USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOSTREAMING,                /* bInterfaceSubClass */
  USBD_AUDIO_INTERFACE_PROTOCOL_UNDEFINED,                     /* bInterfaceProtocol */
  0x00,                                         /* iInterface */
  /* 09 byte*/
  
  /*Class-Specific AS Interface Descriptor */
  USBD_AUDIO_AS_CS_INTERFACE_DESC_SIZE,          /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_SUBTYPE_AS_GENERAL,                      /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_PLAY_TERMINAL_INPUT_ID,      /* bTerminalLink */
  0x01,                                         /* bDelay */
  LOBYTE(USBD_AUDIO_FORMAT_TYPE_PCM),                     /* wFormatTag USBD_AUDIO_FORMAT_TYPE_PCM  0x0001*/
  HIBYTE(USBD_AUDIO_FORMAT_TYPE_PCM),
  /* 07 byte*/

    /*  Audio Type I Format descriptor */
  USBD_USBD_AUDIO_FORMAT_TYPE_I_DESC_SIZE(USB_AUDIO_CONFIG_PLAY_FREQ_COUNT),/* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_SUBTYPE_AS_FORMAT_TYPE,                  /* bDescriptorSubtype */
  USBD_AUDIO_FORMAT_TYPE_I,                          /* bFormatType */ 
  USB_AUDIO_CONFIG_PLAY_CHANNEL_COUNT,         /* bNrChannels */
  USB_AUDIO_CONFIG_PLAY_RES_BYTE,              /* bSubFrameSize :  2 Bytes per frame (16bits) */
  USB_AUDIO_CONFIG_PLAY_RES_BIT,               /* bBitResolution (16-bits per sample) */ 
  USB_AUDIO_CONFIG_PLAY_FREQ_COUNT,             /* bSamFreqType only one frequency supported */ 
#ifdef USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES
 /* Audio sampling frequency coded on 3 bytes */
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_8_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_8_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_8_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_16_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_16_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_16_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_32_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_32_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_32_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_44_1_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_48_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_48_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_48_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_96_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_96_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_96_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_192_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_192_K),
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_192_K*/
#else /*USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES*/
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_PLAY_DEF_FREQ),
#endif /*USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES*/
  /* (0x08 + USB_AUDIO_CONFIG_PLAY_FREQ_COUNT * 3) byte*/
  
  /* USB Play data ep  */
  /* Standard AS Isochronous Audio Data Endpoint Descriptor*/
  USBD_AUDIO_STANDARD_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_DESC_TYPE_ENDPOINT,                       /* bDescriptorType */
  USBD_AUDIO_CONFIG_PLAY_EP_OUT,                /* bEndpointAddress 1 out endpoint*/
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK          /* when feedback is used */
  USBD_EP_TYPE_ISOC|USBD_EP_ATTR_ISOC_ASYNC,    /* bmAttributes */
#else /* USE_AUDIO_PLAYBACK_USB_FEEDBACK*/
  USBD_EP_TYPE_ISOC,                            /* bmAttributes */
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK*/
  LOBYTE(USBD_AUDIO_CONFIG_PLAY_MAX_PACKET_SIZE),/* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
  HIBYTE(USBD_AUDIO_CONFIG_PLAY_MAX_PACKET_SIZE),
  0x01,                                         /* bInterval */
  0x00,                                         /* bRefresh */
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK          /* when feedback is used */
  USB_AUDIO_CONFIG_PLAY_EP_SYNC,                /* bSynchAddress */
#else /* USE_AUDIO_PLAYBACK_USB_FEEDBACK*/
  0x00,                                         /* bSynchAddress */
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK*/
  /* 09 byte*/
  
  /* Class-Specific AS Isochronous Audio Data Endpoint Descriptor*/
  USBD_AUDIO_SPECIFIC_DATA_ENDPOINT_DESC_SIZE,           /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_ENDPOINT,               /* bDescriptorType */
  USBD_AUDIO_SPECIFIC_EP_DESC_SUBTYPE_GENERAL,                       /* bDescriptor */
#ifdef USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES
  USBD_AUDIO_AS_CONTROL_SAMPLING_FREQUENCY,          /* bmAttributes */
  0x00,                                         /* bLockDelayUnits */
  0x00,                                         /* wLockDelay */
  0x00,
#else /*USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES */
  0x00,                                         /* bmAttributes */
  0x00,                                         /* bLockDelayUnits */
  0x00,                                         /* wLockDelay */
  0x00,
#endif /* USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES */
  /* 07 byte*/

  
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
  /*next descriptor specific for synch ep */
  /* USB Play feedback ep  */
  /* Standard AS Isochronous Audio Data Endpoint Descriptor*/
  USBD_AUDIO_STANDARD_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_DESC_TYPE_ENDPOINT,                       /* bDescriptorType */
  USB_AUDIO_CONFIG_PLAY_EP_SYNC,                /* bEndpointAddress 1 out endpoint*/
  USBD_EP_TYPE_ISOC,                            /* bmAttributes */
  LOBYTE(AUDIO_FEEDBACK_EP_PACKET_SIZE),        /* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
  HIBYTE(AUDIO_FEEDBACK_EP_PACKET_SIZE),
  0x01,                                         /* bInterval */
  USB_AUDIO_CONFIG_PLAY_FEEDBACK_REFRESH,       /* bRefresh */
  0,                                            /* bSynchAddress */
  /* 09 byte*/
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */ 
#endif /*USE_USB_AUDIO_PLAYBACK */

  #if  USE_USB_AUDIO_RECORDING  
  /* USB record Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
  /* Standard AS Interface Descriptor */
  USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE,                    /* bLength */
  USB_DESC_TYPE_INTERFACE,                      /* bDescriptorType */
  USBD_AUDIO_CONFIG_RECORD_SA_INTERFACE,        /* bInterfaceNumber */
  0x00,                                         /* bAlternateSetting */
  0x00,                                         /* bNumEndpoints */
  USBD_AUDIO_CLASS_CODE,                       /* bInterfaceClass */
  USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOSTREAMING,                /* bInterfaceSubClass */
  USBD_AUDIO_INTERFACE_PROTOCOL_UNDEFINED,                     /* bInterfaceProtocol */
  0x00,                                         /* iInterface */
  /* 09 byte*/
  
  /* USB record Standard AS Interface Descriptors - Audio streaming operational */
  /* Standard AS Interface Descriptor */
  USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE,                    /* bLength */
  USB_DESC_TYPE_INTERFACE,                      /* bDescriptorType */
  USBD_AUDIO_CONFIG_RECORD_SA_INTERFACE,        /* bInterfaceNumber */
  0x01,                                         /* bAlternateSetting */
  0x01,                                         /* bNumEndpoints */
  USBD_AUDIO_CLASS_CODE,                       /* bInterfaceClass */
  USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOSTREAMING,                /* bInterfaceSubClass */
  USBD_AUDIO_INTERFACE_PROTOCOL_UNDEFINED,                     /* bInterfaceProtocol */
  0x00,                                         /* iInterface */
  /* 09 byte*/
  
  /*Class-Specific AS Interface Descriptor */
  USBD_AUDIO_AS_CS_INTERFACE_DESC_SIZE,          /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_SUBTYPE_AS_GENERAL,                      /* bDescriptorSubtype */
  USB_AUDIO_CONFIG_RECORD_TERMINAL_OUTPUT_ID,   /* bTerminalLink */
  0x01,                                         /* bDelay */
  0x01,                                         /* wFormatTag USBD_AUDIO_FORMAT_TYPE_PCM  0x0001*/
  0x00,
  /* 07 byte*/
  

    /* USB Audio Type I Format descriptor */
  USBD_USBD_AUDIO_FORMAT_TYPE_I_DESC_SIZE(USB_AUDIO_CONFIG_RECORD_FREQ_COUNT),/* bLength */
  USBD_AUDIO_DESC_TYPE_CS_INTERFACE,              /* bDescriptorType */
  USBD_AUDIO_CS_SUBTYPE_AS_FORMAT_TYPE,                  /* bDescriptorSubtype */
  USBD_AUDIO_FORMAT_TYPE_I,                          /* bFormatType */ 
  USB_AUDIO_CONFIG_RECORD_CHANNEL_COUNT,       /* bNrChannels */
  USB_AUDIO_CONFIG_RECORD_RES_BYTE,            /* bSubFrameSize :  2 Bytes per frame (16bits) */
  USB_AUDIO_CONFIG_RECORD_RES_BIT,             /* bBitResolution (16-bits per sample) */ 
  USB_AUDIO_CONFIG_RECORD_FREQ_COUNT,           /* bSamFreqType only one frequency supported */ 
  /* Audio sampling frequency coded on 3 bytes */
#ifdef USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_8_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_8_K),
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_8_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_16_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_16_K),
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_16_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_32_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_32_K),
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_32_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_44_1_K),
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_48_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_48_K),
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_48_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_FREQ_96_K),
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K*/
#else /*USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES*/
  AUDIO_SAMPLE_FREQ(USB_AUDIO_CONFIG_RECORD_DEF_FREQ),
#endif /*USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES*/
  /* 11 byte*/
  
  /* USB record data ep  */
  /* Standard AS Isochronous Audio Data Endpoint Descriptor*/
  USBD_AUDIO_STANDARD_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_DESC_TYPE_ENDPOINT,                       /* bDescriptorType */
  USB_AUDIO_CONFIG_RECORD_EP_IN,                /* bEndpointAddress 1 out endpoint*/
  USBD_EP_TYPE_ISOC,                            /* bmAttributes */
  LOBYTE(USBD_AUDIO_CONFIG_RECORD_MAX_PACKET_SIZE),/* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
  HIBYTE(USBD_AUDIO_CONFIG_RECORD_MAX_PACKET_SIZE),
  0x01,                                         /* bInterval */
  0x00,                                         /* bRefresh */
  0x00,                                         /* bSynchAddress */
  /* 09 byte*/
  
 /* Class-Specific AS Isochronous Audio Data Endpoint Descriptor*/
  USBD_AUDIO_SPECIFIC_DATA_ENDPOINT_DESC_SIZE,   /* bLength */
  USBD_AUDIO_DESC_TYPE_CS_ENDPOINT,       /* bDescriptorType */
  USBD_AUDIO_SPECIFIC_EP_DESC_SUBTYPE_GENERAL,               /* bDescriptor */
#ifdef USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES
  USBD_AUDIO_AS_CONTROL_SAMPLING_FREQUENCY,  /* bmAttributes */
  0x00,                                 /* bLockDelayUnits */
  0x00,                                 /* wLockDelay */
  0x00
#else /*USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES */
  0x00,                                 /* bmAttributes */
  0x00,                                 /* bLockDelayUnits */
  0x00,                                 /* wLockDelay */
  0x00
#endif /* USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES */

#endif /* USE_USB_AUDIO_RECORDING */
  /* 07 byte*/
} ;

/* exported functions ---------------------------------------------------------*/
/**
  * @brief  USB_AUDIO_GetConfigDescriptor
  *         return configuration descriptor
  * @param  desc                             
  * @retval the configuration descriptor size
  */
uint16_t USB_AUDIO_GetConfigDescriptor(uint8_t **desc)
{ 
  if(desc)
  {
    *desc = USBD_AUDIO_ConfigDescriptor;
  }
  return (CONFIG_DESCRIPTOR_SIZE);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
