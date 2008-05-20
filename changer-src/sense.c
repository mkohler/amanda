#include <amanda.h>

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "scsi-defs.h" 
/*
 Handling of Sense codes which are returned from the device
 At the moment the following status us returned
 SENSE_ABORT	       	-> -1 , some strange happend, abort everything
 SENSE_IGNORE	       	-> 0 , nothing special, only info
 SENSE_NO_TAPE	       	-> 1 , this is for tape devices, not tape online (not used any longer ??)
 SENSE_RETRY	       	-> 2 , retry the command
 SENSE_IES	       	-> 3 , initialize element status
 SENSE_TAPE_NOT_LOADED 	-> 4 , no tape loaded
 SENSE_NO              	-> 5 , no sense information
 SENSE_TAPE_NOT_UNLOADED -> 6 , tape is not ejected
 SENSE_IES		-> FF , Sense from initialize element status
 */

	SenseType_T SenseType [] = {
/*
 * Generic one, this is used if not information is found based on the ident of the device
 */
	{ "generic", "", TYPE_TAPE,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "generic", "", TYPE_TAPE,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "generic", "", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "generic", "", TYPE_TAPE,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "generic", "", TYPE_TAPE , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "generic", "", TYPE_TAPE , SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "The drive is not ready, but it is in the process of becoming ready"},
	{ "generic", "", TYPE_TAPE , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "generic", "", TYPE_TAPE,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "generic", "", TYPE_TAPE , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "generic", "", TYPE_TAPE,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "generic", "", TYPE_TAPE , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "generic", "", TYPE_TAPE,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "generic", "", TYPE_TAPE , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "generic", "", TYPE_TAPE,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "generic", "", TYPE_TAPE , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "generic", "", TYPE_TAPE,  SENSE_UNIT_ATTENTION, -1, -1, SENSE_ABORT, "Default for SENSE_UNIT_ATTENTION"},

	{ "generic", "", TYPE_TAPE , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},

	{ "generic", "", TYPE_CHANGER,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "generic", "", TYPE_CHANGER,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "generic", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "generic", "", TYPE_CHANGER,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "generic", "", TYPE_CHANGER , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "generic", "", TYPE_CHANGER , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "generic", "", TYPE_CHANGER,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "generic", "", TYPE_CHANGER , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "generic", "", TYPE_CHANGER,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "generic", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "generic", "", TYPE_CHANGER,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "generic", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "generic", "", TYPE_CHANGER,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "generic", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "generic", "", TYPE_CHANGER,  SENSE_UNIT_ATTENTION, -1, -1, SENSE_ABORT, "Default for SENSE_UNIT_ATTENTION"},

	{ "generic", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "generic", "", TYPE_CHANGER , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/* 
 *
 * ADIC DAT Autochanger
 *
 */
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "DAT AutoChanger", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x28, 0x01, SENSE_IES, "Door opend"},
	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_UNIT_ATTENTION, -1, -1, SENSE_ABORT, "Default for SENSE_UNIT_ATTENTION"},

	{ "DAT AutoChanger", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "DAT AutoChanger", "", TYPE_CHANGER , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},

/*
 *
 *	L500 (for the L500 ATL library)
 * */
	
	{ "L500", "", TYPE_CHANGER,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "L500", "", TYPE_CHANGER,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "L500", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "L500", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x0a, 0x0, SENSE_IGNORE, "Error Log overflow"},
	{ "L500", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x44, 0xc1, SENSE_IGNORE, "EEPROM Copy 1 bad"},
	{ "L500", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x44, 0xc2, SENSE_IGNORE, "EEPROM Copy 2 bad"},
	{ "L500", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x47, 0x0, SENSE_IGNORE, "SCSI parity error"},
	{ "L500", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x48, 0x0, SENSE_IGNORE, "SCSI IDE message received"},
	{ "L500", "", TYPE_CHANGER,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "L500", "", TYPE_CHANGER , SENSE_NOT_READY, 0x0, 0x0, SENSE_ABORT, "Scsi port not initialized"},
	{ "L500", "", TYPE_CHANGER , SENSE_NOT_READY, 0x04, 0x01, SENSE_RETRY, "Becoming ready, scanning magazines, etc"},
	{ "L500", "", TYPE_CHANGER , SENSE_NOT_READY, 0x04, 0x03, SENSE_ABORT, "Unit not ready: manual intervention required: Door Open"},
