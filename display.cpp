
#include "network.h"

using namespace std;

void node::display_node_info()
    {
        // added on 8 th june

        int count_fanouts=0;
        links *temp;
        temp=_fanout_net;
        cout<<"Node Name -->  ";
        cout<<_name<<endl;
        cout<<"Id of the node is "<<_id<<endl;
        cout<<"This node is a "<<_node_type<<endl;
        cout<<" This Node value is "<<_value<<endl;
        while(temp!=NULL)
        {
            count_fanouts++;
           temp=temp->next_value();

        }
        cout<<"this node is connected to "<<count_fanouts<<" number of nets"<<endl;
        temp=_fanout_net;
        while(temp!=NULL)
        {
             (temp->pointer_value())->display_net_info(count_fanouts);
              cout<<" this net is connected to node "<<(temp->pointer_value())->sink_value()->_name<<endl;
              temp=temp->next_value();

        }



    }
//////////////////////////////////////////////////////////////////////////
void net::display_net_info(int count_fanouts)
    {

       cout<<"this node is connected to ";
       cout<<"net id "<<_id<<endl;
       cout<<"the net value is "<<_value<<endl;
       fault *f;
       f=_fault_name;
       while(f!=NULL)
       {
           f->display_name();
           cout<<endl;
           f=f->next_value();
       }
    }

////////////////////////////////////////////////////////////////////////////
void netlist :: display_connection()
{
     node *N1;

     N1= start_node;

     cout<<"Displaying the list of nodes along with their connections "<<endl  ;

     while(N1!=NULL)
     {
        N1->display_node_info();
        N1=N1->next_value();
     }
}

void netlist::count_nodes_nets()
{
    node *N1;
    net *N2;

    int count_nodes=0;
    int count_nets =0;
    int input_count=0, output_count=0;

     N1=start_node;
     N2=start_net;

    while(N1!=NULL)
    {
        if(N1->type_of_node()==input)
      {
          input_count++;

      }
      else if(N1->type_of_node()==output)
      {
          output_count++;

      }

      count_nodes++;
      N1=N1->next_value();
    }

    while(N2!=NULL)
    {
        count_nets++;
        N2=N2->next_value();
    }

    cout<<"Total "<<count_nodes<<" nodes are created"<<endl;
    cout<<"Total "<<input_count<<" input nodes are created"<<endl;
    cout<<"Total "<<output_count<<" output nodes are created"<<endl;
    cout<<"Total "<<count_nets<<" nets are created"<<endl;
/////
//////////////count input and output nodes///////////////////////////////////

}

void netlist::display_all_nets()
{

     net *temp;

     temp=start_net;
     int count=0;
     while(temp!=NULL)
     {   count ++;
         cout<<" count is "<<count<<endl;
         cout<<"id of the net is "<<temp->_id
             <<endl
             <<"source of the net is "<<endl;

             if(temp->_source!=NULL)
                cout<<temp->_source->_name;
             else
                cout<< "NULL"<<endl;

             cout<<"Sink of the net is "<<endl;

             if(temp->sink_value()!=NULL)
                cout<<temp->_sink->_name<<endl;
             else
                cout<<"NULL "<<endl;

              getchar();
              temp=temp->_next;
     }


}

int count_nets(net *asso_net,node *temp_node)
{   // counting number of nets connected to sink of asso_net
    int count_fanout=0;
    links  *temp_net;
//    node *temp_node;
//    temp_node = asso_net->sink_value();

    temp_net = temp_node->_fanout_net;

    if(temp_net==NULL)
        return count_fanout;

    else
    {
       count_fanout=1;
    temp_net=temp_node->_fanout_net->next_value();
    while(temp_net!=NULL)
    {
        count_fanout++;
        temp_net = temp_net->next_value();

    }
    return count_fanout;
    }
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//
///////////////////////////////////////////////////

void netlist::display_gate_info()
{
    node *temp;
    links *l1,*l2;
    int counts=1;

    temp=start_node;

    while(counts<=6)
    {
        if(temp->_node_type==2)
        {
            counts++;
            cout<<"the id of the gate is "<<temp->_id<<endl;
            cout<<"gate fanin are "<<endl;
            l1=temp->_fanin_net;
            while(l1!=NULL)
            {
                cout<<l1->_pointer->_source->_name<<endl;
                getchar();
                l1=l1->_next;
            }

            cout<<"gate fanout are "<<endl;
            l2=temp->_fanout_net;
            while(l2!=NULL)
            {
                cout<<l2->_pointer->_sink->_name<<endl;
                getchar();
                l2=l2->_next;
            }

        }
        temp=temp->_next;
    }

}

void Decision_queue::display()
{
        Decision_queue *temp;

        temp=this;
        while(temp!=NULL)
        {
           temp->n1->display_node_info();
            temp=temp->next;
        }

}
