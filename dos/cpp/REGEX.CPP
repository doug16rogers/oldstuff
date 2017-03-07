#include <alloc.h>
#include <ctype.h>
#include <stdiostr.h>

#include "drstr.hpp"
#include "string2.hpp"
#include "templist.hpp"



#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef ulong
#define ulong unsigned long
#endif



typedef Pointer_List<String> String_List;


typedef uchar boolean;

const uchar false = 0;
const uchar true = 1;


char lower_case(char c)
{
   if ((c >= 'A') && (c <= 'Z')) c += ('a' - 'A');
   return c;
}   // lower_case()


//===========================================================================

class Expression
{
public:

   static boolean case_sensitive;

public:

   Expression()
   {
   }

public:

   virtual ~Expression()
   {
   }

public:

   virtual boolean Accepts(
        char character)
   {
      return false;
   }

public:

   virtual boolean Is_Complete()
   {
      return false;
   }

public:

   virtual void Reset()
   {
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "<undefined>";
      return out;
   }

};   // class Expression


boolean Expression :: case_sensitive = true;

//===========================================================================

class Single_Character : public Expression
{
protected:

   char character;

   boolean is_complete;

public:

   Single_Character(char character)
   {
      this->character = character;
      is_complete = false;
   }

public:

   virtual ~Single_Character()
   {
   }

public:

   virtual boolean Accepts(
        char character)
   {
      boolean acceptable = false;

      if (!is_complete)
      {
         if (Expression::case_sensitive)
         {
            acceptable = (character == this->character);
         }
         else
         {
            acceptable = lower_case(character) == lower_case(this->character);
         }

         if (acceptable)
         {
            is_complete = true;
         }
      }

      return acceptable;
   }

public:

   virtual boolean Is_Complete()
   {
      return is_complete;
   }

public:

   virtual void Reset()
   {
      is_complete = false;
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Single Character '" << character << "'";
      return out;
   }

};   // class Single_Character


//===========================================================================

class Any_Character : public Expression
{
protected:

   boolean is_complete;

public:

   Any_Character()
   {
      is_complete = false;
   }

public:

   virtual ~Any_Character()
   {
   }

public:

   virtual boolean Accepts(
        char character)
   {
      is_complete = true;
      return true;
   }

public:

   virtual boolean Is_Complete()
   {
      return is_complete;
   }

public:

   virtual void Reset()
   {
      is_complete = false;
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Any Character";
      return out;
   }

};   // class Any_Character


//===========================================================================

class Character_Range : public Expression
{
protected:

   char first;
   char last;

   boolean is_complete;

protected:

   boolean is_in(
      char the_character,
      char first_in_group,
      char last_in_group)
   {
      return (the_character >= first_in_group) &&
             (the_character <= last_in_group);
   }

public:

   Character_Range(char first,
                   char last)
   {
      this->first = first;
      this->last = last;
      is_complete = false;
   }

public:

   virtual ~Character_Range()
   {
   }

public:

   virtual boolean Accepts(
        char character)
   {
      boolean acceptable = false;

      if (!is_complete)
      {
         if (Expression::case_sensitive)
         {
            acceptable = is_in(character, first, last);
         }
         else
         {
            acceptable = is_in(lower_case(character),
                               lower_case(first),
                               lower_case(last));
         }

         if (acceptable)
         {
            is_complete = true;
         }
      }

      return acceptable;
   }

public:

   virtual boolean Is_Complete()
   {
      return is_complete;
   }

public:

   virtual void Reset()
   {
      is_complete = false;
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Character Group '" << first << "' - '" << last << "'";
      return out;
   }

};   // class Character_Range


//===========================================================================

class Generic_Repetition : public Expression
{
protected:

   int count;           // count of expressions completed
   int repetitions;     // number of expressions
   Expression* expression;

public:

   virtual boolean Accepts(
        char character)
   {
         //  The generic Accepts() for a repetition expression feeds
         //  the character into the expression.  If the expression
         //  rejects it but is complete, the repetition count is
         //  incremented, the expression is Reset(), and we try again.

      boolean acceptable = expression->Accepts(character);

      if ( !acceptable &&
           expression->Is_Complete() )
      {
         count++;
         expression->Reset();
         acceptable = expression->Accepts(character);
      }

      return acceptable;
   }

public:

