

#include "network.h"

using namespace std;


void netlist::tg()
{
    //cout<<"entered Test generation function"<<endl;
    net *temp_net;    //scan each net
    char *temp_fault; //  name of the fault Sa0 or Sa1
    int status; // check status of assigning values to fault
    int status_prop;
    int test_status;
    temp_net=start_net; // scan each and every net and its fault

    while(temp_net!=NULL)
    {

        fault *f1;
        int v;

   f1=temp_net->fault_name(); // fault on the temp_net

   if(f1==NULL)
   {
       temp_net=temp_net->next_value();
       continue;
   }
   else //if(f1!=NULL)    //if a fault exist
   {
        while(f1!=NULL)//
     {
         ////////////////////////////////////////Initialization Block/////////14 july
          erase_all();// assigns all net and node to a value of -1
          //empty Dfrontier and Jfrontier
         if(Dfrontier!=NULL)
          Dfrontier->empty_queue(&Dfrontier);
         if(Jfrontier!=NULL)
         Jfrontier->empty_queue(&Jfrontier);
         /////////////////////////////////////////////////////////////////////////////
       temp_fault=f1->name(); // store the fault name if the fault in temp_fault
       //cout<<"picked up the first fault to test --"<<f1->name()<<endl;
       //getchar();
       if(strcmp(temp_fault,"Sa0")==0)
       {
          v=1;  // activate the fault by assigning 1 on temp_net
       //   cout<<"activating fault Sa0 by setting one on line--"<<temp_net->id_value()<<endl;
       }
          else if (strcmp(temp_fault,"Sa1")==0)
       {
         v=0; // // activate the fault by assigning 0 on temp_net
         //cout<<"activating fault Sa1 by setting zero on line--"<<temp_net->id_value()<<endl;
       }

          status=assign_check(temp_net,NULL,v,0,1,1);// backward implication of activating fault on net temp_net
          //cout<<"Begins 1 **************************"<<endl;
          if(status==1)
         {
          //  cout<<"returned from backward implication and going for forward implication....."<<endl;
          //  cout<<"2********suucessfull backward impli **below 1****************"<<endl;
           //backward implication succesful
           //pop all the elements from the stack and call forward implication function.
           if(v==0)
          {
            status=assign_check(temp_net,NULL,3,1,1,1);// D bar for sa1 fault
            //cout<<"3************************************"<<endl;
          }
           else
           {
               status=assign_check(temp_net,NULL,2,1,1,1);// D for sa0 fault
             //  cout<<"4**********************************"<<endl;
           }
          if(status==1)//a fault has been activated
         {
         // forward implication also succesful////////////////stopped here
         //cout<<"forward implication also succesfull"<<endl;
         //getchar();
///////////////////////////////Fault has been activated/////////////////////////////////////////////////////////
         //cout<<"Fault has been activated"<<endl;
         //getchar();
         //check D frontier and J frontier

//         if(Dfrontier==NULL)
//          //cout<<"Dfrontier is NULL"<<endl;
//          else
//         {
//             cout<<"Dfrontier is not NULL"<<endl;
//             Dfrontier->display();
//
//         }
       /////////////////9 july////////////////////////////////////////
       Decision_queue *temp_queue;
       ///////////////////////////////////////////////////////////////
      /////scanning Dfrontier /////////////////////////////////////
//       cout<<"Scanning dfrontier"<<endl;
//       getchar();

       temp_queue=Dfrontier;

       while(temp_queue!=NULL)
       {

       //    cout<<"****************"<<endl;
           node *n2;

           //int v;
           top->pop_all(&top,1);//empty stack
           /////stack content/////////////////
         //  cout<<"after emptying stack"<<endl;
           impli_stack *now;
           //now->display(top);
           //////////////////////////////////

           if(temp_queue->tried_value()==0)

           {
               n2=temp_queue->node_value();
  ////////////////////////////////////////////////////////////////////////////////////
  //cout<<"Picked one node from Dfrontier"<<endl;
  //n2->display_node_info();
//cout<<endl;
  /////////////////////////////////////////////////////////////////////////////////////
             status_prop=propagate(n2);
 //            cout<<"5****************************"<<endl;
             if(status_prop==0)// error could not be propagated through node
             {
   //              cout<<"error could not be propagated through node"<<endl;
   //              cout<<"6********************"<<endl;
                 /////////////////////////////
                 //n2->display_node_info();
                 ///////////////////////////
                 temp_queue->assign_tried(1,1,Dfrontier);// tried this node for propagation;
                 temp_queue=temp_queue->next_value();// next node in Dfrontier
                // top->pop_all(&top,0);//revert back all the elements from the stack commented on 14
             }
             if(status_prop==1)
             {
     //            cout<<"error was propageted suucessfully through node"<<endl;
     //            cout<<"7********************************"<<endl;
                 ////////////////////////////
                 //n2->display_node_info();
                 /////////////////////////

                 top->pop_all(&top,1);//empty stack;
                 Dfrontier->remove_node(&temp_queue,&Dfrontier);
                 temp_queue->assign_tried(0,0,Dfrontier);//assign tried attribute of all nodes to 0
                 temp_queue=Dfrontier;
             }
          if(status_prop==3)// error propagated to PO
          {

       //       cout<<"8******************************************"<<endl;
              // now we need to solve all J frontiers to find test vector(the values of PI's )
              test_status=Jsolve();//here 12 july
         //     cout<<"came back from J frontier solve function"<<endl;
              //status of Jfrontier solve is stored in test_status
                break;
          }


       }//temp->tried==0

   }//while(temp!=NULL) scanning dfrontier
if(status_prop==3)
{
    //finished_test(test_status,)
    //open a file and print all the inputs in one line;
    ofstream out("output.txt", ios::out|ios::binary|ios::app);

    if(!out)
    {
        cout<<"cannot open file to write the output"<<endl;

    }
    else
        {

            if(test_status==0)//when a Jfrontier is not solved
            {
                out<<"test failed "<<"\n";
                out.close();
            }
            else
          {
            node *temp;
            int i;

            temp=start_node;

            while(temp->type_of_node()==0)//pi
            {
                if(temp->value()==-1)//dont care condition
                {
                    out<<"X";
                }
                else
                 {
                     out<<temp->value();

                 }

                  temp=temp->next_value();
                if(temp->type_of_node()==1)//PO
                {

                    out<<"\r\n";
                    break;
                }

            }


           out.close();
          }//else jfrontier is solved suucesfully
        //  cout<<"will execute erase operation"<<endl;
          erase_all();// assigns all net and node to a value of -1
          //empty Dfrontier and Jfrontier
          Dfrontier->empty_queue(&Dfrontier);
          Jfrontier->empty_queue(&Jfrontier);


        }//file opened succesfully in append mode
}//status ==3
////////////////////////////////////////////////
  }//status = 1 fault has been activated
   //f1=f1->next_value();


 //}//while bracket
}//status ==1
//backward implication on the fault activation site is successful
   //else bracket
///////need a whle bracket here///////////
  //temp_net=temp_net->next_value();// this test is to be performed when a pattern is generated
  else if(status==0)
  {
      //activation failed
      erase_all();// assigns all net and node to a value of -1
          //empty Dfrontier and Jfrontier
         if(Dfrontier!=NULL)
          Dfrontier->empty_queue(&Dfrontier);
         if(Jfrontier!=NULL)
          Jfrontier->empty_queue(&Jfrontier);
  }
 f1=f1->next_value();// continue to check the other fault on the same net
   }//while f1 is not NULL
 }// else bracket.....if a fault exist on a net
//f1=f1->next;
temp_net=temp_net->next_value();// check the fault on next net
}//while scanning all nets in netlist
//cout<<"scanned fault on all the nets "<<endl;
//cout<<"test generation function is over"<<endl;
}//tg function ends here


