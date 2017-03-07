#ifndef __AR_53PAK_H                   /*see if file hasn't been included*/
#define __AR_53PAK_H                   /*set flag against duplicate inclusion*/
/****************************************************************************
         CSC NAME:  Army 1553 Packet Handler
  CSC DESCRIPTION:  This CSC handles all received 1553 packets.
                    It parses and then passes on responsibility.
****************************************************************************/

#define _reserved_                     /*used for bitfields*/
#define _spare_                        /*used for bitfields*/


/***************************************************************************/
/*  These enums are used to name codes that are used by the packets...     */
/***************************************************************************/
enum ORDNANCE_CODES {                  /*ordnance codes are used below*/
  NO_PREFERENCE=0,                     /*no preference ordnance selection*/
  ATGM=1,                              /*air-to-ground-missile*/
  GUN=2,                               /*guns*/
  HE=3,                                /*high explosives*/
  ILLUM=4,                             /*illuminant*/
  SMOKE=5,                             /*smoke*/
  CHAFF=6,                             /*chaff*/
  NO_PREF7=7,                          /*undefined=no preference*/
  NO_PREF8=8,                          /*undefined=no preference*/
  NO_PREF9=9,                          /*undefined=no preference*/
  NO_PREF10=10,                        /*undefined=no preference*/
  NO_PREF11=11,                        /*undefined=no preference*/
  NO_PREF12=12,                        /*undefined=no preference*/
  NO_PREF13=13,                        /*undefined=no preference*/
  NO_PREF14=14,                        /*undefined=no preference*/
  HELLFIRE=15                          /*hellfire*/
};   /*ordnance codes*/

enum FIRING_ANGLES {                   /*firing angles for field below*/
  DIRECT=0,                            /*direct firing angle*/
  LOW=1,                               /*low angle*/
  HIGH=2,                              /*high angle*/
  DIRECT3=3                            /*undefined=direct*/
};   /*firing angles*/

enum HELLFIRE_MODES {                  /*hellfire modes for field below*/
  SINGLE=0,                            /*single fire*/
  RAPID=1,                             /*rapid fire*/
  RIPPLE=2,                            /*ripple*/
  SINGLE3=3                            /*undefined=single*/
};   /*hellfire modes*/


/*--- Spheroid codes ------------------------------------------------------*/
#define SPHEROID_INTERNATIONAL "IN"
#define SPHEROID_CLARKE        "CL"    /*must set clarke_1866_1880 switch*/
#define SPHEROID_EVEREST       "EV"
#define SPHEROID_BESSEL        "BE"
#define SPHEROID_AUSTRALIAN    "AU"


enum ROTARY_SWITCH_CODES {             /*rotary codes for AAH displays*/
  STBY=0,                              /*STBY, standby*/
  RNG=1,                               /*RNG*/
  FD_LS=2,                             /*FD/LS*/
  NOT_USED3=3,                         /*unused code*/
  TRGT=4,                              /*TRGT, target*/
  CODE=5,                              /*CODE*/
  SP_1=6,                              /*SP-1*/
  ATHS=7,                              /*ATHS -- us*/
  NOT_USED8=8,                         /*unused code*/
  NOT_USED9=9,                         /*unused code*/
  NOT_USED10=10,                       /*unused code*/
  NOT_USED11=11,                       /*unused code*/
  NOT_USED12=12,                       /*unused code*/
  NOT_USED13=13,                       /*unused code*/
  NOT_USED14=14,                       /*unused code*/
  NOT_USED15=15                        /*unused code*/
};   /*rotary switch codes*/


enum COMMAND_CODES {                   /*keyboad command codes for CMS80s*/
  NO_OPERATION=0,                      /*no operation to be performed*/
  CLR_KEY_PRESSED=1,                   /*indicates CLR key was pressed*/
  LINE_KEY_PRESSED=2,                  /*line select key pressed*/
  INDEX_KEY_PRESSED=3,                 /*index page key was pressed*/
  NO_OPERATION4=4                      /*rest are all no-ops (up to 127)*/
};   /*keyboard command codes*/


enum MODEM_COMMANDS {                  /*commands for aux modem transmit*/
  NO_OP=0,                             /*nothing to do*/
  TRANSMIT_MESSAGE=1,                  /*transmit the contained message*/
  NO_OP2=2,                            /*no operation*/
  NO_OP3=3                             /*no operation*/
};   /*auxiliary modem transmit commands*/


enum DATA_TYPES {                      /*data types for bulk-data-in packet*/
  INIT_RESERVED_00=0x00,               /*used for test*/
  INIT_NET_ASSIGNMENTS=0x01,           /*init subscriber net assignments*/
  INIT_NET_PARAMETERS=0x02,            /*init net parameters data*/
  INIT_FREETEXT_PRESETS=0x03,          /*init freetext message presets*/
  INIT_MOVEMENT_PRESETS=0x04,          /*init movement presets*/
  INIT_RECEIVE_AUTH_CODES=0x05,        /*init receive authorization codes*/
  INIT_TRANSMIT_AUTH_CODES=0x06,       /*init transmit authorization codes*/
  INIT_CURRENT_PRESENT_NET=0x07,       /*init current and present net addr*/
  INIT_OFP_VERSION_NUMBER=0x08,        /*REQUEST ONLY -- 12-char ofp number*/
  INIT_RESERVED_55=0x55,               /*used for test*/
  INIT_RESERVED_AA=0xAA,               /*used for test*/
  INIT_RESERVED_FF=0xFF                /*used for test*/
};   /*data types for bulk data in*/


enum PRESET_DATA_SEQUENCES {           /*freetext/movement message presets*/
  PRESET_1=0,                          /*preset number 1*/
  PRESET_2=1,                          /*preset number 2*/
  PRESET_3=2,                          /*preset number 3*/
  PRESET_4=3,                          /*preset number 4*/
  PRESET_5=4,                          /*preset number 5*/
  PRESET_6=5,                          /*preset number 6*/
  PRESET_7=6,                          /*preset number 7*/
  PRESET_8=7                           /*preset number 8*/
};   /*either freetext or movement message presets*/


enum AUTH_CODE_RANGES {                /*receive/transmit auth code ranges*/
  AUTH_CODES_01_25=0,                  /*receive/transmit auth codes 01-25*/
  AUTH_CODES_25_50=1,                  /*receive/transmit auth codes 26-50*/
  AUTH_CODES_51_75=2,                  /*receive/transmit auth codes 51-75*/
  AUTH_CODES_76_00=3                   /*receive/transmit auth codes 76-00*/
};   /*receive/transmit auth code ranges*/


enum TEST_MODES {                      /*test modes for hardware status*/
  TEST_NOT_USED=0x0,                   /*unused*/
  LONG_TEST=0x1,                       /*long test*/
  SHORT_TEST=0x2,                      /*short test*/
  NORMAL_OPERATION=0x3                 /*normal operation*/
};   /*test modes for hardware status*/


enum BAUD_RATE_CODES {                 /*baud rate codes for bulk data in*/
  BAUD_RATE_1200=0,                    /*1200 baud*/
  BAUD_RATE_600=1,                     /*600 baud*/
  BAUD_RATE_300=2,                     /*300 baud*/
  BAUD_RATE_150=3,                     /*150 baud*/
  BAUD_RATE_75=4,                      /*75 baud*/
  BAUD_RATE_75_5=5,                    /*unused is 75 baud*/
  BAUD_RATE_75_6=6,                    /*unused is 75 baud*/
  BAUD_RATE_75_7=7                     /*unused is 75 baud*/
};   /*baud rate codes for bulk data in*/


enum MISSION_STATUSES {                /*mission status for weapons control*/
  STAT_0=0,                            /*not applicable*/
  STAT_1=1,                            /*not applicable*/
  STAT_REQUESTED=2,                    /*air mission request received*/
  STAT_ASSIGNED=3,                     /*air mission assignment to other*/
  STAT_ACCEPTED=4,                     /*air mission accepted*/
  STAT_REJECTED=5,                     /*air mission rejected*/
  STAT_READY=6,                        /*air mission accepted, ready to fire*/
  STAT_7=7,                            /*not applicable*/
  STAT_8=8,                            /*not applicable*/
  STAT_SHOT=9,                         /*ordnance has been shot*/
  STAT_EOM=10,                         /*end of mission*/
  STAT_11=11,                          /*not applicable*/
  STAT_CHECKED=12,                     /*checkfire. fire command aborted*/
  STAT_TOT=13,                         /*time on target established*/

  STAT_NONE                            /*end-of-enum marker*/
};   /*mission statuses*/


enum MISSION_COMMANDS {                /*mission command for weapons control*/
  CMD_0=0,                             /*not applicable*/
  CMD_1=1,                             /*not applicable*/
  CMD_ASSIGN=2,                        /*will assign mission to other craft*/
  CMD_ACCEPT=3,                        /*will send mission accept message*/
  CMD_REJECT=4,                        /*will send mission reject message*/
  CMD_READY=5,                         /*will send ready message*/
  CMD_FIRE=6,                          /*will send request-for-fire message*/
  CMD_SHOT=7,                          /*will send mission shot message*/
  CMD_EOM=8,                           /*will send end-of-mission message*/
  CMD_CHECK=9,                         /*will send checkfire message*/
  CMD_TOT=10,                          /*will send time-on-target message*/
  CMD_TPU=11,                          /*will send target position update*/
  CMD_ABORT=12,                        /*will send mission abort message*/
  CMD_CANCKF=13,                       /*will send cancel checkfire message*/
  CMD_REQTPU=14,                       /*will send request target position*/