   virtual void Reset()
   {
      count = 0;
      expression->Reset();
   }

};   // class Generic_Repetition


//===========================================================================

class Repeat_At_Least : public Generic_Repetition
{
public:

   Repeat_At_Least(
      Expression* expression,
      int repetitions = 0)
   {
      this->expression = expression;
      this->repetitions = repetitions;
      count = 0;
      expression->Reset();
   }

public:

   virtual ~Repeat_At_Least()
   {
   }

public:

   virtual boolean Is_Complete()
   {
      return (count >= repetitions);
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Repeat at least " << repetitions << " times: ";
      expression->Show(out);
      return out;
   }

};   // class Repeat_At_Least


//===========================================================================

class Repeat_At_Most : public Generic_Repetition
{
public:

   Repeat_At_Most(
      Expression* expression,
      int repetitions)
   {
      this->expression = expression;
      this->repetitions = repetitions;
      count = 0;
      expression->Reset();
   }

public:

   virtual ~Repeat_At_Most()
   {
   }

public:

   virtual boolean Accepts(
        char character)
   {
      boolean acceptable = false;

      if (count < repetitions)
      {
         acceptable = Generic_Repetition :: Accepts(character);
         if (count == repetitions)
         {
            acceptable = false;
         }
      }

      return acceptable;
   }

public:

   virtual boolean Is_Complete()
   {
      return (count <= repetitions);
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Repeat at most " << repetitions << " times: ";
      expression->Show(out);
      return out;
   }

};   // class Repeat_At_Most


//===========================================================================

class Repeat_Exactly : public Generic_Repetition
{
public:

   Repeat_Exactly(
      Expression* expression,
      int repetitions)
   {
      this->expression = expression;
      this->repetitions = repetitions;
      count = 0;
      expression->Reset();
   }

public:

   virtual ~Repeat_Exactly()
   {
   }

public:

   virtual boolean Accepts(
        char character)
   {
      boolean acceptable = false;

      if (count < repetitions)
      {
         acceptable = Generic_Repetition :: Accepts(character);
         if (count == repetitions)
         {
            acceptable = false;
         }
      }

      return acceptable;
   }

public:

   virtual boolean Is_Complete()
   {
      boolean is_complete = (count == repetitions);

      if (count == (repetitions - 1))
      {
         is_complete = expression->Is_Complete();
      }

      return is_complete;
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Repeat exactly " << repetitions << " times: ";
      expression->Show(out);
      return out;
   }

};   // class Repeat_Exactly


//===========================================================================

class Expression_List : public Expression, public List<Expression*>
{
public:

   int current_index;

public:

   Expression_List() :
        List<Expression*>(10, 10)
   {
      current_index = 0;
   }

public:

   virtual ~Expression_List()
   {
   }

public:

   virtual boolean Accepts(
        char character);

public:

   virtual boolean Is_Complete()
   {
      boolean is_complete = false;

      if (current_index >= count)
      {
         is_complete = true;
      }

         //  The last expression might be complete even though
         //  it is still accepting characters.

      if (current_index == (count - 1))
      {
         is_complete = base[current_index]->Is_Complete();
      }

      return is_complete;
   }

public:

   virtual void Reset();

public:

   virtual void Retreat(
      int by_count = 1)
   {
      if (current_index >= by_count)
      {
         current_index -= by_count;
      }
   }

public:

   virtual ostream& Show(
        ostream& out);

};   // class Expression_List


   boolean Expression_List :: Accepts(
        char character)
   {
      boolean acceptable = false;

      if (current_index < count)
      {
         acceptable = base[current_index]->Accepts(character);
         if ( (!acceptable) &&
              (base[current_index]->Is_Complete()) )
         {

               //  Current expression rejects the character, but the
               //  expression is complete, so continue.

            current_index++;
            if (current_index < count)
            {
               acceptable = base[current_index]->Accepts(character);
            }
         }
      }

      return acceptable;
   }


   void Expression_List :: Reset()
   {
      for (int i = 0; i < count; i++)
      {
         base[i]->Reset();
      }
      current_index = 0;
   }


   ostream& Expression_List :: Show(
        ostream& out)
   {
      out << "Expression list:\n{\n";
      for (int i = 0; i < count; i++)
      {
         base[i]->Show(out);
         out << '\n';
      }
      out << "}\n";
      return out;
   }

