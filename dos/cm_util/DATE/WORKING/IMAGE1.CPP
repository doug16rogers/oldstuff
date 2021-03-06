/*****************************************************************************
*
*  TITLE:        Image
*
*  DESCRIPTION:  The subprogram "DATE::Image"
*                in the class DATE
*                writes an image of the object
*                date to the given string (or
*                to a local buffer) according
*                to the provided or default
*                image format specifier.
*
*                The format string contains '%'-sequences:
*
*                   %[-][0][<width>][u/l]<control>
*
*                The optional [-] causes left justification.
*                The optional [0] causes zero-padding of the field.
*                The optional [<width>] specifies the width of the field.
*                The optional [u/l] forces strings to upper/lower case.
*                The <control> character may be:
*
*                   '%'  insert the '%' character
*                   'd'  insert day of month
*                   'j'  insert Julian day (day of year)
*                   'm'  insert month as a number
*                   'M'  insert month as a string
*                   'w'  insert day of week as a number (0-6)
*                   'W'  insert day of week as a string
*                   'y'  insert full year
*                   'Y'  insert year modulo 100 (without century)
*
*                The default format string is "%04y.%02m.%02d".
*                For strings use "%W %M %d, %y".
*
*  *k "%n"
*  FILE NAME:    "IMAGE1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <string.h>


#ifndef __DATE_CLASS
#include "date.h"
#endif


/*****************************************************************************
*
*  TITLE:        Write String To Output
*
*  DESCRIPTION:  The procedure "Write_String_To_Output"
*                writes the given string
*                to the given output using the
*                supplied width, and forcing the
*                case as specified.
*                The output string (pointer) is adjusted to
*                the next character.
*
*  REFERENCE:    None.
*
*****************************************************************************/

static void Write_String_To_Output(

     char** output,             // output string pointer
     const char* string_to_write,       // string to write to output
     int width,                 // desired width (negative for left-justify)
     BOOLEAN to_upper_case,     // force string to upper case
     BOOLEAN to_lower_case);    // force string to lower case



/*****************************************************************************
*
*  TITLE:       Image
*
*****************************************************************************/

char* DATE::Image(              // buffer as passed in

     char* into_buffer,         // buffer for image (NULL = local)
     const char* with_format)   // format string, NULL means last_format

{
   if (into_buffer == NULL)
   {
      into_buffer = local_image;
   }

   if (with_format == NULL)
   {
      with_format = last_format;
      if (with_format == NULL)
      {
        with_format = default_format;
      }
   }

   last_format = with_format;

   char* output = into_buffer;         //our copy of output buffer

   BOOLEAN left_justify = false;
   BOOLEAN zero_pad = false;
   BOOLEAN force_upper_case = false;
   BOOLEAN force_lower_case = false;
   static char* format_using[2] = { "%*u", "%0*u" };

   int width = 0;

   while (*with_format)
   {
      left_justify = false;
      zero_pad = false;
      force_upper_case = false;
      force_lower_case = false;
      width = 0;

      if (*with_format != '%')
      {
         *output++ = *with_format++;
         continue; //-----------------------------------------> continue!
      }
      with_format++;

      if (*with_format == '-')
      {
         left_justify = true;
         with_format++;
      }

      if (*with_format == '0')
      {
         zero_pad = true;
         with_format++;
      }

      while (isdigit(*with_format))
      {
         width = (10 * width) + (*with_format - '0');
         with_format++;
      }

      if (left_justify)
      {
         width = -width;
      }

      if (*with_format == 'u')
      {
         force_upper_case = true;
         with_format++;
      }
      else if (*with_format == 'l')
      {
         force_lower_case = true;
         with_format++;
      }

      switch (*with_format++)
      {
         case '%':
            *output++ = '%';
            break;

         case 'd':
            sprintf(output, format_using[zero_pad], width, (int) day);
            while (*output) output++;
            break;

         case 'j':
            sprintf(output, format_using[zero_pad], width, Julian_Day());
            while (*output) output++;
            break;

         case 'm':
            sprintf(output, format_using[zero_pad], width, (int) month);
            while (*output) output++;
            break;

         case 'M':
            Write_String_To_Output(&output, name_of_month[month], width,
                                   force_upper_case, force_lower_case);
            break;

         case 'w':
            sprintf(output, format_using[zero_pad], width, (int) Day_Of_Week());
            while (*output) output++;
            break;

         case 'W':
            Write_String_To_Output(&output, name_of_day[Day_Of_Week()], width,
                                   force_upper_case, force_lower_case);
            break;

         case 'y':
            sprintf(output, format_using[zero_pad], width, year);
            while (*output) output++;
            break;

         case 'Y':
            sprintf(output, format_using[zero_pad], width, year % 100);
            while (*output) output++;
            break;

         default:
            with_format--;
            *output++ = *with_format++;
            break;

      }   // switch

   }   // while

   *output = 0;                 // terminate output string

   return into_buffer;         //return output string

}   // DATE::Image


/*****************************************************************************
*
*  TITLE:       Write String To Output
*
*****************************************************************************/

static void Write_String_To_Output(

     char** output,             // output string pointer
     const char* string_to_write,       // string to write to output
     int width,                 // desired width (negative for left-justify)
     BOOLEAN to_upper_case,     // force string to uppercase
     BOOLEAN to_lower_case)     // force string to lowercase

{
   sprintf(*output, "%*s", width, string_to_write);

   if (to_upper_case)
   {
      strupr(*output);
   }
   else if (to_lower_case)
   {
      strlwr(*output);
   }

   if (width == 0)
   {
      while (*(*output)) (*output)++;
   }
   else
   {
      *output += (width < 0) ? -width : +width;
   }
}   // Write_String_To_Output


