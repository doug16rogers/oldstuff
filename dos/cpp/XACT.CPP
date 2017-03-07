#include <stdiostr.h>
#include <iomanip.h>
#include <fstream.h>
#include <string.h>
#include <dos.h>


#include "string.hpp"

void GetDate(String& date)
{
  struct date d;
  char dat[12];

  getdate(&d);
  sprintf(dat,"%4u.%02u.%02u",d.da_year,d.da_mon,d.da_day);
  date = dat;
}   //GetDate(String&)


int  main(int argc,char* argv[])
{
  String path(0x100);
  String file(0x100);
  String date(12);

  GetDate(date);

  path = "w:\\rogers\\doug\\xact\\";
  file = path + "doug.xac";

  if (argc > 1) file = argv[1];

  ofstream xactfile;

  xactfile.open(file.S(),ios::app);
  if (!xactfile)
  {
    cout << "couldn't open output file \"" << file << "\".\n";
    return 1;
  }

  float amount;
  String source;
  String destination;
  String description;
  String string;

  cout << "To quit, enter 0 for Amount.\n";
  cout << "Appending to file \"" << file << "\".\n";

  xactfile.setf(ios::left        | ios::fixed | ios::showpoint,
                ios::adjustfield | ios::fixed | ios::showpoint);

  while (1)
  {
    cout << "\n";

    cout << "Amount of transaction  : ";  cin >> source;
    if (sscanf(source.S(),"%f",&amount) != 1) break;
    if (amount == 0.0) break;

    cout << "Transaction description: ";  cin >> description;
    cout << "Where money came from  : ";  cin >> source;
    cout << "Where money went to    : ";  cin >> destination;
    cout << "Enter date [" << setw(10)
      << date << "]: ";  cin >> string;

    string.Upper();
    if (string == "TODAY")
      GetDate(date);
    else if (string.Length() == 10)
      date = string;

    source.Upper();
    destination.Upper();

    xactfile << setw(10) << date << " | ";
    xactfile << setw(20) << description << " | ";
    xactfile.setf(ios::right, ios::adjustfield);
    xactfile << setprecision(2) << setw(9) << amount << " | ";
    xactfile.setf(ios::left,  ios::adjustfield);
    xactfile << setw(8)  << source      << " -> ";
    xactfile << setw(8)  << destination << " |\n";
  }
  xactfile.close();
  return 0;
}   //main
