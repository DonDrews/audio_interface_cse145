/**
  ******************************************************************************
  * @file    usbd_audio.h
  * @author  MCD Application Team 
  * @brief   header file for the usbd_audio.c file, it is new implementation of USB audio class which 
  * supports more features.
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
#ifndef __USB_AUDIO_H
#define __USB_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_AUDIO
  * @brief This file is the Header file for usbd_audio.c
  * @{
  */ 


/** @defgroup USBD_AUDIO_Exported_Defines
  * @{
  */ 
 #define USBD_AUDIO_ADC_BCD                                           0x0100
 #define USBD_AUDIO_CLASS_CODE                                        0x01
/* Audio Interface Subclass Codes */
#define USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOCONTROL                   0x01
#define USBD_AUDIO_INTERFACE_SUBCLASS_AUDIOSTREAMING                 0x02
#define USBD_AUDIO_INTERFACE_SUBCLASS_MIDISTREAMING                  0x03
/* Audio Interface Protocol Codes  */
#define USBD_AUDIO_INTERFACE_PROTOCOL_UNDEFINED                      0x00
   
/* Table A-2: Audio Data Format Type I Bit Allocations */
#define USBD_AUDIO_FORMAT_TYPE_PCM                                   0x0001
/* Table A-1: Format Type Codes */
#define USBD_AUDIO_FORMAT_TYPE_I                                     0x01
#define USBD_AUDIO_FORMAT_TYPE_II                                    0x02
#define USBD_AUDIO_FORMAT_TYPE_III                                   0x03
   
/* Audio Descriptor Types */
#define USBD_AUDIO_DESC_TYPE_CS_DEVICE                               0x21
#define USBD_AUDIO_DESC_TYPE_CS_INTERFACE                            0x24
#define USBD_AUDIO_DESC_TYPE_CS_ENDPOINT                             0x25
#define USBD_AUDIO_DESC_TYPE_INTERFACE_ASSOC                         0x0B /* Interface association descriptor */
    /* audio specific descriptor size */
#define USBD_AUDIO_STANDARD_INTERFACE_DESC_SIZE                      0x09
#define USBD_AUDIO_INTERFACE_ASSOC_DESC_SIZE                         0x08
#define USBD_AUDIO_DESC_SIZ                                           0x09
#define USBD_AUDIO_STANDARD_ENDPOINT_DESC_SIZE                       0x09
#define USBD_AUDIO_SPECIFIC_DATA_ENDPOINT_DESC_SIZE                  0x07
#define USBD_AUDIO_INPUT_TERMINAL_DESC_SIZE                          0x0C
#define USBD_AUDIO_FEATURE_UNIT_DESC_SIZE(CH_NB,CTRSIZE)            (0x07+(((CH_NB)+1)*(CTRSIZE)))
#define USBD_AUDIO_OUTPUT_TERMINAL_DESC_SIZE                         0x09
#define USBD_AUDIO_AC_CS_INTERFACE_DESC_SIZE(AS_CNT)                 (0x08 + (AS_CNT))
#define USBD_AUDIO_AS_CS_INTERFACE_DESC_SIZE                         0x07
#define USBD_USBD_AUDIO_FORMAT_TYPE_I_DESC_SIZE(NBFREQ)              (0x08 + (NBFREQ)*3)


/* Class-Specific AS Isochronous Audio Data Endpoint Descriptor bmAttributes */
#define USBD_AUDIO_AS_CONTROL_SAMPLING_FREQUENCY             0x0001 /* D0 = 1*/
#define USBD_AUDIO_AS_CONTROL_PITCH                          0x0002 /* D1 = 1*/
#define USBD_AUDIO_AS_CONTROL_MAX_PACKET_ONLY                0x0080 /* D7 = 1*/

/* Audio Class-Specific Endpoint Descriptor Subtypes*/
#define USBD_AUDIO_SPECIFIC_EP_DESC_SUBTYPE_GENERAL                  0x01 /* EP_GENERAL */