//===========================================================================

class Character_Set : public Expression_List
{
protected:

   boolean is_complete;
   boolean excluding;

public:

   Character_Set(boolean excluding = false)
   {
      is_complete = false;
      this->excluding = excluding;
   }

public:

   virtual ~Character_Set()
   {
   }

public:

   virtual boolean Accepts(
        char character);

public:

   virtual boolean Is_Complete();

public:

   virtual void Reset();

public:

   virtual ostream& Show(
        ostream& out);

};   // class Character_Set


   boolean Character_Set :: Accepts(
        char character)
   {
      boolean acceptable = false;

      if (!is_complete)
      {
         for (int i = 0; i < count; i++)
         {
            if (base[i]->Accepts(character))
            {
               acceptable = true;
               break; //--------------------------------------> break!
            }
         }

         if (excluding)
         {
            acceptable = !acceptable;
         }
      }

      if (acceptable)
      {
         is_complete = true;
      }

      return acceptable;
   }


   boolean Character_Set :: Is_Complete()
   {
      return is_complete;
   }


   void Character_Set :: Reset()
   {
      for (int i = 0; i < count; i++)
      {
         base[i]->Reset();
      }
      is_complete = false;
   }


   ostream& Character_Set :: Show(
        ostream& out)
   {
      out << "Character set ";
      if (excluding)
      {
         cout << "NOT ";
      }
      out << "containing:\n{\n";
      for (int i = 0; i < count; i++)
      {
         base[i]->Show(out);
         out << '\n';
      }
      out << "}\n";
      return out;
   }


//===========================================================================

class Expression_With_Holder : public Expression
{
protected:

   Expression* expression;
   String* holder;

public:

   Expression_With_Holder(
      Expression* expression,
      String* holder)
   {
      this->expression = expression;
      this->holder = holder;
   }

public:

   virtual boolean Accepts(
        char character)
   {
      boolean acceptable = expression->Accepts(character);

      if (acceptable)
      {
         *holder += character;
      }

      return acceptable;
   }

public:

   virtual void Reset()
   {
      holder = "";
      expression->Reset();
   }

public:

   virtual ostream& Show(
        ostream& out)
   {
      out << "Holder (\"" << holder << "\") for: ";
      expression->Show(out);
      return out;
   }

};   // Expression_With_Holder


//===========================================================================

class Expression_Handler
{
public:

   int parentheses;
   int curly_braces;

   char* source;
   char error_string[0x80];

   String_List* holder_list;

   Expression* expression;

public:

   Expression_Handler(
      char* regular_expression_string,
      char* tagged_holder_list[])
   {
      parentheses = 0;
      curly_braces = 0;
      error_string[0] = 0;
      holder_list = new String_List;
      source = regular_expression_string;
      expression = Make_Expression();
   }

public:

   virtual ~Expression_Handler()
   {
      delete expression;
   }

protected:

   virtual Expression* Make_Expression();

protected:

   virtual Expression* Make_Character_Set(boolean excluding);

};   // class Expression_Handler


//===========================================================================

