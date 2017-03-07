#include <stdiostr.h>
#include <fstream.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>


#include "cmdline.hpp"
#include "drstr.hpp"
#include "kb.hpp"
#include "symutil.hpp"


//--------------------------------------------------------------------------
//  global #define's
//--------------------------------------------------------------------------
#define MAX_FILENAME_LENGTH    8
#define DVS_MAIL_DIR           "F:\\MHS\\MAIL\\USERS\\%s\\DVSEMAIL\\"


//--------------------------------------------------------------------------
//  global variables
//--------------------------------------------------------------------------
char dvs_mail_dir_mask[0x80] = DVS_MAIL_DIR;
char user_name[0x20] = "\0";
char user_mail_dir[0x80];

CommandLine command_line(
  + *new Switch("DVS Mail Directory mask", 'd', dvs_mail_dir_mask, "%s")
  + *new Switch("Login/user name", 'l', user_name, "%s")
  );


//--------------------------------------------------------------------------
//  class More
//  Works like DOS more.com pipe.  Stops output when screen is about to
//  scroll.
//--------------------------------------------------------------------------
class More
{
private:
  int rows;                    //number of rows to display at once
  int cols;                    //columns on screen
  int row;                     //current row for output
  char* prompt;                //prompt to display on line
//  ostream* out_stream;         //stream for output
public:
  char key_pressed;            //key pressed when prompt displayed

#define MORE_PROMPT "---- Press SPACEBAR for more ---"
  void Reset(void) { row = 0;  key_pressed = 0; }
  More(int _rows = 24, int _cols = 80, char* _prompt = MORE_PROMPT)
  {
    prompt = _prompt;
    rows = _rows;
    cols = _cols;
    Reset();
  }
  friend ostream& operator << (ostream& o, More& m);
};   //class More


//--------------------------------------------------------------------------
//  friend ostream& operator << (ostream& o, More& m)
//--------------------------------------------------------------------------
ostream& operator << (ostream& o, More& m)
{
  m.row++;
  if (m.row >= m.rows)
  {
    m.Reset();
    m.row++;
    o << m.prompt;
    kbin >> m.key_pressed;
    o << '\r';
    int prompt_length = strlen(m.prompt);
    for (int i = 0; (i < prompt_length); i++)
      o << ' ';
    o << '\r';
  }
  return o;
}  //ostream& operator << (ostream& o, More& m)


//--------------------------------------------------------------------------
// void ShowFile (char*,char*)
// Shows the specified path and file to the screen, using More.
//--------------------------------------------------------------------------
int  ShowFile(                 //success = 1, failure = 0
  char* path,                  //path to file
  char* filename)              //name of file
{
  static char full_filename[0x100];
  static char line[0x100];
  int path_length = strlen(path);

  strcpy(full_filename, path);
  if (path_length > 0) if (path[path_length - 1] != '\\')
    strcat(full_filename, "\\");
  strcat(full_filename, filename);

  ifstream input_file;
  More more;
  input_file.open(full_filename, ios::nocreate);
  if (!input_file) return 0;
  more.Reset();
  cout << "------------------------------------------------------------\n";
  input_file.getline(line, sizeof(line));  //-- get line with heart on it
  while (!input_file.eof())
  {
    input_file.getline(line, sizeof(line));
    cout << line << '\n' << more;
    if (more.key_pressed == ESC) break;
  }
  input_file.close();
}   //int ShowFile(char*,char*)


//--------------------------------------------------------------------------
//  void ShowMailFiles(void)
//--------------------------------------------------------------------------
void ShowMailFiles(void)
{
  struct ffblk file_block;
  char full_dir_list[0x80];
  char file_spec[0x80];
  int done;

  strcpy(full_dir_list, user_mail_dir);
  strcat(full_dir_list, "*.*");

  done = findfirst(full_dir_list, &file_block, 0);
  if (done)
  {
    cout << "No new mail for " << user_name << ".\n";
    return;
  }

  while (!done)
  {
    if (!ShowFile(user_mail_dir, file_block.ff_name))
    {
      cout << "that's strange... couldn't open " << file_spec << ".\n";
    }
    done = findnext(&file_block);
  }   //while
}   //void ShowMailFiles(void)


//--------------------------------------------------------------------------
//  int LoadUserName(void)
//  Loads the username from the environment variable "LOGIN", then sets up
//  the DVS mail directory.
//--------------------------------------------------------------------------
int LoadUserName(void)         //success=1/failure=0
{
  if (user_name[0] != 0) return 1;

  char* login_environment_variable = getenv("LOGIN");
  if (login_environment_variable == NULL)
  {
    cout << "Couldn't locate LOGIN environment variable.  Use -l switch.\n";
    return 0;
  }
  strNcpy(user_name, login_environment_variable, MAX_FILENAME_LENGTH);
  sprintf(user_mail_dir, dvs_mail_dir_mask, user_name);
  return 1;
}   //int LoadUserName(void)


//--------------------------------------------------------------------------
//  int Help(int return_value)
//--------------------------------------------------------------------------
int Help(                      //return the parameter as given
  int return_value)            //value to return as a pass-through mechanism
{
  cout
    << "Switches:\n"
    << "  -l<login_name>  login name [from LOGIN environment variable]\n"
    << "  -d<dir_mask>    mail directory mask ["
                           << dvs_mail_dir_mask << "]\n"
  ;
  return return_value;
}   //int Help(int return_value)


//--------------------------------------------------------------------------
//  int main(int argc, char* argv[])
//  Main program.
//--------------------------------------------------------------------------
int main(                      //DOS return code, success=0, else error code
  int argc,                    //number of commandline arguments
  char* argv[])                //pointer to array of argument pointers
{
  cout << "drmail -- mail reader/deleter, use -? for switches\n\n";

  if (!command_line.Load(argc, argv)) return Help(1);
  if (!LoadUserName()) return Help(2);

  ShowMailFiles();
  return 0;
}   //int main(int argc, char* argv[])


