#ifndef __VARIABLE_H
#define __VARIABLE_H
/*-------------------------------------------------------------------------*\
 | Module:      Variable List Manipulator
 | Description: This module maintains a linked list of variables whose
 |              values are double's.
\*-------------------------------------------------------------------------*/


#ifndef MAX_VARIABLE_LENGTH
#define MAX_VARIABLE_LENGTH 19
#endif


/*-------------------------------------------------------------------------*\
 | Procedure:   SetVariable
 | Description: Sets a variable to the value provided.  If the variable is
 |              not yet created, it creates the variable.
\*-------------------------------------------------------------------------*/

void SetVariable(              /*no output*/
  char* variable,              /*variable to be created/changed*/
  double value);               /*initial/new value of variable*/


/*-------------------------------------------------------------------------*\
 | Procedure:   VariableDefined
 | Description: Returns 1 if a variable with the given name has been
 |              defined via a call to SetVariable.  Returns 0 if not.
\*-------------------------------------------------------------------------*/

int VariableDefined(           /*1 if defined, 0 otherwise*/
  char* name);                 /*name of variable to be found*/


/*-------------------------------------------------------------------------*\
 | Procedure:   GetVariable
 | Description: Returns the current value of a user-defined variable (which
 |              was previously defined via SetVariable).  If the variable is
 |              not yet created, it returns 0.0.
\*-------------------------------------------------------------------------*/

double GetVariable(            /*value of the requested variable*/
  char* variable);             /*variable whose value is requested*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PurgeVariables
 | Description: Purges (deletes) all variables from the user-defined
 |              variables list.
\*-------------------------------------------------------------------------*/

void PurgeVariables(void);     /*no output*/


#endif
