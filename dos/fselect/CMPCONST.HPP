#ifndef __cmpconst_hpp
#define __cmpconst_hpp

#ifndef __types_hpp
#include "types.hpp"
#endif

#ifndef __constrnt_hpp
#include "constrnt.hpp"
#endif

/*****************************************************************************
*
*  TITLE:       Comparison Constraint Class
*
*  DESCRIPTION: The "Comparison_Constraint" class provides a means of
*               comparing objects which are related linearly; that is, one
*               object is or is not greater than or equal to another.
*
*  *k "%n"
*  FILE NAME: ""
*
*  *k "%v"
*  VERSION:   ""
*
*****************************************************************************/

class Comparison_Constraint :

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

public:

  boolean check_for_greater_than;
  boolean check_for_equality;

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*****************************************************************************
*
*  TITLE:       Comparison Constraint
*
*  DESCRIPTION: The "Comparison_Constraint:: Comparison_Constraint" default
*               constructor simply initializes to no checking.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Comparison_Constraint(void)

  {

    checking_for_greater_than = false;
    checking_for_equality = false;

  }   //Comparison_Constraint:: Comparison_Constraint(void)

/*****************************************************************************
*
*  TITLE:       Comparison Constraint
*
*  DESCRIPTION: The "Comparison_Constraint:: Comparison_Constraint"
*               constructor initializes its checking to the values given.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Comparison_Constraint(

    boolean check_for_greater,
    boolean check_for_equal,

    boolean and_negate = false) :

      Constraint(and_negate)

  {

    checking_for_greater_than = check_for_greater;
    checking_for_equality = check_for_equal;

  }   //Comparison_Constraint:: Comparison_Constraint(boolean,boolean,boolean)

/*****************************************************************************
*
*  TITLE:       Is Equal
*
*  DESCRIPTION: The "Comparison_Constraint:: Is_Equal" checks for an equality
*               condition on an object.  It is meant to be overridden in a
*               derived class.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

   virtual boolean Is_Equal(

     File_Info* by_file)

   {

     return false;

   }   //Comparison_Constraint:: Is_Equal(File_Info* by_file)

/*****************************************************************************
*
*  TITLE:       Is Greater
*
*  DESCRIPTION: The "Comparison_Constraint:: Is_Greater" checks for an object
*               being greater than the comparison constraint.  It is meant to
*               be overridden in a derived class.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

   virtual boolean Is_Greater(

     File_Info* by_file)

   {

     return false;

   }   //Comparison_Constraint:: Is_Greater(File_Info* by_file)

/*****************************************************************************
*
*  TITLE:       Is Met
*
*  DESCRIPTION: The "Comparison_Constraint:: Is_Met" function checks the
*               comparison functions as dictated by the data members, then
*               calls Constraint's Is_Met() with the result.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  virtual boolean Is_Met(

    File_Info* by_file)

  {

    if (checking_for_greater_than)
    {
      value = value || Is_Greater(by_file);
    }

    if (checking_for_equality)
    {
      value = value || Is_Equal(by_file);
    }

    return Constraint::Is_Met(by_file);

  }   //Comparison_Constraint:: Is_Met(File_Info* by_file)

};   //class Comparison_Constraint


#endif //__cmpconst_hpp

