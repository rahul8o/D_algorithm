


#ifndef _NETWORK_H_
#define _NETWORK_H_
#include <iostream>
#include<fstream>  // to perform i/o in a file
#include<cstring>
#include<string.h>
#include<math.h>

using namespace std;


enum type { input, output, gate, intermediate_node }; // defined diff types of node in a netlist
class net;  //forward declaration of class
class node;


class Decision_queue{
node *n1;
int tried;
Decision_queue *next;
public:

     void add(node *n)
    {
        n1=n;
        tried=0;
        this->next=NULL;
    }
    void assign_next(Decision_queue *d)
    {
        this->next=d;
    }
    Decision_queue* next_value()
    {
        return this->next;
    }
    void remove_node(Decision_queue **temp,Decision_queue **temp1);
    void empty_queue(Decision_queue **temp);//erase dfrontier and Jfrontier queue
   int tried_value()
    {
        return this->tried;
    }
    void assign_tried(int task,int value,Decision_queue *temp);
    void display();//called by dfrontier
    node * node_value()
    {
        return n1;
    }

};
class impli_stack;

class netlist{
node *start_node;      // address of first node in linklist node
net *start_net;        // address of first net in linklist net
impli_stack *top;
Decision_queue *Dfrontier;
Decision_queue *Jfrontier;
public:
    netlist()
    {
        start_node=NULL;
        start_net=NULL;
        top=NULL;
        Dfrontier=NULL;
        Jfrontier=NULL;
    }
    int scan(char *info, char *data, char *);
    void create_netlist(int, char *,int,char *);
    void count_nodes_nets();         // count number of nodes
    void display_connection();
    void set_value();
    void assign_inputs(char *);
    void assign_values();
    net * start_net_value()
    {
        return this->start_net;
    }
    void display_all_nets();
    void display_gate_info();
    int func(node *);
    void equivalent_faults();// added on 17 june
    void dominant_faults();//added on 18 june
    void tg(); // test generation function
    int assign_check(net *t1, node *n1,int v,int dir, int ass,int act);// assigns and checks contradiction..added on 5th june
    int implication(net *t1,node *n1, int v,int dir,int ass); // implication both backward and forward
    //void push(net *t1, node *n1, int v, int ass,impli_stack *top);// pushes the value on implication stack
    //void pop(impli_stack *top);// pos element from implication stack
    int propagate(node *n2);
    int Jsolve();// addded on 12 july
    void erase_all();
    ~netlist(){}
};
class links;

class node{
int _id;
char _name[10];
type _node_type;
links *_fanout_net;       // address of fanout net
links *_fanin_net;        // address of fanin net
float _delay;
node *_next;             //next node in netlist
int _value;
char _gate_name[10];
public:
    node()
    {
        _value=-1;
        strcpy(_gate_name,"\0");
    }
    void eval();
    void create_pi(char *name, int id, net*);
    void create_po(char *name, int id, net*);
    void create_in_node(char *, char *, int, net *);
    void create_gate_node(char *data,node *, net *, net *,int,char*);
    void add_gate_inputs(char *ip,links *l1,node *gate, node* start_node, net *start_net, net *t1);
    net * create_in_split(node *,net *,node*, net *);
    node * last_node();
    int node_id(){return _id;}
    node* next_value(){return this->_next;}
    int increment_gateid(node *start_node, node *);
    friend int count_nets(net *asso_net, node *);
    friend int node_exist(char *, node *, node *,net **,node *n1);
    friend void netlist::assign_inputs(char *);
    friend int netlist::func(node *);
    friend void netlist::assign_values();
    friend void netlist::display_gate_info();
    void modify_next(node *temp)
    {
        this->_next=temp;

    }
    void check_equaivalence(links *l2,int count_fault);
    void check_dominance();
    friend void netlist::display_all_nets();
    type type_of_node(){return _node_type;}
    friend void netlist::set_value();
    void display_node_info();
    char* gate_name(){return _gate_name;}
    links* fanout_net(){return _fanout_net;}
    links* fanin_net(){return _fanin_net;}
    int value(){return _value;}
    void assign_value(int *v){_value=*v;}

