#ifndef __datautil
#define __datautil
/*****************************************************************************
*
*  TITLE:        Data Utilities
*
*  DESCRIPTION:  The module "Data_Utilities"
*                provides utilities for moving and
*                swapping data in general.
*
*  *k "%n"
*  FILE NAME:    "DATAUTIL.H"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#ifndef __icitypes
#include "icitypes.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------
   Source: "Sum81.c"
  -------------------------------------------------------------------------*/

UINT8 Sum8(             /* the sum */

     UINT8* buffer,     /* the data to generate sum of */
     UINT   length);    /* the number of data bytes */

/*-------------------------------------------------------------------------
   Source: "Sum161.c"
  -------------------------------------------------------------------------*/

UINT16 Sum16(           /* the sum */

     UINT16* buffer,    /* the buffer to perform sum of */
     UINT    length);   /* the length of the buffer */

/*-------------------------------------------------------------------------
   Source: "Sum321.c"
  -------------------------------------------------------------------------*/

UINT32 Sum32(           /* the sum */

     UINT32* buffer,    /* the buffer to perform sum of */
     UINT    length);   /* the length of the buffer */

/*-------------------------------------------------------------------------
   Source: "Move_Da1.c"
  -------------------------------------------------------------------------*/

void Move_Data(

     void* source,      /* source address for move */
     void* target,      /* destination address for move */
     UINT  count);      /* number of bytes to move */

/*-------------------------------------------------------------------------
   Source: "Swap_By1.c"
  -------------------------------------------------------------------------*/

void Swap_Bytes(

     UINT16* data,      /* data words whose bytes must be swapped */
     UINT count);       /* number of 16-bit words to swap */


/*-------------------------------------------------------------------------
   Source: "Swap_321.c"
  -------------------------------------------------------------------------*/

void Swap_32_Bit_Bytes(

     UINT32* data,      /* 32-bit data words whose bytes must be swapped */
     UINT count);       /* number of 32-bit words to swap */


/*-------------------------------------------------------------------------
   Source: "Swap_Un1.c"
  -------------------------------------------------------------------------*/

void Swap_Units(

     void* data,                /* data words whose bytes must be swapped */
     UINT record_count,         /* number of records */
     UINT unit_count,           /* number of units in each record */
     UINT unit_size);           /* size of each unit in the record */

#ifdef __cplusplus
}   // extern "C"
#endif


#endif
