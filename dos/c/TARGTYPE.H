#ifndef __TARGTYPE_H
#define __TARGTYPE_H

#define NOT_GVN 'O'                    /*standard subtype "not given" code*/


/*subtypes are listed after the target types and target strengths*/
enum TARGET_TYPES {                    /*ascii target types*/
  SPECIAL='0',                         /*special type*/
  PERSONEL='1',                        /*personnel*/
  WEAPON='2',                          /*weaponry*/
  MORTAR='3',                          /*mortar*/
  ARTY='4',                            /*artillery*/
  ARMOR='5',                           /*armor*/
  VEHICLE='6',                         /*vehicles*/
  RKTMSL='7',                          /*rockets/missiles*/
  SUPPLY='8',                          /*supply dumps*/
  CENTER='9',                          /*centers*/
  EQUIP='J',                           /*equipments*/
  BUILDING='K',                        /*buildings*/
  BRIDGE='L',                          /*bridges*/
  TERRAIN='M',                         /*terrain features*/
  ASSEMBLY='N',                        /*assembly areas*/
  ADA='O'                              /*air defenses artillery*/
};   /*target types*/


enum TARGET_STRENGTHS {                /*target/friendly strengths*/
  STRENGTH_NOT_GVN='0',                /*strength not given*/
  STRENGTH_1='1',                      /*1*/
  STRENGTH_2='2',                      /*2*/
  STRENGTH_3='3',                      /*3*/
  STRENGTH_4='4',                      /*4*/
  STRENGTH_5='5',                      /*5*/
  STRENGTH_6='6',                      /*6*/
  STRENGTH_7='7',                      /*7*/
  STRENGTH_8='8',                      /*8*/
  STRENGTH_9='9',                      /*9*/
  STRENGTH_10='J',                     /*10*/
  STRENGTH_25='K',                     /*25*/
  STRENGTH_50='L',                     /*50*/
  STRENGTH_100='M',                    /*100*/
  STRENGTH_200='N',                    /*200*/
  STRENGTH_500='O'                     /*500*/
};   /*target/friendly strengths*/


enum SPECIAL_TYPES {                   /*special subtypes*/
  SPECIAL_ON_CALL='0',                 /*on call*/
  SPECIAL_ILL1GUN='1',                 /*illum - 1 gun*/
  SPECIAL_ILL2GUN='2',                 /*illum - 2 gun*/
  SPECIAL_ILL2GUND='3',                /*illum - 2 gun deflection spread*/
  SPECIAL_ILL2GUNR='4',                /*illum - 2 gun range spread*/
  SPECIAL_ILL4GUN='5',                 /*illum - 4 gun*/
  SPECIAL_GAS_NONP='J',                /*gas-nonpersistent*/
  SPECIAL_GAS_PERS='K',                /*gas-persistent*/
  SPECIAL_LEAFLET='L',                 /*leaflets*/
  SPECIAL_NOT_GVN='O'                  /*subtype not given*/
};   /*special subtypes*/


enum PERSONEL_TYPES {                  /*personnel subtypes*/
  PERSONEL_UNKNOWN='0',                /*unknown personnel*/
  PERSONEL_INFANTRY='1',               /*infantry*/
  PERSONEL_OP='2',                     /*observation post*/
  PERSONEL_PATROL='3',                 /*patrol*/
  PERSONEL_WKPTY='4',                  /*work party*/
  PERSONEL_POSITION='5',               /*position*/
  PERSONEL_NOT_GVN='O'                 /*subtype not given*/
};   /*personnel subtypes*/


enum WEAPON_TYPES {                    /*weapons subtype*/
  WEAPON_UNKNOWN='0',                  /*unknown weapon*/
  WEAPON_LT_M_GUN='1',                 /*light machine gun*/
  WEAPON_AT_GUN='2',                   /*anti-tank gun*/
  WEAPON_HVY_GUN='3',                  /*heavy machine gun*/
  WEAPON_RCLR='4',                     /*recoilless rifle*/
  WEAPON_POSITION='5',                 /*position*/
  WEAPON_NOT_GVN='O'                   /*subtype not given*/
};   /*weapons subtypes*/


