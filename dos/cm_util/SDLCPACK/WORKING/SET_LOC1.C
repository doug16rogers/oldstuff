/*****************************************************************************
*
*  TITLE:        Set Local SDLC Address
*
*  DESCRIPTION:  The subprogram "Set_Local_SDLC_Address"
*                sets the SDLC LAN address to be used as
*                the source address for packets built
*                by the module.
*
*  *k "%n"
*  FILE NAME:    "SET_LOC1.C"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    See CSC reference.
*
*****************************************************************************/

#ifndef __sdlcpack
#include "sdlcpack.h"
#endif


void Set_Local_SDLC_Address(

     UINT8 source_address)      /* address of this device on SDLC LAN */

{
   local_sdlc_address = source_address;

}   /* Set_Local_SDLC_Address */


