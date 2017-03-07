#include <stdiostr.h>
#include <string.h>

void main(int argc, char* argv[])
{
  int i;
  if (argc < 2) return;
  if (stricmp(argv[1], "you") != 0) return;
  cout << "No, I'm not";
  for (i = 2; (i < argc); i++) cout << ' ' << argv[i];
  cout << "\b.\n";
}   //main