  CMD_NONE                             /*end-of-enum marker*/
};   /*mission commands*/


enum POSITION_CDU_CODES {              /*codes for R05 W05*/
  DOUBLE_BUFFER  =0,                   /*double buffer data*/
  DISPLAY_CDU1   =0x01,                /*display on CMS80 CDU 1*/
  DISPLAY_CDU2   =0x02,                /*display on CMS80 CDU 2*/
  DISPLAY_BOTH   =0x03                 /*display on both CMS80 CDUs*/
};   /*target position index codes*/


enum CMS80_USAGE_CODES {               /*usage codes for cms80 displays*/
  CMS80_NORMAL_DISPLAY_LINE=0x00,      /*normal display line*/
  CMS80_SCRATCHPAD_DATA=0x01,          /*scratchpad data in packet*/
  CMS80_ANNUNCIATOR_LINE_DATA=0x02,    /*annunciator line data*/
  CMS80_NOTUSED3=0x03                  /*unused up to 0xFF*/
};   /*cms80 usage codes*/


enum CMS80_CONTROL_CODES {             /*control codes for lines*/
  CMS80_NO_OPERATION=0x00,             /*no operation*/
  CMS80_CLEAR=0x01,                    /*clear scracthpad or annunciator*/
  CMS80_LOAD=0x02,                     /*load scratchpad or annunciator*/
  CMS80_FLASH=0x03,                    /*load & flash scratchpad or ann.*/
  CMS80_NOTUSED4=0x04                  /*not used up to 0xFF*/
};   /*cms80 control codes*/

enum VARLEN_CONTROLS {                 /*VARIABLE_LENGTH_xx control codes*/
  VARLEN_USE_NET_PARAMETERS=0x00,      /*use the current net parameters*/
  VARLEN_ENTIRE_NET=0x01,              /*send to all subscribers in net*/
  VARLEN_NOT_USED_02=0x02,             /*not used*/
  VARLEN_NOT_USED_03=0x03,             /*not used*/
  VARLEN_SEND_TO_CHAN1=0x04,           /*send message to channel 1*/
  VARLEN_SEND_TO_CHAN2=0x05,           /*send message to channel 2*/
  VARLEN_SEND_TO_CHAN3=0x06,           /*send message to channel 3*/
  VARLEN_SEND_TO_CHAN4=0x07            /*send message to channel 4*/
};   /*variable length message control codes*/


enum MESSAGE_MODES {                   /*message modes for R18*/
  NO_ACTION=0,
  MSG_REQUEST=1,
  MSG_DELETE=2,
  NO_ACTION3=3
};   /*message mode for channel message requests*/


/***************************************************************************/
/*  These are the Army Interface 1553 Receive subaddress definitions.      */
/***************************************************************************/

/*=== R01 Verify Service Request ==========================================*/
typedef struct {                       /*VERIFY_SERVICE_REQUEST packet, R01*/
/*word 01*/
  unsigned   _spare_:3;                    /*3 spare bits must be 0*/
  unsigned   word_count:5;                 /*5-bit word count only goes to 8*/
  unsigned   message_subaddress:5;         /*message subaddress for verify?*/
  unsigned   transmit_receive:1;           /*1==transmit/0==receive*/
  unsigned   _spare_:2;                    /*2 spare bits must be 0*/
/*words 02-08*/
  unsigned   data[7];                      /*up to 7 data words*/
} VERIFY_SERVICE_REQUEST;              /*end of struct*/


/*=== R02 Load Verify Receive =============================================*/
typedef struct {                       /*LVF_RECEIVE packet*/
/*word 01 - blank*/
  unsigned   _spare_:16;                   /*blank for now*/
} LVF_RECEIVE;                         /*end of struct*/


/*=== R03 Weapons Status ==================================================*/

typedef struct {                       /*WEAPONS_STATUS packet, R03*/
/*word 01 - target identifier*/
  unsigned   target_idL:7;                 /*least significant target ID char*/
  unsigned   _spare_:1;                    /*spare, must be 0 (7-bit ascii)*/
  unsigned   target_idM:7;                 /*most significant target ID char*/
  unsigned   _spare_:1;                    /*spare, must be 0 (7-bit ascii)*/
/*word 02 - weapons status*/
  unsigned   _spare_:13;                   /*spare, must be 0*/
  unsigned   time_of_flight_valid:1;       /*time-of-flight valid flag*/
  unsigned   mode_ordnance_valid:1;        /*mode/ordnance valid flag*/
  unsigned   launch_status:1;              /*launch status, 1==launched*/
/*word 03 - spare*/
  unsigned   _spare_:16;                   /*spare, 0*/
/*word 04 - time of flight, seconds*/
  unsigned   time_of_flight_01:4;          /*BCD time of flight, 0.1 seconds*/
  unsigned   time_of_flight_1:4;           /*BCD time of flight, 1 s*/
  unsigned   time_of_flight_10:4;          /*BCD time of flight, 10 s*/
  unsigned   time_of_flight_100:4;         /*BCD time of flight, 100 s*/
} WEAPONS_STATUS;                      /*end of struct*/


/*=== R05 Target Position =================================================*/

typedef struct {                       /*TARGET_POSITION packet*/
/*word 01 - UTM zone*/
  unsigned   utm_zone_ascii:7;             /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*spare, must be 0, for ASCII above*/
  unsigned   utm_zone_1:4;                 /*BCD UTM zone, 1's weight*/
  unsigned   utm_zone_10:4;                /*BCD UTM zone, 10's weight*/
/*word 02 - UTM area*/
  unsigned   utm_area_ascii0:7;            /*BCD UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*spare, must be zero, for ascii*/
  unsigned   utm_area_ascii1:7;            /*BCD UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*spare, must be zero, for ascii*/
/*word 03 - UTM easting, meters*/
  unsigned   utm_easting_10:4;             /*BCD utm easting, 10 meters*/
  unsigned   utm_easting_100:4;            /*BCD utm easting, 100 m*/
  unsigned   utm_easting_1k:4;             /*BCD utm easting, 1 km*/
  unsigned   utm_easting_10k:4;            /*BCD utm easting, 10 km*/
/*word 04 - UTM northing, meters*/
  unsigned   utm_northing_10:4;            /*BCD utm northing, 10 meters*/
  unsigned   utm_northing_100:4;           /*BCD utm northing, 100 m*/
  unsigned   utm_northing_1k:4;            /*BCD utm northing, 1 km*/
  unsigned   utm_northing_10k:4;           /*BCD utm northing, 10 km*/
/*word 05 - target position index*/
  unsigned   target_position_index:2;      /*target index for CDU control*/
                                       /*..see POSITION_CDU_CODES*/
  unsigned   _reserved_:14;                /*reserved word, must be zero*/
/*word 06 - reserved*/
  unsigned   _reserved_:16;                /*reserved, 0*/
/*word 07 - reserved*/
  unsigned   _reserved_:16;                /*reserved, 0*/
/*word 08 - altitude, meters*/
  unsigned   altitude_1:4;                 /*BCD altitude, 1 meter*/
  unsigned   altitude_10:4;                /*BCD altitude, 10 m*/
  unsigned   altitude_100:4;               /*BCD altitude, 100 m*/
  unsigned   altitude_1k:4;                /*BCD altitude, 1 km, 0Ah=negative*/
} TARGET_POSITION;                     /*end of struct*/


