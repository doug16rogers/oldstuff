#include <stdiostr.h>

#include "templist.hpp"


class Object;

class Register;

typedef Sorted_List<Register*> Register_List;


class Register
{
   static Register_List* register_list;

public:

   Object* object;
   char* name;

public:

   Register(Object* object, const char* name)
   {
      this->object = object;
      this->name = name;
      if (register_list == NULL)
      {
         register_list = new Register_List(100, 50);
      }
      register_list->Insert(this);
   }

   virtual Object* Create_Object(ushort of_type);

};   // class Manager

static Register_List* Register::register_list;


int Sorted_List<Register*>:: Compare(
   Register* reg_1,
   Register* reg_2)
{
   return strcmp(reg_1->name, reg_2->name);
}   // Sorted_List




class Object
{
public:
   static const char* name;


   virtual Object* Create()
   {
      return this;
   }

   virtual const char* Name()
   {
      return Object::name;
   }

};   // class Object







const char* Object::name = "Object";


Register(Object, Object::name);
