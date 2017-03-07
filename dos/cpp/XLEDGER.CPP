#include <stdiostr.h>
#include <iomanip.h>
#include <fstream.h>
#include <string.h>


#include "string.hpp"


#define LEFT       resetiosflags(ios::adjustfield) << setiosflags(ios::left)
#define RIGHT      resetiosflags(ios::adjustfield) << setiosflags(ios::right)


void StripFromEnd(String& s,char c = ' ')
{
  while (s.Length() && (s.S()[s.Length()-1] == c)) s--;
}   //StringFromEnd(String&,char)




int  main(int argc,char* argv[])
{
  String path(0x100);
  String file(0x100);

  path = "w:\\rogers\\doug\\xact\\";
  file = path + "doug.xac";

  if (argc > 1) file = argv[1];

  ifstream xactfile;

  xactfile.open(file.S(),ios::nocreate);
  if (!xactfile)
  {
    cout << "couldn't open input file \"" << file << "\".\n";
    return 1;
  }

  float amount;
  float balance = 0.0;
  String string;
  String blank(2);
  String ledger;
  String other;
  String source;
  String destination;
  String description;

  blank = ' ';

  cout << "Reading transactions from file \"" << file << "\".\n";
  cout << "Enter ledger name: ";  cin >> ledger;
  if (ledger.Length() == 0) return 0;
  ledger.Upper();

  cout << "Enter initial balance: ";  cin >> string;
  sscanf(string.S(),"%f",&balance);

  cout << setprecision(2);
  cout << setiosflags(ios::fixed|ios::showpoint);

  cout << RIGHT;
  cout << setw(27) << "PREVIOUS BALANCE" << " | ";
  cout << LEFT;
  cout << setw(8)  << String("Account") << " | ";
  cout << RIGHT;
  cout << setw(10) << String("Credit") << " | ";
  cout << setw(10) << String("Debit")  << " | ";
  cout << setw(10) << balance << " |\n";

  cout << LEFT;

  char from_ledger;
  char to_ledger;

  while (xactfile)
  {
    xactfile >> string;
    if (string.Length() < 68) continue;

    description = string.Sub( 0,30);  StripFromEnd(description);
    source      = string.Sub(46, 8);  StripFromEnd(source);
    destination = string.Sub(58, 8);  StripFromEnd(destination);
    if (sscanf(string.Sub(33,10).S(),"%f",&amount) != 1) continue;

    source.Upper();
    destination.Upper();

    from_ledger = (ledger == source);
    to_ledger   = (ledger == destination);
    if (!from_ledger && !to_ledger) continue;

    if (from_ledger)
      other = destination;
    else
      other = source;

    if (from_ledger) balance -= amount;
    if (to_ledger  ) balance += amount;

    cout << LEFT;
    cout << setw(27) << description << " | ";
    cout << setw(8)  << other       << " | ";

    cout << RIGHT;
    cout << setw(10);
    if (to_ledger) cout << amount; else cout << blank;
    cout << " | ";
    cout << setw(10);
    if (from_ledger) cout << amount; else cout << blank;
    cout << " | ";

    cout << setw(10) << balance << " |\n";
  }
  xactfile.close();
  return 0;
}   //main
