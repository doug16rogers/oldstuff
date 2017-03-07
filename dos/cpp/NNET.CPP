#include <stdiostr.h>
#include <iomanip.h>
#include <math.h>

#include "kb.hpp"


#ifndef NN_INPUTS
#define NN_INPUTS  4
#endif

#ifndef NN_OUTPUTS
#define NN_OUTPUTS 4
#endif

#ifndef NN_TYPE
#define NN_TYPE double
#endif

#define NN_FIRST_ID    0x100


class NN_Node;


typedef struct NN_Link_str
{
  NN_Node* i_node;
  NN_Node* o_node;
  NN_TYPE weight;
  NN_TYPE delta;
  struct NN_Link_str* next;
} NN_Link;


class NN_Node
{
private:
  int id;
  int i_size;
  int o_size;
  int i_count;
  int o_count;
  NN_TYPE value;
  NN_TYPE thresh;
  typedef NN_Node* Node_Pointer;
  Node_Pointer* i_list;
  Node_Pointer* o_list;
  NN_TYPE* o_weight;
public:
  static int current_id;
  static int input_size;
  static int output_size;
  static NN_TYPE (*net_function)(NN_TYPE);
  static NN_TYPE (*dnet_function)(NN_TYPE);

  int ID(void) { return id; }
  NN_TYPE Value(void) { return value; }
  NN_TYPE Value(NN_TYPE _value)
  {
    NN_TYPE old = value;
    value = _value;
    return old;
  }
  NN_TYPE Threshhold(void) { return thresh; }
  NN_TYPE Threshhold(NN_TYPE _thresh)
  {
    NN_TYPE old = thresh;
    thresh = _thresh;
    return old;
  }
  int InputSize(void)  { return i_size; }
  int OutputSize(void) { return o_size; }
  int Inputs(void)  { return i_count; }
  int Outputs(void) { return o_count; }

  NN_Node* Input(int i);
  NN_Node* Output(int i);
  NN_TYPE OutputWeight(int i);

  void Reset(void) { value = 0; }
  void Fire(void);

  NN_Node& operator >> (NN_Node& n);
  NN_Node& operator << (NN_TYPE _weight);

  NN_Node(int _inputs = NN_INPUTS, int _outputs = NN_OUTPUTS);
  ~NN_Node();
};   //class NN_Node


inline NN_Node* NN_Node::Input(int i)
{
  if (i >= i_count)
    return NULL;
  else
    return i_list[i];
}


inline NN_Node* NN_Node::Output(int i)
{
  if (i >= o_count)
    return NULL;
  else
    return o_list[i];
}   //NN_Node* NN_Node::Output(int i)


inline NN_TYPE NN_Node::OutputWeight(int i)
{
  if (i >= o_count)
    return 0.0;
  else
    return o_weight[i];
}   //NN_TYPE NN_Node::OutputWeight(int i)


void NN_Node::Fire(void)
{
  int i;
  for (i = 0; (i < o_count); i++)
  {
    o_list[i]->value += value * o_weight[i];
  }
  value = 0.0;
}   //void NN_Node::Fire(void)


inline NN_Node& NN_Node::operator >> (NN_Node& n)
{
  if ((o_count >= o_size) || (n.i_count >= n.i_size)) return *this;
  o_list[o_count++] = &n;
  n.i_list[n.i_count++] = this;
  return *this;
}   //NN_Node& NN_Node::operator << (NN_Node& n)


inline NN_Node& NN_Node::operator << (NN_TYPE _weight)
{
  if (o_count) o_weight[o_count-1] = _weight;
  return *this;
}   //NN_Node& NN_Node::operator << (NN_TYPE weight)


NN_Node::NN_Node(int _inputs, int _outputs)
{
  i_size = 0;
  o_size = 0;
  i_count = 0;
  o_count = 0;
  id = 0;

  i_list = new Node_Pointer[_inputs];
  if (i_list == NULL) return;
  i_size = _inputs;
  o_list = new Node_Pointer[_outputs];
  if (o_list == NULL) { delete i_list; return; }
  o_weight = new NN_TYPE[_outputs];
  if (o_weight == NULL) { delete i_list; delete o_list; return; }
  o_size = _outputs;
  id = current_id++;
}   //NN_Node::NN_Node(int _inputs, int _outputs)


NN_Node::~NN_Node()
{
  if (i_size) delete i_list;
  if (o_size)
  {
    delete o_list;
    delete o_weight;
  }
  i_size = 0;
  o_size = 0;
}   //NN_Node::~NN_Node()


int NN_Node::current_id  = NN_FIRST_ID;




ostream& operator << (ostream& o,NN_Node& n)
{
  o << "Node " << setw(3) << n.ID() << '\n';

  int i;
  for (i = 0; (i < n.Inputs()); i++)
  {
    o << "  <- node " << setw(3) << n.Input(i)->ID() << '\n';
  }
  for (i = 0; (i < n.Outputs()); i++)
  {
    o << "  -> node " << setw(3) << n.Output(i)->ID()
      << "  weight = " << n.OutputWeight(i) << '\n';
  }
  return o;
}   //ostream& operator << (ostream& o,NN_Node& n)



void main(void)
{
  NN_Node in1,in2;
  NN_Node hid,out;
  double v;

  cout << "======================================================\n";
  (in1 >> out) << +1.0;
  (in1 >> hid) << +1.0;
  (in2 >> hid) << +1.0;
  (in2 >> out) << +1.0;
  (hid >> out) << -2.0;
  hid.Threshhold(1.5);
  out.Threshhold(0.5);
  cout << in1 << in2 << hid << out;

  for (int i=0;i<4;i++)
  {
    out.Reset();
    hid.Reset();
    cout << "Enter input 1: ";
    cin >> v;
    in1.Value(v);
    in1.Fire();
    cout << "Enter input 2: ";
    cin >> v;
    in2.Value(v);
    in2.Fire();
    if (hid.Value() > hid.Threshhold()) hid.Fire();
    if (out.Value() > out.Threshhold())
      cout << "Output was 1.\n";
    else
      cout << "Output was 0.\n";
  }
  kbin.Get();
}   //main


