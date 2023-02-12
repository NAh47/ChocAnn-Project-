#pragma once

#ifndef _CHOCAN_H
#define _CHOCAN_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>

#include "memberStruct.h"
#include "providerStruct.h"
#include "providerServiceStruct.h"
const string Build_Directory {"."};


// Accessible only to Providers
bool check_provider_num(string pNum); 
string check_member_num(string mNum); 
bool record_member_session(provider_Service_Record pServiceRecord);
bool write_service_report(provider_Service_Record record);

// Accessible only to ChocAn Operators
bool create_provider_record(void);
bool create_member_record(void);
bool update_member_record(void);
bool update_provider_record(void);
bool delete_provider_record(void);
bool delete_member_record(void);

// Reports to-be-run
void member_report(void);
void run_member_report(string&);
void provider_report(void);
void run_provider_report(string&);
bool run_eft_report(const string &);
void run_all_reports(void);

// Operator/Provider main functions
void provider_terminal(void);
void operator_terminal(void);
void terminal_selection(void);

void unit_tests_chocancpp();


#endif // _CHOCAN_H
