

#include "network.h"

using namespace std;


////////////////////////////////////////////////////////////////
int char_int_id (char *);
void dissect_info(char *, int, char *,char *);
int seperate_id(char *data);
int node_exist(char *data, node *prev_node,node *start_node, net **, node *n1);
int count_nets(net *asso_net, node *);
////////////////////////////////////////////////////////////////





int node_exist(char *data, node *prev_node,node *start_node, net** prev_net, node *n1)
{

    node *temp;
    int i;

    char name[10];

      for(i=0;data[i]!=' ';i++)
      {
       name[i]=data[i];
      }
       name[i]='\0';
     //cout<<"copied name suucesfully "<<name<<endl;

    temp=start_node;

    while(temp->_next!=n1)// node n1 is recently created and doesnot have a name of node.
    {
       // cout<<"name entered while loop "<<endl;
        if((temp->_node_type == 1)&&(strcmp(name,(temp->_name))==0))
       {
      //          cout<<"name and type matched "<<endl;

                prev_node=temp;
                *(prev_net)=(temp->_fanin_net->pointer_value());
        //        cout<<"prev_net->id_value() "<<(*(prev_net))->id_value();
            //    getchar();
                if(prev_net==NULL)
                cout<<"it is NULL in check function"<<endl;
                return 1;

        }
        temp=temp->_next;

    }
    //cout <<"returned with 0"<<endl;
    return 0;
}




void node::add_gate_inputs(char *ip,links *l1,node *gate, node* start_node, net *start_net, net *t1)
{
    //cout<<"adding input "<<ip<<endl;

    node *temp; // to search input ip in start_node list

    temp=start_node;

    while(temp!=NULL)
    {
        if(strcmp(ip,(temp->_name))==0)
        {
            net *net_ip;

            if(temp->_fanout_net==NULL)// case 1
            {
                net *cr_net;
                cr_net=new net;
                cr_net->create_net(temp,gate,1,start_net);

                links *last_links;// last link which will point to cr_net
                net *last_net;
                last_links = l1->last_link();

                last_links->create_links(cr_net);

                last_net=t1->last_net();

                last_net->modify_next(cr_net);
                links *new_l;
                new_l= new links;

                temp->_fanout_net= new_l;
                new_l->create_links(cr_net);

            }// case 1 bracket
            else //case 2
            {
                net_ip=temp->_fanout_net->pointer_value();
                node *temp_ip;

                temp_ip=net_ip->sink_value();

                if(temp_ip == NULL) // case 2.a
                {
                    net_ip->modify_net(0,gate);
                    links *last_links2;

                    last_links2=l1->last_link();

                    last_links2->create_links(net_ip);

                } // case 2.a closing bracket

            else //case 2.b
            {
                type case_node;
                case_node=temp_ip->type_of_node();
                int c=1;
               //   cout<<"case node"<<case_node<<endl;
                if(case_node==3) // IN node
                {
                    links *p;
                    p = temp_ip->_fanout_net->next_value();
                    while(p!=NULL)
                    {
                        c++;
                        p=p->next_value();
                    }
                }//in node case bracket
                if((case_node==1)||(case_node==2)||(c==1))//case 2.b.1
                {
                    node *m1, *last_node1;
                    links *last_links3;
                    net *t2;
                    m1=new node;
                    ////////here on 2:30 pm
                    last_node1=gate->last_node();
                    last_node1->_next=m1;

                    t2=new net;
//                     cout<<"entered case 2.b.1"<<endl;
//                    cout<<"node splitted is "<<temp->_name<<endl;
//                    ;
                    t2=m1->create_in_split(gate,net_ip,temp_ip,start_net);

                    last_links3=l1->last_link();
                    last_links3->create_links(t2);
                } //case 2.b.1 closing bracket
                else if (c>1) //case 2.b.2
                {
                    net *new_net1, *temp_last;
                    new_net1=new net;
                    temp_last = t1->last_net();
                    temp_last -> modify_next(new_net1);
                    new_net1->create_net(temp_ip,gate,4,start_net);

                    links *last_links4;

                    last_links4= l1->last_link();
                    last_links4->create_links(new_net1);

                } //case 2.b.2
            }
            }
        }
        temp=temp->next_value();
    }//while loop bracket

}
/// here at 2:50 pm


