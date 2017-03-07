#ifndef __constrnt_hpp
#define __constrnt_hpp

#ifndef __icitypes
#include "icitypes.h"
#endif

#ifndef __fileinfo_hpp
#include "fileinfo.hpp"
#endif

/*****************************************************************************
*
*  TITLE:       Constraint Class
*
*  DESCRIPTION: The class "Constraint" provides a base constraint type which
*               may be derived from.
*
*  *k "%n"
*  FILE NAME: ""
*
*  *k "%v"
*  VERSION:   ""
*
*****************************************************************************/

class Constraint
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

private:

   BOOLEAN negated;     //set to true when the constraint is to be negated

public:

   BOOLEAN value;       //value so far calculated by the constraint

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*****************************************************************************
*
*  TITLE:       Constraint
*
*  DESCRIPTION: The default constructor "Constraint:: Constraint" simply
*               resets its value to false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

   Constraint(void)
   {

      negated = false;
      value = false;

   }   //Constraint:: Constraint(void)

/*****************************************************************************
*
*  TITLE:       Constraint
*
*  DESCRIPTION: The constructor "Constraint:: Constraint" loads an initial
*               value into the constraint.
*
*  REFERENCE:   None.
*
*****************************************************************************/

   Constraint(

        BOOLEAN initial_value)          //initial value of constraint

   {

      negated = false;
      value = initial_value;

   }   //Constraint:: Constraint(BOOLEAN initial_value)


/*****************************************************************************
*
*  TITLE:       Is_Met
*
*  DESCRIPTION: The "Constraint:: Is_Met" function returns the BOOLEAN true if
*               the given constraint is non-zero, and false if it is zero.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

   virtual BOOLEAN Is_Met(

        File_Info* by_file)     //item to check against this constraint

   {

      if (negated)
      {
         return value == false;
      }
      else
      {
         return value != false;
      }

   }   //Constraint:: Is_Met(File_Info* by_file)

/*****************************************************************************
*
*  TITLE:       Negate
*
*  DESCRIPTION: The "Constraint:: Negate" procedure tells the constraint to
*               negate its calculated value when it is returned by Is_Met().
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

   virtual void Negate(void)
   {

      negated = !negated;

   }

};   //class Constraint


#endif //__constrnt_hpp