/* needed? */
	{ "L500", "", TYPE_CHANGER , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
  	{ "L500", "", TYPE_CHANGER,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

/*	Not used by L500
	{ "L500", "", TYPE_CHANGER , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "L500", "", TYPE_CHANGER,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},
*/
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x0,  0x0,  SENSE_ABORT, "Hardware Error"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x3a, 0x80, SENSE_ABORT, "Media not present"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x40, 0x84, SENSE_ABORT, "POST soft failure"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x44, 0x80, SENSE_ABORT, "Loader Communications timeout"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x44, 0x81, SENSE_ABORT, "Loader communications UART error or buffer overflow"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x44, 0x86, SENSE_ABORT, "bad status returned from loader"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x44, 0xc3, SENSE_ABORT, "EEPROM both copies bad"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x44, 0xff, SENSE_ABORT, "Unexpected status from test"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x70, SENSE_ABORT, "Cartridge has no home"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x71, SENSE_ABORT, "Loader mechanism problem"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x72, SENSE_ABORT, "Tape drive handle problem"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x73, SENSE_IGNORE, "No cartridge in drive during unload"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x74, SENSE_ABORT, "Loader mechanism problem, after retries"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x75, SENSE_ABORT, "Timeout moving cartridge from drive"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x76, SENSE_ABORT, "Timeout unloading cartridge into slot"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x77, SENSE_ABORT, "Couldn't unlock door after retries"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x78, SENSE_ABORT, "Error during SCAN MAGAZINE"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x79, SENSE_ABORT, "Couldn't lock door after retries"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x80, SENSE_ABORT, "Unexpected door open"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x81, SENSE_ABORT, "Didn't find all expected slots during elevator movement"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x82, SENSE_ABORT, "Cartridge alreay in drive during LOAD CARTRIDGE"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x83, SENSE_ABORT, "Slot empty during LOAD CARTRIDGE"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x84, SENSE_ABORT, "Cleaning Tape expired"},
	{ "L500", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x80, 0x85, SENSE_ABORT, "Cleaning Failed"},
	{ "L500", "", TYPE_CHANGER,  SENSE_HARDWARE_ERROR,   -1,   -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x0, 0x0,   SENSE_ABORT, "Illegal Request"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x1a, 0x0,  SENSE_ABORT, "Parameter length error"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x20, 0x0,  SENSE_ABORT, "SCSI invalid opcode"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x21, 0x01, SENSE_ABORT, "Invalid element address"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x00, SENSE_ABORT, "Invalid CDB"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x81, SENSE_ABORT, "Invalid mode on WRITE BUFFER"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x82, SENSE_ABORT, "Invalid drive specified"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x83, SENSE_ABORT, "SEND DIAG Invalid test number"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x86, SENSE_ABORT, "Invalid offset on WRITE BUFFER"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x87, SENSE_ABORT, "Invalid size on WRITE BUFFER"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x89, SENSE_ABORT, "Image data too large on WRITE BUFFER"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x8b, SENSE_ABORT, "Invalid image for CUP"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x8c, SENSE_ABORT, "Non-immediate command during CUP"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x8e, SENSE_ABORT, "Invalid personality for CUP"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x24, 0x8f, SENSE_ABORT, "Bad controller image EDC"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x25, 0x0,  SENSE_ABORT, "Invalid LUN"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x26, 0x0,  SENSE_ABORT, "Parameter list error: invalid field"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x26, 0x01, SENSE_ABORT, "Parameter list error: parameter not supported"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x39, 0x0,  SENSE_ABORT, "Saving parameters not supported"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x3a, 0x81, SENSE_ABORT, "Cleaning Slot empty"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x3a, 0x82, SENSE_ABORT, "Cleaning slot doesn't have a cleaning slot"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x3b, 0x0d, SENSE_ABORT, "Destination element full"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x3b, 0x0e, SENSE_ABORT, "Source slot or drive empty"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x3d, 0x0,  SENSE_ABORT, "SCSI invalid ID message"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x53, 0x0,  SENSE_ABORT, "Media Load/Eject failure"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x53, 0x01, SENSE_ABORT, "Cartridge failed to unload"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0xf1, 0x0,  SENSE_ABORT, "Command unspecified"},
	{ "L500", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0xf1, 0x02, SENSE_ABORT, "Unrecognized loader command"},
	{ "L500", "", TYPE_CHANGER,  SENSE_ILLEGAL_REQUEST , -1, -1,    SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "L500", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x0,  0x0,  SENSE_RETRY, "Unit Attention"},
	{ "L500", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x28, 0x0,  SENSE_RETRY, "Not ready to Ready transition"},
	{ "L500", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x29, 0x0,  SENSE_RETRY, "Reset occured"},
	{ "L500", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x2a, 0x01, SENSE_ABORT, "Mode parameters changed"},
	{ "L500", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x3f, 0x01, SENSE_ABORT, "Microcode has changed"},
	{ "L500", "", TYPE_CHANGER,  SENSE_UNIT_ATTENTION, -1, -1,     SENSE_ABORT, "Default for SENSE_UNIT_ATTENTION"},

/*	Not used by L500
	{ "L500", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},
*/
	{ "L500", "", TYPE_CHANGER,  SENSE_ABORTED_COMMAND, 0x43, 0x0, SENSE_ABORT, "SCSI message error"},
	{ "L500", "", TYPE_CHANGER,  SENSE_ABORTED_COMMAND, 0x47, 0x0, SENSE_ABORT, "SCSI parity error"},
	{ "L500", "", TYPE_CHANGER,  SENSE_ABORTED_COMMAND, 0x48, 0x0, SENSE_ABORT, "SCSI IDE message received"},
	{ "L500", "", TYPE_CHANGER,  SENSE_ABORTED_COMMAND, 0x49, 0x0, SENSE_ABORT, "SCSI invalid message"},
	{ "L500", "", TYPE_CHANGER,  SENSE_ABORTED_COMMAND, 0x4e, 0x0, SENSE_ABORT, "SCSI overlapped commands"},
	
	{ "L500", "", TYPE_CHANGER,  SENSE_ABORTED_COMMAND, -1, -1, SENSE_ABORT, "Default for SENSE_ABORTED_COMMAND"},
	{ "L500", "", TYPE_CHANGER , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},

/*
 * HP C1553A Tape
 */
	{ "C1553A", "", TYPE_TAPE,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "C1553A", "", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "C1553A", "", TYPE_TAPE , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "C1553A", "", TYPE_TAPE , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "C1553A", "", TYPE_TAPE , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "C1553A", "", TYPE_TAPE , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "C1553A", "", TYPE_TAPE , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "C1553A", "", TYPE_TAPE , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "C1553A", "", TYPE_TAPE,  SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},

	{ "C1553A", "", TYPE_TAPE , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},

	{ "C1553A", "", TYPE_CHANGER,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "C1553A", "", TYPE_CHANGER , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "C1553A", "", TYPE_CHANGER , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "C1553A", "", TYPE_CHANGER , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "C1553A", "", TYPE_CHANGER , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "C1553A", "", TYPE_CHANGER , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "C1553A", "", TYPE_CHANGER , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "C1553A", "", TYPE_CHANGER , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "C1553A", "", TYPE_CHANGER,  SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "C1553A", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "C1553A", "", TYPE_CHANGER , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * HP C1537A Tape
 */
	{ "C1537A", "", TYPE_TAPE,  SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "C1537A", "", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "C1537A", "", TYPE_TAPE , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "C1537A", "", TYPE_TAPE , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "C1537A", "", TYPE_TAPE , SENSE_NOT_READY, 0x04, 0x0, SENSE_RETRY, "tape is being ejected"},
	{ "C1537A", "", TYPE_TAPE , SENSE_NOT_READY, 0x04, 0x01, SENSE_RETRY, "tape is being loaded"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "C1537A", "", TYPE_TAPE , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "C1537A", "", TYPE_TAPE , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "C1537A", "", TYPE_TAPE , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "C1537A", "", TYPE_TAPE , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "C1537A", "", TYPE_TAPE , SENSE_UNIT_ATTENTION, 0x28, 0x0, SENSE_RETRY, "Not Ready to Ready Transition"},
	{ "C1537A", "", TYPE_TAPE,  SENSE_UNIT_ATTENTION , -1, -1, SENSE_ABORT, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "C1537A", "", TYPE_TAPE,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "C1537A", "", TYPE_TAPE , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * Tandberg Tape
 */
	{ "TDS 1420", "", TYPE_TAPE, SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "TDS 1420", "", TYPE_TAPE, SENSE_IES, 0x83, 0x0, SENSE_IES, "IES"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_IES, 0x83, 0x1, SENSE_IES, "IES"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_IES, 0x83, 0x4, SENSE_IGNORE, "IES"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},

	{ "TDS 1420", "", TYPE_TAPE, SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "TDS 1420", "", TYPE_TAPE, SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "TDS 1420", "", TYPE_TAPE, SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "TDS 1420", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "TDS 1420", "", TYPE_TAPE, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "TDS 1420", "", TYPE_TAPE, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "TDS 1420", "", TYPE_TAPE, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * DLT 8000 Tape
 * 
 * */
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL, 0x0, 0x0,   SENSE_NO, "No Sense"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL, 0x0, 0x01,  SENSE_NO, "Unexpected FM encountered"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL, 0x0, 0x02,  SENSE_NO, "EOM encountered"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL, 0x0, 0x04,  SENSE_NO, "BOM encountered"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL, 0x5d, 0x00, SENSE_NO, "Failure prediction threshold exceeded"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL, 0x27, 0x82, SENSE_NO, "Data safety write protect"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NULL , -1, -1,    SENSE_RETRY, "Default for SENSE_NULL"},

	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0, 0x0, SENSE_IGNORE, "Recovered Error"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0, 0x17, SENSE_IGNORE, "Cleaning requested"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0a, 0x00, SENSE_IGNORE, "Error log overflow"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x0a, 0x80, SENSE_IGNORE, "Error log generated"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x37, 0x0,  SENSE_IGNORE, "Rounded parameter"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x38, 0x08, SENSE_IGNORE, "repositioning error"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x44, 0xc1, SENSE_IGNORE, "EEPROM copy1 area bad"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x44, 0xc2, SENSE_IGNORE, "EEPROM copy2 area bad"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x47, 0x00, SENSE_IGNORE, "SCSI parity error"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x48, 0x00, SENSE_IGNORE, "IDE Message received"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x51, 0x00, SENSE_IGNORE, "Erase Failure"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x53, 0x01, SENSE_IGNORE, "Unload Tape failure"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x5b, 0x01, SENSE_IGNORE, "Threshold met"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x5b, 0x02, SENSE_IGNORE, "Log counter at maximum"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x80, 0x02, SENSE_IGNORE, "Cleaning requested"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x80, 0x03, SENSE_IGNORE, "Softe error exceeds threshold"},
/*	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_RECOVERED_ERROR, 0x47, 0x0, SENSE_IGNORE, "Scsi Parity Error"}, */
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_RECOVERED_ERROR , -1, -1, SENSE_RETRY, "Default for SENSE_RECOVERED_ERROR"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_NOT_READY, 0x0, 0x0, SENSE_IGNORE, "Not Ready (this shouldn't happen should it?)"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "The drive is not ready, but it is in the process of becoming ready"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_NOT_READY, 0x30,0x02, SENSE_ABORT, "Incompatible tape format"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_NOT_READY, 0x30,0x03, SENSE_ABORT, "Cleaning Cartridge in drive"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_NOT_READY, 0x5a,0x01, SENSE_ABORT, "Asynchronous eject occurred"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_MEDIUM_ERROR, 0x0, 0x0, SENSE_ABORT, "Medium Error"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_MEDIUM_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_MEDIUM_ERROR"},

	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_HARDWARE_ERROR, 0x0, 0x0, SENSE_ABORT, "Hardware Error"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_HARDWARE_ERROR , -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},

	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE , SENSE_UNIT_ATTENTION, 0x28, 0x0, SENSE_RETRY, "Not ready to ready transition"},
	{ "DLT8000", "QUANTUM", TYPE_TAPE,  SENSE_UNIT_ATTENTION, -1, -1, SENSE_ABORT, "Default for SENSE_UNIT_ATTENTION"},

	{ "DLT8000", "QUANTUM", TYPE_TAPE , -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},

