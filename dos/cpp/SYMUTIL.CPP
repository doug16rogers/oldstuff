#include "symutil.hpp"


SymbolTable::SymbolTable(int _size,CompareFunc _compare)
{
  if (_compare==NULL) compare=(CompareFunc)strcmp; else compare=_compare;
  if (_size<1) size=SIZE; else size=_size;
  base=NULL;
  curr=NULL;
  count=0;
}   //SymbolTable::SymbolTable(int,CompareFunc)


void* SymbolTable::Insert(void* sym)
{
  if (base==NULL) {
    base=(SYMBOL_NODE*) new BYTE[size+sizeof(SYMBOL_NODE)];
    if (base==NULL) return NULL;
    count++;
    base->top=NULL;
    base->left=NULL;
    base->right=NULL;
    base->is_left=0;
    base->is_right=0;
    memcpy(base->data,sym,size);
    return base->data;
  }

  int cmp;
  SYMBOL_NODE* node=base;
  SYMBOL_NODE* last;

  while (node!=NULL) {
    last=node;
    cmp=compare(sym,node->data);
    if (cmp==0) { curr=node; return node->data; }
    if (cmp<0) node=node->left; else node=node->right;
  }   //while

  node=(SYMBOL_NODE*) new BYTE[size+sizeof(SYMBOL_NODE)];
  if (node==NULL) return NULL;
  count++;
  node->top=last;
  node->left=NULL;
  node->right=NULL;
  node->is_left=0;
  node->is_right=0;
  memcpy(node->data,sym,size);
  if (cmp<0) {
    last->left=node;
    node->is_left=1;
  }
  else {
    last->right=node;
    node->is_right=1;
  }
  curr=node;
  return node->data;
}   //SymbolTable::Insert(void*)


void* SymbolTable::Find(void* sym)
{
  if (base==NULL) return NULL;
  int cmp;
  SYMBOL_NODE* node=base;
  while (node!=NULL) {
    cmp=compare(sym,node->data);
    if (cmp==0) break;
    if (cmp<0) node=node->left; else node=node->right;
  }   //while
  curr=node;
  if (node==NULL) return NULL;
  return node->data;
}   //SymbolTable::Find(void*)


void* SymbolTable::First(void)
{
  if (base==NULL) return NULL;
  curr=base;
  while (curr->left!=NULL) curr=curr->left;
  return curr->data;
}   //SymbolTable::First(void)


void* SymbolTable::Next(void)
{
  if (curr==NULL) return NULL;
  if (curr->right != NULL) {           //if a right node's available..
    curr=curr->right;                  //..go right
    while (curr->left != NULL) curr=curr->left;   //..then go far to left
  }
  else {
    while (curr->is_right) curr=curr->top;
    curr=curr->top;
  }
  if (curr==NULL) return NULL;
  return curr->data;
}    //SymbolTable::Next(void)


void* SymbolTable::Last(void)
{
  if (base==NULL) return NULL;
  curr=base;
  while (curr->right!=NULL) curr=curr->right;
  return curr->data;
}   //SymbolTable::Last(void)


void* SymbolTable::Prev(void)
{
  if (curr==NULL) return NULL;
  if (curr->left != NULL) {            //if a left node's available..
    curr=curr->left;                   //..go left
    while (curr->right!= NULL) curr=curr->right;   //..then go far to right
  }
  else {
    while (curr->is_left) curr=curr->top;
    curr=curr->top;
  }
  if (curr==NULL) return NULL;
  return curr->data;
}    //SymbolTable::Prev(void)


void SymbolTable::Purge(void)
{
  if (base==NULL) return;
  while ((base->left!=NULL)||(base->right!=NULL)) {
    curr=base;
    while ((curr->left!=NULL) || (curr->right!=NULL))
      if (curr->left !=NULL) curr=curr->left; else curr=curr->right;
    if (curr->is_right)
      curr->top->right=NULL;
    else
      curr->top->left=NULL;
    delete curr;
  }   //while
  delete base;
  count=0;
}   //SymbolTable::Purge(void)