//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int netlist :: scan( char *info, char *data, char *gate_name)  // scans the line and provides data to create node
{
    int k,i;  // for loop(i) and k for data[k]
    char ch1;  // to update data till a detection is made
    char dummy[20]={'\0'};   // initialized
           for(i=0;info[i]!='\0';i++)       //started scanning info
    {


       if((info[i]=='(')||(info[i] == '='))    // if detected ( or = sign
    {

        //cout<<"detected ( or ="<<endl;
        //getchar();

        if(strcmp(dummy,"INPUT")==0)      // if line is of INPUT
        {
       //     cout<<"dummy is equal to INPUT"<<endl;
           // getchar();
            k=0;
            i++;
            do
              {
                ch1=info[i];
                data[k]=ch1; // input node name is stored in data
                i++;
                k++;
              }while(info[i]!=')');
              data[k]='\0';
               return 1;    // scaned a line in file and is recongnized as primary input line

        }

       else if(strcmp(dummy,"OUTPUT")==0)  //// if line is of OUTPUT
       {

           k=0;
           i++;
           do
            {
                ch1=info[i];
                data[k]=ch1;
                i++;
                k++;

           }while(info[i]!=')');
               data[k]='\0';
           return 2;

      }
    else  // detected = sign
    {

        dissect_info(dummy,i,info,data);

        strcpy(gate_name,dummy);       //transferring gate name to gate_name

        return 3;
    }


    } // bracket of detection of ( or = sign

     ch1=info[i];       // till a ( or = is detected
     dummy[i]=ch1;


    }// bracket of for



} // bracket of scan function
////////////////////////////////////////////////////////////////////////////
void dissect_info(char *dummy, int i, char *info, char *data)
{
    char ch1,ip1[10],ip2[10],input_node[10],gate_name[10];
    int j,k;
    strcpy(data, dummy); // name of node prior to = sign is stored in data
     //cout<<"name of the output node is "<<data<<endl<<dummy;
    for(j=i+1,k=0;info[j]!='(';j++,k++) //for loop for finding the name of gate
    {
        ch1=info[j];
        dummy[k]=ch1;

    }
    dummy[k+1]='\0';    //dummy has the gate name
    strcpy(gate_name,dummy);
    //cout<<"gate name is "<<gate_name<<endl;

   for(j=j+1,k=0;info[j]!=')';j++,k++)
   {
       if(info[j]==',')
       {
         input_node[k]='\0';
         strcpy(ip1,input_node);
          //reset a string
         k=-1;
       }
       else
       {
       ch1=info[j];
       input_node[k]=ch1;
       }
   }
   input_node[k]='\0';
   strcpy(ip2,input_node);

 //cout<<"two input node are ip1 and ip2 "<<ip1<<","<<ip2<<endl;

  strcat(data, " ");
  strcat(data, ip1);
  strcat(data, " ");
  strcat(data, ip2); // data for create_in_node
  //cout<<"data "<<data;
}