int netlist::assign_check(net *t1,node *n1,int v,int dir, int ass,int act)
{
     int value,status;


    if(ass==0) // assignment for node
    {
  //       cout<<"entered assign_check function for node"<<endl;
        // n1->display_node_info();
    //     cout<<endl;
        value=n1->value();
      //  cout<<"value on the node is "<<value<<endl;
        //if((act==1)&&(dir==1)) fault will never be activated on a node. always a net
        if((value!=-1)&&(value!=v)) //checking
        {
            //there is a contradiction
            return 0; //assignment failed
        }

        else if(value==-1)
        {
            impli_stack *temp;
            temp = new impli_stack;
            temp->push(NULL,n1,value,0,&top);// push old value of n1 on to implication stack

       //     cout<<"pushed the old value --> of the node"<< value <<endl;
            //temp->display(top);
            n1->assign_value(&v);
         //   cout<<"call implication function in "<< dir <<"direction"<<endl;
            status=implication(NULL,n1,v,dir,ass);
           // cout<<"9*******************************"<<endl;

            if(status==0)
            {
                impli_stack *tempo;
                tempo=new impli_stack;
                tempo->pop(&top);
               //pop(impli_stack *top);
                return 0;
            }
            else if(status==1)
            {
                //no contradiction;
                return 1;// assignment succesful

            }
            else if(status==3)// added on 11 july
            {
                return 3;
            }

        }//else if(value==-1)
        else if(value==v)
        {
            //v is already assigned no further implication required
            if(dir==0)//backward direction
            return 1; // no contradiction assignment successful
            else
            {
            //    cout<<"call implication function in "<< dir <<"direction"<<endl;
            status=implication(NULL,n1,v,dir,ass);
            //cout<<"10***************************"<<endl;
             return status;//14 july
//         if(status==1)
//            {
//                //no contradiction;
//                return 1;// assignment succesful
//
//            }
//            else
//                return 0;

            }
        }
    }//ass=0
///////////////////////////////////////////////////////////////
    else if(ass==1)// assignment for net
    {
        //cout<<"entered assign_check function for net"<<endl;
        //t1->display_net_info(1);
       // cout<<endl;

        value = t1->value();
        //cout<<"value in the net is "<<value<<endl;
        if((act==1)&&(dir==1))//during activation in forward direction
        {
         //   cout<<"entered block to activate-- "<<v <<"on net --"<<t1->id_value()<<endl;
           // getchar();
            t1->assign_value(&v);
//            if(value==0)
//            {
//                //change the value of the net Dbar
//                t1->assign_value(3);
//            }
//            else if (value==1)
//            {
//                t1->assign_value(2);//assign a D to the net
//            }
        }

      if((value!=-1)&&(value!=v))
      {
          //cout<<"there is a contradiction for a value-- "<<v<<" on line --"<<t1->id_value()<<endl;
          //getchar();
          // there is a contradiction
          return 0;
      }
      else if((value==-1)||(act == 1))
      {
          //  cout<<"call implication function in"<< dir <<"direction"<<endl;
            impli_stack *temp;
            temp = new impli_stack;
          temp->push(t1,NULL,value,1,&top);
          //cout<<"pushed the old value --> of the net "<<value<<endl;
          //temp->display(top);//////////8:34

          //getchar();
          if(act!=1) // no activation
          {
              t1->assign_value(&v);// assignment of value to net
          }
          //cout<<"reahced here"<<endl;
          status=implication(t1,NULL,v,dir,ass);
          //cout<<"11*************************"<<endl;
          if(status==0)
          {
            //  cout<<"returned with 0 from"<< dir << "implication function"<<endl;
              //getchar();
              impli_stack *tempo;
              tempo->pop(&top);

              return 0;
          }
          else if(status==1)
          {
              //cout<<"returned with 1 from"<< dir <<" implication function"<<endl;
              //getchar();
              //no contradiction in implication
              return 1; // success in assignment
          }
         else if(status==3)
         {
             //cout<<"returned with 3 from"<< dir <<" implication function"<<endl;
              //getchar();
             //go to finish test _function
             return 3;

         }
      }
      else if(value==v)
      {
          //cout<<"value already-- "<<v<<"from implication function"<<endl;
            //  getchar();
              return 1;

            //v is already assigned no further implication required
//            if(dir==0)//backward direction
//            return 1; // no contradiction assignment successful
//            else
//            {
//                cout<<"call implication function in "<< dir <<"direction"<<endl;
//                status=implication(t1,NULL,v,dir,ass);
//                cout<<"12**************************"<<endl;
//           if(status==1)
//            {
//                //no contradiction;
//                return 1;// assignment succesful
//
//            }
//            else
//                return 0;
//
//            }
//
//      } // commented on 11 july

    }
 }

}// func ends
///////////////////////////////////////////////////////////////////////////////////////