#define USBD_EP_ATTR_ISOC_NOSYNC                          0x00 /* attribute no synchro */
#define USBD_EP_ATTR_ISOC_ASYNC                           0x04 /* attribute synchro by feedback  */
#define USBD_EP_ATTR_ISOC_ADAPT                           0x08 /* attribute synchro adaptative  */
#define USBD_EP_ATTR_ISOC_SYNC                            0x0C /* attribute synchro synchronous  */
    
   
/* USB AUDIO CLASS REQUESTS BREQUEST TYPES */
#define USBD_AUDIO_REQ_SET_CUR                             0x01
#define USBD_AUDIO_REQ_GET_CUR                             0x81
#define USBD_AUDIO_REQ_SET_MIN                             0x02
#define USBD_AUDIO_REQ_GET_MIN                             0x82
#define USBD_AUDIO_REQ_SET_MAX                             0x03
#define USBD_AUDIO_REQ_GET_MAX                             0x83
#define USBD_AUDIO_REQ_SET_RES                             0x04
#define USBD_AUDIO_REQ_GET_RES                             0x84
#define USBD_AUDIO_REQ_SET_MEM                             0x05
#define USBD_AUDIO_REQ_GET_MEM                             0x85
#define USBD_AUDIO_REQ_GET_STAT                            0xFF   


/* Feature Unit Controls */
#define USBD_AUDIO_CONTROL_FEATURE_UNIT_MUTE          0x01 
#define USBD_AUDIO_CONTROL_FEATURE_UNIT_VOLUME        0x02 
  
  /* Feature Unit Control Selectors */
#define USBD_AUDIO_FU_MUTE_CONTROL                                    0x01 
#define USBD_AUDIO_FU_VOLUME_CONTROL                                  0x02 

/* definition of end point controls */                                         
#define USBD_AUDIO_CONTROL_EP_SAMPL_FREQ               0x01
#define USBD_AUDIO_CONTROL_EP_PITCH                   0x02
   
/* configuration of current implementation of audio class */
#define USBD_AUDIO_AS_INTERFACE_COUNT 0x02
#define USBD_AUDIO_MAX_IN_EP 5
#define USBD_AUDIO_MAX_OUT_EP 5
#define USBD_AUDIO_MAX_AS_INTERFACE 2
#define USBD_AUDIO_EP_MAX_CONTROL 3
#define USBD_AUDIO_CONFIG_CONTROL_UNIT_COUNT 0x02
#define USBD_AUDIO_FEATURE_MAX_CONTROL 2  
#define AUDIO_FEEDBACK_EP_PACKET_SIZE                 0x03
/**
  * @}
  */ 


/** @defgroup USBD_AUDIO_Exported_TypesDefinitions
  * @{
  */
/* Audio Control Interface Descriptor Subtypes */
typedef enum 
{
  USBD_AUDIO_CS_AC_SUBTYPE_UNDEFINED                               = 0x00,
  USBD_AUDIO_CS_AC_SUBTYPE_HEADER                                  = 0x01,
  USBD_AUDIO_CS_AC_SUBTYPE_INPUT_TERMINAL                          = 0x02,
  USBD_AUDIO_CS_AC_SUBTYPE_OUTPUT_TERMINAL                         = 0x03,
  USBD_AUDIO_CS_AC_SUBTYPE_MIXER_UNIT                              = 0x04,
  USBD_AUDIO_CS_AC_SUBTYPE_SELECTOR_UNIT                           = 0x05,
  USBD_AUDIO_CS_AC_SUBTYPE_FEATURE_UNIT                            = 0x06,
  USBD_AUDIO_CS_AC_SUBTYPE_PROCESSING_UNIT                         = 0x07,
  USBD_AUDIO_CS_AC_SUBTYPE_EXTENSION_UNIT                          = 0x08,
}USBD_AUDIO_SpecificACInterfaceDescSubtypeTypeDef;

typedef enum
{
  USBD_AUDIO_TERMINAL_IO_USB_UNDEFINED                             = 0x0100 ,
  USBD_AUDIO_TERMINAL_IO_USB_STREAMING                             = 0x0101 ,
  USBD_AUDIO_TERMINAL_IO_USB_VENDOR_SPECIFIC                       = 0x01FF ,
  USBD_AUDIO_TERMINAL_I_UNDEFINED                                  = 0x0200 ,
  USBD_AUDIO_TERMINAL_I_MICROPHONE                                 = 0x0201 ,
  USBD_AUDIO_TERMINAL_I_DESKTOP_MICROPHONE                         = 0x0202 ,
  USBD_AUDIO_TERMINAL_O_UNDEFINED                                  = 0x0300 ,
  USBD_AUDIO_TERMINAL_O_SPEAKER                                    = 0x0301 ,
  USBD_AUDIO_TERMINAL_O_HEADPHONES                                 = 0x0302 
}USBD_AUDIOTerminalTypeDef;

