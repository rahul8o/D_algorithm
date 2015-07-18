

#include "network.h"

using namespace std;


//incrementing id of net and node and also incrementing the node and net is linked list

links * links :: last_link()// is to find the _fanin or fanout nets last links block
{

   if(this->_pointer==NULL) //this is an empty links block
   {
       return this;
   }
   else
   {

      links *last, *newlink;
      last = this;

       while(last->_next!=NULL)
       {
           last=last->_next;

       }
       newlink=new links;
       last->_next= newlink;
       return newlink;
   }

}

node * node::last_node()
{
    if(this->_next==NULL)
    {
        return this;
    }
    else
    {
       node *last;
       last=this;
       while(last->_next!=NULL)
       {
           last=last->_next;

       }
       return last;
    }
}


net * net::last_net()
{
    if(this->_next==NULL) //this is an empty links block
   {
       return this;
   }
   else
   {
       net *last;
       last=this;
       while(last->_next!=NULL)
       {
           last=last->_next;

       }
       return last;
   }
}

int node::increment_gateid(node *start_node, node *gate)
{
    node *temp;
    int idmax=1;
    int flag=0;

    temp=start_node;

    while(temp->_next!=NULL)
    {
        if(temp->_node_type==2)
        {
            flag=1;
            if(idmax<temp->_id)
            {
                idmax=temp->_id;
            }
        }
       temp=temp->_next;
    }
    if(flag)
       {
        return (idmax+1);
       }
    else
        return 1;

}



int net::increment_netid(net *start_net)
    {

        net *temp, *temp1;
        int max_id,id1,no;
        temp=start_net;
        temp1=temp->next_value();


        max_id=temp->id_value();

        while(temp1->next_value()!=NULL)
        {
              id1=temp1->id_value();
  //            cout<<"comparing max_id with "<<max_id<<" id1 "<<id1<<endl;
            if(max_id < id1)
            {
                max_id=id1;

            }


            temp1=temp1->next_value();

        }
        no=max_id+1;
//        cout<<"new max_id value is "<<no<<endl;


        //cout<<"max id is "<<no<<endl;
        return no;
    }



/////////////////////////////////////////////////////////////////////
