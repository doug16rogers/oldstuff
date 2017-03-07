#include <stdiostr.h>
#include <iomanip.h>
#include <math.h>

#include "kb.hpp"


#ifndef NN_TYPE
#define NN_TYPE double
#endif

#ifndef NN_START_LINK
#define NN_START_LINK 0
#endif

#ifndef NN_START_NODE
#define NN_START_NODE 0
#endif


typedef NN_TYPE (*NN_Function)(NN_TYPE sum);


class NN_Node;
class NN_Link;


//===========================================================================
class NN_Link_To_Link
{
public:
  NN_Link* link;
  NN_Link_To_Link* next;

  void Add(NN_Link* link);

  NN_Link_To_Link(NN_Link* _link = NULL)
  {
    link = _link;
    next = NULL;
  }
};   //class NN_Link_To_Link


void NN_Link_To_Link::Add(NN_Link* link)
{
  NN_Link_To_Link* base = this;
   while (base->next != NULL) base = base->next;
  base->next = new NN_Link_To_Link(link);
}   //NN_Link* NN_Link_To_Link* Add(NN_Link* link)


//===========================================================================
class NN_Node
{
private:
  static int current_node;

  int id;
  NN_TYPE value;
  NN_TYPE thresh;
  NN_Link_To_Link* i_link;
  NN_Link_To_Link* o_link;
  void AddInput (NN_Link* input);
  void AddOutput(NN_Link* output);
  void Init(NN_TYPE _value = 0, NN_TYPE _thresh = 0);
public:
//  static NN_Function net_function  =  ATANOverThreshhold;
//  static NN_Function dnet_function = dATANOverThreshhold;

  int ID(void) { return id; }
  NN_TYPE Value(void) { return value; }
  NN_TYPE Value(NN_TYPE _value);
  NN_TYPE Thresh(void) { return thresh; }
  NN_TYPE Thresh(NN_TYPE _thresh);

  NN_Node(NN_TYPE _value, NN_TYPE _thresh = 0)
  {
    Init(_value,_thresh);
  }
  NN_Node(void) { Init(); }
  ~NN_Node();

  friend class NN_Link;
  friend ostream& operator << (ostream& o, NN_Node& n);
  friend void Fire(void);
};   //class NN_Node


void NN_Node::Init(NN_TYPE _value, NN_TYPE _thresh)
{
  id = current_node++;
  value = _value;
  thresh = _thresh;
  i_link = NULL;
  o_link = NULL;
}   //void NN_Node::Init(NN_TYPE _value)


NN_Node::~NN_Node()
{
  NN_Link_To_Link* temp;
  while (i_link != NULL)
  {
    temp = i_link->next;
    delete i_link;
    i_link = temp;
  }
  while (o_link != NULL)
  {
    temp = o_link->next;
    delete o_link;
    o_link = temp;
  }
}   //NN_Node::~NN_Node()


NN_TYPE NN_Node::Value(NN_TYPE _value)
{
  NN_TYPE old = value;
  value = _value;
  return old;
}   //NN_TYPE NN_Node::Value(NN_TYPE _value)


NN_TYPE NN_Node::Thresh(NN_TYPE _thresh)
{
  NN_TYPE old = thresh;
  thresh = _thresh;
  return old;
}   //NN_TYPE NN_Node::Thresh(NN_TYPE _thresh)


void NN_Node::AddInput(NN_Link* link)
{
  if (i_link == NULL)
    i_link = new NN_Link_To_Link(link);
  else
    i_link->Add(link);
}   //void NN_Node::AddInput(NN_Link* link)


void NN_Node::AddOutput(NN_Link* link)
{
  if (o_link == NULL)
    o_link = new NN_Link_To_Link(link);
  else
    o_link->Add(link);
}   //void NN_Node::AddOutput(NN_Link* link)


int NN_Node::current_node = NN_START_NODE;  //inc'd to create new id's


//===========================================================================
class NN_Link
{
private:
  static int current_link;
  static NN_Link* base;
  static NN_Link* curr;

  int id;
  NN_Node* i_node;
  NN_Node* o_node;
  NN_TYPE weight;
  NN_TYPE delta;
  NN_Link* next;
  void Init(NN_TYPE _weight = 0, NN_TYPE _delta = 0);
public:

  int ID(void) { return id; }
  NN_Node* From(void)  { return i_node; }
  NN_Node* To(void)    { return o_node; }
  NN_TYPE Weight(void) { return weight; }
  NN_TYPE Weight(NN_TYPE _weight);
  NN_TYPE operator << (NN_TYPE _weight) { return Weight(_weight); }
  NN_TYPE Delta(void) { return delta; }
  NN_TYPE Delta(NN_TYPE _delta);

  void Join(NN_Node& in_node, NN_Node& out_node);

  NN_Link(void) { Init(); }
  NN_Link(NN_TYPE _weight, NN_TYPE _delta = 0) { Init(_weight,_delta); }

