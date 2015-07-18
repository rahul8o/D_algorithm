
#include "network.h"

using namespace std;


int netlist::Jsolve()
{
    Decision_queue *tempj;
    node *nj;
    int case_gate;
    int status,flag;
    tempj=Jfrontier;
    while(tempj!=NULL)
    {

//        cout<<"Pick a node from Jfrontier"<<endl;

        nj=tempj->node_value();

  //      cout<<"the type of gate "<<endl;
        case_gate=nj->type_gate();

        switch(case_gate)
        {
        case 1: //nand gate
                int op_value;
                op_value=nj->fanout_net()->pointer_value()->value();// value of gate output net
                if(op_value==1)// if the value is 1
                {

                    links *l1;
                    l1=nj->fanin_net();
                    //a while loop to check if there is a input with value 0
                    while(l1!=NULL)
                    {
                        net *t1;
                        t1=l1->pointer_value();
                        if(t1->value()==0)
                        {
                            return 1;
                        }
                        l1=l1->next_value();

                    }
                    l1=nj->fanin_net();
    //                cout<<"scanning all input nets of gate nj"<<endl;
                    while(l1!=NULL)
                    {
                        net *t1;
                        t1=l1->pointer_value();
                        if(t1->value()==-1)// unknown value of an input net
                        {

                            status=assign_check(t1,NULL,0,0,1,0);
                            if(status==1)// succesfully assigned the value on the net
                            {
                              Jfrontier->remove_node(&tempj,&Jfrontier);//removed the node from J frontier
                              tempj=Jfrontier;// picked a new node from Jfrontoer
                              flag=1;//suucessfully solved one node from jfrontier
                              break;//go outside while loop....scanning of input is over
                            }
                            else if(status==0)
                            {
                                //reverse decision
                                status=assign_check(t1,NULL,1,0,1,0);
                                if(status==0)
                                {
                                    //test failed
                                    return 0;
                                }
                                l1=l1->next_value(); // now check next net for assignment
                            }//status=0

                        }//net value is -1
                    }//while l1!=NULL
                    if(flag!=1)//not able to assign a value 0 to input not even once
                    {
                        return 0;//test failed
                    }

                }//if(op_value==1) value has to be one it can not be zero
                 break;
                 default:break;
        }//switch
    }//while
    return 1;// all the jfrontier solved succesfully
}//function