/*=== R06 Fuel and Stores =================================================*/
typedef struct {                       /*FUEL_AND_STORES packet*/
/*word 01 - fuel weight, pounds*/
  unsigned  fuel_weight_valid:1;           /*fuel weight is valid when==1*/
  unsigned  _spare_:3;                     /*spare, must be 0*/
  unsigned  fuel_weight_10:4;              /*BCD fuel weight, 10 pounds*/
  unsigned  fuel_weight_100:4;             /*BCD fuel weight, 100 lb*/
  unsigned  fuel_weight_1k:4;              /*BCD fuel weight, 1k lb*/
/*word 02 - ammunition, rounds*/
  unsigned  ammo_valid:1;                  /*ammo count is valid when==1*/
  unsigned  _spare_:3;                     /*spare, must be 0*/
  unsigned  ammo_10:4;                     /*BCD ammo count, 10 rounds*/
  unsigned  ammo_100:4;                    /*BCD ammo count, 100 rds*/
  unsigned  ammo_1k:4;                     /*BCD ammo count, 1k rds*/
/*word 03 - rockets*/
  unsigned  rockets_valid:1;               /*rockets count is valid when==1*/
  unsigned  _spare_:7;                     /*spare, must be 0*/
  unsigned  rockets_1:4;                   /*BCD rockets count, units*/
  unsigned  rockets_10:4;                  /*BCD rockets, 10*/
/*word 04 - air-to-air rockets*/
  unsigned  a_a_rockets_valid:1;           /*a/a rockets count is valid when==1*/
  unsigned  _spare_:7;                     /*spare, must be 0*/
  unsigned  a_a_rockets_1:4;               /*BCD a/a rockets count, units*/
  unsigned  a_a_rockets_10:4;              /*BCD a/a rockets, 10*/
/*word 05 - TOW missiles*/
  unsigned  tows_valid:1;                  /*tow missiles count is valid when==1*/
  unsigned  _spare_:7;                     /*spare, must be 0*/
  unsigned  tows_1:4;                      /*BCD tow missiles count, units*/
  unsigned  tows_10:4;                     /*BCD tow missiles count, 10*/
/*word 06 - hellfire missiles*/
  unsigned  hellfires_valid:1;             /*hellfire missiles count is valid when==1*/
  unsigned  _spare_:7;                     /*spare, must be 0*/
  unsigned  hellfires_1:4;                 /*BCD hellifre missiles count, units*/
  unsigned  hellfires_10:4;                /*BCD hellfire missiles count, 10*/
/*word 07 - free air temperature, degrees celcius (-43 to +80)*/
  unsigned  fat_valid:1;                   /*free air temperature is valid*/
  unsigned  _spare_:6;                     /*0*/
  unsigned  fat_positive:1;                /*positive temp flag (1=temp>0)*/
  unsigned  fat_1:4;                       /*BCD free air temp, 1*/
  unsigned  fat_10:4;                      /*BCD free air temp, 10*/
/*word 08 - reserved*/
  unsigned  _reserved_:16;                 /*reserved, 0*/
} FUEL_AND_STORES;                     /*end of struct*/


/*=== R07 Present Position ================================================*/
typedef struct {                       /*PRESENT_POSITION packet*/
/*word 01 - UTM zone*/
  unsigned   utm_zone_ascii:7;             /*UTM zone ID letter - ASCII*/
  unsigned   utm_ncd:1;                    /*non-computed data, 1=ignore data*/
  unsigned   utm_zone_1:4;                 /*BCD UTM zone, 1's weight*/
  unsigned   utm_zone_10:4;                /*BCD UTM zone, 10's weight*/
/*word 02 - UTM area*/
  unsigned   utm_area_ascii0:7;            /*BCD UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*spare, must be zero, for ascii*/
  unsigned   utm_area_ascii1:7;            /*BCD UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*spare, must be zero, for ascii*/
/*word 03 - UTM easting, meters*/
  unsigned   utm_easting_10:4;             /*BCD utm easting, 10 meters*/
  unsigned   utm_easting_100:4;            /*BCD utm easting, 100 m*/
  unsigned   utm_easting_1k:4;             /*BCD utm easting, 1 km*/
  unsigned   utm_easting_10k:4;            /*BCD utm easting, 10 km*/
/*word 04 - UTM northing, meters*/
  unsigned   utm_northing_10:4;            /*BCD utm northing, 10 meters*/
  unsigned   utm_northing_100:4;           /*BCD utm northing, 100 m*/
  unsigned   utm_northing_1k:4;            /*BCD utm northing, 1 km*/
  unsigned   utm_northing_10k:4;           /*BCD utm northing, 10 km*/
/*word 05 - UTM spheroid, spheroid codes are #define'd in enum section*/
  unsigned   spheroid_ascii0:7;            /*least sig. spheroid code char*/
  unsigned   _spare_:1;                    /*spare, must be zero -- ascii fill*/
  unsigned   spheroid_ascii1:7;            /*most sig. spheroid code char*/
  unsigned   clarke_1866_1880:1;           /*clarke 1866==1, clarke 1880==0*/
/*word 06 - reserved*/
  unsigned   _reserved_:16;                /*reserved, 0*/
/*word 07 - present aircraft heading, degrees*/
  unsigned   heading_invalid:1;            /*heading info invalid when==1*/
  unsigned   _spare_:3;                    /*spare, must be 0*/
  unsigned   heading_1:4;                  /*BCD heading, 1 degree*/
  unsigned   heading_10:4;                 /*BCD heading, 10 degrees*/
  unsigned   heading_100:4;                /*BCD heading, 100 degrees*/
/*word 08 - altitude, meters*/
  unsigned   alt_ncd:1;                    /*data ignored when==1*/
  unsigned   _spare_:1;                    /*spare, must be 0*/
  unsigned   altitude_10:4;                /*BCD altitude, 10 meters*/
  unsigned   altitude_100:4;               /*BCD altitude, 100 m*/
  unsigned   altitude_1k:4;                /*BCD altitude, 1 km*/
/*word 09 - time of day, HH:MM*/
  unsigned   minutes_1:4;                  /*BCD minutes, x1*/
  unsigned   minutes_10:4;                 /*BCD minutes, x10*/
  unsigned   hours_1:4;                    /*BCD hours, x1*/
  unsigned   hours_10:4;                   /*BCD hours, x10*/
/*word 10 - time of day, SS*/
  unsigned   time_valid:1;                 /*time values are valid when==1*/
  unsigned   _spare_:3;                    /*spare, must be 0*/
  unsigned   seconds_1:4;                  /*BCD seconds, x1*/
  unsigned   seconds_10:4;                 /*BCD seconds, x10*/
/*word 11 - laser code*/
  unsigned   laser_code0:4;                /*BCD laser code, digit 0 (least sig)*/
  unsigned   laser_code1:4;                /*BCD laser code, digit 1 (x10)*/
  unsigned   laser_code2:4;                /*BCD laser code, digit 2 (x100)*/
  unsigned   laser_code3:4;                /*BCD laser code, digit 4 (x1000)*/
/*word 12 - magnetic variation, degress*/
  unsigned   magnetic_variation_01:4;      /*BCD mag var, x0.1 degrees*/
  unsigned   magnetic_variation_1:4;       /*BCD mag var, x1 degrees*/
  unsigned   magnetic_variation_10:4;      /*BCD mag var, x10 degrees*/
  unsigned   magnetic_variation_100:1;     /*binary mag var, x100*/
  unsigned   _spare_:2;                    /*spare, must be 0*/
  unsigned   west_east:1;                  /*variation's sign: 0==east, 1==west*/
} PRESENT_POSITION;                    /*end of struct*/


/*=== R08/16 AAH Keyboard 1/2 =============================================*/
typedef struct {                       /*AAH_KEYBOARD packet*/
/*word 01 - keyboard input*/
  unsigned   ascii_character:7;            /*character pressed into scratchpad*/
  unsigned   _spare_:1;                    /*ascii pad, must be 0*/
  unsigned   rotary_switch_code:4;         /*rotary switch, ROTARY_SWITCH_CODES*/
  unsigned   keyboard_data:1;              /*ascii input valid on first == 1*/
  unsigned   rollover:1;                   /*rollover bit*/
  unsigned   mission_in_progress:1;        /*must be 1 or packet ignored*/
  unsigned   toggle:1;                     /*toggles on each update*/
} AAH_KEYBOARD;                        /*end of struct*/


/*=== R08/16 CMS80 Keyboard CDU 1/2 =======================================*/
typedef struct {                       /*CMS80_KEYBOARD packet*/
/*word 01 - keyboard input*/
  unsigned   transfer_time_tag:8;          /*increments when new data present*/
  unsigned   command_code:7;               /*command code, enum COMMAND_CODES*/
  unsigned   _spare_:1;                    /*spare, must be 0*/
/*word 02 - line key code*/
  unsigned   _spare_:8;                    /*spare, must be 0*/
  unsigned   line_key_code:7;              /*line key codes (see [cduutil.h])*/
  unsigned   _spare_:1;                    /*spare, 0*/
/*word 03 - reserved*/
  unsigned   _spare_:7;                    /*spare, 0*/
  unsigned   display:1;                    /*display when==1*/
  unsigned   _spare_:8;                    /*spare, 0*/
/*word 04 - reserved*/
  unsigned   _reserved_:16;                /*entire word is reserved*/
/*word 05 - reserved*/
  unsigned   _reserved_:16;                /*entire word is reserved*/
/*word 06 - reserved*/
  unsigned   _reserved_:16;                /*entire word is reserved*/
/*word 07 - scratchpad count and summary*/
  unsigned   octal_present:1;              /*1==octals (0-7) in scratchpad*/
  unsigned   decimal_present:1;            /*1==decimals (8-9) in scratchpad*/
  unsigned   hex_present:1;                /*1==hex numbers (A-F) in scratchpad*/
  unsigned   alpha_present:1;              /*1==alphas (G-Z) in scratchpad*/
  unsigned   period_present:1;             /*1==decimal point (.) in scratchpad*/
  unsigned   nonalpha_present:1;           /*1==other characters in scratchpad*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   periods_present:1;            /*1==more than one period (.) in sp*/
  unsigned   character_count:5;            /*number of characters in scratchpad*/
  unsigned   _spare_:3;                    /*spare, 0*/
/*word 08-17 - scratchpad characters*/
  unsigned   scratchpad_chars[10];         /*scratchpad chars, LSB=char 1,MSB=2*/
                                       /*  MSB of last is 0, so 19 chars*/
} CMS80_KEYBOARD;                      /*end of struct*/


