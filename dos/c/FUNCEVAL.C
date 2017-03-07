/*-------------------------------------------------------------------------*\
 | Module:      Function Dumper
 | Compiler:    Borland/Turbo C
 | Description: This module contains procedures which assist in the
 |              translation from text and the evaluation of real-valued
 |              functions of a single variable.
\*-------------------------------------------------------------------------*/
#include <stdio.h>
#include <conio.h>
#include <math.h>


#include "funceval.h"

#define MAX_VARIABLE_LENGTH 19




/*-------------------------------------------------------------------------*\
 | Data:        variable_list
 | Description: This is the pointer to the base of the user-defined
 |              variables linked list.
\*-------------------------------------------------------------------------*/

typedef struct VariableEntry_struct    /*for user-defined variables*/
{
  char   variable[MAX_VARIABLE_LENGTH+1];
  double value;
  struct VariableEntry_struct* next;
} VariableEntry;

static VariableEntry* user_variable_list = NULL;


/*-------------------------------------------------------------------------*\
 | Data:        function_name_list
 | Description: This data element holds a list of all function names known
 |              to this module.
\*-------------------------------------------------------------------------*/

typedef double (*RealFunction)(double x);

typedef struct                 /*structure for entries in table to follow...*/
{
  char* name;
  RealFunction func;
} FunctionNameEntry;

static FunctionNameEntry* function_name_list[] =
{
  { "abs",   fabs   },
  { "acos",  acos   },
  { "asin",  asin   },
  { "atan",  atan   },
  { "ceil",  ceil   },
  { "cos",   cos    },
  { "cosh",  cosh   },
  { "exp",   exp    },
  { "floor", floor  },
  { "log",   log    },
  { "log10", log10  },
  { "sin",   sin    },
  { "sinh",  sinh   },
  { "sqrt",  sqrt   },
  { "tan",   tan    },
  { "tanh",  tanh   },

  { NULL, NULL }
};   /*function_name_list*/


/*-------------------------------------------------------------------------*\
 | Procedure:   p
 | Description: d
\*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*\
 | Procedure:   GetRealFunction
 | Description: Translates a function from its name to a pointer to a
 |              function taking a real (double) argument.
 |              Returns NULL on failure.
\*-------------------------------------------------------------------------*/

RealFunction GetFunction(      /*pointer to function for given name*/
  char* name)                  /*name for which a function is sought*/
{
  FunctionNameEntry* entry = function_name_list;

  while (entry->name != NULL)
  {
    if (stricmp(entry->name, name) == 0) break;
  }
  return entry->func;
}   /*GetFunction*/


/*-------------------------------------------------------------------------*\
 | Procedure:   SetVariable
 | Description: Sets a variable to the value provided.  If the variable is
 |              not yet created, it creates the variable.
\*-------------------------------------------------------------------------*/

void SetVariable(              /*no output*/
  char* variable,              /*variable to be created/changed*/
  double value)                /*initial/new value of variable*/
{
  VariableEntry* entry = user_variable_list;

  if (user_variable_list == NULL)
  {
    user_variable_list = (VariableEntry*) malloc(sizeof(*user_variable_list));
    if (user_variable_list == NULL) return;
    entry = user_variable_list;
  }
  else
  {
    while (entry->next != NULL)
    {
      if (stricmp(entry->name, name) == 0)
      {
        entry->value = value;
        return;
      }   /*if entry already exists*/
      entry = entry->next;
    }   /*while*/

    entry->next = (VariableEntry*) malloc(sizeof(*entry));
    if (entry->next == NULL) return;
  }   /*else list already exists*/

  strncpy(entry->variable, variable, MAX_VARIABLE_LENGTH);
  entry->variable[MAX_VARIABLE_LENGTH] = 0;
  entry->value = value;
  entry->next  = NULL;
}   /*SetVariable*/


/*-------------------------------------------------------------------------*\
 | Procedure:   GetVariable
 | Description: Returns the current value of a user-defined variable (which
 |              was previously defined via SetVariable).  If the variable is
 |              not yet created, it returns 0.0.
\*-------------------------------------------------------------------------*/

double GetVariable(            /*value of the requested variable*/
  char* variable)              /*variable whose value is requested*/
{
  VariableEntry* entry = user_variable_list;

  while (entry != NULL)
  {
    if (stricmp(entry->variable, variable) == 0) return entry->value;
  }
  return 0.0;
}   /*GetVariable*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PurgeVariables
 | Description: Purges (deletes) all variables from the user-defined
 |              variables list.
\*-------------------------------------------------------------------------*/

void PurgeVariables(void)      /*no output*/
{
  VariableEntry* entry = user_variable_list;
  VariableEntry* next;

  while (entry != NULL)
  {
    next = entry->next;
    free(entry);
    entry = next;
  }
}   /*GetVariable*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PopRPNStack
 | Description: Deletes the top value from the RPN stack by moving the next
 |              values up one notch.  Returns the deleted value.
\*-------------------------------------------------------------------------*/

double PopRPNStack(void)       /*value on top of stack*/
{
  int i;
  double top_value = rpn_stack[0];

  for (i = 1; (i < RPN_STACK_SIZE); i++)
    rpn_stack[i] = rpn_stack[i+1];
}   /*PopRPNStack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PushRPNStack
 | Description: Pushes a new value onto the top of the RPN calculator's
 |              stack.
\*-------------------------------------------------------------------------*/

void PushRPNStack(             /*no output*/
  double value)                /*value to push onto top of stack*/
{
  int i;
  double top_value = rpn_stack[0];

  for (i = (RPN_STACK_SIZE - 1); (i > 0); i--)
    rpn_stack[i] = rpn_stack[i-1];
  rpn_stack[0] = value;
}   /*PushRPNStack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   ClearRPNStack
 | Description: Clears (sets to 0.0) each element of the RPN calculator's
 |              stack.
\*-------------------------------------------------------------------------*/

void ClearRPNStack(void)       /*no output*/
{
  int i;

  for (i = 0; (i < RPN_STACK_SIZE); i++)
    rpn_stack[i] = 0.0;
}   /*ClearRPNStack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   EvaulateFunctionString
 | Description: Translates and evaluates a string containing functions,
 |              operators and variables.  Returns the final value of the
 |              calculation, which is done in Reverse Polish Notation
 |              (RPN).
\*-------------------------------------------------------------------------*/

