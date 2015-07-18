
#include "network.h"

using namespace std;


////////fault equivalence //////////////////////////////////////////////////////
void netlist::equivalent_faults() // find equivalent faults for the netlist
{
    node *temp; // to scan each node;
    int count_fault;
    links *l1,*l2;
    fault *f;

    temp=start_node;

    while(temp!=NULL) // till all nodes are scanned
    {
        if(temp->type_of_node()==2) // gate node
        {
            if(strcmp(temp->gate_name(),"NAND")==0)// a.1.1.1
            {
                count_fault=0;
                l1=temp->fanout_net();
                l2=temp->fanin_net();
                f=l1->pointer_value()->fault_name();

                while(f!=NULL)
                {
                    if(strcmp(f->name(),"Sa1")==0) // for nand gate sa1 at the output pairs with sa0 at input
                    {
                        count_fault=1;
                        temp->check_equaivalence(l2,count_fault);
                        break;
                    }
                    f=f->next_value();
                }
                if(count_fault==0)
                {
                    temp->check_equaivalence(l2,count_fault);
                }
            }//a.1.1.1



            }//a.1.1
              temp=temp->next_value();
        }//while

    } // function ends

void node ::check_equaivalence(links *l2,int count_fault)
{
    int flag=-1;
    if(l2!=NULL)
    {
        fault *f;
        f=l2->pointer_value()->fault_name();

        flag=count_fault;

        while(f!=NULL)
        {
            if(strcmp(f->name(),"Sa0")==0)
            {
                count_fault++;
                check_equaivalence(l2->next_value(),count_fault);

            if(count_fault>1)
                //delete_fault(f,l2);
                l2->pointer_value()->modify_fault(&f,1);
            return;

            }//if
            f=f->next_value();
        }//while
      }//if

      if(flag==count_fault)
      {
          check_equaivalence(l2->next_value(),count_fault);
      }
}//func ends

void net::modify_fault(fault **f,int ind)
{
    if(ind==0) // delete all faults
    {
        fault **f1;
        fault *f2;


      if((*f)!=NULL)
        {
            f2=((*f)->next_value());
            f1=&f2;
            delete(*f);
            if(f1!=NULL)
            delete(*f1);
            _fault_name=NULL;
        }
    }
    else
    {
       if((*f)->next_value()!=NULL)
      {
        this->_fault_name=(*f)->next_value();
        delete (*f);
      }
      else
     {
        if(this->_fault_name==(*f))
        {
             this->_fault_name=NULL;
             delete(*f);
        }
        else
        {
            delete(*f);
            this->_fault_name->modify_next(NULL);
        }
      }
    }
}
///////////fault dominance//////////////////////////////////////////////////////////////////////////

void netlist::dominant_faults()
{
    //cout<<"entered dominant faults "<<endl;
 node *temp;
 temp=start_node;
 links *l2;
 net *t1;
 while(temp!=NULL)
 {
     //cout<<"entered dominant faults while loop 1"<<endl;
     if(temp->type_of_node()==1)//PO
 {
     l2=temp->fanin_net();
     while(l2!=NULL)
     {
     //  cout<<"entered dominant faults while loop 2"<<endl;
     t1=l2->pointer_value();
     if(t1->source_value()->type_of_node()==2)//gate node
     {
          fault *f;
          f=t1->fault_name();
          // delete all faults;
          t1->modify_fault(&f,0);
     }
     t1->source_value()->check_dominance();
     l2=l2->next_value();
    }
 }
 temp=temp->next_value();
 }

}

void node::check_dominance()
{
    int count_fault=0;
    links *l2;
    net *t1;
    fault *f;
    if(this->type_of_node()==2)// gate node
    {
        l2=this->fanin_net();

        while(l2!=NULL)
        {
   //         cout<<"entered dominant faults while loop 3"<<endl;
          t1=l2->pointer_value();

           if(t1->source_value()->type_of_node()==2)//source is a gate node
           {
               fault *f;
               f=t1->fault_name();
          // delete all faults;
              t1->modify_fault(&f,0);
           }
           else // if  the souce is IN or PI input
           {
               fault *f;
               f=t1->fault_name();

               if(strcmp(this->gate_name(),"NAND")==0)
               {
                    while(f!=NULL)
                    {
     //                   cout<<"entered dominant faults while loop 4"<<endl;
                        //getchar();
                        if(strcmp(f->name(),"Sa0")==0)
                        {
                            if(count_fault==1)
                            {
       //                         cout<<"enetered count_fault=1 cond"<<endl;
                                t1->modify_fault(&f,1);
                                break;
                            }
                            else
                                count_fault++;
                        }
                        f=f->next_value();
           //             cout<<t1->id_value();
                    }
         //             cout<<"out of loop 4"<<endl;
               }
           }
           l2=l2->next_value();
       }//while(l2!=NULL)

       l2=this->fanin_net();
       while(l2!=NULL)
        {
            //cout<<"entered dominant faults while loop 5"<<endl;
            t1=l2->pointer_value();
           t1->source_value()->check_dominance();
           l2=l2->next_value();
       }
    }
    else if(this->type_of_node()==3)// stem node
    {
        l2=this->fanin_net();
        t1=l2->pointer_value();
       if(t1->fault_name()!=NULL)
       {
       if(t1->source_value()->type_of_node()==2)//gate node
        {

          fault *f;
          f=t1->fault_name();
          // delete all faults;
          t1->modify_fault(&f,0);
        }
      t1->source_value()->check_dominance();
    }
    else  // the net has been traversed already
        return;
  }
  else if(this->type_of_node()==0)
    return;

}

