#ifndef __attconst_hpp
#define __attconst_hpp

#ifndef __icitypes
#include "icitypes.h"
#endif

#ifndef __constrnt_hpp
#include "constrnt.hpp"
#endif

/*****************************************************************************
*
*  TITLE:       Attribute Constraint
*
*  DESCRIPTION: The class "Attribute_Constraint" constrains a file attribute
*               to a certain value.  The attribute being selected is a bit,
*               or set of bits.
*
*  *k "%n"
*  FILE NAME: ""
*
*  *k "%v"
*  VERSION:   ""
*
*****************************************************************************/

class Attribute_Constraint :

   public Constraint

{

//===========================================================================
//
//  Class types
//
//---------------------------------------------------------------------------

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

   UINT16 bits_to_check;        // which attribute bit(s) to check

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*****************************************************************************
*
*  TITLE:       Attribute Constraint
*
*  DESCRIPTION: The constructor "Attribute_Constraint:: Attribute_Constraint"
*               is the default constructor for an attribute constraint.  It
*               sets the bit to none.
*
*  REFERENCE:   None.
*
*****************************************************************************/

   Attribute_Constraint(void)
   {

      bits_to_check = 0x0000;

   }   //Attribute_Constraint:: Attribute_Constraint(void)

/*****************************************************************************
*
*  TITLE:       Attribute Constraint
*
*  DESCRIPTION: The constructor "Attribute_Constraint:: Attribute_Constraint"
*               sets the attribute bits to check to the ones provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

   Attribute_Constraint(

        UINT16 _bits_to_check,          // bits to check in file attribute
        boolean and_negate = false) :   // whether to negate or not

      Constraint(and_negate)    // pass negation on to base Constraint

   {

      bits_to_check = _bits_to_check;

   }   //Attribute_Constraint:: Attribute_Constraint(void)

/*****************************************************************************
*
*  TITLE:       Is Met
*
*  DESCRIPTION: The "Attribute_Constraint:: Is_Met" function checks the
*               atrributes dictated by the data member, then calls
*               Constraint's Is_Met() with the result.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

   virtual boolean Is_Met(

        File_Info* by_file)

   {

      if ((File_Info -> attributes & bits_to_check) != 0)
      {

         value = true;

      }

      return Constraint::Is_Met(by_file);

    }   //Attribute_Constraint:: Is_Met(File_Info* by_file)

};   //class n


#endif //__attconst_hpp

