#include <stdiostr.h>


class Object
{
public:

   Object* copy;
   int copy_count;

   Object()
   {
      copy_count = 0;
      object_copy = NULL;
   }

   Object(Object& object)
   {
      copy = &object;
      object.copy_count++;
   }

   Object(Object* object)
   {
      object_copy = object;
      object->copy_count++;
   }

   virtual void Copy(Object& object)
   {
      if (object.copy != NULL)
      {
         copy = object.copy;
         copy->copy_count++;
      }
      else
      {
         copy = &object;
         object.copy_count++;
      }
   }

   virtual void Copy_Deep(Object& object)
   {
      copy = NULL;
      copy_count = 0;
   }

};   // class Object
