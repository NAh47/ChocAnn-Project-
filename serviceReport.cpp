#include<iostream>
#include <string.h>
#include <fstream>
#include <ctime>
#include "chocan.h"


bool write_service_report(provider_Service_Record record){
 fstream myFile;
 string memberFilename = "../Member_Records\\" +record.memberNumber +".txt"; //this can be edited to the correct path of the member record folder
 string providerFilename = "../Provider_Records\\" + record.providerNumber +".txt";
 string fee = to_string(record.feeToBePaid);
 time_t now = time(0);
 tm *ltm = localtime(&now);
 record.dateandtime = "";
 if(1 + ltm->tm_mon < 10)
 {
     record.dateandtime += "0";
 }
 record.dateandtime += to_string(ltm->tm_mon + 1) + "-";
 if(ltm->tm_mday < 10)
 {
     record.dateandtime += "0";
 }
 record.dateandtime += to_string(ltm->tm_mday) + "-";
 record.dateandtime += to_string(ltm->tm_year + 1900) + " ";
 if(ltm->tm_hour < 10)
 {
     record.dateandtime += "0";
 }
 record.dateandtime += to_string(ltm->tm_hour) + ":";
 if(ltm->tm_min < 10)
 {
     record.dateandtime += "0";
 }
 record.dateandtime += to_string(ltm->tm_min) + ":";
 if(ltm->tm_sec < 10)
 {
     record.dateandtime += "0";
 }
 record.dateandtime += to_string(ltm->tm_sec);
 myFile.open(memberFilename, ios::in);
 getline(myFile, record.memberName);
 record.memberName = record.memberName.substr(record.memberName.find(": ") + 1, record.memberName.size());
 myFile.close();
 myFile.clear();
 myFile.open(providerFilename, ios::in);
 getline(myFile, record.providerName);
 record.providerName = record.providerName.substr(record.providerName.find(": ") + 1, record.providerName.size());
 myFile.close();
 myFile.clear();


 myFile.open(memberFilename,ios::app);

 if(myFile.is_open()){
     myFile<<"\nDate Of Service: " + record.dateOfService ;
     myFile<<"\nProvider Name: " + record.providerName ;
     myFile<<"\nService Name: " + record.serviceName << endl;
     myFile.close();

}
else return myFile.is_open();
myFile.close();
myFile.clear();

myFile.open(providerFilename,ios::app);

 if(myFile.is_open()){
     myFile<<"\nDate Of Service: " + record.dateOfService ;
     myFile<<"\nDate and Time Received: " + record.dateandtime;
     myFile<<"\nMember Name: " + record.memberName ;
     myFile<<"\nMember Number: " + record.memberNumber ;
     myFile<<"\nService Code: " + record.serviceCode ;
     myFile<<"\nComments: " + record.providerComments ;
     myFile<<"\nFee To Be Paid: " + fee << endl;
     
     myFile.close();

    }
else return myFile.is_open();


return myFile.is_open();
}