/*
 * DLT 7000 Tape
 */
	{ "DLT7000", "", TYPE_TAPE, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "DLT7000", "", TYPE_TAPE, SENSE_NOT_READY, 0x4, 0x2, SENSE_TAPE_NOT_ONLINE, "Logical Unit not ready, in progress becoming ready"},
	{ "DLT7000", "", TYPE_TAPE, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "DLT7000", "", TYPE_TAPE, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "DLT7000", "", TYPE_TAPE, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "DLT7000", "", TYPE_TAPE, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "DLT7000", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "DLT7000", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "DLT7000", "", TYPE_TAPE, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * DLT 4000 Tape
 */
	{ "DLT4000", "", TYPE_TAPE, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "DLT4000", "", TYPE_TAPE, SENSE_NOT_READY, 0x4, 0x2, SENSE_TAPE_NOT_ONLINE, "Logical Unit not ready, in progress becoming ready"},
	{ "DLT4000", "", TYPE_TAPE, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "DLT4000", "", TYPE_TAPE,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "DLT4000", "", TYPE_TAPE, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "DLT4000", "", TYPE_TAPE, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "DLT4000", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "DLT4000", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "DLT4000", "", TYPE_TAPE, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * AIT VLS DLT Library
 */
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x2, SENSE_TAPE_NOT_ONLINE, "Logical Unit not ready, in progress becoming ready"},
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "VLS_DLT", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},

	{ "VLS_DLT", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},
	
	{ "VLS_DLT", "", TYPE_CHANGER, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * AIT VLS SDX Library
 */
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x2, SENSE_TAPE_NOT_ONLINE, "Logical Unit not ready, in progress becoming ready"},
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "VLS_SDX", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "VLS_SDX", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "VLS_SDX", "", TYPE_CHANGER, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * Exabyte 85058 Tape
 */
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "Logical Unit not ready, in progress becoming ready"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE,  SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	{ "EXB-85058HE-0000", "", TYPE_TAPE, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found"},
/*
 * Exabyte 10e Library (Robot)
 */
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NULL, 0x0, 0x0, SENSE_RETRY, "Retry, no sense"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NULL, 0x90, 0x2, SENSE_ABORT, "Illegal Request"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NULL , 0x90, 0x3, SENSE_IES, "IES"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "Logical Unit not ready, in progress becoming ready"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x85, SENSE_ABORT, "Library door is open"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x85, SENSE_ABORT, "The data cartridge magazine is missing"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x89, SENSE_ABORT, "The library is in CHS Monitor mode"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8C, SENSE_RETRY, "The library is performing a power-on self test"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8D, SENSE_ABORT, "The library is in LCD mode"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8E, SENSE_ABORT, "The library is in Sequential mode"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "EXB-10e", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x91, 0x0, SENSE_CHM_FULL, "CHM full during reset"},
	{ "EXB-10e", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_RETRY, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "EXB-10e", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "EXB-10e", "", TYPE_CHANGER, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found for EXB-10e"},

/*
 * Exabyte 210 Library (Robot)
 */
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NULL, 0x0, 0x0, SENSE_RETRY, "Retry, no sense"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NULL, 0x90, 0x2, SENSE_ABORT, "Illegal Request"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NULL , 0x90, 0x3, SENSE_IES, "IES"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "Logical Unit not ready, in progress becoming ready"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x85, SENSE_ABORT, "Library door is open"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x85, SENSE_ABORT, "The data cartridge magazine is missing"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x89, SENSE_ABORT, "The library is in CHS Monitor mode"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8C, SENSE_RETRY, "The library is performing a power-on self test"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8D, SENSE_ABORT, "The library is in LCD mode"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8E, SENSE_ABORT, "The library is in Sequential mode"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "EXB-210", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x91, 0x0, SENSE_CHM_FULL, "CHM full during reset"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x21, 0x01, SENSE_ABORT, "Invalid element address"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x24, 0x00, SENSE_ABORT, "Invalid Invert Field"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3b, 0x0d, SENSE_ABORT, "Destination element occupied"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3b, 0x0e, SENSE_ABORT, "Source Element empty"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3b, 0x87, SENSE_ABORT, "Cartridge stuck in tape"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3b, 0x90, SENSE_ABORT, "Source cart is loaded inside the tape drive and not accessible"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x80, 0x03, SENSE_ABORT, "Source magazine not installed"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x80, 0x04, SENSE_ABORT, "Destination magazine no installed"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x80, 0x05, SENSE_ABORT, "Source tape drive not installed"},
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x80, 0x06, SENSE_ABORT, "Destination tape drive not installed"},
	
	{ "EXB-210", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_RETRY, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x0, SENSE_IES, "Label questionable"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x1, SENSE_IGNORE, "Cannot read bar code label"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x2, SENSE_ABORT, "Cartzridge magazine not present"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x3, SENSE_IES, "Label and full status questionable"},
       	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x4, SENSE_ABORT, "Tape drive not installed"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x7, SENSE_IES, "Full status questionable"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x8, SENSE_IGNORE, "Bar code label upside down"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0x9, SENSE_IGNORE, "No bar code label"},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, 0x83, 0xa, SENSE_IGNORE, ""},
	{ "EXB-210", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "EXB-210", "", TYPE_CHANGER, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found for EXB-10e"},

/*
 * Exabyte 230D Library (Robot)
 */
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NULL, 0x0, 0x0, SENSE_RETRY, "Retry, no sense"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NULL, 0x90, 0x2, SENSE_ABORT, "Illegal Request"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NULL , 0x90, 0x3, SENSE_IES, "IES"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NULL , -1, -1, SENSE_RETRY, "Default for SENSE_NULL"},

	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Logical Unit not ready, no additional sense"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "Logical Unit not ready, in progress becoming ready"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x85, SENSE_ABORT, "Library door is open"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x85, SENSE_ABORT, "The data cartridge magazine is missing"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x89, SENSE_ABORT, "The library is in CHS Monitor mode"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8C, SENSE_RETRY, "The library is performing a power-on self test"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8D, SENSE_ABORT, "The library is in LCD mode"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x8E, SENSE_ABORT, "The library is in Sequential mode"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x30, 0x3, SENSE_RETRY, "The tape drive is being cleaned"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY, 0x3A, 0x0, SENSE_TAPE_NOT_ONLINE, "No Tape online"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_NOT_READY , -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},

	{ "EXB-230D", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x0, 0x0, SENSE_RETRY, "Unit Attention"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_UNIT_ATTENTION , -1, -1, SENSE_RETRY, "Default for SENSE_UNIT_ATTENTION"},
	
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x0, 0x0, SENSE_ABORT, "Illegal Request"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x91, 0x0, SENSE_CHM_FULL, "CHM full during reset"},
	{ "EXB-230D", "", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST , -1, -1, SENSE_RETRY, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{ "EXB-230D", "", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "EXB-230D", "", TYPE_CHANGER, -1, 0x0, 0x0, SENSE_ABORT, "Nothing Found for EXB-10e"},
/*
 * Spectra TreeFrog  library
 */
 	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_NULL, 0x0, 0x0, SENSE_NO, "No Sense, Unit Ready"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_NULL, -1, -1, SENSE_NO, "No Sense, Unit Ready"},
	
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x0, SENSE_RETRY, "Unit Not Ready"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x1, SENSE_RETRY, "Unit is Becoming Ready"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_NOT_READY, 0x4, 0x83, SENSE_ABORT, "Door is open, Robot is Disabled"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_NOT_READY, -1, -1, SENSE_RETRY, "Default for SENSE_NOT_READY"},
	
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x4C, 0x0, SENSE_ABORT, "Unit Failed Initialization"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x84, 0x4, SENSE_ABORT, "DRAM Memory Failure"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x84, 0x4, SENSE_ABORT, "Two ore More SCSI ID's in the library are tehe same"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x81, 0x4, SENSE_ABORT, "Tape may be broken;of tape is a cleaning tape;or Drive B is broken"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x87, 0x0, SENSE_ABORT, "Bad FPROM or invalid device in socket"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x87, 0x1, SENSE_ABORT, "FPROM Erase Operation Failed"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x87, 0x2, SENSE_ABORT, "FFPROM Write Operation Failed"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x85, 0x1, SENSE_ABORT, "Robot not Initialized"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x85, 0x99, SENSE_ABORT, "Generic Robotics  Error"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x85, 0x2, SENSE_ABORT, "Long Axis Robotics Error"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x85, 0x3, SENSE_ABORT, "Short Axis Robotics Error"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, 0x85, 0x4, SENSE_ABORT, "Ambient Light Detected"},
	{ "215", "SPECTRA", TYPE_CHANGER, SENSE_HARDWARE_ERROR, -1, -1, SENSE_ABORT, "Default for SENSE_HARDWARE_ERROR"},
	
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x1A, 0x0, SENSE_ABORT, "Parameter List Length Error"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x20, 0x0, SENSE_ABORT, "Invalid Command Code"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x21, 0x01, SENSE_ABORT, "Invalid Element Address"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x24, 0x0, SENSE_ABORT, "Invalid Field in CDB"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x25, 0x0, SENSE_ABORT, "LUN Not Supported"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x26, 0x0, SENSE_ABORT, "Invalid Parameter Field"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3b, 0xd, SENSE_ABORT, "Medium Destination is full"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3b, 0xe, SENSE_ABORT, "Medium Source Element is Full"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x3d, 0x80, SENSE_ABORT, "Disconnects Must be Allowed"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x80, 0x18, SENSE_ABORT, "Conflict, Element is Reserved"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x81, 0x2, SENSE_ABORT, "Library is Full of Tapes"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, 0x81, 0x3, SENSE_ABORT, "Grip Arm not Empty"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_ILLEGAL_REQUEST, -1 , -1, SENSE_ABORT, "Default for SENSE_ILLEGAL_REQUEST"},
	
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x28, 0x0, SENSE_IES, "Inventory possible Altered"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x29, 0x0, SENSE_RETRY, "A Reset Has Occured"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_UNIT_ATTENTION, 0x24, 0x1, SENSE_IGNORE, "Mode Parameter Have CHanged"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_UNIT_ATTENTION, -1, -1, SENSE_IGNORE, "Default for SENSE_UNIT_ATTENTION"},
	
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x83, 0x00, SENSE_ABORT, "Barcode Label is Unread"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x83, 0x01, SENSE_ABORT, "Problem Reading Barcode"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x83, 0x11, SENSE_ABORT, "Tape in Drive & Unmounted"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x84, 0x00, SENSE_ABORT, "Unsupported SCSI Command"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x84, 0x01, SENSE_ABORT, "No Response from SCSI Target"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x84, 0x02, SENSE_ABORT, "Check Condition form Target"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x84, 0x03, SENSE_ABORT, "SCSI ID Same as Library's ID"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x84, 0x08,  SENSE_ABORT, "Busy Condition from Target"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC, 0x84, 0x18, SENSE_ABORT, "SCSI Reservation Conflict"},
	{"215", "SPECTRA", TYPE_CHANGER, SENSE_VENDOR_SPECIFIC,  -1, -1, SENSE_ABORT, "Default for SENSE_VENDOR_SPECIFIC"},
	 
	{ "215", "SPECTRA", TYPE_CHANGER,  SENSE_CHG_ELEMENT_STATUS, -1, -1, SENSE_IGNORE, "Default for SENSE_CHG_ELEMENT_STATUS"},

	{ "215", "SPECTRA", TYPE_CHANGER, -1, 0x0, 0x0, SENSE_ABORT, "Nothing found for Spectra/215"},
	
	{ NULL, "", 0x0, -1, 0x0, 0x0, 0x0, ""},

	};