////////////////////////////////////////////////////////////////////////////
void netlist :: create_netlist(int value, char *data, int id, char *gate_name)
{
    node *n1, *temp,*g1;  // g1 for the creation of gate
    node *start;

    n1=new node; // dynamicalled created a pointer to object of node class;

    net *t1, *startnet;
    t1 = new net; // dynamicalled created a pointer to object of net class;

////same steps in creation of input, output and in nodes////////////////
    if(start_node==NULL)     //node created begins from this node
        {
                start_node=n1;
    //            cout<<"start_node is no more NULL"<<endl;
        }
        else
        {
            temp=n1;
            start=start_node;
            while(start->next_value()!=NULL)
            {
                start=start->next_value();
            }
            start->modify_next(temp);

        }
        ///////////////////////////////////////////////////////////////////////
    if(value==1) // create primary input node
    {
        ////////////////////////////////////
        if (start_net==NULL)
            {
                start_net=t1;
      //          cout<<" start_net is no more NULL"<<endl;

            }
            else
            {

                startnet=start_net;
                while(startnet->next_value()!=NULL)
                {
                    startnet=startnet->next_value();

                }
                startnet->modify_next(t1);
            }
/////////////////////////code copied on 1st june

     //   cout<< "creating primary input"<<endl;
        //cout<<data<<endl;
        //cout<<"id"<<id;
        id=char_int_id(data);
       n1->create_pi(data,id,t1); // sending the address of new net to be created after creating PI.
        ////
    } // if value = 1 closing bracket.
/////////////////////////////////////////////////////////
    else if(value==2)
    {
        ///////////////////////////////
        if (start_net==NULL)
            {
                start_net=t1;
        //        cout<<" start_net is no more NULL"<<endl;

            }
            else
            {

                startnet=start_net;
                while(startnet->next_value()!=NULL)
                {
                    startnet=startnet->next_value();

                }
                startnet->modify_next(t1);
            }
//////////////////////////////////////////////////////
               //  cout<<"creating primary output";
       //  cout<<"output id"<<id;
        id=char_int_id(data);
        n1->create_po(data,id,t1);

     }
///////////////////////////////////////////////////////////////////
       else if(value==3)
    {
           node *prev_node;
           net *prev_net;

           int check;
             //cout<<"value is 3"<<endl;
            //cout<<"data has "<<data<<endl;
            check = node_exist(data,prev_node,start_node,&prev_net,n1);
          //  cout<<"value of check is "<<check<<endl;
            if(check) // to check whether the fanout node already exist or not.
           { // the node will exist in only when case when it is a primary output node. In that case i need than
               //fanin net of output to be connected to the gate fanout net


            //  cout<<"output of gate node already created  "<<endl;
             // getchar();


                n1->create_gate_node(data,start_node,start_net,prev_net,0,gate_name);

            }
           else
            {

                ///////////////////////////////////////////

                if (start_net==NULL)
            {
                start_net=t1;
               // cout<<" start_net is no more NULL"<<endl;

            }
            else
            {

                startnet=start_net;
                while(startnet->next_value()!=NULL)
                {
                    startnet=startnet->next_value();

                }
                startnet->modify_next(t1);
            } // created and attached a new net to start_net list
//////////////////////////////////////////////////////////////
              id=seperate_id(data);
              //n1->create_in_node(data,gate_name,id,t1);// output of gate node created
      //       cout<<"output of gate node created"<<endl;

         //////create  a gate///////////////

          //g1=new node;
          //n1->modify_next(g1);
          n1->create_gate_node(data,start_node,start_net,t1,id,gate_name); //date 20 may 2015
          }
          // t1 is the fanout net of gate node
    /////////////////////////////create a gate node/////////////////////

    }

} // function end bracket

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void node::create_in_node(char *data, char *gate_name, int id, net *t1)
{
    int i;
   _id=id;
   // copy name to _name
   for( i=0 ; data[i]!=' ';i++)  // seperate node_name from data
   {
       _name[i]=data[i];     // _name[i] contains the node name of output of gate
   }

   _name[i]='\0';
   _node_type=intermediate_node;
   _delay=1.0;
   this->_next = NULL;
   this->_fanout_net=NULL;
   //this->_fanout_net=NULL;
   ////////////////////////////////////create a links object///////////////////
   links *l1;
   l1= new links;
   _fanin_net=l1;

   ///////////////////////create a new net from node//////////////////////////

    t1 -> create_net(NULL, this,3,NULL);
    l1->create_links(t1);
    //cout<<"in the create in node func"<<endl;
}
//

///////////////////////////////////////////////////////////////////////////////

