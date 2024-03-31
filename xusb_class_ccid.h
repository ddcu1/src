/******************************************************************************
* Copyright (C) 2017 - 2022 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
 ******************************************************************************/

/*****************************************************************************/
/**
 *
 * @file xusb_class_ccid.h
 *
 * This file contains definitions used in the Mass Storage class code.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.0   sg  06/06/16  First release
 * 1.4   BK  12/01/18 Renamed the file and added changes to have a common
 *		      example for all USB IPs.
 *
 * </pre>
 *
 *****************************************************************************/

#ifndef XUSB_CLASS_CCID_H
#define XUSB_CLASS_CCID_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xusb_ch9.h"

/************************** Constant Definitions *****************************/
/*
 * CCID opcodes
 */
#define CCID_OPCODE_GET_CAPABILITIES          0x61
#define CCID_OPCODE_SLOT_STATUS               0x62
#define CCID_OPCODE_PARAMETER_EXCHANGE        0x65
#define CCID_OPCODE_RESET_CARD                0x67
#define CCID_OPCODE_PROTOCOL_EXCHANGE_T0      0x68
#define CCID_OPCODE_PROTOCOL_EXCHANGE_T1      0x69
#define CCID_OPCODE_ESCAPE                    0x6A
#define CCID_OPCODE_ICC_POWER_ON              0x6B
#define CCID_OPCODE_ICC_POWER_OFF             0x6C
#define CCID_OPCODE_GET_SLOT_STATUS           0x6F

/* Virtual Flash memory related definitions.
 */
#ifdef __MICROBLAZE__
/* 16MB due to limited memory on AXIUSB platform. */
#define VFLASH_SIZE			0x1000000	/* 16MB space */
#else
#define VFLASH_SIZE			0x10000000	/* 256MB space */
#endif
#define VFLASH_BLOCK_SIZE	0x200
#define VFLASH_NUM_BLOCKS	(VFLASH_SIZE/VFLASH_BLOCK_SIZE)

/* Class request opcodes.
 */
#define USB_CLASSREQ_CCID_RESET               0xFF 

/* Smart Card Reader states (May need modification) */
#define USB_EP_STATE_COMMAND        0
#define USB_EP_STATE_DATA_IN        1
#define USB_EP_STATE_DATA_OUT       2
#define USB_EP_STATE_STATUS         3

/**************************** Type Definitions ******************************/

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif

/*
 * The following structures define USB storage class requests. The details of
 * the contents of those structures are not important in the context of this
 * example.
 */

typedef struct {
	u32 dCBWSignature;
	u32 dCBWTag;
	u32 dCBWDataTransferLength;
	u8  bmCBWFlags;
	u8  cCBWLUN;
	u8  bCBWCBLength;
	u8  CBWCB[16];
} attribute(USB_CBW);

typedef struct {
	u32 dCSWSignature;
	u32 dCSWTag;
	u32 dCSWDataResidue;
	u8  bCSWStatus;
} attribute(USB_CSW);

typedef	struct {
	u8 deviceType;
	u8 rmb;
	u8 version;
	u8 blah;
	u8 additionalLength;
	u8 sccs;
	u8 info0;
	u8 info1;
	u8 vendorID[8];
	u8 productID[16];
	u8 revision[4];
} attribute(SCSI_INQUIRY);

typedef struct {
	u8  reserved[3];
	u8  listLength;
	u32 numBlocks;
	u8  descCode;
	u8  blockLengthMSB;
	u16 blockLength;
} attribute(SCSI_CAP_LIST);

typedef struct {
	u32 numBlocks;
	u32 blockSize;
} attribute(SCSI_READ_CAPACITY);

typedef struct {
	u8  opCode;
	u8  reserved1;
	u32 block;
	u8  reserved2;
	u16 length;
	u8  control;
} attribute(SCSI_READ_WRITE);

typedef struct {
	u8  opCode;
	u8  immed;
	u8  reserved1;
	u8  reserved2;
	u8  start;
	u8  control;
} attribute(SCSI_START_STOP);


// CCID 

// Get Capabilities
typedef struct {
    u8  bMessageType;  // Rbm slot/sequence
    u32 dwLength;      // Length of data to follow
    u8  bSlot;         // Slot number
    u8  bSequence;     // Sequence number
} attribute(CCID_GET_CAPABILITIES);

// Parameter Exchange
typedef struct {
    u8  bMessageType; 
    u32 dwLength;      
    u8  bSlot;        
    u8  bSequence;    
    u8  bLevel;       // Exchange level
    u8  dwParams[4];  // Parameters (content depends on exchange level)
} attribute(CCID_PARAMETER_EXCHANGE);

// ICC Power On
typedef struct {
    u8  bMessageType;   
    u32 dwLength;       
    u8  bSlot;           
    u8  bSequence;      
    u8  bPowerSelect;   // Power select (voltage selection)
    u8  abRFU[2];       // Reserved for future use
} attribute(CCID_ICC_POWER_ON);

// Protocol Exchange (Example - T=0 Protocol)
typedef struct {
    u8  bMessageType;  
    u32 dwLength;     
    u8  bSlot;        
    u8  bSequence;    
    u8  bBWI;         // Block wait time integer 
    u16 wLevelParameter;  // Level parameter
    /*  ... (APDU data would follow) */
} attribute(CCID_PROTOCOL_EXCHANGE_T0); 

// Protocol Exchange (Example - T=1 Protocol)
typedef struct {
	u8  bMessageType;  
	u32 dwLength;     
	u8  bSlot;        
	u8  bSequence;    
	u8  bBWI;         // Block wait time integer 
	u8  bLevelParameter;  // Level parameter
	/*  ... (APDU data would follow) */
} attribute(CCID_PROTOCOL_EXCHANGE_T1);


#ifdef __ICCARM__
#pragma pack(pop)
#endif

/************************** Function Prototypes ******************************/
void ClassReq(struct Usb_DevData *InstancePtr, SetupPacket *SetupData);
void ParseCBW(struct Usb_DevData *InstancePtr);
void SendCSW(struct Usb_DevData *InstancePtr, u32 Length);

#ifdef __cplusplus
}
#endif

#endif /* XUSB_CLASS_STORAGE_H */
