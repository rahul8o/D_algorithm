

#include "network.h"

using namespace std;

int main()
{

    char info1[20],info[20],ch1;
    int inp[20];         // line scanned in a file
    char data[10]={'\0'};
    char gate_name[10]={'\0'}; // initialized data needed for creating a node
    int value;             // decide the
    // type of node to be created
    netlist ob1;           // object of circuit to be created
    int id,i,k;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    fstream read("c17.txt");
if(!read)
    {
    cout<<"unable to open file";
    return 0;
    }

while(!read.eof()) // reads till the end of the file
{
    read.getline(info1,20,'\n');

  for(i=0,k=0;info1[i]!='\0';i++,k++)
    {

           ch1=info1[i];

           if(ch1==' ')
           {
               k--;
               if(k<0)
                {break;}

                continue;


           }

           info[k]=ch1;


    }
    if(k>0)
    {
        info[k]='\0';
        //cout<<"data read is "<<info<<endl;
        //getchar();
      value=ob1.scan(info,data,gate_name); // calls func to know type of node(value) and data needed to create node
      ob1.create_netlist(value,data,id,gate_name);

    }

 ////////////////////////////////////////////////////////////////////////////////////////////////////

}  // while bracket

//////////////////////////////////////////////////////////
//cout<<endl;
//ob1.count_nodes_nets();
//cout<<endl;
//ob1.display_gate_info();
//cout<<endl;
//ob1.display_connection();
//cout<<endl;
//ob1.display_all_nets();


read.close();

// commented all the code related to evaluation of output of gate
//fstream inputs("inputs.txt");
//
//if(!inputs)
// {
//     cout<<"unable to open the file";
//     return 0;
// }
// else
// {
//     while(!inputs.eof())
//     {
//         inputs.getline(info,20,'\n');
//           //assign_inputs to pi variables
//         //ob1.assign_values(info);// a member function of netlist class
//          ob1.assign_inputs(info);
//
//     }
// }
cout<<endl;
cout<<endl;


cout<<"evaluating dominant faults "<<endl;
//ob1.equivalent_faults();
ob1.dominant_faults();
cout<<"Test pattern generation begins now"<<endl;
cout<<"Test generation is completed "<<endl;
cout<<endl;
//getchar();
//////////////////////////////////////////////
ob1.tg();
////////////////////////////////////////////

//ob1.count_nodes_nets();
//cout<<endl;
//ob1.display_connection();
//cout<<endl;
//
//
//cout<<endl;
//ob1.display_all_nets();



return 1;



}  // int closing bracket