void node::create_gate_node(char *data, node *start_node, net *start_net,net *t1,int id, char *gate_name)
{
   char op[10],ip[100],ch1;
   int i,k;
   net *net_temp;
   node *temp;
   //_id=1;
   this->_next = NULL;

  if(id==0)   // added on 16june
    _id=increment_gateid(start_node,this);
  else
  _id=id;

   strcpy(_gate_name,gate_name);// gate name
//  cout<<_gate_name;
//  cout<<endl;
//  getchar();

  _node_type=gate;   // node type is gate
  _delay=1.0;

  ////////////////////////////create links////////////////
   links *l1, *l2;
   int flag=0;
   l1= new links;
   l2= new links;
   _fanin_net=l1;
   _fanout_net=l2;
   int ind; //indicator of source or sink of net
   net *asso_net; //net to associated with gate;
////////////////////////////////////////////////////////////////////////

//  //cout<<"in create gate func"<<endl;
//   //getchar();
//   // copy name to _name
   for( i=0 ; data[i]!=' ';i++)
   {
       ch1=data[i];
       op[i]=ch1;
   }
   op[i]='\0';

   if(id==0)
   {
       strcat(op,"_gate");
   } // new gate format on 16 june is g8_gate

    strcpy(_name,op);  // gate given the name of output node on 16 june
   /////////////////////////////////////////
// not found ip1 and ip2
// scan the entire node list and associate the nets of op , ip1 and ip2 nodes with gate node.
//temp = start_node;

//////////////////create fanout net of gate/////////////////////////////////
if(id==0)
{
l2->associate_links(1,t1,this,start_net);

ind=1;// change source of the net

t1->modify_net(ind,this); // change source of the net
}
else
{
    t1->create_net(this,NULL,1,NULL);
    l2->create_links(t1);
}


//cout<<"net modified is "<<t1->id_value();
//cout<<"sink of the node is "<<t1->sink_value()->_name;
//cout<<"source of the node is "<<t1->source_value()->_name;
//getchar();

///////////////////////////////////////////////////////////////////////////


while(data[i]!='\0')  //separating all inputs here
{


    for(i=i+1,k=0;data[i]!='\0';i++,k++)
   {
       if(data[i]==' ')
        break;

       ch1=data[i];
       ip[k]=ch1;

   }
   ip[k]='\0';


 add_gate_inputs(ip,l1,this,start_node,start_net,t1); //added on 29 may 2015

}
}


net * node::create_in_split(node *gate,net *net_ip, node *temp_ip,net *start_net)
{
    _id=1;// dummy id for in node;
    strcpy(_name,"Stem1");  // dummy name for in node;
    _node_type=intermediate_node;
/////////////////create links for fanin and fanout///////////
links *l1, *l2;
 l1=new links;
 l2=new links;

_fanin_net=l1;
_fanout_net=l2;

node *temp;// to store sink of old net;

temp=net_ip->sink_value();
net_ip->modify_net(0, this);// changed the sink of net_ip
l1->create_links(net_ip);
//cout<<"chnage the net_ip sink "<<endl;
///////create a fanout net//////////////////
net *t1, *temp_net;
t1=new net;

temp_net=start_net;

while(temp_net->next_value()!=NULL)
{
 temp_net=temp_net->next_value();
}
temp_net->modify_next(t1);

t1->create_net(this,temp,4,start_net);
/////////////////////change the temp_ip fanin net to t1/////////////////
links *l4_ip;
l4_ip=temp_ip->_fanin_net;

while(l4_ip!=NULL)
{
    if(l4_ip->pointer_value()==net_ip)
    {
        l4_ip->associate_links(2,t1,temp_ip,start_net);
        break;
    }
    else
        l4_ip=l4_ip->next_value();
}



////////////////////////////////////////////////////////////////////////
l2->create_links(t1);


/////////////////////create another net connected to gate node//////////////
net *t2;
t2=new net;

t1->modify_next(t2);

t2->create_net(this,gate,4,start_net);
//////////create links l3//////////

links *l3;
l3=new links;

l3->create_links(t2);

l2->modify_next(l3);


return t2;

} // end of function

void net::modify_net(int ind, node *temp)
{ //  cout<<"in modify net"<<endl;
    if (ind==1)
    {

        this->_source=temp;


    }
    else if(ind==0)
    {
        this->_sink=temp;

    }/////still incomplete
}