/*=== R13 Transparent Receive =============================================*/
typedef struct {                       /*TRANSPARENT_RX_1553 packet*/
/*word 01 - blank*/
  unsigned   _spare_:16;                   /*blank for now*/
} TRANSPARENT_RX_1553;                 /*end of struct*/


/*=== R15 Variable Length Message Transmit ================================*/
typedef struct {                       /*VARIABLE_LENGTH_TX general*/
/*word 01 - control*/                  /*format I*/
  unsigned   protocol:3;                   /*protocol of message*/
  unsigned   word_count:5;                 /*word count for this block*/
  unsigned   timetag:5;                    /*total number of blocks in message*/
  unsigned   _spare_:3;                    /*spare, 0*/
/*word 02 - block control*/
  unsigned   control:3;                    /*control field, see VARLEN_CONTROLS*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   total_blocks:6;               /*total number of blocks in message*/
  unsigned   block:6;                      /*block of this packet in message*/
/*word 03-32 - transmitted data...*/
  unsigned   data[30];                     /*up to 60 data bytes...*/
} VARIABLE_LENGTH_TX;

typedef struct {                       /*VARIABLE_LENGTH_TX_TACFIRE packet*/
/*word 01 - control*/                  /*format I*/
  unsigned   protocol:3;                   /*protocol of message*/
  unsigned   word_count:5;                 /*word count for this block*/
  unsigned   timetag:5;                    /*total number of blocks in message*/
  unsigned   _spare_:3;                    /*spare, 0*/
/*word 02 - block control*/
  unsigned   control:3;                    /*control field, see VARLEN_CONTROLS*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   total_blocks:6;               /*total number of blocks in message*/
  unsigned   block:6;                      /*block of this packet in message*/
/*word 03 - destination*/
  unsigned   _spare_:8;                    /*spare, 0*/
  unsigned   destination:7;                /*destination address or channel*/
  unsigned   _spare_:1;                    /*spare, 0*/
/*word 04 - auth codes*/
  unsigned   auth_code_2:7;                /*second auth code*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   auth_code_1:7;                /*first auth code*/
  unsigned   _spare:1;                     /*spare, 0*/
/*word 05 - test/data, source*/
  unsigned   source:7;                     /*local subscriber address, fill if 0*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   test_data:7;                  /*test/data character*/
  unsigned   _spare_:1;                    /*spare, 0*/
/*word 06 - subtype*/
  unsigned   data_1:7;                     /*data byte number 1*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   subtype:7;                    /*message subtype*/
  unsigned   _spare_:1;                    /*spare, 0*/
/*word 07-32 - more data bytes*/
  unsigned   data[26];                     /*data bytes LSB:3,5..55, MSB:2,4..54*/
} VARIABLE_LENGTH_TX_TACFIRE;          /*end of struct*/

typedef struct {                       /*VARIABLE_LENGTH_TX_AFAPD packet*/
/*word 01 - control*/                  /*format I*/
  unsigned   protocol:3;                   /*protocol of message*/
  unsigned   word_count:5;                 /*word count for this block*/
  unsigned   timetag:5;                    /*total number of blocks in message*/
  unsigned   _spare_:3;                    /*spare, 0*/
/*word 02 - block control*/
  unsigned   control:3;                    /*control field, see VARLEN_CONTROLS*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   total_blocks:6;               /*total number of blocks in message*/
  unsigned   block:6;                      /*block of this packet in message*/
/*word 03-10 - destination*/
  unsigned   destination_hi_lo[8];
/*word 11-32 - more data bytes*/
  unsigned   data[22];                     /*data bytes...*/
} VARIABLE_LENGTH_TX_AFAPD;            /*end of struct*/


/*=== R18 Channel Message Request =========================================*/
typedef struct {                       /*CHANNEL_MESSAGE_REQUEST packet*/
/*word 01 - blank*/
  unsigned   block:6;                      /*block number (when in MSG REQUEST)*/
  unsigned   message_mode:2;               /*message mode*/
  unsigned   message_id:4;                 /*message id number 0-11*/
  unsigned   channel:2;                    /*channel for command*/
  unsigned   _spare_:2;                    /*blank*/
} CHANNEL_MESSAGE_REQUEST;             /*end of struct*/


/*=== R26 Auxiliary Modem Transmit ========================================*/
typedef struct {                       /*AUX_MODEM_TRANSMIT packet*/
/*word 01 - timetag/command*/
  unsigned   command:2;                    /*modem command, see MODEM_COMMANDS*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   timetag:6;                    /*increments on new command*/
  unsigned   channel:2;                    /*channel for transmit*/
/*word 02 - message destination*/
  unsigned   _spare_:8;                    /*spare, 0*/
  unsigned   destination:7;                /*destination address ID (0-9,A-Z)*/
  unsigned   _spare_:1;                    /*ascii-izing spare, 0*/
/*word 03 - authentication codes*/
  unsigned   auth_code_2:7;                /*authorization code character 2*/
  unsigned   _spare_:1;                    /*ascii-izing spare, 0*/
  unsigned   auth_code_1:7;                /*authorization code character 1*/
  unsigned   _spare_:1;                    /*ascii-izing spare, 0*/
/*word 04 - test-data character/local subscriber ID*/
  unsigned   local_subscriber_id:7;        /*local subscriber id (0-9,A-Z)*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   test_data_character:7;        /*msg test/data char or special msgs*/
  unsigned   _spare_:1;                    /*spare, 0*/
/*word 05 - tacfire message type/first data character*/
  unsigned   data_1:7;                     /*first ascii character of message*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   tacfire_type:7;               /*TACFIRE data type ('T', etc)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 06-23 - rest of message data characters*/
  unsigned   data_chars[18];               /*MSB=2,4,6...36, LSB=3,5,7...37*/
} AUX_MODEM_TRANSMIT;                  /*end of struct*/


/*=== R29 Selftest Control Word ===========================================*/
typedef struct {                       /*SELFTEST_CONTROL packet*/
/*word 01 - selftest control word*/
  unsigned   initiate_selftest:1;          /*initiate system BIT when == 1*/
  unsigned   _spare_:15;                   /*all other bits are spare, 0*/
} SELFTEST_CONTROL;                    /*end of struct*/


/*=== R30 Bulk Data In ====================================================*/
typedef struct {                       /*BULK_DATA_IN packet further below*/
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*type of data to be initialized*/
  unsigned   data[31];                     /*31 data words; use structs below*/
} BULK_DATA_IN_HEADER;                 /*end of struct*/

/*--- R30 (01) Subscriber Net Assignments ---------------------------------*/
typedef struct {
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==01*/
/*word 02-19 - net assignments from 0-9,A-Z, see [armydata.h] for net codes*/
  unsigned  two_net_codes[18];             /*MSB=0,2,..A,..Y;LSB=1,3,..B,..Z*/
} R30_NET_ASSIGNMENT;

