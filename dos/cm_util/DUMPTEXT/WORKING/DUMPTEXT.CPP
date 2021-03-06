#include <ctype.h>
#include <stdarg.h>
#include <stdiostr.h>
#include <string.h>

#include "dumptext.h"

static char hex_table[16] = "0123456789ABCDEF";


/*****************************************************************************
*
*  TITLE:        Hex Byte String
*
*  DESCRIPTION:  The subprogram "Hex_Byte_String"
*                dumps (only) up to 16 bytes into a string
*                in hexadecimal format.
*
*                It returns a pointer to the resulting
*                ASCIIZ string.
*
*  REFERENCE:    None.
*
*****************************************************************************/

char* Hex_Byte_String(
   void* data,
   int length)
{
   static char text[72];
   char* s;
   char* a;
   unsigned char* input = (unsigned char*) data;

   int i;

   if (length > 16)
   {
      length = 16;
   }

   strcpy(text, "-- -- -- --  -- -- -- --  -- -- -- --  -- -- -- --  "
                "|................|");
   s = text;
   a = &text[53];

   for (i = 0; i < length; i++)
   {
      *s++ = hex_table[*input >> 4];
      *s++ = hex_table[*input & 0x0f];

      if (isprint(*input))
      {
         *a = *input;
      }

      input++;
      a++;
      s++;
      if ((i & 0x03) == 3)
      {
         s++;
      }
   }

   return text;

}   // Hex_Byte_String

/*****************************************************************************
*
*  TITLE:        Hex Word String
*
*  DESCRIPTION:  The subprogram "Hex_Word_String"
*                dumps (only) up to 8 16-bit words into a string
*                in hexadecimal format.
*
*                It returns a pointer to the resulting
*                ASCIIZ string.
*
*  REFERENCE:    None.
*
*****************************************************************************/

char* Hex_Word_String(
   void* data,
   int length)
{
   static char text[66];
   char* s;
   char* a;
   unsigned int* input = (unsigned int*) data;

   int i;

   if (length > 8)
   {
      length = 8;
   }

   strcpy(text, "---- ---- ---- ----  ---- ---- ---- ----  "
                "|.. .. .. .. .. .. .. ..|");
   s = text;
   a = &text[43];

   for (i = 0; i < length; i++)
   {
      *s++ = hex_table[(*input >> 12) & 0x0f];
      *s++ = hex_table[(*input >> 8) & 0x0f];
      *s++ = hex_table[(*input >> 4) & 0x0f];
      *s++ = hex_table[*input & 0x0f];

      if (isprint(*input >> 8))
      {
         *a = (*input >> 8);
      }
      a++;

      if (isprint(*input & 0xff))
      {
         *a = *input;
      }
      a++;
      a++;

      input++;
      s++;
      if ((i & 0x03) == 3)
      {
         s++;
      }
   }

   return text;

}   // Hex_Word_String

/*****************************************************************************
*
*  TITLE:        Dump Text
*
*  DESCRIPTION:  The "Dump_Text" function dumps strings to an ostream to be
*                displayed in both hex and ASCII.
*
*  REFERENCE:    None.
*
*****************************************************************************/

ostream& Dump_Text(ostream& out,
                   void* data,
                   int length,
                   char* in_front_of_each_line)
{
   static char front_text[0x40];
   int front_length;

   for (int i = 0; i < length; i+= 16)
   {
      if (in_front_of_each_line != NULL)
      {
         front_length = sprintf(front_text, in_front_of_each_line, i, i);
         out.write(front_text, front_length);
      }
      out.write(Hex_Byte_String(((char*)data) + i, length - i), 71);
   }

   return out;

}   //Dump_Text(ostream&,char*,int[,char*])


/*****************************************************************************
*
*  TITLE:        Dump Words
*
*  DESCRIPTION:  The "Dump_Words" function dumps data words to an ostream to
*                be displayed in both hex and ASCII.
*
*  REFERENCE:    None.
*
*****************************************************************************/

ostream& Dump_Words_Text(ostream& out,
                         void* data,
                         int length,
                         char* in_front_of_each_line)
{
   static char front_text[0x20];

   for (int i = 0; i < length; i+= 8)
   {
      if (in_front_of_each_line != NULL)
      {
         sprintf(front_text, in_front_of_each_line, i, i);
         out << front_text;
      }
      out << Hex_Word_String(((unsigned int*)data) + i, length - i) << '\n';
   }

   return out;

}   //Dump_Words_Text(ostream&,char*,int[,char*])


/*****************************************************************************
*
*  TITLE:        Format
*
*  DESCRIPTION:  The "Format" function formats a string as printf() would.  It
*                is meant to be included in streaming operations.
*
*  REFERENCE:    None.
*
*****************************************************************************/

const char* Format(char* format, ...)
{
   static char temp[0x100];
   va_list va;

   va_start(va, format);
   vsprintf(temp, format, va);
   va_end(va);

   return temp;

}   //Format