  friend int GroupLink(
    NN_Node* from, int from_count,
    NN_Node* to,   int to_count,
    NN_TYPE _weight = 0);
  friend ostream& operator << (ostream& o, NN_Link& l);
  friend void ShowLinks(void);
  friend void Fire(void);
};   //class NN_Link


void NN_Link::Init(NN_TYPE _weight, NN_TYPE _delta)
{
  id = current_link++;
  i_node = NULL;
  o_node = NULL;
  weight = _weight;
  delta  = _delta;
  if (base == NULL)
  {
    base = this;
    curr = this;
    next = NULL;
  }
  else
  {
    curr->next = this;
    curr = this;
    next = NULL;
  }
}   //void NN_Link::Init(NN_TYPE _weight, NN_TYPE _delta)


NN_TYPE NN_Link::Weight(NN_TYPE _weight)
{
  NN_TYPE old = weight;
  weight = _weight;
  return old;
}   //NN_TYPE NN_Link::Weight(NN_TYPE _weight)


NN_TYPE NN_Link::Delta(NN_TYPE _delta)
{
  NN_TYPE old = delta;
  delta = _delta;
  return old;
}   //NN_TYPE NN_Link::Delta(NN_TYPE _delta)


void NN_Link::Join(NN_Node& in_node, NN_Node& out_node)
{
  i_node = &in_node;
  o_node = &out_node;
  in_node.AddInput(this);
  out_node.AddOutput(this);
}   //void NN_Link::Join(Node& in_node, Node& out_node)


int NN_Link::current_link = NN_START_LINK;
NN_Link* NN_Link::base = NULL;
NN_Link* NN_Link::curr = NULL;


//===========================================================================
ostream& operator << (ostream& o, NN_Link& l)
{
  o << "Link " << l.ID()
    << ", node " << l.From()->ID() << "->" << l.To()->ID()
    << "  weight=" << l.Weight()
    << "  delta=" << l.Delta()
    << '\n';
  return o;
}   //ostream& operator << (ostream& o, NN_Node& n)


ostream& operator << (ostream& o, NN_Node& n)
{
  o << "Node " << n.ID()
    << " = " << n.Value()
    << " (" << n.Thresh() << ')'
    << '\n';
  NN_Link_To_Link* base;
  for (base = n.i_link; (base != NULL); base = base->next)
    cout << "  " << *(base->link);
  for (base = n.o_link; (base != NULL); base = base->next)
    cout << "  " << *(base->link);
  return o;
}   //ostream& operator << (ostream& o, NN_Node& n)


NN_Link& operator >> (NN_Node& from, NN_Node& to)
{
  NN_Link& l = *new NN_Link;
  l.Join(from,to);
  return l;
}   //NN_Link& operator >> (NN_Node& from, NN_Node& to)


int  GroupLink(
  NN_Node* from, int from_count,
  NN_Node* to,   int to_count,
  NN_TYPE _weight)
{
  NN_Link* link;
  int n = 0;
  int f,t;
  for (f = 0; (f < from_count); f++)
  {
    for (t = 0; (t < to_count); t++)
    {
      link = &(from[f] >> to[t]);
      if (link == NULL) return n;
      link->Weight(_weight);
      n++;
    }   //for
  }   //for
  return n;
}   //GroupLink


void ShowLinks(void)
{
  NN_Link* base = NN_Link::base;

  while (base != NULL)
  {
    cout << *base;
    base = base->next;
  }
}   //void ShowLinks(void)


void Fire(void)
{
  NN_Link* base = NN_Link::base;

  while (base != NULL)
  {
    if (base->i_node->value > base->i_node->thresh)
      base->o_node->value += base->weight * base->i_node->value;
    base = base->next;
  }   //while
}   //void Fire(void)


//===========================================================================

#define INPUTS 2
NN_Node in[INPUTS];
NN_Node hid(0,+1.5);
NN_Node out(0,+0.5);


void main(void)
{
  int i;

  cout << "========================================================\n";

  for (i = 0; (i < INPUTS); i++) in[i].Thresh(0.5);

  GroupLink(in,INPUTS,&hid,1,1.0);
  GroupLink(in,INPUTS,&out,1,1.0);
  (hid >> out).Weight(-2.0);

  cout << "Input nodes...\n";
  for (i = 0; (i < INPUTS); i++) cout << in[i];
  cout << "Hidden node...\n";
  cout << hid;
  cout << "Output node...\n";
  cout << out;

  ShowLinks();

  do
  {
    NN_TYPE v;

    hid.Value(0);
    out.Value(0);
    for (i = 0; (i < INPUTS); i++)
    {
      cout << "Enter input " << i << ": ";
      cin >> v;
      in[i].Value(v);
    }

    Fire();

    if (out.Value() > out.Thresh())
      cout << "Output is 1.\n";
    else
      cout << "Output is 0.\n";

    cout << "Press <ESC> to quit, or anything else to carry on.\n";
  } while (kbin.Get() != ESC);
}   //main
