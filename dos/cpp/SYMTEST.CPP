
typedef struct {
  char name[80];
  char value[80];
} FUN;


class FunSymbol : public SymbolTable {
public:
  FUN* Insert(FUN* f) { return (FUN*)SymbolTable::Insert((void*)f); }
  FUN* First(void) { return (FUN*)SymbolTable::First(); }
  FUN* Next() { return (FUN*)SymbolTable::Next(); }

  FunSymbol(int _size=SIZE,CompareFunc _compare=(CompareFunc)strcmp) :
    SymbolTable(_size,_compare) { }
  ~FunSymbol() { }
};   //class FunSymbol



FUN fun;


void main(void)
{
  FunSymbol sym(sizeof(FUN));
  FUN* f;

  printf("====================================================\n");
  while (1) {
    printf("Enter symbol name : ");  gets(fun.name);
    if (!fun.name[0]) break;
    printf("Enter symbol value: ");  gets(fun.value);
    sym.Insert(&fun);
  }
  printf("----------------------------------------------------\n");
  for (f=sym.First();f;f=sym.Next())
    printf("%20s = \"%s\"\n",f->name,f->value);
}   //main

