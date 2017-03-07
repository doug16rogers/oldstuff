#include <ctype.h>
#include <stdiostr.h>
#include <fstream.h>
#include <string.h>

#include "w:\rogers\cpp\util\templist.hpp"


#define MAP_FILE  "gip.map"
#define SYM_FILE  "gip.sym"


char map_file[0x100] = MAP_FILE;
char sym_file[0x100] = SYM_FILE;

ifstream* map_stream;
ifstream* sym_stream;


char* New_String(
     char* to_copy)
{
   if (to_copy == NULL)
   {
      return NULL; //-----------------------------------------> return!
   }

   char* copy = new char[strlen(to_copy) + 1];

   if (copy == NULL)
   {
      return NULL; //-----------------------------------------> return!
   }

   strcpy(copy, to_copy);

   return copy;

}   // New_String




class Item
{
public:
   char* unit;
   char* name;
   unsigned long address;

public:

   Item(
      char* unit,
      char* name)
   {
      this->unit = NULL;
      this->name = NULL;

      if ((unit != NULL) && (unit[0] != 0))
      {
         this->unit = New_String(unit);
         if (this->unit != NULL)
         {
           strlwr(this->unit);
         }
      }

      if ((name != NULL) && (name[0] != 0))
      {
         this->name = New_String(name);
         if (this->name != NULL)
         {
           strlwr(this->name);
         }
      }
   }

public:

   ~Item()
   {
      delete[] unit;
      delete[] name;
   }

};   // class Item


int Sorted_List<Item*>:: Compare(
     Item* item_1,
     Item* item_2)
{
     // if exactly one of them is a wildcard, they're different

   if ((item_1->name == NULL) ^ (item_2->name == NULL))
   {
      if (item_1->name == NULL)
      {
         return -1; //----------------------------------------> return!
      }
      else
      {
         return +1; //----------------------------------------> return!
      }
   }

   int comparison = 0;

   if (item_1->name != NULL)
   {
      comparison = strcmp(item_1->name, item_2->name);
      if (comparison != 0)
      {
         return comparison; //--------------------------------> return!
      }
   }

   if ((item_1->unit == NULL) ^ (item_2->unit == NULL))
   {
      if (item_1->unit == NULL)
      {
         return -1; //----------------------------------------> return!
      }
      else
      {
         return +1; //----------------------------------------> return!
      }
   }

   if (item_1->unit != NULL)
   {
      comparison = stricmp(item_1->unit, item_2->unit);

      if (comparison != 0)
      {
         return comparison; //--------------------------------> return!
      }
   }

   return 0;

}   // Sorted_List<Item*>:: Compare


class Item_List : public Sorted_List<Item*>
{
public:

   Item_List(
        int start,
        int delta) :
        Sorted_List<Item*>(start, delta)
   {
   }

public:

   Item* Match(
        char* unit,
        char* name);

};   // class Item_List



int Compare_For_Match(
     Item* item,
     char* unit,
     char* name)
{
   int comparison;

   if (item->name != NULL)
   {
      int i = 0;

      while (name[i] != 0)
      {
         if (name[i] != item->name[i])
         {
            if (i < 15)         // map file may have truncated name
            {
               return (name[i] - item->name[i]); //------------> return!
            }
            else
            {
               break;
            }
         }
         i++;
      }
   }
   else
   {
      if (strcmp(name, "constagg") == 0)
      {
         return +1;
      }
   }

   if (item->unit != NULL)
   {
      comparison = strcmp(unit, item->unit);
      if (comparison != 0)
      {
         return comparison; //--------------------------------> return!
      }
   }

   return 0;

}   // Compare_For_Match


Item* Item_List :: Match(
     char* unit,
     char* name)
{
   strlwr(unit);
   strlwr(name);

   for (int i = 0; i < count; i++)
   {
      if (Compare_For_Match(base[i], unit, name) == 0)
      {
         return base[i];
      }
   }

   return NULL;

/*
   if (count == 0) return 0;

   int bottom = 0;
   int middle = count / 2;
   int top = count;
   int result;

   strlwr(unit);
   strlwr(name);

   do
   {
      result = Compare_For_Match(base[middle], unit, name);

      if (result == 0) break;

      if (result > 0)   // object > middle object
      {
         bottom = middle + 1;
      }
      else
      {
         top = middle;
      }
      middle = (bottom + top) / 2;
   } while (bottom < top);

   if (result == 0)
   {
      return base[middle];
   }
   else
   {
      return NULL;
   }
*/
}   // Item_List :: Match



