#include "network.h"

using namespace std;


///////////evaluation of netlist////////////////////////////////////////////
void netlist::assign_inputs(char *info)
{
   node *temp;
   int i=0;
   links *l1;
   temp=start_node;

           while(temp->_node_type==0)  // only for input nodes
       {
             for(i=0;info[i]!='\0';i++)
             {

               temp->_value=info[i]-'0'; // assigning value to the node
               l1=temp->_fanout_net;
             while(l1!=NULL) // all fanout nets of input nodes
            {
               l1->_pointer->_value=info[i]-'0';
               l1=l1->_next;
            }

               temp=temp->_next;
           }

     }

          cout<<"reached out of while loop"<<endl;
          cout<<"temp ->name"<<temp->_name;
//           if((temp->_node_type==0)&&(info[i]=='\0'))
//       {
//           cout<<" sufficient input values are not provided"<<endl;
//           getchar();
//
//       }



  node *op1, *op2;
  links *l3;
  net *t1;

       op1=temp;
       op2=temp->_next;



  while((op1->_value==-1)||(op2->_value==-1))// till output does not get valid values
  {
       op1->_value=func(op1);
       cout<<"output of op1->_name "<<op1->_name<<" "<<op1->_value<<endl;
       op2->_value=func(op2);
       cout<<"output of op2->_name "<<op2->_name<<" "<<op2->_value<<endl;

  }
}


int netlist::func(node *op)
{
    getchar();
    int flag=0;
    links *l3;
    net *t1;
    int value;
    int check=0;
       l3=op->_fanin_net;

cout<<"name of op "<<op->_name<<endl;
cout<<" name of fanin of op"<<op->_fanin_net->_pointer->_source->_name<<endl;
if(op->_fanout_net!=NULL)
    cout<<" name of fanout of op"<<op->_fanout_net->_pointer->_sink->_name<<endl;
getchar();

       while(l3!=NULL)
       {
          t1=l3->_pointer;
          if(flag==1)
          {
              cout<<"entered here after changing l3 in if block"<<endl;
              cout<<"t1 value is"<<t1->_value<<endl;
              cout<<"t1 source is "<<t1->_source->_name<<endl;
          }
          if(t1->_value==-1)
          {
            value=func(t1->_source);
              if(t1->_source->_node_type!=2)//gate
              {
                  t1->_value=t1->_source->_value=value;

              }
              else if(t1->_source->_node_type==2)
              {
//                  l3=l3->_next;// try commenting it
//                  if(l3!=NULL)        // commented on 16 june 6:40pm
//                  {
//                      continue;
//                  }
                  (t1->_source)->eval();
                  //t1->_value=op->_value=t1->_source->_value;
                  t1->_value=t1->_source->_value;
                  cout<<"after evaluation gate value is "<<t1->_source->_value<<endl;
                  cout<<"node evaluated is "<<t1->_source->_name<<endl;
                  check=1;

              }

          }
          if(l3!=NULL)
          {
              flag=1;
              cout<<"enetred here for sink"<<l3->_pointer->_sink->_name<<endl;
              cout<<"enetred here for source"<<l3->_pointer->_source->_name<<endl;
              l3=l3->_next;
              if(l3==NULL)
                cout<<"L3 is NULL"<<endl;
              else
                    cout<<"not NULL"<<endl;

          }

       }//while
//       if(check)
//        return (op->_value);
//       else
        return(t1->_value);

}


void node::eval()
{
    cout<<"reached gate evaluation for node"<<this->_name<<endl;
    cout<<"gate fanin nodes are"<<this->_fanin_net->_pointer->_source->_name<<endl;
    cout<<this->_fanin_net->_next->_pointer->_source->_name<<endl;

    getchar();

    links *l1;
    net *t1;

    l1=this->_fanin_net;

    while(l1!=NULL)
    {
        t1=l1->_pointer;
        cout<<"in the evaluation t1 value is "<<t1->_value<<endl;
        if(t1->_value==0)
        {
            this->_value=1;
            return;

        }
        l1=l1->_next;
    }
      this->_value=0;
}
