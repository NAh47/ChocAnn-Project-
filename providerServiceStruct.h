#pragma once
#ifndef PROVIDER_SERVICE_STRUCT_H
#define PROVIDER_SERVICE_STRUCT_H
#include <string>
#include <ctime>

using namespace std;

struct provider_Service_Record
{
    string dateOfService;
    string dateandtime;
    string providerName;
    string providerNumber;
    string memberName;
    string memberNumber;
    string serviceName;
    string serviceCode;
    int feeToBePaid;    
    string providerComments;  // If no comments , will be saved as "Comments: N/A"
    
    // Constructor w/ parameters + member initializer list
    /*provider_Service_Record(string servDate,  ENTER CTIME DATE HERE  string provName, string provNum
        , string mName, string mNum, string sName, string sCode, int sFee, string pComments) : dateOfService(servDate)
        , providerName(provName), providerNumber(provNum), memberName(mName), memberNumber(mNum), serviceName(sName)
        , serviceCode(sCode), feeToBePaid(sFee), providerComments(pComments){}*/
};
#endif //PROVIDER_SERVICE_STRUCT_H
