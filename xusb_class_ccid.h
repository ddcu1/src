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
extern "C"
{
#endif

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xusb_ch9.h"
#include "ccid_config.h"

/************************** Constant Definitions *****************************/
/*
 * Mass storage opcodes.
 */
#define USB_RBC_TEST_UNIT_READY 0x00
#define USB_RBC_REQUEST_SENSE 0x03
#define USB_RBC_FORMAT 0x04
#define USB_RBC_INQUIRY 0x12
#define USB_RBC_MODE_SELECT 0x15
#define USB_RBC_MODE_SENSE 0x1a
#define USB_RBC_STARTSTOP_UNIT 0x1b
#define USB_RBC_MEDIUM_REMOVAL 0x1e
#define USB_UFI_GET_CAP_LIST 0x23
#define USB_RBC_READ_CAP 0x25
#define USB_RBC_READ 0x28
#define USB_RBC_WRITE 0x2a
#define USB_RBC_VERIFY 0x2f
#define USB_SYNC_SCSI 0x35

/* Virtual Flash memory related definitions.
 */
#ifdef __MICROBLAZE__
/* 16MB due to limited memory on AXIUSB platform. */
#define VFLASH_SIZE 0x1000000 /* 16MB space */
#else
#define VFLASH_SIZE 0x10000000 /* 256MB space */
#endif
#define VFLASH_BLOCK_SIZE 0x200
#define VFLASH_NUM_BLOCKS (VFLASH_SIZE / VFLASH_BLOCK_SIZE)

/* Class request opcodes.
 */
#define USB_CLASSREQ_MASS_STORAGE_RESET 0xFF
#define USB_CLASSREQ_GET_MAX_LUN 0xFE

/* SCSI machine states
 */
#define USB_EP_STATE_COMMAND 0
#define USB_EP_STATE_DATA_IN 1
#define USB_EP_STATE_DATA_OUT 2
#define USB_EP_STATE_STATUS 3

	/**************************** Type Definitions ******************************/

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif

	// Payload for PC_to_RDR_IccPowerOn
	typedef struct
	{
		u8 bPowerSelect; // 00h: Auto, 01h: 5V, 02h: 3V, 03h: 1.8V
		u8 abRFU[2];	 // Reserved for future use
	} PC_to_RDR_IccPowerOnPayload;

	// Payload for PC_to_RDR_IccPowerOff, PC_to_RDR_GetSlotStatus, PC_to_RDR_GetParameters, PC_to_RDR_ResetParameters, PC_to_RDR_IccClock
	typedef struct
	{
		u8 abRFU[3]; // Reserved
	} NoPayload;

	// Payload for PC_to_RDR_Mechanical
	typedef struct
	{
		u8 abRFU[2];
	} NoPayload1;

	// Payload for PC_to_RDR_XfrBlock
	typedef struct
	{
		u8 bBWI;
		u16 wLevelParameter;
		u8 abData[MAX_DATA_SIZE]; // Replace MAX_DATA_SIZE with the appropriate size
	} PC_to_RDR_XfrBlockPayload;

	// Payload for PC_to_RDR_T0APDU
	typedef struct
	{
		u8 bmChanges;
		u8 bClassGetResponse;
		u8 bClassEnvelope;
	} PC_to_RDR_T0APDU_Payload;

	// payload varies based on the protocol
	typedef struct
	{
		u8 bmFindexDindex;
		u8 bmTCCKST0;
		u8 bGuardTimeT0;
		u8 bWaitingIntegerT0;
		u8 bClockStop;
	} ProtocolDataStructureT0;

	typedef struct
	{
		u8 bmFindexDindex;
		u8 bmTCCKST1;
		u8 bGuardTimeT1;
		u8 bmWaitingIntegersT1;
		u8 bClockStop;
		u8 bIFSC;
		u8 bNadValue;
	} ProtocolDataStructureT1;

	// Payload for PC_to_RDR_Escape
	typedef struct
	{
		u8 abData[MAX_ESCAPE_DATA_SIZE];
	} PC_to_RDR_EscapePayload;

	typedef struct
	{
		u8 bTimeOut;
		u8 bmFormatString;
		u8 bmPINBlockString;
		u8 bmPINLengthFormat;
		u16 wPINMaxExtraDigit;
		u8 bEntryValidationCondition;
		u8 bNumberMessage;
		u16 wLangID;
		u8 bMsgIndex;
		u8 bTeoPrologue[3]; // For T=1
							// abPINApduByte would be a dynamically sized array,
							// handle its allocation separately
	} PINVerificationDataStructure;

	typedef struct
	{
		// ... similar fields as PINVerificationDataStructure ...
		u8 bInsertionOffsetOld;
		u8 bInsertionOffsetNew;
		u8 bConfirmPIN;
		// ... other fields ...
	} PINModificationDataStructure;

	typedef struct
	{
		u8 bPINOperation;
		union
		{
			PINVerificationDataStructure verifyData;
			PINModificationDataStructure modifyData;
			// Add other operation-specific structs as needed
		} operationData;
	} PC_to_RDR_SecurePayload;

	// Payload for PC_to_RDR_SetDataRateAndClockFrequency
	typedef struct
	{
		u32 dwClockFrequency;
		u32 dwDataRate;
	} PC_to_RDR_SetDataRateAndClockFrequencyPayload;

	// Message payload for CCID bulk out message
	typedef union
	{
		PC_to_RDR_IccPowerOnPayload powerOnPayload;
		NoPayload noPayload;
		NoPayload1 noPayload1;
		PC_to_RDR_XfrBlockPayload xfrBlockPayload;
		ProtocolDataStructureT0 protocolDataStructureT0;
		ProtocolDataStructureT1 protocolDataStructureT1;
		PC_to_RDR_T0APDU_Payload t0APDUPayload;
		PC_to_RDR_EscapePayload escapePayload;
		PINVerificationDataStructure pinVerifyData;
		PINModificationDataStructure pinModifyData;
		PC_to_RDR_SecurePayload securePayload;
		PC_to_RDR_SetDataRateAndClockFrequencyPayload setDataRateAndClockFrequencyPayload;
	} CCID_MessagePayload;

	typedef struct
	{
		u8 bMessageType;			 // Identifies the command type
		u8 bSlot;					 // Slot number
		u8 bSeq;					 // Sequence number
		u32 dwLength;				 // Message-specific data length
		CCID_MessagePayload payload; // Payload (variable length, interpretation depends on bMessageType)
	} CCID_BulkOutMessage;

	typedef struct
	{
		u8 lastBulkOutCommand;
		u8 lastBulkOutSeqNum;
		bool isFailingBulkOut;
	} SlotState;

	// Response data for RDR_to_PC_DataBlock
	typedef struct
	{
		u8 abData[]; // Flexible array member for dynamic size
	} RDR_to_PC_DataBlock;

	// Response data for RDR_to_PC_SlotStatus
	typedef struct
	{
		u8 bClockStatus; // Clock status (00h: running, 01h: stopped low, etc.)
	} RDR_to_PC_SlotStatus;

	typedef struct
	{
		u8 bProtocolNum;
		union
		{
			ProtocolDataStructureT0 protocolDataT0;
			ProtocolDataStructureT1 protocolDataT1;
		} protocolData;
	} RDR_to_PC_Parameters;

	typedef struct
	{
		u8 abData[MAX_ESCAPE_DATA_SIZE]; // Replace MAX_ESCAPE_DATA_SIZE
	} RDR_to_PC_Escape;

	typedef struct
	{
		u32 dwClockFrequency;
		u32 dwDataRate;
	} RDR_to_PC_DataRateAndClockFrequency;

	typedef struct
	{
		u8 bMessageType;
		u32 dwLength;
		u8 bSlot;
		u8 bSeq;
		u8 bStatus;
		u8 bError;
		u8 bChainParameter;
		union
		{
			RDR_to_PC_DataBlock dataBlock;	 // For basic data block responses
			RDR_to_PC_SlotStatus slotStatus; // For slot status responses
			RDR_to_PC_Parameters parameters; // For parameter responses
			RDR_to_PC_Escape escapeData;	 // For escape responses
			RDR_to_PC_DataRateAndClockFrequency dataRateAndClockFrequency;
		} responseData;
	} BulkInMessage;

	/*
	 * The following structures define USB storage class requests. The details of
	 * the contents of those structures are not important in the context of this
	 * example.
	 */
	typedef struct
	{
		u32 dCBWSignature;
		u32 dCBWTag;
		u32 dCBWDataTransferLength;
		u8 bmCBWFlags;
		u8 cCBWLUN;
		u8 bCBWCBLength;
		u8 CBWCB[16];
	} attribute(USB_CBW);

	typedef struct
	{
		u32 dCSWSignature;
		u32 dCSWTag;
		u32 dCSWDataResidue;
		u8 bCSWStatus;
	} attribute(USB_CSW);

	typedef struct
	{
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

	typedef struct
	{
		u8 reserved[3];
		u8 listLength;
		u32 numBlocks;
		u8 descCode;
		u8 blockLengthMSB;
		u16 blockLength;
	} attribute(SCSI_CAP_LIST);

	typedef struct
	{
		u32 numBlocks;
		u32 blockSize;
	} attribute(SCSI_READ_CAPACITY);

	typedef struct
	{
		u8 opCode;
		u8 reserved1;
		u32 block;
		u8 reserved2;
		u16 length;
		u8 control;
	} attribute(SCSI_READ_WRITE);

	typedef struct
	{
		u8 opCode;
		u8 immed;
		u8 reserved1;
		u8 reserved2;
		u8 start;
		u8 control;
	} attribute(SCSI_START_STOP);

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

#endif /* XUSB_CLASS_CCID_H */
