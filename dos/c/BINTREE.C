#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifndef BYTE
#define BYTE unsigned char
#endif


typedef struct {
  unsigned tp_traversed:1;
  unsigned lt_traversed:1;
  unsigned rt_traversed:1;
  unsigned tp_flag:1;
  unsigned lt_flag:1;
  unsigned rt_flag:1;
} FLAGS;


typedef struct _NODE {
  int   val;
  FLAGS flag;
  struct _NODE* tp;
  struct _NODE* lt;
  struct _NODE* rt;
} NODE;


NODE* BaseNode=NULL;


NODE* FindNode(int val,int insert)
{
  NODE* node=BaseNode;
  NODE* last=BaseNode;

  if (node==NULL) {
    if (insert) {
      node=malloc(sizeof(NODE));
      if (!node) return NULL;
      BaseNode=node;
      node->tp=NULL;
      node->lt=NULL;
      node->rt=NULL;
      *(BYTE*)&(node->flag)=0;
      node->val=val;
    }
    return node;
  }   //if first entry

  while (node) {
    if (node->val==val) break;
    last=node;
    if (val<node->val)
      node=node->lt;
    else
      node=node->rt;
  }   //while

  if (node || !insert) return node;

  node=malloc(sizeof(NODE));
  if (!node) return NULL;
  node->tp=last;
  node->lt=NULL;
  node->rt=NULL;
  node->val=val;

  if (val<last->val)
    last->lt=node;
  else
    last->rt=node;

  return node;
}   //FindNode

#define InsertNode(v)    FindNode((v),1)


void ListNodes(NODE* node)
{
  int lval=0,rval=0;

  if (!node) return;
  if (node->lt) lval=node->lt->val;
  if (node->rt) rval=node->rt->val;
  printf("%4u  %4u  %4u\n",node->val,lval,rval);
  ListNodes(node->lt);
  ListNodes(node->rt);
}   //ListNodes




int vals[]={40,50,60,20,30,10,70,80,100,44,48,46,15,13,14,99,0};


void main(void)
{
  int i;

  printf("========================\n");
  for (i=0;vals[i];i++) InsertNode(vals[i]);
  ListNodes(BaseNode);
}   //main