enum MORTAR_TYPES {                    /*mortar subtypes*/
  MORTAR_UNKNOWN='0',                  /*unknown mortar type*/
  MORTAR_LIGHT='1',                    /*light mortar*/
  MORTAR_MEDIUM='2',                   /*medium heavy mortar*/
  MORTAR_HEAVY='3',                    /*heavy*/
  MORTAR_VERY_HVY='4',                 /*very heavy*/
  MORTAR_POSITION='5',                 /*position*/
  MORTAR_NOT_GVN='O',                  /*subtype not given*/
};   /*mortar subtypes*/


enum ARTY_TYPES {                      /*artillery subtypes*/
  ARTY_UNKNOWN='0',                    /*unknown artillery*/
  ARTY_LIGHT='1',                      /*light artillery*/
  ARTY_MEDIUM='2',                     /*medium artillery*/
  ARTY_HEAVY='3',                      /*heavy artillery*/
  ARTY_NOT_GVN='O'                     /*subtype not given*/
};   /*artillery subtypes*/


enum ARMOR_TYPES {                     /*armor subtypes*/
  ARMOR_UNKNOWN='0',                   /*unknown armor*/
  ARMOR_LIGHT='1',                     /*light*/
  ARMOR_MEDIUM='2',                    /*medium*/
  ARMOR_HEAVY='3',                     /*heavy*/
  ARMOR_APC='4',                       /*armored personnel carrier*/
  ARMOR_POSITION='5',                  /*position*/
  ARMOR_NOT_GVN='O'                    /*subtype not given*/
};   /*armor subtypes*/


enum VEHICLE_TYPES {                   /*vehicle types*/
  VEHICLE_UNKNOWN='0',                 /*unknown vehicle*/
  VEHICLE_LT_WHEEL='1',                /*light wheeled*/
  VEHICLE_HVYWHEEL='2',                /*heavy wheeled*/
  VEHICLE_RECON='3',                   /*reconnaissance*/
  VEHICLE_BOAT='4',                    /*boats*/
  VEHICLE_AIRCRAFT='5',                /*aircraft*/
  VEHICLE_HELICPTR='6',                /*helicopter*/
  VEHICLE_NOT_GVN='O'                  /*subtype not given*/
};   /*vehicle subtypes*/


enum RKTMSL_TYPES {                    /*rocket/missile subtypes*/
  RKTMSL_UNKNOWN='0',                  /*unknown rocket/missile*/
  RKTMSL_APERS='1',                    /*anti-personnel*/
  RKTMSL_LT_MSL='2',                   /*light missile*/
  RKTMSL_MDM_MSL='3',                  /*medium missile*/
  RKTMSL_HVY_MSL='4',                  /*heavy missile*/
  RKTMSL_ANTITANK='5',                 /*anti-tank missile*/
  RKTMSL_POSITION='6',                 /*position*/
  RKTMSL_NOT_GVN='O'                   /*subtype not given*/
};   /*rocket/missile subtypes*/


enum SUPPLY_TYPES {                    /*supply dump subtypes*/
  SUPPLY_UNKNOWN='0',                  /*unknown*/
  SUPPLY_AMMO='1',                     /*ammunition*/
  SUPPLY_OIL='2',                      /*oil/petroleum (PTL)*/
/*  SUPPLY_PTL='2',*/                      /*petroleum*/
  SUPPLY_BRG_EQPT='4',                 /*bridging equipment*/
  SUPPLY_CLASS_I='5',                  /*class I*/
  SUPPLY_CLASS_II='6',                 /*class II*/
  SUPPLY_NOT_GVN='O'                   /*subtype not given*/
};   /*supply subtypes*/