void node::create_po(char *name, int id, net *t1)
{
    int i;
   _id=id;
   net *temp;
   //cout<<"in create po func"<<endl;
//   getchar();
   // copy name to _name
   for( i =0 ; name[i]!='\0';i++)
   {
       _name[i]=name[i];
   }
   _name[i]='\0';
   _node_type=output;
   _delay=1.0;
   this->_next = NULL;
   this->_fanout_net=NULL;
   //this->_fanout_net=NULL;
   ////////////////////////////////////create a links object///////////////////
   links *l1;
   l1= new links;
   _fanin_net=l1;

  //////////////////////////create a new net towards the node////////////
    t1 -> create_net(NULL, this,2,NULL);// added NULL on 21 may for startnet
    //cout<<"in create po func ti id value is "<<t1->id_value();

    l1->create_links(t1);
    //cout<<"in create po func ti id value is "<<t1->id_value();

}
/////////////////////////////////////////////////////////////////////////
void node::create_pi(char *name, int id,net *t1)// creats node, net and links
{
    int i;
   _id=id;
   // copy name to _name
   for( i = 0 ; name[i]!='\0';i++)
   {
       _name[i]=name[i];
   }
   _name[i]='\0';
   _node_type=input;
   _delay=1.0;
   this->_next = NULL;
   this->_fanin_net=NULL;
   //this->_fanout_net=NULL;
   ////////////////////////////////////create a links object///////////////////
   links *l1;
   l1= new links;
   _fanout_net=l1;

   /////////////////////////////////////////////////create a new net from node////////////

    t1 -> create_net(this, NULL,1,NULL);
    l1->create_links(t1);
}
////////////////////////////////////////////////////////////////////
    void links::create_links(net *t1)
    {
        this->_pointer = t1;
        this->_next = NULL;
    }

//////////////////////////////////////////////////////////
void net::create_net(node *sr, node *sk,int value,net *start_net)
{
    //cout<<"in func create_net"<<endl;
    _source=sr;
   _sink=sk;
   _delay=1.0;
   this->_next=NULL; // added on 20 may
    if(value ==1)
    {
        _id=sr->node_id();


    }
    else if((value == 2)||(value==3))  // in case of output node
    {
        _id=sk->node_id();
        //_id=increment_netid(start_net);// modified on 1st june





    }
   else if(value==4) // net created after splitting a net to connect a gate
   {
       _id=increment_netid(start_net);  // here at 4:00pm on 20th may, assigned maximum net value to _id



   }

}



void links::associate_links(int no, net *asso_net, node *gate, net *start_net) // function called by gates _fanin_net /_fanout_net elements
{
    int count_fanout;

    if (no==1) // associate fanout link to gate.
    {
        this->_pointer=asso_net;
        this->_next=NULL;
    }

    if (no==2) // change the fanin net of gate after creating a stem node
    {
        this->_pointer=asso_net;
    }


}




///////////////////////////////////////////////////////////////////////
int seperate_id(char *data)
{
    char sep[10],ch1;
    int i,id;
    //cout<<"in seperate_id func"<<endl;
    for(i=0;data[i]!='\0';i++)
{
    if(data[i]==' ')
    {

        sep[i]='\0';
        id = char_int_id(sep);
      //  cout<<"returned from char_int_id func"<<endl;
        return id;
    }

    ch1=data[i];
    sep[i]=ch1;
}

}


int char_int_id(char *data)   // converts char to int.
{
    //cout<<"data in char_int_id "<<data<<endl;
    int sum=0;
    int t=0;
    float z;
    int len=0;
    int l;
    while(data[t]!='\0')
    {
        len++;
        t++;
    }
    l=len-2;
    for(int m = 1 ; data[m]!='\0';m++)
    {     while(l>=0)
         {

          t = (data[m]-'0');
          z = pow(10,(l));

          l--;
          m++;
          sum=sum+(t*z);


         }
    }

     return sum;
}
////////////////////////////////////////////////////////
