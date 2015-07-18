

#include "network.h"

using namespace std;

int netlist::propagate(node *n2)// assign non controlling values to rest of the nets
{
           int case_gate;
           int status;
         //cout<<"entered the propagation function"<<endl;
           case_gate=n2->type_gate();
           switch(case_gate)
           {
              case 1 :
           //            cout<<"gate is nand"<<endl;
                       //nand gate
                       links *l1;
                       l1=n2->fanin_net();
                       while(l1!=NULL)
                       {
                           if(l1->pointer_value()->value()==-1)// for unknown quantity
                           {
                               /////net info ////////////
             //                  cout<<"assigning a non controlling value on net"<<endl;
                               //l1->pointer_value()->display_net_info(1);
                               ////////////////////////////

                               status=assign_check(l1->pointer_value(),NULL,1,0,1,0);//backward
               //                cout<<"29***********************"<<endl;
                               if(status==0)
                               {
                                   return 0;
                               }
                               else if (status==1)
                               {
                                   //status=assign_check(l1->pointer_value(),NULL,1,1,1,0);//forward blocked on 11 july
                                   // only check the forward implication of assignment on net do not call a assign_check
                                   // function on that net in forward direction       // on 11 july
                 //                  cout<<"backward implication succesfull ....now forward implication on net"<<endl;
                                   //l1->pointer_value()->display_net_info(1);
                                   status=implication(l1->pointer_value(),NULL,1,1,1);// forward implication
                   //                cout<<"30***************************"<<endl;
                               /////////////////////////////////////////////////////////////


                               ///////////////////////////////////////////////////////////////
                                   if(status==0)
                                   {
                                       return 0;
                                   }
                               }
                           }//value==-1
                          else if (l1->pointer_value()->value()==0)
                          {
                     //         cout<<"31**************************"<<endl;
                              return 0;
                          }
                         l1=l1->next_value();
                       }//while(l1!=NULL)
                       //reached here means it survived a return 0 on all the nets
                       return status;// if its is 3 then a error propagetd to output//here on 11 july
                       break;
                       default:break;
           }//switch
}//function

void Decision_queue::assign_tried(int task,int value,Decision_queue *temp)
    {   if(task==1)
        this->tried=value;
        else if(task==0)
        {
            //tried all the nodes to 0;
            while(temp!=NULL)
            {
                temp->tried=0;
                temp=temp->next;
            }
        }

    }

void Decision_queue::remove_node(Decision_queue **temp,Decision_queue **temp1)
{
   if(this==(*temp))//Dfrontier node is to be removed
   {
       (*temp1)=(*temp)->next;
        delete(*temp);
   }
   else
   {
       Decision_queue *t;
       while(t->next!=(*temp))
       {
          t=t->next;
       }
       if(t->next==(*temp))
       {
           t->next=(*temp)->next;
           delete (*temp);
       }

   }
}

void Decision_queue::empty_queue(Decision_queue **temp)//erase dfrontier and Jfrontier queue
{
    Decision_queue *temp1;

    while((*temp)!=NULL)
    {
        temp1=*temp;
        (*temp)=temp1->next;
        delete temp1;
    }
}


void netlist::erase_all()
{
   net *erase_net;
   node *erase_node;

   //assign -1 to all nets
   erase_net=start_net;
   while(erase_net!=NULL)
   {   int value;
   value=-1;
       erase_net->assign_value(&value);
       erase_net=erase_net->next_value();
   }
   erase_node=start_node;
   while(erase_node!=NULL)
   {
       int value;
       value=-1;
       erase_node->assign_value(&value);
       erase_node=erase_node->next_value();
   }
}