/* Audio Streaming Interface Descriptor Subtypes */
typedef enum 
{
  USBD_AUDIO_CS_SUBTYPE_AS_UNDEFINED                               = 0x00,
  USBD_AUDIO_CS_SUBTYPE_AS_GENERAL                                 = 0x01,
  USBD_AUDIO_CS_SUBTYPE_AS_FORMAT_TYPE                             = 0x02,
  USBD_AUDIO_CS_SUBTYPE_AS_FORMAT_SPECIFIC                         = 0x03
}USBD_AUDIO_SpecificASInterfaceDescSubtypeTypeDef;

/* The feature Unit callbacks */
typedef struct
{
   int8_t  (*GetMute)    (uint16_t /*channel*/,uint8_t* /*mute*/, uint32_t /* privatedata*/);
   int8_t  (*SetMute)    (uint16_t /*channel*/,uint8_t /*mute*/, uint32_t /* privatedata*/);
   int8_t  (*SetCurVolume)    (uint16_t /*channel*/,uint16_t /*volume*/, uint32_t /* privatedata*/);
   int8_t  (*GetCurVolume)    (uint16_t /*channel*/,uint16_t* /*volume*/, uint32_t /* privatedata*/);
   uint16_t MaxVolume; 
   uint16_t MinVolume;
   uint16_t ResVolume;
   int8_t  (*GetStatus)     (uint32_t /*privatedata*/);
}USBD_AUDIO_FeatureControlCallbacksTypeDef;


/* the Unit callbacks , used when a control is called (Get_Cur, Set Cur ....) */
typedef union
{
   USBD_AUDIO_FeatureControlCallbacksTypeDef* feature_control;
}USBD_AUDIO_ControlCallbacksTypeDef;
/** Audio Unit  supported cmd and related callbacks */


/* Next strucure define an audio Unit */
typedef struct
{
    uint8_t id; /* Unit Id */
    USBD_AUDIO_SpecificACInterfaceDescSubtypeTypeDef type; /* type of Unit */
    uint16_t control_req_map; /* a map of requests GET_CUR, GET_MAX, ....*/
    uint16_t control_selector_map; /* List of supported control , for example Mute and Volume */
    USBD_AUDIO_ControlCallbacksTypeDef Callbacks; /* list of callbacks */
    uint32_t  private_data; /* used as the last arguement of each callback */
}USBD_AUDIO_ControlTypeDef;

