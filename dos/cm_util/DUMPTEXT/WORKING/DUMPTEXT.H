#ifndef OSTREAM_DUMP_MANIPULATOR
#define OSTREAM_DUMP_MANIPULATOR

#include <iostream.h>


/*****************************************************************************
*
*  TITLE:        Dump Text
*
*  DESCRIPTION:  The "Dump_Text" function dumps strings to an ostream to be
*                displayed in both hex and ASCII.
*
*                The string in_front_of_each_line is printed using
*                standard printf() formatting; the index of the current
*                byte (always a multiple of 16) is passed.
*
*  REFERENCE:    None.
*
*****************************************************************************/

ostream& Dump_Text(
   ostream& out,
   void* data,
   int length,
   char* in_front_of_each_line = "%04x: ");


/*****************************************************************************
*
*  TITLE:        Dump Manipulator
*
*  DESCRIPTION:  The "Dump_Manipulator" class provides
*                the mechanism to get at the Dump_Text()
*                function from within a stream operator.
*
*  REFERENCE:    None.
*
*****************************************************************************/

class Dump_Manipulator
{
   void* data;
   int length;
   char* in_front;

public:

   Dump_Manipulator(void* _data, int _length, char* _in_front)
       : data(_data), length(_length), in_front(_in_front)
   {
   }

   friend ostream& operator << (ostream& os, const Dump_Manipulator& dump)
   {
     Dump_Text(os, dump.data, dump.length, dump.in_front);
     return os;
   }

};   // class Dump_Manipulator


/*****************************************************************************
*
*  TITLE:        Dump
*
*  DESCRIPTION:  The "Dump" function creates a
*                Dump_Manipulator object to call
*                Dump_Text().
*
*                The string in_front_of_each_line is printed using
*                standard printf() formatting; the index of the current
*                byte (always a multiple of 16) is passed.
*
*  REFERENCE:    None.
*
*****************************************************************************/

inline Dump_Manipulator Dump(
   void* data,
   int length,
   char* in_front = "%04x: ")
{
   return Dump_Manipulator(data, length, in_front);

}   // Dump


/*****************************************************************************
*
*  TITLE:        Dump Words Text
*
*  DESCRIPTION:  The "Dump_Words_Text" function dumps data words to an ostream
*                to be displayed in both hex and ASCII.
*
*  REFERENCE:    None.
*
*****************************************************************************/

ostream& Dump_Words_Text(
   ostream& out,
   void* data,
   int length,
   char* in_front_of_each_line = "%04x: ");


/*****************************************************************************
*
*  TITLE:        Dump Words Manipulator
*
*  DESCRIPTION:  The "Dump_Words_Manipulator" class provides
*                the mechanism to get at the Dump_Words_Text()
*                function from within a stream operator.
*
*  REFERENCE:    None.
*
*****************************************************************************/

class Dump_Words_Manipulator
{
   void* data;
   int length;
   char* in_front;

public:

   Dump_Words_Manipulator(void* _data, int _length, char* _in_front)
        : data(_data), length(_length), in_front(_in_front)
   {
   }

   friend ostream& operator << (ostream& os,
                                const Dump_Words_Manipulator& dump)
   {
      Dump_Words_Text(os, dump.data, dump.length, dump.in_front);
      return os;
   }

};   // class Dump_Words_Manipulator


/*****************************************************************************
*
*  TITLE:        Dump Words
*
*  DESCRIPTION:  The "Dump_Words" function creates a
*                Dump_Words_Manipulator object to call
*                Dump_Words_Text().
*
*  REFERENCE:    None.
*
*****************************************************************************/


inline Dump_Words_Manipulator Dump_Words(
   void* data,
   int length,
   char* in_front = "%04x: ")
{
   return Dump_Words_Manipulator(data, length, in_front);

}   // Dump_Words


/*****************************************************************************
*
*  TITLE:        Format
*
*  DESCRIPTION:  The "Format" function formats a string as printf() would.
*                It is meant to be included in streaming operations.
*
*  REFERENCE:    None.
*
*****************************************************************************/

const char* Format(
   char* format,
   ...);


#endif
