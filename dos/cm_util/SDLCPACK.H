#ifndef __sdlcpack
#define __sdlcpack
/*****************************************************************************
*
*  TITLE:        SDLC Packet
*
*  DESCRIPTION:  The module "SDLC_Packet"
*                provides SDLC packet definitions
*                and creation methods.
*
*                All New_XXXX packets are returned as pointers
*                to newly allocated SDLC_PACKET to allow
*                passing of the packets in a generic fashion.
*
*  *k "%n"
*  FILE NAME:    "SDLCPACK.H"
*
*  *k "%v"
*  VERSION:      "5"
*
*  REFERENCE:    ICI SDLC Packet Document.
*
*****************************************************************************/

#ifndef __icitypes
#include "icitypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

     /* packet types... */

#define SDLC_SYSTEM_STATUS_PACKET   ((UINT8) 0x2)
#define SDLC_COMMAND_PACKET         ((UINT8) 0x0)

     /* commands... */

#define READ_EEPROM_COMMAND         ((UINT8) 0x07)
#define WRITE_EEPROM_COMMAND        ((UINT8) 0x08)
#define HIGH_SPEED_READ_COMMAND     ((UINT8) 0x11)
#define SYSTEM_STATUS_COMMAND       ((UINT8) 0x63)

     /* SDLC destination addresses... */

#define DSP1_ADDRESS                ((UINT8) 0x32)
#define DSP2_ADDRESS                ((UINT8) 0x33)
#define GIP_ADDRESS                 ((UINT8) 0x48)

     /* 32-bit checksum constant for read/write commands */

#define CHECKSUM_CONSTANT           ((UINT32) 0xDEADBEEFUL)



typedef struct
{
   BIT destination : 8;
   BIT source : 8;
   BIT size : 16;
   BIT type : 8;
   UINT8 data[1034];
   UINT8 _checksum_;    /* appended just after data, not necessarily here */

} SDLC_PACKET;


#define SDLC_DATA_SIZE          ( sizeof(((SDLC_PACKET*)NULL)->data) )
#define SDLC_HEADER_SIZE        ( sizeof(SDLC_PACKET) - SDLC_DATA_SIZE )

    /* NOTE: SDLC_HEADER_SIZE includes the checksum! */



typedef struct
{
   UINT8  command;      /* command word, always 0x07 */
   UINT8  size;         /* number of 32-bit words to read */
   UINT32 address;      /* address from which to read */

} READ_EEPROM_PACKET;


#ifdef __TURBOC__ /*-------------------------------------------------------*/

typedef struct
{
   BIT    command :  8; /* command word, always 0x11 */
   UINT16 count_lsb;    /* number of 32-bit words to read, 16 lsb's */
   UINT8  count_msb;    /*    most significant 8 bits */
   UINT32 address;      /* address from which to read */

} HIGH_SPEED_READ_PACKET;

#else /*-------------------------------------------------------------------*/

typedef struct
{
   BIT    command :  8; /* command word, always 0x11 */
   BIT    count   : 24; /* number of 32-bit words to read */
   UINT32 address;      /* address from which to read */

} HIGH_SPEED_READ_PACKET;

#endif /*-------------------------------------------------------------------*/

#define MAX_WRITE_EEPROM_DATA   0x100

typedef struct
{
   UINT8  command;      /* command word, always 0x08 */
   UINT8  size;         /* number of 32-bit words to write */
   UINT32 address;      /* address to which to write */
   UINT32 checksum;     /* sums with data words to 0xdeadbeef */
   UINT32 data[MAX_WRITE_EEPROM_DATA];  /* 32-bit data words to write */

} WRITE_EEPROM_PACKET;


typedef WRITE_EEPROM_PACKET READ_EEPROM_RESPONSE_PACKET;


typedef struct
{
   UINT8  command;      /* command word, always 0x08 */
   UINT8  code;         /* return code from DSP; 0 => success */

} WRITE_EEPROM_RESPONSE_PACKET;


typedef struct
{
   UINT8 command;       /* command word, always 0x63 */
   UINT8 code;

} SYSTEM_STATUS_PACKET;

typedef SYSTEM_STATUS_PACKET SYSTEM_STATUS_RESPONSE_PACKET;



/*-------------------------------------------------------------------------
   Source: "Class_D1.c"
  -------------------------------------------------------------------------*/