void DumpSense()
{
	SenseType_T *pwork = (SenseType_T *)&SenseType;

	while (pwork->ident != NULL)	
	{
		if (pwork->sense == -1)
		{
			printf("\n");
		} else {
			printf("%s %s %02X %02X %02X %d %s\n",pwork->ident, pwork->vendor,
				pwork->sense,
				pwork->asc,
				pwork->ascq,
				pwork->ret,
				pwork->text);
		}
		pwork++;
	}
}

/*
 * Decode the Sense Key,ASC, ASCQ and device type, and return an action Type.
 *	type describes the device type as returned by the inquiry command
 *	ignsense changes the way the ASC/ASCQ values are handled.
 *	 if = 0 the sense key is also used for decoding,
 *	 if > 0 the sense key is ignored in the search, and only ASC/ASCQ will be checked
 *	sense is the sense key returned by the scsi command
 *	ASC is the ASC value returned by the scsi command
 *	ASCQ is the ASCQ value returned by the scsi command
 *	text is a pointer to store the clear text reason from the table.
 *	
 *	TODO:
 *	
*/
int Sense2Action(char *ident,
		unsigned char type,
		unsigned char ignsense,
		unsigned char sense,
		unsigned char asc,
		unsigned char ascq,
		char **text)
{
	/*
	 * Decode the combination of sense key, ASC, ASCQ and return what to do with this
	 * status
	 * A future extension could be to call direct a function which handles this case
	 *
	 */
	SenseType_T *pwork = (SenseType_T *)&SenseType;
	SenseType_T *generic = NULL;
        int in = 0;

	dbprintf(("Sense2Action START : type(%d), ignsense(%d), sense(%02X), asc(%02X), ascq(%02X)\n",
		type,
		ignsense,
		sense,
		asc,
		ascq));
	
	while (pwork->ident != NULL)	
	{
		if (strcmp(pwork->ident, "generic") == 0 && pwork->type == type && generic == NULL)
		{
			generic = pwork;
		}
		
		if (strcmp(pwork->ident, ident) == 0 && pwork->type == type)
                {
			in = 1;
                } else {
			if (in == 1)
			{
				dbprintf(("Sense2Action       : no match\n"));
				break;
			}
			pwork++;
			continue;
		}

		if (in == 1)
		{
			/* End of definitions for this device */
			if (pwork->sense == -1)
			{
				*text = (char *)stralloc(pwork->text);
				dbprintf(("Sense2Action   END : no match for %s %s\n",
					pwork->ident,
					pwork->vendor));
				return(pwork->ret);
			}
			
			if (ignsense)
			{
				if (pwork->asc ==  asc && pwork->ascq == ascq)
				{
					*text = (char *)stralloc(pwork->text);
					dbprintf(("Sense2Action END(IGN) : match for %s %s  return -> %d/%s\n",
						pwork->ident,
						pwork->vendor,
						pwork->ret,
						*text));	
					return(pwork->ret);
				}
				pwork++;
				continue;
			} 
			
			if (pwork->sense == sense)
			{
				/* Matching ASC/ASCQ, if yes return the defined result code */
				if (pwork->asc ==  asc && pwork->ascq == ascq)
				{
					*text = (char *)stralloc(pwork->text);
					dbprintf(("Sense2Action   END : match for %s %s  return -> %d/%s\n",
						pwork->ident,
						pwork->vendor,
						pwork->ret,
						*text));		
					return(pwork->ret);
				}
				
				/* End of definitions for this sense type, if yes return the default
				 * for this type
				 */
				if ( 	pwork->asc == -1 && pwork->ascq == -1)
				{
					*text = (char *)stralloc(pwork->text);
					dbprintf(("Sense2Action   END : no match for %s %s  return -> %d/%s\n",
						pwork->ident,
						pwork->vendor,
						pwork->ret,
						*text));	
					return(pwork->ret);
				}			
				pwork++;
				continue;
			}
			
			pwork++;
			continue;
		}
		
		pwork++;
	}

	/* 
	 * Ok no match found, so lets return the values from the generic table
	 */
	dbprintf(("Sense2Action generic start :\n"));
	while (generic->ident != NULL)
	{	  
		if (generic->sense == -1)
		{
			*text = (char *)stralloc(generic->text);
			dbprintf(("Sense2Action generic END : match for %s %s  return -> %d/%s\n",
				generic->ident,
				generic->vendor,
				generic->ret,
				*text));	
			return(generic->ret);
		}
		
		if (ignsense)
		{
			if (generic->asc ==  asc && generic->ascq == ascq)
			{
				*text = (char *)stralloc(generic->text);
				dbprintf(("Sense2Action generic END(IGN) : match for %s %s  return -> %d/%s\n",
					generic->ident,
					generic->vendor,
					generic->ret,
					*text));	
				return(generic->ret);
			}
			generic++;
			continue;
		} 
			
		if (generic->sense == sense)
		{
			/* Matching ASC/ASCQ, if yes return the defined result code */
			if (generic->asc ==  asc && generic->ascq == ascq)
			{
				*text = (char *)stralloc(generic->text);
				dbprintf(("Sense2Action generic END : match for %s %s  return -> %d/%s\n",
					generic->ident,
					generic->vendor,
					generic->ret,
					*text));	
				return(generic->ret);
			}
			
			/* End of definitions for this sense type, if yes return the default
			 * for this type
			 */
			if ( 	generic->asc == -1 && generic->ascq == -1)
			{
				*text = (char *)stralloc(generic->text);
				dbprintf(("Sense2Action generic END : no match for %s %s  return -> %d/%s\n",
					generic->ident,
					generic->vendor,
					generic->ret,
					*text));	
				return(generic->ret);
			}			
			generic++;
			continue;
		}
		generic++;
	}	

	dbprintf(("Sense2Action END:\n"));
	*text = (char *)stralloc("No match found");
	return(SENSE_ABORT);
}