/*--- R30 (02) Net Parameters Data ----------------------------------------*/
typedef struct {
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==02*/
/*word 02 - net parameters (block/baud/radio/preamble), net 1*/
  unsigned   preamble1_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble1_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code1:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate1:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single1:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 03 - net parameters (auth mode/net type/monitor), net 1*/
  unsigned   net_type1:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode1:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor1_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor1_1:4;                 /*BCD monitor time, x1s*/
/*word 04 - net parameters (block/baud/radio/preamble), net 2*/
  unsigned   preamble2_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble2_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code2:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate2:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single2:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 05 - net parameters (auth mode/net type/monitor), net 2*/
  unsigned   net_type2:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode2:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor2_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor2_1:4;                 /*BCD monitor time, x1s*/
/*word 06 - net parameters (block/baud/radio/preamble), net 3*/
  unsigned   preamble3_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble3_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code3:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate3:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single3:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 07 - net parameters (auth mode/net type/monitor), net 3*/
  unsigned   net_type3:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode3:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor3_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor3_1:4;                 /*BCD monitor time, x1s*/
/*word 08 - net parameters (block/baud/radio/preamble), net 4*/
  unsigned   preamble4_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble4_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code4:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate4:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single4:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 09 - net parameters (auth mode/net type/monitor), net 4*/
  unsigned   net_type4:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode4:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor4_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor4_1:4;                 /*BCD monitor time, x1s*/
/*word 10 - net parameters (block/baud/radio/preamble), net 5*/
  unsigned   preamble5_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble5_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code5:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate5:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single5:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 11 - net parameters (auth mode/net type/monitor), net 5*/
  unsigned   net_type5:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode5:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor5_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor5_1:4;                 /*BCD monitor time, x1s*/
/*word 12 - net parameters (block/baud/radio/preamble), net 6*/
  unsigned   preamble6_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble6_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code6:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate6:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single6:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 13 - net parameters (auth mode/net type/monitor), net 6*/
  unsigned   net_type6:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode6:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor6_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor6_1:4;                 /*BCD monitor time, x1s*/
/*word 14 - net parameters (block/baud/radio/preamble), net 7*/
  unsigned   preamble7_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble7_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code7:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate7:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single7:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 15 - net parameters (auth mode/net type/monitor), net 7*/
  unsigned   net_type7:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode7:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor7_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor7_1:4;                 /*BCD monitor time, x1s*/
/*word 16 - net parameters (block/baud/radio/preamble), net 8*/
  unsigned   preamble8_01:4;               /*BCD preamble time, x0.1 seconds*/
  unsigned   preamble8_1:4;                /*BCD preamble time, x1 s*/
  unsigned   radio_code8:4;                /*radio select code, RADIO_CODES*/
  unsigned   baud_rate8:3;                 /*radio baud rate, BAUD_RATE_CODES*/
  unsigned   double_single8:1;             /*block mode, 1==DOUBLE, 0=SINGLE*/
/*word 17 - net parameters (auth mode/net type/monitor), net 8*/
  unsigned   net_type8:1;                  /*net type, 1==air, 0==TACFIRE*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auth_mode8:1;                 /*auth mode, 1==manual, 0==none*/
  unsigned   monitor8_01:4;                /*BCD monitor time, x0.1s*/
  unsigned   monitor8_1:4;                 /*BCD monitor time, x1s*/
/*word 18 - broadcast and team addresses*/
  unsigned   team_address:7;               /*ascii team net address (0-9,A-Z)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   broadcast_address:7;          /*ascii broadcast address (0-9,A-Z)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 19 - auto auth subscriber/auto auth mode*/
  unsigned   receive_auto_auth:1;          /*1==enabled receive auto auth*/
  unsigned   transmit_auto_auth:1;         /*1==enabled transmit auto auth*/
  unsigned   _spare_:6;                    /*spare, 0*/
  unsigned   auto_auth_address:7;          /*auto auth net address (0-9,A-Z)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
} R30_NET_PARAMETER;

/*--- R30 (03) Freetext Presets -------------------------------------------*/
typedef struct {
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==03*/
/*word 02-19 - freetext message presets, data sequence tells which*/
  unsigned   two_data_chars[18];           /*data, MSB=1,3,..35; LSB=2,4,..36*/
} R30_FREETEXT_PRESET;

/*--- R30 (04) Movement Presets -------------------------------------------*/
typedef struct {
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==02*/
/*word 02-19 - movement message preset, data sequence tells which*/
  unsigned   two_data_chars[16];           /*data, MSB=1,3,..31; LSB=2,4,..32*/
} R30_MOVEMENT_PRESET;

/*--- R30 (05/06) Receive/Transmit Authorization Codes --------------------*/
typedef struct {
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==05 or 06*/
/*word 02-26 - receive/transmit auth code pairs, data seq tells which 25*/
  unsigned   auth_code_chars[25];          /*rx/tx auth codes, MSB=MS char*/
/*word 27 - receive/transmit authorization line number*/
  unsigned   _spare_:8;                    /*spare, 0*/
  unsigned   auth_line_1:4;                /*BCD rx/tx auth line number, x1*/
  unsigned   auth_line_10:4;               /*BCD rx/tx auth line number, x10*/
} R30_AUTH_CODES;

/*--- R30 (07) Current Net/Present Net Address ----------------------------*/
typedef struct {
/*word 01 - initialization control word, see DATA_TYPES and DATA_SEQUENCES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==01*/
/*word 02 - current net/present net address*/
  unsigned   present_net:7;                /*present subscriber id (0-9,A-Z)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   current_net:3;                /*current net for present subscriber*/
  unsigned   _spare_:5;                    /*spare, 0*/
} R30_CURRENT_PRESENT_NET;


/*--- R30 (08) Operational Flight Program (OFP) Part Number ----------------*/
typedef struct {                       /*used only for data requests..*/
/*word 01 - initialization control word, see DATA_TYPES*/
  unsigned   data_sequence:7;              /*sequence for multi-packet inits*/
  unsigned   data_request:1;               /*1==data requested*/
  unsigned   data_type:8;                  /*data type==08*/
/*word 02 - ofp part number, 12 7-bit ascii characters*/
  unsigned   ofp_version[6];               /*MSB=1,3..11, LSB=2,4..12*/
} R30_OFP_VERSION;


typedef union {                        /*now stick'm all together..*/
  BULK_DATA_IN_HEADER     header;      /*..just control words/bits*/
  R30_NET_ASSIGNMENT      net_assignment;
  R30_NET_PARAMETER       net_parameter;
  R30_FREETEXT_PRESET     freetext_preset;
  R30_MOVEMENT_PRESET     movement_preset;
  R30_AUTH_CODES          auth_codes;
  R30_CURRENT_PRESENT_NET current_present_net;
  R30_OFP_VERSION         ofp_version;
} BULK_DATA_IN;                        /*this is the whole thing*/


/***************************************************************************/
/*  These are the Army Interface 1553 Transmit subaddress definitions.     */
/***************************************************************************/

/*=== T01 Service Request =================================================*/
typedef struct {                       /*SERVICE_REQUEST, up to eight*/
/*word 01 - service request buffer 1*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count1:5;                /*word count as in 1553 command word*/
  unsigned   data1:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r1:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 02 - service request buffer 2*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count2:5;                /*word count as in 1553 command word*/
  unsigned   data2:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r2:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 03 - service request buffer 3*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count3:5;                /*word count as in 1553 command word*/
  unsigned   data3:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r3:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 04 - service request buffer 4*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count4:5;                /*word count as in 1553 command word*/
  unsigned   data4:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r4:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 05 - service request buffer 5*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count5:5;                /*word count as in 1553 command word*/
  unsigned   data5:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r5:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 06 - service request buffer 6*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count6:5;                /*word count as in 1553 command word*/
  unsigned   data6:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r6:6;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 07 - service request buffer 7*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count7:5;                /*word count as in 1553 command word*/
  unsigned   data7:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r7:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
/*word 08 - service request buffer 8*/
  unsigned   _reserved_:3;                 /*reserved, 0*/
  unsigned   word_count8:5;                /*word count as in 1553 command word*/
  unsigned   data8:5;                      /*data to tx/rx as in 1553 comm word*/
  unsigned   t_r8:1;                       /*tx/rx bit, as in 1553 command word*/
  unsigned   _spare_:2;                    /*spare, 0*/
} SERVICE_REQUEST;


/*=== T03 Weapons Control =================================================*/
typedef struct {                       /*WEAPONS_CONTROL packet*/
/*--- primary/latest/active/channel 1 mission -----------------------------*/
/*word 01 - target identifier*/
  unsigned   ch1_target_id:7;              /*target id, obs. mission num (1-8)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   ch1_observer_net:7;           /*target id, obs. net address*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 02 - laser code*/
  unsigned   _spare_:14;                   /*0*/
  unsigned   ch1_altitude_valid:1;         /*1==altitude info is valid*/
  unsigned   ch1_position_valid:1;         /*1==target position info valid*/
/*word 03 - weapons mode/ordnance*/
  unsigned   ch1_ordnance_code:4;          /*ordnance code, see ORDNANCE_CODES*/
  unsigned   ch1_rounds:4;                 /*rounds of ordnance (0-9)*/
  unsigned   ch1_firing_angle:2;           /*firing angle, see FIRING_ANGLES*/
  unsigned   ch1_hellfire_mode:2;          /*hellfire mode, see HELLFIRE_MODES*/
  unsigned   ch1_loal_lobl:1;              /*1==LOAL, 0==LOBL*/
  unsigned   ch1_hellfire_mission:1;       /*1==hellfire mission, 0==nonhellfire*/
  unsigned   ch1_shot_laser_code:1;        /*shot: 1==laser code 2, 0==code 1*/
  unsigned   ch1_command:1;                /*command: 1==WR, 0==AMC*/
/*word 04 - target UTM zone*/
  unsigned   ch1_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   ch1_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   ch1_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 05 - target UTM area*/
  unsigned   ch1_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   ch1_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 06 - target UTM easting, meters*/
  unsigned   ch1_utm_easting_10:4;         /*BCD tgt utm easting, 10 meters*/
  unsigned   ch1_utm_easting_100:4;        /*BCD tgt utm easting, 100 m*/
  unsigned   ch1_utm_easting_1k:4;         /*BCD tgt utm easting, 1 km*/
  unsigned   ch1_utm_easting_10k:4;        /*BCD tgt utm easting, 10 km*/
/*word 07 - target UTM northing, meters*/
  unsigned   ch1_utm_northing_10:4;        /*BCD tgt utm northing, 10 meters*/
  unsigned   ch1_utm_northing_100:4;       /*BCD tgt utm northing, 100 m*/
  unsigned   ch1_utm_northing_1k:4;        /*BCD tgt utm northing, 1 km*/
  unsigned   ch1_utm_northing_10k:4;       /*BCD tgt utm northing, 10 km*/
/*word 08 - target altitude, meters*/
  unsigned   ch1_altitude_1:4;             /*BCD altitude, x1 meters*/
  unsigned   ch1_altitude_10:4;            /*BCD altitude, x10m*/
  unsigned   ch1_altitude_100:4;           /*BCD altitude, x100m*/
  unsigned   ch1_altitude_1k:4;            /*BCD altitude, x1km*/
/*word 09 - reserved*/
  unsigned   _reserved_:16;                /*0*/
/*word 10 - mission command/status*/
  unsigned   ch1_mission_status:8;         /*see enum MISSION_STATUSES*/
  unsigned   ch1_mission_command:8;        /*see enum MISSION_COMMANDS*/
/*word 11 - target type/target strength*/
  unsigned   ch1_target_strength:8;        /*ascii, see [targtype.h]*/
  unsigned   ch1_target_type1:8;           /*ascii, see [targtype.h]*/
/*word 12 - laser code 1*/
  unsigned   ch1_lasercode1_1:4;           /*BCD laser code 1, 1*/
  unsigned   ch1_lasercode1_10:4;          /*BCD laser code 1, 10*/
  unsigned   ch1_lasercode1_100:4;         /*BCD laser code 1, 100*/
  unsigned   ch1_lasercode1_1k:4;          /*BCD laser code 1, 1k*/
/*word 13 - laser code 2*/
  unsigned   ch1_lasercode2_1:4;           /*BCD laser code 2, 1*/
  unsigned   ch1_lasercode2_10:4;          /*BCD laser code 2, 10*/
  unsigned   ch1_lasercode2_100:4;         /*BCD laser code 2, 100*/
  unsigned   ch1_lasercode2_1k:4;          /*BCD laser code 2, 1k*/
/*word 14 - rounds 2*/
  unsigned   ch1_laser2_rounds:4;          /*BCD rounds for lasercode 2*/
  unsigned   _spare_:12;                   /*0*/
/*word 15 - reserved*/
  unsigned   _reserved_:16;                /*0*/
/*word 16 - reserved*/
  unsigned   _reserved_:16;                /*reserved, 0*/
/*--- secondary/channel 2 mission... --------------------------------------*/
/*word 17 - target identifier*/
  unsigned   ch2_target_id:7;              /*target id, obs. mission num (1-8)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   ch2_observer_net:7;           /*target id, obs. net address*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 18 - laser code*/
  unsigned   _spare_:14;                   /*0*/
  unsigned   ch2_altitude_valid:1;         /*1==altitude info is valid*/
  unsigned   ch2_position_valid:1;         /*1==target position info valid*/
/*word 19 - weapons mode/ordnance*/
  unsigned   ch2_ordnance_code:4;          /*ordnance code, see ORDNANCE_CODES*/
  unsigned   ch2_rounds:4;                 /*rounds of ordnance (0-9)*/
  unsigned   ch2_firing_angle:2;           /*firing angle, see FIRING_ANGLES*/
  unsigned   ch2_hellfire_mode:2;          /*hellfire mode, see HELLFIRE_MODES*/
  unsigned   ch2_loal_lobl:1;              /*1==LOAL, 0==LOBL*/
  unsigned   ch2_hellfire_mission:1;       /*1==hellfire mission, 0==nonhellfire*/
  unsigned   ch2_shot_laser_code:1;        /*shot: 1==laser code 2, 0==code 1*/
  unsigned   ch2_command:1;                /*command: 1==WR, 0==AMC*/
/*word 20 - target UTM zone*/
  unsigned   ch2_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   ch2_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   ch2_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 21 - target UTM area*/
  unsigned   ch2_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   ch2_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 22 - target UTM easting, meters*/
  unsigned   ch2_utm_easting_10:4;         /*BCD tgt utm easting, 10 meters*/
  unsigned   ch2_utm_easting_100:4;        /*BCD tgt utm easting, 100 m*/
  unsigned   ch2_utm_easting_1k:4;         /*BCD tgt utm easting, 1 km*/
  unsigned   ch2_utm_easting_10k:4;        /*BCD tgt utm easting, 10 km*/
/*word 23 - target UTM northing, meters*/
  unsigned   ch2_utm_northing_10:4;        /*BCD tgt utm northing, 10 meters*/
  unsigned   ch2_utm_northing_100:4;       /*BCD tgt utm northing, 100 m*/
  unsigned   ch2_utm_northing_1k:4;        /*BCD tgt utm northing, 1 km*/
  unsigned   ch2_utm_northing_10k:4;       /*BCD tgt utm northing, 10 km*/
/*word 24 - target altitude, meters*/
  unsigned   ch2_altitude_1:4;             /*BCD altitude, x1 meters*/
  unsigned   ch2_altitude_10:4;            /*BCD altitude, x10m*/
  unsigned   ch2_altitude_100:4;           /*BCD altitude, x100m*/
  unsigned   ch2_altitude_1k:4;            /*BCD altitude, x1km*/
/*word 25 - reserved*/
  unsigned   _reserved_:16;                /*0*/
/*word 26 - mission command/status*/
  unsigned   ch2_mission_status:8;         /*see enum MISSION_STATUSES*/
  unsigned   ch2_mission_command:8;        /*see enum MISSION_COMMANDS*/
/*word 27 - target type/target strength*/
  unsigned   ch2_target_strength:8;        /*ascii, see [targtype.h]*/
  unsigned   ch2_target_type1:8;           /*ascii, see [targtype.h]*/
/*word 28 - laser code 1*/
  unsigned   ch2_lasercode1_1:4;           /*BCD laser code 1, 1*/
  unsigned   ch2_lasercode1_10:4;          /*BCD laser code 1, 10*/
  unsigned   ch2_lasercode1_100:4;         /*BCD laser code 1, 100*/
  unsigned   ch2_lasercode1_1k:4;          /*BCD laser code 1, 1k*/
/*word 29 - laser code 2*/
  unsigned   ch2_lasercode2_1:4;           /*BCD laser code 2, 1*/
  unsigned   ch2_lasercode2_10:4;          /*BCD laser code 2, 10*/
  unsigned   ch2_lasercode2_100:4;         /*BCD laser code 2, 100*/
  unsigned   ch2_lasercode2_1k:4;          /*BCD laser code 2, 1k*/
/*word 30 - rounds 2*/
  unsigned   ch2_laser2_rounds:4;          /*BCD rounds for lasercode 2*/
  unsigned   _spare_:12;                   /*0*/
/*word 31 - reserved*/
  unsigned   _reserved_:16;                /*0*/
/*word 32 - reserved*/
  unsigned   _reserved_:16;                /*reserved, 0*/
} WEAPONS_CONTROL;


/*=== T05 Tactical Target Data ============================================*/
typedef struct {                       /*TARGET_DATA packet*/
/*word 01 - local net address/message valid*/
  unsigned   message_valid:1;              /*message valid flag*/
  unsigned   _spare_:7;                    /*spare, 0*/
  unsigned   local_net_address:7;          /*local net address*/
  unsigned   _spare_:1;                    /*spare, ascii-izer, 0*/
/*--- tactical target 1 ---------------------------------------------------*/
/*word 02 - target identifier*/
  unsigned   tt1_target_id:7;              /*obs. mission num (1-8,arty:A-D)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   tt1_observer_net:7;           /*target id, obs. net address*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 03 - UTM zone*/
  unsigned   tt1_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt1_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tt1_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 04 - target UTM area*/
  unsigned   tt1_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt1_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 05 - target UTM easting, meters*/
  unsigned   tt1_utm_easting_10:4;         /*BCD tgt utm easting, 10 meters*/
  unsigned   tt1_utm_easting_100:4;        /*BCD tgt utm easting, 100 m*/
  unsigned   tt1_utm_easting_1k:4;         /*BCD tgt utm easting, 1 km*/
  unsigned   tt1_utm_easting_10k:4;        /*BCD tgt utm easting, 10 km*/
/*word 06 - target UTM northing, meters*/
  unsigned   tt1_utm_northing_10:4;        /*BCD tgt utm northing, 10 meters*/
  unsigned   tt1_utm_northing_100:4;       /*BCD tgt utm northing, 100 m*/
  unsigned   tt1_utm_northing_1k:4;        /*BCD tgt utm northing, 1 km*/
  unsigned   tt1_utm_northing_10k:4;       /*BCD tgt utm northing, 10 km*/
/*word 07 - target description*/
  unsigned   tt1_target_type:4;            /*target type, see enum*/
  unsigned   tt1_target_subtype:4;         /*target subtype, see enum*/
  unsigned   tt1_target_strength:7;        /*target strength, see enum*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 08 - target paring (attack/scout)*/
  unsigned   tt1_scout_net_address:7;      /*scout craft's net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt1_attack_net_address:7;     /*attack net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*--- tactical target 2 ---------------------------------------------------*/
/*word 09 - target identifier*/
  unsigned   tt2_target_id:7;              /*obs. mission num (1-8,arty:A-D)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   tt2_observer_net:7;           /*target id, obs. net address*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 10 - UTM zone*/
  unsigned   tt2_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt2_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tt2_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 11 - target UTM area*/
  unsigned   tt2_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt2_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 12 - target UTM easting, meters*/
  unsigned   tt2_utm_easting_10:4;         /*BCD tgt utm easting, 10 meters*/
  unsigned   tt2_utm_easting_100:4;        /*BCD tgt utm easting, 100 m*/
  unsigned   tt2_utm_easting_1k:4;         /*BCD tgt utm easting, 1 km*/
  unsigned   tt2_utm_easting_10k:4;        /*BCD tgt utm easting, 10 km*/
/*word 13 - target UTM northing, meters*/
  unsigned   tt2_utm_northing_10:4;        /*BCD tgt utm northing, 10 meters*/
  unsigned   tt2_utm_northing_100:4;       /*BCD tgt utm northing, 100 m*/
  unsigned   tt2_utm_northing_1k:4;        /*BCD tgt utm northing, 1 km*/
  unsigned   tt2_utm_northing_10k:4;       /*BCD tgt utm northing, 10 km*/
/*word 14 - target description*/
  unsigned   tt2_target_type:4;            /*target type, see enum*/
  unsigned   tt2_target_subtype:4;         /*target subtype, see enum*/
  unsigned   tt2_target_strength:7;        /*target strength, see enum*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 15 - target paring (attack/scout)*/
  unsigned   tt2_scout_net_address:7;      /*scout craft's net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt2_attack_net_address:7;     /*attack net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*--- tactical target 3 ---------------------------------------------------*/
/*word 16 - target identifier*/
  unsigned   tt3_target_id:7;              /*obs. mission num (1-8,arty:A-D)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   tt3_observer_net:7;           /*target id, obs. net address*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 17 - UTM zone*/
  unsigned   tt3_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt3_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tt3_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 18 - target UTM area*/
  unsigned   tt3_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt3_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 19 - target UTM easting, meters*/
  unsigned   tt3_utm_easting_10:4;         /*BCD tgt utm easting, 10 meters*/
  unsigned   tt3_utm_easting_100:4;        /*BCD tgt utm easting, 100 m*/
  unsigned   tt3_utm_easting_1k:4;         /*BCD tgt utm easting, 1 km*/
  unsigned   tt3_utm_easting_10k:4;        /*BCD tgt utm easting, 10 km*/
/*word 20 - target UTM northing, meters*/
  unsigned   tt3_utm_northing_10:4;        /*BCD tgt utm northing, 10 meters*/
  unsigned   tt3_utm_northing_100:4;       /*BCD tgt utm northing, 100 m*/
  unsigned   tt3_utm_northing_1k:4;        /*BCD tgt utm northing, 1 km*/
  unsigned   tt3_utm_northing_10k:4;       /*BCD tgt utm northing, 10 km*/
/*word 21 - target description*/
  unsigned   tt3_target_type:4;            /*target type, see enum*/
  unsigned   tt3_target_subtype:4;         /*target subtype, see enum*/
  unsigned   tt3_target_strength:7;        /*target strength, see enum*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 22 - target paring (attack/scout)*/
  unsigned   tt3_scout_net_address:7;      /*scout craft's net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt3_attack_net_address:7;     /*attack net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*--- tactical target 4 ---------------------------------------------------*/
/*word 23 - target identifier*/
  unsigned   tt4_target_id:7;              /*obs. mission num (1-8,arty:A-D)*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
  unsigned   tt4_observer_net:7;           /*target id, obs. net address*/
  unsigned   _spare_:1;                    /*ascii-izer, 0*/
/*word 24 - UTM zone*/
  unsigned   tt4_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt4_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tt4_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 25 - target UTM area*/
  unsigned   tt4_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt4_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 26 - target UTM easting, meters*/
  unsigned   tt4_utm_easting_10:4;         /*BCD tgt utm easting, 10 meters*/
  unsigned   tt4_utm_easting_100:4;        /*BCD tgt utm easting, 100 m*/
  unsigned   tt4_utm_easting_1k:4;         /*BCD tgt utm easting, 1 km*/
  unsigned   tt4_utm_easting_10k:4;        /*BCD tgt utm easting, 10 km*/
/*word 27 - target UTM northing, meters*/
  unsigned   tt4_utm_northing_10:4;        /*BCD tgt utm northing, 10 meters*/
  unsigned   tt4_utm_northing_100:4;       /*BCD tgt utm northing, 100 m*/
  unsigned   tt4_utm_northing_1k:4;        /*BCD tgt utm northing, 1 km*/
  unsigned   tt4_utm_northing_10k:4;       /*BCD tgt utm northing, 10 km*/
/*word 28 - target description*/
  unsigned   tt4_target_type:4;            /*target type, see enum*/
  unsigned   tt4_target_subtype:4;         /*target subtype, see enum*/
  unsigned   tt4_target_strength:7;        /*target strength, see enum*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 29 - target paring (attack/scout)*/
  unsigned   tt4_scout_net_address:7;      /*scout craft's net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tt4_attack_net_address:7;     /*attack net address*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
} TARGET_DATA;


/*=== T06 Tactical Friendly Data ==========================================*/
typedef struct {                       /*FRIENDLY_DATA packet*/
/*--- tactical friendly data 1 --------------------------------------------*/
/*word 01 - friendly net address*/
  unsigned   tf1_net_address:7;            /*net address of friendly target*/
  unsigned   _spare_:9;                    /*spare, 0*/
/*word 02 - friendly UTM zone*/
  unsigned   tf1_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf1_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tf1_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 03 - target UTM area*/
  unsigned   tf1_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf1_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 04 - friendly UTM easting, meters*/
  unsigned   tf1_utm_easting_10:4;         /*BCD friendly utm easting, 10m*/
  unsigned   tf1_utm_easting_100:4;        /*BCD friendly utm easting, 100m*/
  unsigned   tf1_utm_easting_1k:4;         /*BCD friendly utm easting, 1km*/
  unsigned   tf1_utm_easting_10k:4;        /*BCD friendly utm easting, 10km*/
/*word 05 - friendly UTM northing, meters*/
  unsigned   tf1_utm_northing_10:4;        /*BCD friendly utm northing, 10m*/
  unsigned   tf1_utm_northing_100:4;       /*BCD friendly utm northing, 100m*/
  unsigned   tf1_utm_northing_1k:4;        /*BCD friendly utm northing, 1km*/
  unsigned   tf1_utm_northing_10k:4;       /*BCD friendly utm northing, 10km*/
/*--- tactical friendly data 2 --------------------------------------------*/
/*word 06 - friendly net address*/
  unsigned   tf2_net_address:7;            /*net address of friendly target*/
  unsigned   _spare_:9;                    /*spare, 0*/
/*word 07 - friendly UTM zone*/
  unsigned   tf2_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf2_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tf2_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 08 - target UTM area*/
  unsigned   tf2_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf2_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 09 - friendly UTM easting, meters*/
  unsigned   tf2_utm_easting_10:4;         /*BCD friendly utm easting, 10m*/
  unsigned   tf2_utm_easting_100:4;        /*BCD friendly utm easting, 100m*/
  unsigned   tf2_utm_easting_1k:4;         /*BCD friendly utm easting, 1km*/
  unsigned   tf2_utm_easting_10k:4;        /*BCD friendly utm easting, 10km*/
/*word 10 - friendly UTM northing, meters*/
  unsigned   tf2_utm_northing_10:4;        /*BCD friendly utm northing, 10m*/
  unsigned   tf2_utm_northing_100:4;       /*BCD friendly utm northing, 100m*/
  unsigned   tf2_utm_northing_1k:4;        /*BCD friendly utm northing, 1km*/
  unsigned   tf2_utm_northing_10k:4;       /*BCD friendly utm northing, 10km*/
/*--- tactical friendly data 3 --------------------------------------------*/
/*word 11 - friendly net address*/
  unsigned   tf3_net_address:7;            /*net address of friendly target*/
  unsigned   _spare_:9;                    /*spare, 0*/
/*word 12 - friendly UTM zone*/
  unsigned   tf3_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf3_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tf3_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 13 - target UTM area*/
  unsigned   tf3_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf3_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 14 - friendly UTM easting, meters*/
  unsigned   tf3_utm_easting_10:4;         /*BCD friendly utm easting, 10m*/
  unsigned   tf3_utm_easting_100:4;        /*BCD friendly utm easting, 100m*/
  unsigned   tf3_utm_easting_1k:4;         /*BCD friendly utm easting, 1km*/
  unsigned   tf3_utm_easting_10k:4;        /*BCD friendly utm easting, 10km*/
/*word 15 - friendly UTM northing, meters*/
  unsigned   tf3_utm_northing_10:4;        /*BCD friendly utm northing, 10m*/
  unsigned   tf3_utm_northing_100:4;       /*BCD friendly utm northing, 100m*/
  unsigned   tf3_utm_northing_1k:4;        /*BCD friendly utm northing, 1km*/
  unsigned   tf3_utm_northing_10k:4;       /*BCD friendly utm northing, 10km*/
/*--- tactical friendly data 4 --------------------------------------------*/
/*word 16 - friendly net address*/
  unsigned   tf4_net_address:7;            /*net address of friendly target*/
  unsigned   _spare_:9;                    /*spare, 0*/
/*word 17 - friendly UTM zone*/
  unsigned   tf4_utm_zone_ascii:7;         /*UTM zone ID letter - ASCII*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf4_utm_zone_1:4;             /*BCD tgt UTM zone, 1's weight*/
  unsigned   tf4_utm_zone_10:4;            /*BCD tgt UTM zone, 10's weight*/
/*word 18 - target UTM area*/
  unsigned   tf4_utm_area_ascii0:7;        /*BCD tgt UTM area, least sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
  unsigned   tf4_utm_area_ascii1:7;        /*BCD tgt UTM area, most sig. ascii*/
  unsigned   _spare_:1;                    /*ascii-izing 0*/
/*word 19 - friendly UTM easting, meters*/
  unsigned   tf4_utm_easting_10:4;         /*BCD friendly utm easting, 10m*/
  unsigned   tf4_utm_easting_100:4;        /*BCD friendly utm easting, 100m*/
  unsigned   tf4_utm_easting_1k:4;         /*BCD friendly utm easting, 1km*/
  unsigned   tf4_utm_easting_10k:4;        /*BCD friendly utm easting, 10km*/
/*word 20 - friendly UTM northing, meters*/
  unsigned   tf4_utm_northing_10:4;        /*BCD friendly utm northing, 10m*/
  unsigned   tf4_utm_northing_100:4;       /*BCD friendly utm northing, 100m*/
  unsigned   tf4_utm_northing_1k:4;        /*BCD friendly utm northing, 1km*/
  unsigned   tf4_utm_northing_10k:4;       /*BCD friendly utm northing, 10km*/
} FRIENDLY_DATA;


/*=== T07 Hardware Status =================================================*/
typedef struct {                       /*Hardware Status packet*/
/*word 01 - hardware status*/
  baud_rate:3;                         /*baud rate, enum BAUD_RATE_CODES*/
  _spare_:1;                           /*0*/
  radio_code:2;                        /*radio code, enum RADIO_CODES*/
  test_mode:2;                         /*modem test mode*/
  key_line_active:1;                   /*activity on key line?*/
  internal_interrupt_active:1;         /*internal interrupt active*/
  message_alert:1;                     /*message alert when==1*/
  _spare_:5;                           /*0*/
} HARDWARE_STATUS;


/*=== T08/16 AAH Display Lines 1 and 2 ====================================*/
typedef struct {                       /*AAH_LINES_1_2 packet*/
/*word 01..12 - CDU line 1*/
  unsigned   line1[24];                    /*line 1 of display*/
/*word 13..24 - CDU line 2*/
  unsigned   line2[24];                    /*line 2 of display*/
} AAH_TWOLINES;


/*=== T09/17 AAH Display Lines 3 and 4 ====================================*/
typedef struct {                       /*AAH_LINES_3_4 packet*/
/*word 01..12 - CDU line 3*/
  unsigned   line3[24];                    /*line 3 of display*/
/*word 13..24 - CDU line 4*/
  unsigned   line4[24];                    /*line 4 of display*/
} AAH_LINES_3_4;


/*=== T08/16 CMS80 Display Control ========================================*/
typedef struct {                       /*CMS80_DISPLAY_CONTROL packet*/
/*word 01 - timetag*/
  unsigned   time_tag:8;                   /*timetag increments w/fresh data*/
  unsigned   slew_repeat:1;                /*repeat slew switch function if==1*/
/*word 02 - spare*/
  unsigned   _spare_:16;                   /*0*/
/*word 03 - data valid code*/
  unsigned   _spare_:8;                    /*0*/
  unsigned   buffer_valid:8;               /*ACh==buffer valid*/
} CMS80_DISPLAY_CONTROL;


/*=== T09/17 CMS80 Display Lines 1 and 2 ==================================*/
typedef struct {                       /*CMS80_LINES_1_2 packet*/
/*word 01 - usage/timetag for line 1*/
  unsigned   line1_time_tag:8;             /*timetag increments by 1 for new*/
  unsigned   line1_usage_code:8;           /*usage code, enum CMS80_USAGE_CODES*/
/*word 02 - line 1 control/count*/
  unsigned   line1_count:8;                /*count (usage modes 01 & 02)*/
  unsigned   line1_control_code:8;         /*control code, enum CONTROL_CODES*/
/*word 03 - usage/timetag for line 2*/
  unsigned   line2_time_tag:8;             /*timetag increments by 1 for new*/
  unsigned   line2_usage_code:8;           /*usage code, enum CMS80_USAGE_CODES*/
/*word 04 - line 1 control/count*/
  unsigned   line2_count:8;                /*count (usage modes 01 & 02)*/
  unsigned   line2_control_code:8;         /*control code, enum CONTROL_CODES*/
/*word 05..15 - 22 characters for display line 1*/
  unsigned   line1[11];                    /*chars, MSB=2,4..22, LSB=1,3..21*/
/*word 16..26 - 22 characters for display line 2*/
  unsigned   line2[11];                    /*chars, MSB=2,4..22, LSB=1,3..21*/
} CMS80_TWOLINES;


/*=== T10/18 CMS80 Display Lines 3 and 4 ==================================*/
typedef struct {                       /*CMS80_LINES_3_4 packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} CMS80_LINES_3_4;


/*=== T11/19 CMS80 Display Lines 5 and 6 ==================================*/
typedef struct {                       /*CMS80_LINES_5_6 packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} CMS80_LINES_5_6;


/*=== T12/20 CMS80 Display Lines 7 and 8 ==================================*/
typedef struct {                       /*CMS80_LINES_7_8_packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} CMS80_LINES_7_8;


/*=== T13 Transparent Packet Transmit =====================================*/
typedef struct {                       /*TRANSPARENT_TX_1553 packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} TRANSPARENT_TX_1553;


/*=== T15 Variable Length Message Receive =================================*/
typedef struct {                       /*VARIABLE_LENGTH_RX packet*/
/*word 01 - control*/
  unsigned   protocol:3;                   /*protocol of message*/
  unsigned   word_count:5;                 /*word count for this block*/
  unsigned   timetag:5;                    /*total number of blocks in message*/
  unsigned   _spare_:3;                    /*spare, 0*/
/*word 02 - block control*/
  unsigned   block:6;                      /*which block in series*/
  unsigned   last_block:1;                 /*1==last block in series*/
  unsigned   _spare_:1;                    /*spare, 0*/
  unsigned   message_id:4;                 /*message id*/
  unsigned   channel:2;                    /*channel on which message came in*/
  unsigned   _spare_:2;
/*word 03-32 - transmitted data...*/
  unsigned   data[30];                     /*up to 60 data bytes...*/
} VARIABLE_LENGTH_RX;


/*=== T21-24 Channels 1-4 Configuration ===================================*/
typedef struct {                       /*CHANNEL_CONFIGURATION packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} CHANNEL_CONFIGURATION;


/*=== T25-28 Receive Message Buffers 1-4 ==================================*/
typedef struct {                       /*RX_MESSAGE_BUFFER packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} RX_MESSAGE_BUFFER;


/*=== T29 Test Monitor ====================================================*/
typedef struct {                       /*TEST_MONITOR packet, FFh=true*/
/*word 01 - selftest bits*/
  unsigned   _spare_:1;                    /*0*/
  unsigned   avionics_controller:1;        /*A13*/
  unsigned   audio_switch_assembly:1;      /*A1*/
  unsigned   modem_analog_cc:1;            /*A7, cc="circuit card"*/
  unsigned   modem_digital_cc:1;           /*A6*/
  unsigned   r_w_memory_cc:1;              /*A11*/
  unsigned   program_memory_cc0:1;         /*A8*/
  unsigned   program_memory_cc1:1;         /*A10*/
  unsigned   program_memory_cc2:1;         /*A11*/
  unsigned   cpu_cc:1;                     /*A12*/
  unsigned   subsystem_interface_1553:1;   /*A5*/
  unsigned   encode_decode_1553:1;         /*A3*/
  unsigned   validation_1553:1;            /*A4*/
  unsigned   analog_cc:1;                  /*A2*/
  unsigned   bit_in_progress:1;            /*selftest in progress if=1*/
  unsigned   selftest_complete:1;          /*selftest complete, not on powerup*/
/*word 02 - modem monitor*/
  unsigned   net_busy:8;                   /*FFh=true,00h=false*/
  unsigned   net_busy_delay:8;             /*FFh=true*/
/*word 03 - modem monitor*/
  unsigned   preamble_detected:8;          /*FFh=true*/
  unsigned   voice_detected:8;             /*FFh=true*/
/*word 04 - modem monitor*/
  unsigned   crew_key_pressed:8;           /*FFh=true*/
  unsigned   transmitter_busy:8;           /*FFh=true*/
/*word 05 - modem monitor*/
  unsigned   receiver_busy:8;              /*FFh=true*/
  unsigned   modem_test_in_progress:8;     /*FFh=true*/
/*word 06 - modem monitor*/
  unsigned   tx_transfer_in_progress:8;    /*FFh=true*/
  unsigned   rx_transfer_in_progress:8;    /*FFh=true*/
/*word 07 - modem message control*/
  unsigned   message_time_tag:8;           /*increments on new message*/
  unsigned   rx_message_valid:8;           /*ACh=message received since powerup*/
/*word 08..31 - 48 characters with error flag*/
  unsigned   message_data[24];             /*LSB=2,4..48; MSB=1,3..47*/
                                       /*each byte's msb is rx error flag*/
} TEST_MONITOR;


/*=== T30 Bulk Data Out ===================================================*/
typedef struct {                       /*BULK_DATA_OUT packet*/
/*word 01 - spare*/
  unsigned   _spare_:16;                   /*entire word is spare*/
} BULK_DATA_OUT;


#endif                                 /*if this file not already included*/