int netlist::implication(net *t1, node *n1, int v,int dir,int ass)
{
    type element;
    int status;
    if(dir==0)// backward implication
    {
        //cout<<"entered BI for net"<<endl;
        if(ass==1)// net
        {
            element=t1->source_value()->type_of_node();
            if(element==0) //PI
            {
               // cout<<"entered PI for net BI"<<endl;
                status=assign_check(NULL,t1->source_value(),v,dir,0,0);
               // cout<<"13*******************************"<<endl;
                return status;
            }//PI///////here
         else if(element==3)
         {
             status=assign_check(NULL,t1->source_value(),v,dir,0,0);
             //cout<<"14*****************************************"<<endl;
             if(status==0)
             {
                 return 0; // implication failed
             }
             else if(status==1)
             {
                 links *l1;
                 l1=t1->source_value()->fanout_net();
                 while(l1!=NULL)
                 {
                     net *t2;
                     t2=l1->pointer_value();
                     if(t2!=t1) // do not do forward implication on same net which has assigned a value on stem in BI
                     {  //ignore the net t2 = t1 on 14 july
                         status=assign_check(t2,NULL,v,1,1,0);// forward assignment on net of stem
              //           cout<<"15*****************************"<<endl;
                     if(status==0)
                     {
                         return 0;//implication failed
                     }
                    else if(status==3)
                     {
                         return 3;
                     }
                     else
                        l1=l1->next_value();
                     }//t2!=t1
                     else  ////ignore the net t2 = t1 on 14 july
                        l1=l1->next_value();
                 }//while
                 if(l1==NULL)
                  {
                     //return 1;
                     return status;
                  }
             }//else if(status =1)
         }//IN node

         else if(element==2)
         {
             status=assign_check(NULL,t1->source_value(),v,0,0,0);// assign net value to source gate as it is
             //cout<<"16*********************************************"<<endl;
             return status;
         }

        }//ass=1 net
        //////////////////////////////////////////////////////////////////////////////////////////
    else if(ass==0)//implication backward for node
    {
       // cout<<"entered the implication function for node in backward direction"<<endl;
        //n1->display_node_info();
        element=n1->type_of_node();
        if(element==0)//PI
        {
            //no backward implications
            return 1;
        }//PI
        else if (element==3)//IN
        {
            net *t2;
            t2=n1->fanin_net()->pointer_value();
            status=assign_check(t2,NULL,v,dir,1,0);
         //   cout<<"17************************************"<<endl;
            return status;
        }
        else if(element==2)
        {
           // cout<<"enetred element 2 ie GATE case"<<endl;
            int case_gate,count_inputs,countx,count1;
            count_inputs=0;
            countx=0;
            count1=0;
            net *tempx_net;
            case_gate=n1->type_gate();
            switch(case_gate)
            {
                case 1:
             //             cout<<"entered case 1"<<endl;
                          links *l1;//nand
                          l1=n1->fanin_net();
                          while(l1!=NULL)
                          {
                              count_inputs++;
                              l1=l1->next_value();
                          }
                          l1=n1->fanin_net();
                          while(l1!=NULL)
                          {
                              if(v==0)// assign 1 on all nets
                              {
                                  status=assign_check(l1->pointer_value(),NULL,1,dir,1,0);
               //                   cout<<"18**************************"<<endl;
                                  if(!status)
                                  {
                                      return 0;
                                  }
                                  else if(status==1)//initially if(status) changed to if(status==1) and if(status==3)
                                    //to ease the stopaage of test generetion algo
                                  {
                                      l1=l1->next_value();
                                      if(l1==NULL)  // added on 14 july// all the inputs have been assigned a 1 value
                                        {return status;}

                                        continue;
                                  }
                                  else if(status==3)
                                  {return 3;}
                              }//v=0
                              else if(v==1)
                              {
                 //                 cout<<"entered the case when ip is 1"<<endl;

                                  if(l1->pointer_value()->value()==-1)
                                  {
                   //                   cout<<"enterd -1 case"<<endl;
                                      tempx_net=l1->pointer_value();
                                      countx++;
                                      //l1=l1->next_value();
                                      if(countx==2)// add to jfrontier
                                      {
                     //                    cout<<"countx is 2"<<endl;
                                        Decision_queue *J1,*temp;
                                      J1=new Decision_queue;
                                      if(Jfrontier==NULL)
                                      {
                       //                   cout<<"Jfrontier is NULL"<<endl;
                                          Jfrontier=J1;
                                          J1->assign_next(NULL);
                                          J1->add(n1);
                                      }
                                      else
                                      {
                                         temp=Jfrontier;
                                         while(temp->next_value()!=NULL)
                                        {
                                            temp=temp->next_value();

                                        }
                                        temp->assign_next(J1);
                                        J1->add(n1);
                                      }
                                    return 1;
                                  }//countx==2
//                                  else // only input value is -1 // commented on 14 july
//                                    continue;
                                 }//v==-1

                                 else if((l1->pointer_value()->value()==1)||(l1->pointer_value()->value()==2)||(l1->pointer_value()->value()==3))
                                  {
                         //             cout<<"entered 1 case"<<endl;
                                      count1++;
                                  }
                                 else if(l1->pointer_value()->value()==0)// if any ip is 0 then return 1 and need do
                                 {  // no further backward implication required
                                     return 1; // since output 1 is justified by a 0 on input
                                      //14 july
                                 }

                                  if((countx==1)&&(count1==(count_inputs-1)))//changed from else if to if on 14 july
                                  {
                                              status=assign_check(tempx_net,NULL,0,0,1,0);//assign 0 to ip with value -1

                                      return status;
                                  }//countx==1 //only one ip is -1 and the rest are at 1 or d or dbar

                     //15july

                              }//v==1

                              l1=l1->next_value();
                    }//while
                    break;
                    default: break;
            }//switch
        }//type=gate
    }//ass=0// node
    }//dir=0 backward implication



    ////////////////////////////////////forward implication////////////////////////////
else if(dir==1)// forward implication
{
    //cout<<"entered forward implication block"<<endl;
   // getchar();
    if(ass==1)//net
    {
      // cout<<"implication on a net"<<endl;
        type sink_node;
        sink_node=t1->sink_value()->type_of_node();
        //cout<<"Element is"<<sink_node<<endl;
       // getchar();
//        if(t1==NULL)
//        {
//            cout<<" t1 is NULL"<<endl;
//        }
//        else
//            cout<<"t1 is not NULL"<<endl;
        if(sink_node==1)//PO
        {
            status=assign_check(NULL,t1->sink_value(),v,dir,0,0);
         //   cout<<"19******************************"<<endl;
            return status;
        }
       else if(sink_node==3)//IN
       {
           status=assign_check(NULL,t1->sink_value(),v,dir,0,0);
           //cout<<"20******************************"<<endl;
           return status;
       }
       else if(sink_node==2)//gate
       {
           //cout<<"entered here for forward implication of net"<<endl;
           //t1->display_net_info(1);
           //getchar();
           int case_gate;
           case_gate=t1->sink_value()->type_gate();
           switch(case_gate)
           {


               case 1:
            //             cout<<"entered switch for gate"<<endl;
                         //t1->sink_value()->display_node_info();
                         if(v==0)
                       {
                           status=assign_check(NULL,t1->sink_value(),1,1,0,0);
              //             cout<<"21********************************"<<endl;
                           return status;
                       }
                       else if(v==1||v==2||v==3)// if the other input is 1, D, Dbar
                       {
                //           cout<<"v is 1,2 or 3"<<endl;
                           links *l1;
                           int flag=0;
                           int flag1=0,flag2=0;
                           int ip_value;
                           l1=t1->sink_value()->fanin_net();
                           while(l1!=NULL)
                            {
                  //              cout<<"entered while loop"<<endl;
                                ip_value=l1->pointer_value()->value();
                               if(ip_value==-1)
                               {
                    //               cout<<"-1"<<endl;
                                   flag=1;// an input is x
                               }
                               else if(ip_value==0)
                               {
                      //             cout<<"0"<<endl;
                                   return 1;
                               }
                              else if(ip_value==2)//D
                              {
                        //          cout<<"D"<<endl;
                                  flag1=1;
                              }
                              else if(ip_value==3)//Dbar
                              {
                          //        cout<<"Dbar"<<endl;
                                  flag2=1;
                              }

                               l1=l1->next_value();

                           }//while
                           if(flag==1)
                           {
                            //        cout<<"entered Flag is 1 case"<<endl;
                                   if((v==2)||(v==3))// only if v is D or D bar a d frontier will be created
                                   {
                                     Decision_queue *D1,*temp;
                                      D1=new Decision_queue;
                                      if(Dfrontier==NULL)
                                      {
                              //            cout<<"Dfrontier is NULL"<<endl;
                                          Dfrontier=D1;
                                          D1->assign_next(NULL);
                                          D1->add(t1->sink_value());
                                          //if(Dfrontier == NULL)
                                //            cout<<"Dfrontier is still NULL"<<endl;
                                      }
                                      else
                                      {
                                         temp=Dfrontier;
                                         while(temp->next_value()!= NULL)
                                        {
                                            temp=temp->next_value();

                                        }
                                        temp->assign_next(D1);
                                        D1->add(t1->sink_value());
                                  //      cout<<"added n1 to Dfrontier"<<endl;
                                       // t1->sink_value()->display_node_info();
                                      }
                                   }
//                                  }add_Dfrontier(t1->sink_value());
                                 return 1;
                           }//flag=1
                           else if((flag1==1)&&(flag2==0))
                           {

                               if((v==2)||(v==1))
                                {
                                    //cout<<"entered here for v=1 and v2=D"<<endl;
                                    //getchar();
                                    status=assign_check(NULL,t1->sink_value(),3,1,0,0);//assign D bar to node gate
                                    //cout<<"22*****************************"<<endl;
                                    return status;
                                }
                                else if(v==3)
                                {
                                    status=assign_check(NULL,t1->sink_value(),1,1,0,0);//assign 1 to node gate;
                                    //cout<<"23**********************************"<<endl;
                                    return status;
                                }
//                               else if(v==1)//one input is D and all other are 1 so assign D bar at output
//                               {
//                                   status=assign_check()
//                               }

                            }
                          else if((flag1==0)&&(flag2==1))
                          {
                              if(v==2)//D
                              {
                                  status=assign_check(NULL,t1->sink_value(),1,1,0,0);
                                  //cout<<"24********************************"<<endl;
                                  return status;
                              }
                              else if((v==3)||(v==1))// D bar
                              {
                                  status=assign_check(NULL,t1->sink_value(),2,1,0,0);
                                  //cout<<"25************************************"<<endl;
                                  return status;
                              }

                          }
                          else if((flag1==1)&&(flag2==1))
                          {
                             status=assign_check(NULL,t1->sink_value(),1,1,0,0);
                             //cout<<"26*********************************"<<endl;
                             return status;

                          }
                          else//all the inputs are one added on 14 july
                          {
                              status=assign_check(NULL,t1->sink_value(),0,1,0,0);//assign 0 at the output of node
                              //cout<<"26a********************************************";
                              return status;
                          }



                       }//v=1||2||3
                     break;
                     default: break;
           }//switch
       }//gate
    }//ass=1//net
  else if (ass==0)//node
  {   type element;
      element=n1->type_of_node();
      if(element==1)//PO
      {
         // cout<<"Final PO Block**********************"<<endl;
          //no forward implication
          //return 1;
          if((v==2)||(v==3)) //only when D or Dbar is propagated to output  on 14 july
          {
              return 3;//11 july

          }
          else
            return 1; // if the value is not d or dbar
      }
      else if(element==2)
      {
          status=assign_check(n1->fanout_net()->pointer_value(),NULL,v,1,1,0);
          //cout<<"27*****************************************"<<endl;
          return status;
      }
      else if (element==3)
      {
          links *l1;
          l1=n1->fanout_net();
          while(l1!=NULL)
          {
              status=assign_check(l1->pointer_value(),NULL,v,1,1,0);
            //  cout<<"28****************************************"<<endl;
              if(status==3)
              {
                  return 3;
              }
              else if(status==1)
              {
               l1=l1->next_value();
               if(l1==NULL)
               {
                   return status;//14 july all forward assignment successful
               }
               continue;
              }
              else if(status==0)
              {
                  return 0;
              }
          }//while

      }//in node
  }//ass==0
}//dir=1
}//func...........
///////////////////////////////////////
void impli_stack::push(net *t1,node *n1, int value, int ass,impli_stack **top)
    {
        //impli_stack *temp;
        if((*top)==NULL)//first element
        {
            //temp = new impli_stack;
              (*top)=this;
              if(*top==NULL)
              {
              //    cout<<"Top is still NULL"<<endl;
              }
              this->prev=NULL;
            (*top)->old_value=value;
            if(ass==0)//node
            {
                (*top)->element_type=0;
                (*top)->net_addr=NULL;
                (*top)->node_addr=n1;

            }
            else if(ass==1)//net
            {

                (*top)->element_type=1;
                (*top)->node_addr=NULL;
                (*top)->net_addr=t1;
            }
        }
        else
        {

            //temp = new impli_stack;
            this->prev=*top;
            (*top)=this;
            (*top)->old_value=value;
            if(ass==0)//node
            {
                (*top)->element_type=0;
                (*top)->net_addr=NULL;
                (*top)->node_addr=n1;

            }
            else if(ass==1)//net
            {

                (*top)->element_type=1;
                (*top)->node_addr=NULL;
                (*top)->net_addr=t1;
            }
        }
    }