#ifdef USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
/* The EP control callbacks */
typedef struct
{
   int8_t  (*GetCurFrequency)    (uint32_t* /*freq*/, uint32_t /* privatedata*/);
   int8_t  (*SetCurFrequency)    (uint32_t /*freq*/,uint8_t* /* restart_req*/ , uint32_t /* privatedata*/);
   uint32_t MaxFrequency; 
   uint32_t MinFrequency;
   uint32_t ResFrequency;
}USBD_AUDIO_EndPointControlCallbacksTypeDef;
#endif /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES */
/* Structure Define a data endpoint and it's callbacks */
 typedef struct 
 {
   uint8_t ep_num; 
   uint16_t control_name_map; /* a bitmap of supported commands : get_cur|set_cur ...*/
   uint16_t control_selector_map; /* a bitmap of supported controls : freq, pitsh ... */
   uint8_t* buf;
   uint16_t length;
   int8_t  (*DataReceived)     ( uint16_t/* data_len*/,uint32_t/* privatedata*/); /* called for OUT EP when data is received */
   uint8_t*  (*GetBuffer)    (uint32_t /* privatedata*/, uint16_t* packet_length); /* called for IN and OUt  EP to get working buffer */
   uint16_t  (*GetMaxPacketLength)    (uint32_t /*privatedata*/); /* Called beforre openeing the EP to get Max Size length */
   int8_t  (*GetState)     (uint32_t/*privatedata*/);
#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
   USBD_AUDIO_EndPointControlCallbacksTypeDef control_cbk;
#endif /*USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES*/
   uint32_t  private_data;/* used as the last arguement of each callback */
 }  USBD_AUDIO_EP_DataTypeDef;
 
 
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK  
 /* Structure Define a feedback endpoint and it's callbacks */
 typedef struct 
 {
   uint8_t  ep_num; /* endpoint number */
   uint8_t feedback_data[AUDIO_FEEDBACK_EP_PACKET_SIZE]; /* buffer used to send feedback */
   uint32_t      (*GetFeedback)     (  uint32_t/* privatedata*/); /* return  count of played sample  since last ResetRate */
   uint32_t private_data;
 }  USBD_AUDIO_EP_SynchTypeDef;
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
 
 
/* Strucure define Audio streaming interface */
typedef struct USBD_AUDIO_AS_Interface
{
    uint8_t interface_num; /* audio streaming interface num */
    uint8_t max_alternate; /* audio streaming interface most greate  alternate num */
    uint8_t alternate;/* audio streaming interface current  alternate  */
    USBD_AUDIO_EP_DataTypeDef data_ep; /* audio streaming interface main data EP  */
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK 
    uint8_t synch_enabled;
    USBD_AUDIO_EP_SynchTypeDef synch_ep; /* synchro ep description */
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
    void  (*SofReceived)     ( uint32_t/*privatedata*/);
    int8_t  (*SetAS_Alternate)     ( uint8_t/*alternate*/,uint32_t/*privatedata*/);
    int8_t  (*GetState)     (uint32_t/*privatedata*/);
    uint32_t  private_data; /* used as the last arguement of each callback */      
}USBD_AUDIO_AS_InterfaceTypeDef;


/* Structure define the whole audio function will be initialized by application*/
typedef struct
{
  uint8_t control_count; /* the count  of Unit controls */
  uint8_t as_interfaces_count;/* the count  of audio streaming interface */
  USBD_AUDIO_ControlTypeDef controls[USBD_AUDIO_CONFIG_CONTROL_UNIT_COUNT]; /* list of Unit control */
  USBD_AUDIO_AS_InterfaceTypeDef as_interfaces[USBD_AUDIO_AS_INTERFACE_COUNT];/* the list  of audio streaming interface */
}USBD_AUDIO_FunctionDescriptionfTypeDef;

/* Structure define audio interface */
typedef struct
{
    int8_t  (*Init)         (USBD_AUDIO_FunctionDescriptionfTypeDef* /* as_desc*/ , uint32_t /*privatedata*/);
    int8_t  (*DeInit)       (USBD_AUDIO_FunctionDescriptionfTypeDef* /* as_desc*/,uint32_t /*privatedata*/);
    int8_t  (*GetConfigDesc) (uint8_t ** /*pdata*/, uint16_t * /*psize*/, uint32_t /*private_data*/);
    int8_t  (*GetState)     (uint32_t privatedata);
    uint32_t private_data;  
}USBD_AUDIO_InterfaceCallbacksfTypeDef;
                    

/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 
 /* to use in configuration descriptor */
#define AUDIO_SAMPLE_FREQ(frq)      (uint8_t)(frq), (uint8_t)(((frq) >> 8)), (uint8_t)(((frq) >> 16))
#define AUDIO_FREQ_TO_DATA(frq , bytes)      do{\
                                                  (bytes)[0]= (uint8_t)(frq);\
                                                  (bytes)[1]= (uint8_t)(((frq) >> 8));\
                                                  (bytes)[2]= (uint8_t)(((frq) >> 16));\
                                               }while(0);

#define AUDIO_FREQ_FROM_DATA(bytes)      (((uint32_t)((bytes)[2]))<<16)| (((uint32_t)((bytes)[1]))<<8)| (((uint32_t)((bytes)[0])))
/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_ClassTypeDef  USBD_AUDIO;
#define USBD_AUDIO_CLASS    &USBD_AUDIO
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
uint8_t  USBD_AUDIO_RegisterInterface  (USBD_HandleTypeDef   *pdev, 
                                        USBD_AUDIO_InterfaceCallbacksfTypeDef *aifc);

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif  /* __USB_AUDIO_H */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