enum CENTER_TYPES {                    /*centers subtypes*/
  CENTER_UNKNOWN='0',                  /*unknown center*/
  CENTER_SMALL='1',                    /*small center*/
  CENTER_BN='2',                       /*batallion*/
  CENTER_REGT='3',                     /*regiment*/
  CENTER_DIVISION='4',                 /*division*/
  CENTER_FORWARD='5',                  /*forward*/
  CENTER_NOT_GVN='O'                   /*subtype not given*/
};   /*centers subtypes*/


enum EQUIP_TYPES {                     /*equipment subtypes*/
  EQUIP_UNKNOWN='0',                   /*unknown equipment*/
  EQUIP_RADAR='1',                     /*radar*/
  EQUIP_EW='2',                        /*electronic warfare*/
  EQUIP_SLT='3',                       /*search light*/
  EQUIP_GUIDANCE='4',                  /*guidance*/
  EQUIP_LOUDSPKR='5',                  /*loudspeaker*/
  EQUIP_NOT_GVN='O'                    /*subtype not given*/
};   /*equipment subtypes*/


enum BUILDING_TYPES {                  /*building subtypes*/
  BUILDING_UNKNOWN='0',                /*unknown building*/
  BUILDING_WOODEN='1',                 /*wooden*/
  BUILDING_MASNRY='2',                 /*brick/stone*/
  BUILDING_CONCRETE='3',               /*concrete*/
  BUILDING_METAL='4',                  /*metal*/
  BUILDING_SPECIAL='5',                /*special-purpose building*/
  BUILDING_NOT_GVN='O'                 /*subtype not given*/
};   /*building subtypes*/


enum BRIDGE_TYPES {                    /*bridge subtypes*/
  BRIDGE_UNKNOWN='0',                  /*unknown bridge type*/
  BRIDGE_FOOT_PON='1',                 /*foot pontoon*/
  BRIDGE_VEH_PONT='2',                 /*vehicle pontoon*/
  BRIDGE_CONCRETE='3',                 /*concrete*/
  BRIDGE_WOODEN='4',                   /*wooden*/
  BRIDGE_STEEL='5',                    /*steel*/
  BRIDGE_SITE='6',                     /*site*/
  BRIDGE_RAFT='7',                     /*raft*/
  BRIDGE_FERRY='8',                    /*ferry*/
  BRIDGE_NOT_GVN='O'                   /*subtype not given*/
};   /*bridge subtypes*/


enum TERRAIN_TYPES {                   /*terrain feature subtypes*/
  TERRAIN_UNKNOWN='0',                 /*unknown terrain feature*/
  TERRAIN_ROAD='1',                    /*roadway*/
  TERRAIN_JUNCTION='2',                /*junction*/
  TERRAIN_HILL='3',                    /*hill*/
  TERRAIN_DEFILE='4',                  /*defile*/
  TERRAIN_LDG_STRP='5',                /*landing strip*/
  TERRAIN_RAILROAD='6',                /*railroad*/
  TERRAIN_NOT_GVN='O'                  /*subtype not given*/
};   /*terrain feature subtypes*/


enum ASSEMBLY_TYPES {                  /*assembly area subtypes*/
  ASSEMBLY_UNKNOWN='0',                /*unknown assembly area*/
  ASSEMBLY_TROOPS='1',                 /*troops*/
  ASSEMBLY_TRP_VEH='2',                /*troops and vehicles*/
  ASSEMBLY_TRP_MECH='3',               /*mechanized troops*/
  ASSEMBLY_TRP_ARM='4',                /*troops and armor*/
  ASSEMBLY_NOT_GVN='O'                 /*subtype not given*/
};   /*assembly area subtypes*/


enum ADA_TYPES {                       /*air defenses artillery subtypes*/
  ADA_UNKNOWN='0',                     /*unknown air defenses artillery*/
  ADA_LIGHT='1',                       /*light*/
  ADA_MEDIUM='2',                      /*medium*/
  ADA_HEAVY='3',                       /*heavy*/
  ADA_MISSILE='4',                     /*missile*/
  ADA_POSITION='5',                    /*position*/
  ADA_NOT_GVN='O'                      /*subtype not given*/
};   /*air defenses artillery subtypes*/


#endif                                 /*if file not already included*/