////////////////////////////////////////////
void impli_stack::pop(impli_stack **top)
 {
        int value;
       impli_stack *temp;
           // remove top element and assign previous element as Top element

        if((*top)==NULL)
        {
            //the stack is alreardy empty
            return;
        }


            temp=(*top); // reassign the old values to the node and net
            if((*top)->element_type==0)//node
            {
                value=(*top)->old_value;
                //cout<<value<<" in pop function"<<endl;
                (*top)->node_addr->assign_value(&value);
            }
            else if((*top)->element_type==1)//net
            {
                value=(*top)->old_value;
                //cout<<value<<" in pop function"<<endl;
                (*top)->net_addr->assign_value(&value);
            }
                (*top)=temp->prev;
                 delete temp;//
}

void impli_stack::pop_all(impli_stack **top,int task)
    {
        impli_stack *temp;
        if(task==0)//empty with reverting to old values
        {
            //cout<<"in pop all reverting to old values"<<endl;
            while((*top)!=NULL)
        {
            temp=(*top); // reassign the old values to the node and net
            if((*top)->element_type==0)//node
            {   int value;
                value = (*top)->old_value;
                (*top)->node_addr->assign_value(&value);
            }
            else if((*top)->element_type==1)//net
            {
                int value;
                value = (*top)->old_value;
                (*top)->net_addr->assign_value(&value);
            }
            (*top)=temp->prev;
            delete temp;//
        }
      }//task=1
      else if(task==1)//empty stack
      {
          //cout<<"in pop empty stack"<<endl;
          while((*top)!=NULL)
          {
              temp=*top;
              (*top)=temp->prev;
              delete temp;
          }
      }
   }