    int type_gate(){

                   if(strcmp(_gate_name,"NAND")==0)
                    {
                        return 1;
                    }
                    else if(strcmp(_gate_name,"NOR")==0)
                            {
                                return 2;
                            }
                    else if(strcmp(_gate_name,"AND")==0)
                            {
                                return 3;
                            }
                    else if(strcmp(_gate_name,"OR")==0)
                            {
                                return 4;
                            }
                    else if(strcmp(_gate_name,"NOT")==0)
                            {
                                return 5;
                            }
                   }
    ~node(){}
};

class links{
links *_next;
net *_pointer;
public:
    links()
    {
        this->_pointer=NULL;
        this->_next=NULL;
    }
    friend void node::eval();
    void create_links(net *);
    friend void netlist::display_gate_info();
    void associate_links(int no, net *asso_net, node *gate, net *start_net);
    links* next_value(){return this->_next;}
    net* pointer_value(){return this->_pointer;}
    links * last_link();
    friend int netlist::func(node*);
    friend void netlist::assign_inputs(char *);
    friend void netlist::assign_values();
    void modify_next(links *temp)
    {
        this->_next=temp;

    }
    ~links(){}
};

class fault //added on 12 june 2015
{
    char _name[4];
    fault *_next;
public:
    fault(){}
    fault(fault *f1)
    {
        strcpy(f1->_name,"Sa0");
//        cout<<f1->_name;
        f1->_next=this;
        strcpy(this->_name,"Sa1");
//        cout<<this->_name;
        this->_next=NULL;

    }
    void display_name()
    {   cout<<"name of fault on the net are "<<_name;
        cout<<endl;
    }
    char* name(){return _name;}
    fault* next_value(){return _next;}
    void modify_next(fault *f){this->_next=f;}
    ~fault(){}

};



class net{
    int _id;
    node *_source;
    node *_sink;
    float _delay;
    net *_next;
    int _value;
    fault *_fault_name; // added on 12 june
public:
    net()
    {
        _value=-1;
        fault *f1,*f2;
        f1=new fault;
        f2=new fault(f1);
        _fault_name=f1;


    }
    friend void node::eval();
    net* next_value(){return this->_next;}
    net * last_net();
    void modify_next(net *temp)
    {
        this->_next=temp;
        temp->_next=NULL;

    }
    void modify_fault(fault **f,int ind);
    int id_value(){return _id;}
    void modify_net(int ind, node *);
    void create_net(node *sr, node* sk,int value,net *); // returns net address which is stored in fanout_net of node object
    node * sink_value(){return this->_sink;}
    node * source_value(){return this->_source;}
    void display_net_info(int);
    friend void netlist::display_gate_info();
    friend void netlist::assign_inputs(char *);
    int increment_netid(net *);
    friend int count_nets(net *asso_net, node *);
    friend void netlist::set_value();
    friend void netlist::display_all_nets();
    friend int netlist::func(node *);
    friend void netlist::assign_values();
    int value(){return _value;}
    void assign_value(int *v){ _value=*v;}
    fault* fault_name(){return _fault_name;}
~net(){}
};

class impli_stack{
net *net_addr;
node *node_addr;
int old_value;
int element_type;
impli_stack *prev;
//impli_stck *top;
//impli_stck *prev;
public:
    impli_stack()
    {
        net_addr=NULL;
        node_addr=NULL;
        old_value=-1;
        element_type=-1;
        prev=NULL;
    }
    void push(net *t1,node *n1, int value, int ass,impli_stack **top);
    //void pop(impli_stack *top);
    void pop(impli_stack **top);
    void pop_all(impli_stack **top,int task);
    void display(impli_stack *top)
    {
        if(top==NULL)
        {

             cout<<"stack is empty"<<endl;
             return ;

        }
        //cout<<"reached here"<<endl;
        cout<<top->old_value;
        cout<<endl;
        if(top->element_type==0)//node
        { // cout<<"reached in if case"<<endl;
           top->node_addr->display_node_info();
        }
        else if(top->element_type==1)
        {
            cout<<"reached here in else if "<<endl;
            top->net_addr->display_net_info(1);

        }
        //cout<<"made an exit from stack display"<<endl;
    }

};


#endif
