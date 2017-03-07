#ifndef __class_String
#define __class_String

#include <iostream.h>
#include <string.h>


#define STRING_SIZE_DELTA       10


class String
{
protected:

   static int delta;

public:

   int length;

protected:

   int size;

public:

   char* text;

public:

   String()
   {
      length = 0;
      size = 0;
      text = NULL;
   }

public:

   String(
      int size)
   {
      length = 0;
      text = new char[size];
      if (text != NULL)
      {
         text[0] = 0;
         this->size = size;
      }
      else
      {
         this->size = 0;
      }
   }

public:

   String(
      const char* string,
      int size = 0)
   {
      if (string != NULL)
      {
         if (size <= 0)
         {
            size = strlen(string) + 1;
         }
         text = new char[size];
         if (text != NULL)
         {
            length = size - 1;
            strncpy(text, string, length);
            text[length] = 0;
         }
         else
         {
            length = 0;
            size = 0;
         }
      }
      else
      {
         length = 0;
         size = 0;
         text = NULL;
      }
   }

public:

   virtual ~String()
   {
      delete[] text;
   }

public:

   operator int (void)
   {
      return length;
   }

public:

   operator char * (void)
   {
      return text;
   }

public:

   void Upper(void)
   {
      if (text != NULL)
      {
         strupr(text);
      }
   }

public:

   void Lower(void)
   {
      if (text != NULL)
      {
        strlwr(text);
      }
   }

public:

   String Extract(
      int first,
      int count)
   {
      String string;
      if ( (first >= 0) &&
           (first <= length))
      {
         if (count > (size - length))
         {
            count = size - length;
         }
         string.text = new char[count + 1];
         if (string.text != NULL)
         {
            string.size = count + 1;
            string.length = count;
            memcpy(string.text, &text[first], count);
            string.text[length] = 0;
         }
      }
      return string;
   }

public:

  String& operator -- (void)
  {
      if (length > 0)
      {
         length--;
         text[length] = 0;
      }
      return *this;
  }

public:

   String& operator ++ (void)
   {
      if (length < (size - 1))
      {
         char* new_text = new char [size + STRING_SIZE_DELTA];
         if (new_text != NULL)
         {
            strcpy(new_text, text);
            delete[] text;
            size += STRING_SIZE_DELTA;
            text = new_text;
            text[length] = ' ';
            length++;
            text[length] = 0;
         }
      }
      return *this;
   }

public:

   char& operator [] (
      int index)
   {
      if ( (index < 0) ||
           (index > length))
      {
         index = 0;
      }
      return text[index];

   }   // String :: operator [] (int)


public:

   void operator = (
      const String& string)
   {
      char* new_text = new char[string.length + 1];
      if (new_text != NULL)
      {
         size = string.length + 1;
         length = string.length;
         strcpy(new_text, string.text);
         delete[] text;
         text = new_text;
      }
   }

public:

   void operator = (const char* s)
   {
      int new_length = strlen(s);
      char* new_text = new char[new_length + 1];
      if (new_text != NULL)
      {
         size = new_length + 1;
         length = new_length;
         strcpy(new_text, s);
         delete[] text;
         text = new_text;
      }
   }

public:

   void operator = (const char c)
   {
      if (size < 2)
      {
         char* new_text = new char[2];
         if (new_text != NULL)
         {
            size = 2;
            length = 1;
            new_text[0] = c;
            new_text[1] = 0;
            delete[] text;
            text = new_text;
         }
      }
      else
      {
         text[0] = c;
         text[1] = 0;
         length = 1;
      }
   }

public:

   String operator + (char* s)
   {
      int s_length = strlen(s);
      String string(length + s_length + 1);

      if (string.size > 0)
      {
         strcpy(string.text, text);
         strcpy(&string.text[length], s);
         string.length = string.size - 1;
      }
      return string;
   }

public:

   String operator + (char c)
   {
      String string(length + 1 + 1);
      if (string.size > 0)
      {
         strcpy(string.text, text);
         string.text[length] = c;
         string.length++;
         string.text[length] = 0;
      }
      return string;
   }

public:

   String operator + (const String& s)
   {
      String string(length + s.length + 1);
      if (string.size > 0)
      {
         strcpy(string.text, text);
         strcpy(&string.text[length], s.text);
         string.length = string.size - 1;
      }
      return string;
   }

public:

   void operator += (const String& string)
   {
      if ((length + string.length) < (size - 1))
      {
         char* new_text = new char[length + string.length + 1];
         if (new_text != NULL)
         {
            strcpy(new_text, text);
            strcpy(&new_text[length], string.text);
            length += string.length;
            size = length + 1;
            delete[] text;
            text = new_text;
         }
      }
      else
      {
         strcpy(&text[length], string.text);
         length += string.length;
      }
   }

public:

   void operator += (const char* s)
   {
      int s_length = strlen(s);
      if ((length + s_length) < (size - 1))
      {
         char* new_text = new char[length + s_length + 1];
         if (new_text != NULL)
         {
            strcpy(new_text, text);
            strcpy(&new_text[length], s);
            length += s_length;
            size = length + 1;
            delete[] text;
            text = new_text;
         }
      }
      else
      {
         strcpy(&text[length], s);
         length += s_length;
      }
   }

public:

   void operator += (const char c)
   {
      int initial_length = length;
      ++(*this);
      if (length > initial_length)
      {
         text[initial_length] = c;
      }
   }

public:

   int operator <  (String& string) { return strcmp(text, string.text) <  0; }
   int operator == (String& string) { return strcmp(text, string.text) == 0; }
   int operator >  (String& string) { return strcmp(text, string.text) >  0; }
   int operator >= (String& string) { return strcmp(text, string.text) >= 0; }
   int operator != (String& string) { return strcmp(text, string.text) != 0; }
   int operator <= (String& string) { return strcmp(text, string.text) <= 0; }

   int operator <= (char* s) { return strcmp(text, s) <= 0; }
   int operator <  (char* s) { return strcmp(text, s) <  0; }
   int operator == (char* s) { return strcmp(text, s) == 0; }
   int operator >  (char* s) { return strcmp(text, s) >  0; }
   int operator >= (char* s) { return strcmp(text, s) >= 0; }
   int operator != (char* s) { return strcmp(text, s) != 0; }

   friend int operator <  (char* s, String& string)
   {
      return strcmp(s, string.text) < 0;
   }

   friend int operator == (char* s, String& string)
   {
      return strcmp(s, string.text) == 0;
   }

   friend int operator >  (char* s, String& string)
   {
      return strcmp(s, string.text)  > 0;
   }

   friend int operator >= (char* s, String& string)
   {
      return strcmp(s, string.text) >= 0;
   }

   friend int operator != (char* s, String& string)
   {
      return strcmp(s, string.text) != 0;
   }

   friend int operator <= (char* s, String& string)
   {
      return strcmp(s, string.text) <= 0;
   }

   friend ostream& operator << (ostream& o,String& s);
   friend String&  operator >> (istream& i,String& s);

};   //class String


#endif