Expression* Expression_Handler :: Make_Expression()
{
   Expression_List* expression_list = new Expression_List;
   Expression* expression = NULL;

   error_string[0] = 0;

   if (expression_list == NULL)
   {
      strcpy(error_string, "out of memory");
      return NULL; //-----------------------------------------> return!
   }

   while ( (*source != 0) &&
           (error_string[0] == 0) )
   {
      switch (*source++)
      {
         case '[':
            boolean exclude_characters = false;
            if (*source == '~')
            {
               source++;
               exclude_characters = true;
            }
            expression = Make_Character_Set(exclude_characters);
            expression_list->Insert(expression);
            break;

         case '{':
            curly_braces++;
            expression = Make_Expression();
            String* string = new String;
            holders->Insert(string);
            expression_list->Insert(
                  new Expression_With_Holder(expression, string));
            break;

         case '}':

               // only at end of current expression if we previously ran
               // into a curly brace..

            if (curly_braces > 0)
            {
               curly_braces--;
               return expression_list; //--------------------> return!
            }

               // otherwise, it's a real '}' character

            expression = new Single_Character('}');
            expression_list->Insert(expression);
            break;

         case '(':
            parentheses++;
            expression = Make_Expression();
            expression_list->Insert(expression);
            break;

         case ')':

               // only at end of current expression if we previously ran
               // into a parenthesis...

            if (parentheses > 0)
            {
               parentheses--;
               return expression_list; //--------------------> return!
            }

               // otherwise, it's a real ')' character

            expression = new Single_Character(')');
            expression_list->Insert(expression);
            break;

         case '?':
            expression = new Any_Character;
            expression_list->Insert(expression);
            break;

         case '/':
            if ( (expression == NULL) ||        // check for first or last
                 (*source == 0) )
            {
               expression = new Single_Character('/');
               expression_list->Insert(expression);
               break; //--------------------------------------> break!
            }

            int repetitions = 0;

            while ( (*source != 0) &&
                    isdigit(*source) )
            {
               repetitions *= 10;
               repetitions += (*source - '0');
               source++;
            }

            switch (*source++)
            {
               case '-':
                  expression = new Repeat_At_Most(expression, repetitions);
                  expression_list->Remove();
                  expression_list->Insert(expression);
                  break;

               case '+':
                  expression = new Repeat_At_Least(expression, repetitions);
                  expression_list->Remove();
                  expression_list->Insert(expression);
                  break;

               case '=':
                  expression = new Repeat_Exactly(expression, repetitions);
                  expression_list->Remove();
                  expression_list->Insert(expression);
                  break;

               default:
                  source--;
                  expression = new Repeat_Exactly(expression, repetitions);
                  expression_list->Remove();
                  expression_list->Insert(expression);
                  break;

            }   // switch
            break;

         case '*':
            expression = new Repeat_At_Least(new Any_Character, 0);
            expression_list->Insert(expression);
            break;

         case '+':
            if (expression == NULL)
            {
               expression = new Single_Character('+');
            }
            else
            {
               expression = new Repeat_At_Least(expression, 1);
               expression_list->Remove();
               expression_list->Insert(expression);
            }
            break;

         default:
            char ch;
            source--;
            source = strcchr(&ch, source);
            expression = new Single_Character(ch);
            expression_list->Insert(expression);

      }   // switch

   }   // while

   return expression_list;

}   // Expression_Handlers :: Make_Expression


Expression* Expression_Handler :: Make_Character_Set(boolean excluding)
{
   Character_Set* character_set = new Character_Set(excluding);

   char first;
   char second = 0;

   while ( (*source != 0) &&
           (*source != ']') )
   {
      source = strcchr(&first, source);
      second = 0;

      if (*source == '-')
      {
         source++;
         if (*source == 0)
         {
            sprintf(error_string, "unclosed character range \"%c-\"", first);
            return character_set; //--------------------------> return!
         }
         source = strcchr(&second, source);
         character_set->Insert(new Character_Range(first, second));
      }
      else
      {
         character_set->Insert(new Single_Character(first));
      }

   }   // while

   if (*source == ']')
   {
      source++;
   }

   return character_set;

}   // Expression_Handlers :: Make_Character_Set


//===========================================================================


Expression_Handler* builder;


void main(
)//   int argc,
//   char* argv[])
{
   cout << "------------------------------------------------------" << endl;

//   cout << "farcoreleft() = " << farcoreleft()
//        << "   coreleft() = " << coreleft() << endl;

   char* holders[10];

   char input_string[0x100];

   cout << "Enter a regular expression: ";
   cin.getline(input_string, sizeof(input_string));

   builder = new Expression_Handler(input_string, holders);
   builder->expression->Show(cout);

   do
   {
      cout << "Enter a string to test: ";
      cin.getline(input_string, sizeof(input_string));

      builder->expression->Reset();

      for (int i = 0; input_string[i] != 0; i++)
      {
         cout << i << ": '" << input_string[i] << "' ";
         if (builder->expression->Accepts(input_string[i]))
         {
            cout << "accepted" << endl;
         }
         else
         {
            cout << "died" << endl;
            break; //-----------------------------------------> break!
         }
      }

   } while (*input_string != 0);

   cout << "\n-------------------\n";
   builder->expression->Show(cout);


//   cout << "farcoreleft() = " << farcoreleft()
//        << "   coreleft() = " << coreleft() << endl;

   delete builder;

//   cout << "farcoreleft() = " << farcoreleft()
//        << "   coreleft() = " << coreleft() << endl;

   return;

}   // main