Item_List* item_list = new Item_List(100, 50);



int  Get_Map_Item(
     char* s,
     char* unit,
     char* item,
     unsigned long* address,
     unsigned long* length)
{
   *unit = 0;
   *item = 0;
   *address = 0UL;
   *length = 0UL;

   if ((s[0] < 'A') || (s[0] > 'Z'))
   {
      return 0; //--------------------------------------------> return!
   }

   int i = 0;
   while ( ((s[i] >= 'A') && (s[i] <= 'Z')) ||
           (s[i] == '_') )
   {
      unit[i] = s[i];
      i++;
   }
   unit[i] = 0;
   int unit_length = i;

   char* dollar = strchr(s, '$');

   if (dollar == NULL)
   {
      return 0; //--------------------------------------------> return!
   }

   dollar += 6;         // skips "$_Aaaa"

   if (unit_length < 8)
   {
      dollar += unit_length;
   }
   else
   {
      dollar += 8;
   }

   i = 0;
   while (*dollar != ' ')
   {
      item[i] = *dollar++;
      i++;
   }
   item[i] = 0;

     // remove trailing "###"

   if (i > 3)
   {
     i -= 3;
     item[i] = 0;
   }

     // remove trailing "_"

   if ((i > 1) && (item[i-1] == '_'))
   {
     i -= 1;
     item[i] = 0;
   }


   while (*dollar && (isspace(*dollar)))
   {
      dollar++;
   }

   sscanf(dollar, "%lx %lx", address, length);

   return 1;

}   // Get_Map_Item


int  Get_Requested_Item(
     char* s,
     char* unit,
     char* item)
{
   *unit = 0;
   *item = 0;

   while (*s && isspace(*s))
   {
      s++;
   }

   if (*s == 0)
   {
      return 0; //--------------------------------------------> return!
   }

   char* dot = strchr(s, '.');

   if (dot == NULL)
   {
      while (*s && !isspace(*s))
      {
         *item++ = *s++;
      }
      *item = 0;
   }
   else
   {
      *dot = 0;
      strcpy(unit, s);
      dot++;
      while (*dot && !isspace(*dot))
      {
         *item++ = *dot++;
      }
      *item = 0;
   }

   return 1;

}   // Get_Requested_Item



int main(
     int argument_count,
     char* argument_list[])
{
   char line[0x100];
   char unit_name[0x100];
   char item_name[0x100];
   unsigned long address;
   unsigned long length;

   cout << "/""* listsym <map-file> <symbol-file> "
        << "-- defaults: gip.map gip.sym *""/"
        << endl;

   if (argument_count > 1)
   {
      strcpy(map_file, argument_list[1]);
   }

   if (argument_count > 2)
   {
      strcpy(sym_file, argument_list[2]);
   }


   map_stream = new ifstream(map_file);

   if (!*map_stream)
   {
      cout << "  /* could not open map file " << map_file << " */" << endl;
      return 1;
   }

   sym_stream = new ifstream(sym_file);

   if (!*sym_stream)
   {
      cout << "  /* could not open symbol file " << sym_file << " */" << endl;
      return 2;
   }

   while (*sym_stream && !sym_stream->eof())
   {
      sym_stream->getline(line, sizeof(line));
      if (Get_Requested_Item(line, unit_name, item_name))
      {
         Item* item = new Item(unit_name, item_name);
         if (!item_list->Insert(item))
         {
            delete item;
         }
      }
   }

   sym_stream->close();

/*
   for (int i = 0; i < item_list->Count(); i++)
   {
      cout << "/""* " << i << ": ";
      cout << "unit=\"" << item_list->At(i)->unit << "\" ";
      cout << "name=\"" << item_list->At(i)->name << "\"";
      cout << " *""/";
      cout << endl;
   }
*/

   while (*map_stream && !map_stream->eof())
   {
      map_stream->getline(line, sizeof(line));

      if (Get_Map_Item(line, unit_name, item_name, &address, &length))
      {
         Item* item = item_list->Match(unit_name, item_name);
         if (item != NULL)
         {
            cout << "redefine ord4 ";
            if (item->unit != NULL)
            {
               cout << item->unit;
            }
            else
            {
               cout << unit_name;
            }

            cout << "__";

            if (item->name != NULL)
            {
               cout << item->name;
            }
            else
            {
               cout << item_name;
            }
            cout << " = " << hex << "0" << address;
            cout << "  /""* length=0" << hex << length << " *""/";
            cout << endl;
         }
      }
   }   // while

   map_stream->close();

   return 0;

}   // main