extern UINT8 local_sdlc_address;


/*-------------------------------------------------------------------------
   Source: "Fill_Hig1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* Fill_High_Speed_Read_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT32 word_count);        /* number of 32-bit words to read */

/*-------------------------------------------------------------------------
   Source: "Fill_Re1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* Fill_Read_EEPROM_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT   word_count);        /* number of 32-bit words to read */

/*-------------------------------------------------------------------------
   Source: "Fill_SD1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* Fill_SDLC_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination,         /* to whom the packet goes (DSPx_ADDRESS) */
     UINT  packet_type,         /* type of packet (command, etc) */
     UINT  data_size,           /* count of bytes in data portion of packet */
     void* data);               /* packet's data portion; NULL = do not use */

/*-------------------------------------------------------------------------
   Source: "Fill_Sy1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* Fill_System_Status_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination);        /* to whom the packet goes (DSPx_ADDRESS) */

/*-------------------------------------------------------------------------
   Source: "Fill_Wr1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* Fill_Write_EEPROM_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32  address,           /* EEPROM address for write, unswapped */
     UINT32* data,              /* the list of 32-bit words to write */
     UINT    word_count);       /* number of 32-bit words to write */

/*-------------------------------------------------------------------------
   Source: "Get_Rea1.c"
  -------------------------------------------------------------------------*/

BOOLEAN Get_Read_EEPROM_Response(       /* response was valid */

     SDLC_PACKET* packet,       /* packet from which to extract response */
     UINT32*  address,          /* EEPROM address from which data was read */
     UINT32** data,             /* pointer to words read (inside packet) */
     UINT*    word_count);      /* number of 32-bit words read */

/*-------------------------------------------------------------------------
   Source: "Get_SDL1.c"
  -------------------------------------------------------------------------*/

BOOLEAN Get_SDLC_Packet(        /* whether packet is valid */

     SDLC_PACKET* packet);      /* packet to check */

/*-------------------------------------------------------------------------
   Source: "Get_Sys1.c"
  -------------------------------------------------------------------------*/

BOOLEAN Get_System_Status_Response(     /* whether response was valid */

     SDLC_PACKET* packet,       /* packet from which to extract response */
     UINT8* status_code);       /* status code from DSP */

/*-------------------------------------------------------------------------
   Source: "Get_Wri1.c"
  -------------------------------------------------------------------------*/

BOOLEAN Get_Write_EEPROM_Response(       /* whether response was valid */

     SDLC_PACKET* packet);      /* packet from which to extract response */

/*-------------------------------------------------------------------------
   Source: "Insert_1.c"
  -------------------------------------------------------------------------*/

void Insert_Checksum(

     SDLC_PACKET* packet);      /* packet into which to insert the checksum */

/*-------------------------------------------------------------------------
   Source: "New_Hig1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* New_High_Speed_Read_Packet(

     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT32 word_count);        /* number of 32-bit words to read */

/*-------------------------------------------------------------------------
   Source: "New_Rea1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* New_Read_EEPROM_Packet(

     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT   word_count);        /* number of 32-bit words to read */

/*-------------------------------------------------------------------------
   Source: "New_SDL1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* New_SDLC_Packet(

     UINT8 destination,         /* to whom the packet goes (DSPx_ADDRESS) */
     UINT  packet_type,         /* type of packet (command, etc) */
     UINT  data_size,           /* count of bytes in data portion of packet */
     void* data);               /* packet's data portion; NULL = do not use */

/*-------------------------------------------------------------------------
   Source: "New_Sys1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* New_System_Status_Packet(

     UINT8 destination);        /* to whom the packet goes (DSPx_ADDRESS) */

/*-------------------------------------------------------------------------
   Source: "New_Wri1.c"
  -------------------------------------------------------------------------*/

SDLC_PACKET* New_Write_EEPROM_Packet(

     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32  address,           /* EEPROM address for write, unswapped */
     UINT32* data,              /* the list of 32-bit words to write */
     UINT    word_count);       /* number of 32-bit words to write */

/*-------------------------------------------------------------------------
   Source: "Set_Loc1.c"
  -------------------------------------------------------------------------*/

void Set_Local_SDLC_Address(

     UINT8 source_address);     /* address of this device on SDLC LAN */


#ifdef __cplusplus
}   // extern "C"
#endif

#endif  /* if not already included */

