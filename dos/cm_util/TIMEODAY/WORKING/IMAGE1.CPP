/*****************************************************************************
*
*  TITLE:        Image
*
*  DESCRIPTION:  The subprogram "Image"
*                of class TIME_OF_DAY
*                writes a textual image of
*                the time to the given
*                buffer (or, if NULL is
*                supplied as the buffer,
*                to a local buffer)
*                according to the format specified
*                in the format string.
*
*                It returns the buffer written.
*
*
*                The format string contains '%'-sequences:
*
*                   %[-][0][<width>]<control>
*
*                The optional [-] causes left justification.
*                The optional [0] causes zero-padding of the field.
*                The optional [<width>] specifies the width of the field.
*                The <control> character may be:
*
*                   '%'          insert the '%' character
*                   'a' or 'p'   insert 'a'/'p' according to am/pm time
*                   'A' or 'P'   insert 'A'/'P' according to am/pm time
*                   'c'          insert the second count since 00:00:00
*                   'h'          insert the hour
*                   'H'          insert the hour according to am/pm time
*                   'm'          insert the minute
*                   'M'          insert the millisecond
*                   's'          insert the second
*
*                The default format string is "%02h:%02m:%02s".
*                An example to am/pm time is "%H:%02m:%02s %am".
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <ctype.h>
#include <stdio.h>

#include "timeoday.h"


char* TIME_OF_DAY::Image(       // buffer into which result was written

     char* into_buffer,         // buffer to hold the image
     char* with_format)         // NULL = last format used for object
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

   char* format = with_format;
   char* buffer = into_buffer;

   BOOLEAN left_justify = false;
   BOOLEAN zero_pad = false;
   static char* format_using[2] = { "%*u", "%0*u" };
   static char* long_format_using[2] = { "%*lu", "%0*lu" };

   int width = 0;

   while (*format != 0)
   {
      if (*format != '%')
      {
         *buffer++ = *format++;
         continue; //------------------------> continue
      }

      format++;

      left_justify = false;
      zero_pad = false;
      width = 0;

      if (*format == '-')
      {
         left_justify = true;
         format++;
      }

      if (*format == '0')
      {
         zero_pad = true;
         format++;
      }

      while (isdigit(*format))
      {
         width = (10 * width) + (*format - '0');
         format++;
      }

      if (left_justify)
      {
         width = -width;
      }

      switch (*format++)
      {
         case '%':
            *buffer++ = '%';
            break;

         case 'a':
         case 'p':
            if (hour < 12)
            {
               *buffer++ = 'a';
            }
            else
            {
               *buffer++ = 'p';
            }
            break;

         case 'A':
         case 'P':
            if (hour < 12)
            {
               *buffer++ = 'a';
            }
            else
            {
               *buffer++ = 'p';
            }
            break;

         case 'c':
            sprintf(buffer, long_format_using[zero_pad], width, Second_Count());
            while (*buffer) buffer++;
            break;

         case 'h':
            sprintf(buffer, format_using[zero_pad], width, (int) hour);
            while (*buffer) buffer++;
            break;

         case 'H':
            if (hour > 12)
            {
               sprintf(buffer, format_using[zero_pad], width, (int) hour - 12);
            }
            else
            {
               sprintf(buffer, format_using[zero_pad], width, (int) hour);
            }
            while (*buffer) buffer++;
            break;

         case 'm':
            sprintf(buffer, format_using[zero_pad], width, (int) minute);
            while (*buffer) buffer++;
            break;

         case 'M':
            sprintf(buffer, format_using[zero_pad], width, (int) millisecond);
            while (*buffer) buffer++;
            break;

         case 's':
            sprintf(buffer, format_using[zero_pad], width, (int) second);
            while (*buffer) buffer++;
            break;

        default:
            format--;
            *buffer++ = *format++;

      }   //switch

    }   //while

    *buffer = 0;

    return into_buffer;

}   // TIME_OF_DAY::Image

