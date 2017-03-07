#include <stdio.h>


void main(void)
{
  int c;
  enum { seeking_comment, got_slash, seeking_star, got_star };
  int mode = seeking_comment;

  while ((c = getchar()) != EOF)
  {
    switch (mode)
    {
    case seeking_comment:
      if (c == '/')
        mode = got_slash;
      else
        putchar(c);
      break;

    case got_slash:
      switch (c)
      {
      case '*':
        putchar(' ');
        putchar(' ');
        mode = seeking_star;
        break;
      case '/':
        putchar('/');
        mode = got_slash;
        break;
      default:
        putchar('/');
        putchar(c);
        mode = seeking_comment;
      }
      break;

    case seeking_star:
      if (c == '\n') putchar(c); else putchar(' ');
      if (c == '*') mode = got_star;
      break;

    case got_star:
      if (c == '\n') putchar(c); else putchar(' ');
      switch (c)
      {
      case '*':
        mode = got_star;
        break;
      case '/':
        mode = seeking_comment;
        break;
      default:
        mode = seeking_star;
      }
      break;
    }   /*switch on mode*/
  }   /*while*/
}   /*main*/

