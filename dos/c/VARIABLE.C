/*-------------------------------------------------------------------------*\
 | Module:      Variable List Manipulator
 | Description: This module maintains a linked list of variables whose
 |              values are double's.
\*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable.h"


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
 | Procedure:   EqualUpToWhite
 | Description: Compares two strings (case-insensitively) until the end of
 |              either string, or until either string contains whitespace.
 |              Returns the length of the strings if they are equal.
 |              Returns 0 if they are not equal.
\*-------------------------------------------------------------------------*/

int  EqualUpToWhite(           /*zero if equal, 1 if s1>s2, -1 if s1<s2*/
  char* string1,               /*first string to compare*/
  char* string2)               /*second string to compare*/
{
  char c1 = 1;
  char c2 = 1;
  int i = 0;

  if (string1 == NULL) return -1;
  if (string2 == NULL) return +1;

  while ((c1 != 0) && (c2 != 0))
  {
    c1 = string1[i];
    c2 = string2[i];
    i++;
    c1 = toupper(c1);
    c2 = toupper(c2);
    if (isspace(c1)) c1 = 0;
    if (isspace(c2)) c2 = 0;
    if (c1 != c2) return 0;
  }   /*forever*/
  return i-1;
}   /*EqualUpToWhite*/


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
      if (EqualUpToWhite(entry->variable, variable))
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
 | Procedure:   VariableDefined
 | Description: Returns 1 if a variable with the given name has been
 |              defined via a call to SetVariable.  Returns 0 if not.
\*-------------------------------------------------------------------------*/

int VariableDefined(           /*1 if defined, 0 otherwise*/
  char* variable)              /*name of variable to be found*/
{
  VariableEntry* entry = user_variable_list;

  while (entry != NULL)
  {
    if (EqualUpToWhite(entry->variable, variable)) return 1;
    entry = entry->next;
  }
  return 0;
}   /*VariableDefined*/


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
    if (EqualUpToWhite(entry->variable, variable)) return entry->value;
    entry = entry->next;
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
}   /*PurgeVariables*/


