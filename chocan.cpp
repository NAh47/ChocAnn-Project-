
#include<iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <fstream>
#include <assert.h>
#include <list>
#include <iterator>
#include <filesystem>
#include "chocan.h"
#include "validate.h"

using namespace std;

struct duration {
    int month;
    int date;
    int year;
};
struct serv_info {
    string date_of_service;
    string provider_name;
    string service_name;
};

struct basic_info {
    string name;
    string id;
    string str_addr;
    string city;
    string state;
    string zip;
};
struct services {
    string date_of_service;
    string time_received;
    string member_name;
    string member_id;
    string service_code;
    string fee;
};

/***********************************************************************************************************************************
                                                    PROVIDER ONLY           
***********************************************************************************************************************************/
bool check_provider_num(string pNum) {
    ifstream read(Build_Directory + "/Provider_Records/" + pNum + ".txt");
    if (read) {
        read.close();
        return true;
    } else {
        read.close();
        return false;
    }
    /*
        Search for provider file using provider num, passed as a string
        
        If provider file exists 
            True
        Else
            False
    */
    return "NULL";
}


string check_member_num(string mNum)
{
    ifstream read(Build_Directory + "/Member_Records/" + mNum + ".txt");
    string status;
    string temp;

    if(read)
    {
        while (getline(read, status))
        {
            temp = status.substr(0, status.find(":"));
            if (temp == "Member Status")
            {
                status = status.substr(status.find(":") + 1, status.size());
                break;
            }
        }
        if(status == " Valid")
        {
            return "Member Valid";
        }
        else if(status == " Suspended")
        {
            return "Member Suspended";
        }
    }
    else
    {
        return "Invalid Number";
    }
    /*
        Search for member number

        Find member file
        In file, find status
        
        If status == validated
            Return "Validated"
        else if status == suspended
            Return "Member suspended"
        else
            Return "Invalid Number" if number does not exist
    */
    return "NULL";

}

bool verify_service(provider_Service_Record& serviceRecord)
{
    bool valid = false;
    string file_line;
    string parse;
    ifstream read(Build_Directory + "/Provider_Directory/directory.txt");
    while(getline(read, file_line))
    {
        parse = file_line.substr(file_line.find(",") + 1, file_line.size());
        parse = parse.substr(0, parse.find(","));
        if(serviceRecord.serviceCode == parse)
        {
            valid = true;
            break;
        }
    }
    if(valid == false)
    {
        throw 1;
    }
    cout << "Service name: " << file_line.substr(0, file_line.find(",")) << endl;
    do
    {
        parse = getline_format("Is this the correct service? y/n:", "c", "Please enter y or n.");
        if(parse != "n" && parse != "y")
        {
            cout << "Please enter y or n" << endl;
        }
    } while(parse != "n" && parse != "y");
    if(parse == "y")
    {
        serviceRecord.serviceName = file_line.substr(0, file_line.find(","));
        parse = file_line.substr(file_line.find(",") + 1, file_line.size());
        serviceRecord.feeToBePaid = stoi(parse.substr(parse.find(",") + 1, parse.size()));
        valid = true;
    }
    if(parse == "n")
    {
        valid = false;
    }
    return valid;
}



bool record_member_session(provider_Service_Record pServiceRecord)
{
    bool valid;
    string user_input;
    try
    {
        do
        {

            pServiceRecord.dateOfService = getline_format(
                    "Please enter the date the service was provided in the format MM-DD-YYYY:",
                    "ii-ii-iiii",
                    "Error: Please enter date in the format MM-DD-YYYY");
            valid = verify_date(pServiceRecord.dateOfService);
            if (valid == false)
            {
                cout << "Error: Invalid date" << endl;
            }
        } while (valid == false);
        do
        {
            try
            {
                pServiceRecord.serviceCode = getline_format(
                        "Please enter the service code of the 6 digit service that was provided:", "iiiiii",
                        "Error: Please enter a number with 6 digits.");
                valid = verify_service(pServiceRecord);
            }
            catch (int err)
            {
                cout << "Error: Invalid Service Code" << endl;
            }
        } while (valid == false);
        pServiceRecord.providerComments = getline_limit(
                "Please enter any comments you have about the session/member up to 100 characters:", 100,
                "Error: A maximum of 100 characters can be entered.");
        cout << "Session data" << endl;
        cout << "Date of service: " << pServiceRecord.dateOfService << endl;
        cout << "Service name: " << pServiceRecord.serviceName << endl;
        cout << "Comments: " << pServiceRecord.providerComments << endl;
        do
        {
            user_input = getline_format("Would you like to save this session data? y/n:", "c", "Error: Please enter y or n.");
            if(user_input != "y" && user_input != "n")
            {
                cout << "Error: Please enter y or n." << endl;
            }
        } while(user_input != "y" && user_input != "n");
        if(user_input == "y")
        {
            write_service_report(pServiceRecord);
        }
        else
        {
            throw user_input;
        }
    }
    catch(string& err)
    {
        return false;
    }
    return true;
}



/***********************************************************************************************************************************
                                                    CHOC-AN OPERATOR ONLY           
***********************************************************************************************************************************/

static provider_record get_provider_information(void)
{
    provider_record providerRecord;
    do
    {
        providerRecord.providerName = getline_limit("Please enter the provider's name:", 25,
                                                    "Please enter a name with 25 characters or less.");
        if(providerRecord.providerName.size() == 0)
        {
            cout << "Provider name cannot be empty." << endl;
        }
    } while(providerRecord.providerName.size() == 0);

    providerRecord.providerNum = getline_format("Please enter the provider's 9 digit number:", "iiiiiiiii",
                                                    "Please enter a 9 digit number.");
    do
    {
        providerRecord.providerAddress = getline_limit("Please enter the provider's address:", 25,
                                                       "Please enter an address with 25 characters or less.");
        if(providerRecord.providerAddress.size() == 0)
        {
            cout << "Provider address cannot be empty." << endl;
        }
    } while(providerRecord.providerAddress.size() == 0);
    do
    {
        providerRecord.providerCity = getline_limit("Please enter the provider's city:", 14,
                                                    "Please enter a city name with 14 characters or less.");

        if(providerRecord.providerCity.size() == 0)
        {
            cout << "Provider city cannot be empty." << endl;
        }
    } while(providerRecord.providerCity.size() == 0);
    providerRecord.providerState = getline_format("Please enter the 2 letter identifier for the state the provider lives in:", "cc", "Please enter exactly 2 letters.");
    providerRecord.providerZip = getline_format("Please enter the provider's zip code:", "iiiii", "Please enter a 5 digit number.");
    return providerRecord;
}

bool create_provider_record(void) {
    /*
        Provider Data passed to function as a struct

        Create a new .txt file IN Provider_Records Folder
            Naming Convention: Use Provider Number (passed as a parameter) 
            Format: 9-Digit-Provider-Number.txt

            Save struct data in .txt file, seperating values with commas
    */

	string path;
	string directory = Build_Directory + "/Provider_Records/";
    provider_record pRecord = get_provider_information();
	path = directory + pRecord.providerNum + ".txt";
	cout << path << endl;

	ofstream RecordFile(path);

	RecordFile << "Provider Name: " << pRecord.providerName << endl <<
			"Provider Number: " << pRecord.providerNum << endl <<
			"Provider Street Address: " << pRecord.providerAddress << endl <<
			"Provider City: " << pRecord.providerCity << endl <<
			"Provider State: " << pRecord.providerState << endl <<
			"Provider Zip Code: " << pRecord.providerZip << endl;

	RecordFile.close();
	return 1;
}


static member_record get_member_information(void)
{
    member_record memberRecord;
    do
    {
        memberRecord.memberName = getline_limit("Please enter the member's name:", 25,
                                                    "Please enter a name with 25 characters or less.");
        if(memberRecord.memberName.size() == 0)
        {
            cout << "Provider name cannot be empty." << endl;
        }
    } while(memberRecord.memberName.size() == 0);

    memberRecord.memberNum = getline_format("Please enter the member's 9 digit number:", "iiiiiiiii",
                                                "Please enter a 9 digit number.");
    do
    {
        memberRecord.memberAddress = getline_limit("Please enter the member's address:", 25,
                                                       "Please enter an address with 25 characters or less.");
        if(memberRecord.memberAddress.size() == 0)
        {
            cout << "Provider address cannot be empty." << endl;
        }
    } while(memberRecord.memberAddress.size() == 0);
    do
    {
        memberRecord.memberCity = getline_limit("Please enter the member's city:", 14,
                                                    "Please enter a city name with 14 characters or less.");

        if(memberRecord.memberCity.size() == 0)
        {
            cout << "Provider city cannot be empty." << endl;
        }
    } while(memberRecord.memberCity.size() == 0);
    memberRecord.memberState = getline_format("Please enter the 2 letter identifier for the state the member lives in:", "cc", "Please enter exactly 2 letters.");
    memberRecord.memberZip = getline_format("Please enter the member's zip code:", "iiiii", "Please enter a 5 digit number.");
    return memberRecord;
}


bool create_member_record(void) {
    /*
        Member Data passed to function as a struct

        Create a new .txt file IN Member_Records Folder
            Naming Convention: Use Member Number (passed as a parameter)
            Format: 9-Digit-Member-Number.txt

            Save struct data in .txt file, seperating values with commas
    */
	string path;
	string directory = Build_Directory + "/Member_Records/";
    member_record mRecord = get_member_information();
	path = directory + mRecord.memberNum + ".txt";
	cout << path << endl;

	ofstream RecordFile(path);

	RecordFile << "Member Name: " << mRecord.memberName << endl <<
			"Member Number: " << mRecord.memberNum << endl <<
			"Member Street Address: " << mRecord.memberAddress << endl <<
			"Member City: " << mRecord.memberCity << endl <<
			"Member State: " << mRecord.memberState << endl <<
			"Member Zip Code: " << mRecord.memberZip << endl <<
            "Member Status: Valid" << endl;

	RecordFile.close();
	return 1;


}


static member_record get_new_member_data()
{
    member_record memberRecord;
    ofstream write;
    ifstream read;
    bool updating = true;
    bool updating_status = true;
    bool valid_id;
    do
    {
        memberRecord.memberNum = getline_format("Please enter the member's 9 digit number:", "iiiiiiiii",
                                                "Error: Please enter a 9 digit number.");
        read.open(Build_Directory + "/Member_Records/" + memberRecord.memberNum + ".txt");
        if(read)
        {
            valid_id = true;
            getline(read, memberRecord.memberName);
            memberRecord.memberName = memberRecord.memberName.substr(memberRecord.memberName.find(": ") + 2,
                                                                     memberRecord.memberName.size());
            getline(read, memberRecord.memberNum);
            memberRecord.memberNum = memberRecord.memberNum.substr(memberRecord.memberNum.find(": ") + 2,
                                                                   memberRecord.memberNum.size());
            getline(read, memberRecord.memberAddress);
            memberRecord.memberAddress = memberRecord.memberAddress.substr(
                    memberRecord.memberAddress.find(": ") + 2, memberRecord.memberAddress.size());
            getline(read, memberRecord.memberCity);
            memberRecord.memberCity = memberRecord.memberCity.substr(memberRecord.memberCity.find(": ") + 2,
                                                                     memberRecord.memberCity.size());
            getline(read, memberRecord.memberState);
            memberRecord.memberState = memberRecord.memberState.substr(
                    memberRecord.memberState.find(": ") + 2, memberRecord.memberState.size());
            getline(read, memberRecord.memberZip);
            memberRecord.memberZip = memberRecord.memberZip.substr(memberRecord.memberZip.find(": ") + 2,
                                                                   memberRecord.memberZip.size());
            getline(read, memberRecord.memberStatus);
            memberRecord.memberStatus = memberRecord.memberStatus.substr(memberRecord.memberStatus.find(": ") + 2,
                                                                         memberRecord.memberStatus.size());
        }
        else
        {
            valid_id = false;
        }
        read.close();
        read.clear();
    } while(valid_id == false);
    while(updating == true)
    {
        cout << "Update Name:   1" << endl;
        cout << "Update Address:2" << endl;
        cout << "Update City:   3" << endl;
        cout << "Update State:  4" << endl;
        cout << "Update Zip:    5" << endl;
        cout << "Update Status  6" << endl;
        cout << "Confirm Changes  7" << endl;
        switch(stoi(getline_format("Please enter a number corresponding to what you want to update:", "i", "Error: Please enter a 1 digit number between 1 and 7.")))
        {
            case 1:
                do
                {
                    memberRecord.memberName = getline_limit("Please enter a name up to 25 characters:", 25,
                                                            "Error: Please enter a name with up to 25 characters.");
                    if(memberRecord.memberName.size() == 0)
                    {
                        cout << "Error: Name cannot be empty." << endl;
                    }
                } while(memberRecord.memberName.size() == 0);
                break;
            case 2:
                do
                {
                    memberRecord.memberAddress = getline_limit("Please enter an address up to 25 characters:", 25, "Error: Please enter an address up to 25 characters.");
                    if(memberRecord.memberAddress.size() == 0)
                    {
                        cout << "Error: Address cannot be empty." << endl;
                    }
                } while(memberRecord.memberAddress.size() == 0);
                break;
            case 3:
                do
                {
                    memberRecord.memberCity = getline_limit("Please enter a city name up to 14 characters:", 14, "Error: Please enter a name up to 14 characters.");
                    if(memberRecord.memberCity.size() == 0)
                    {
                        cout << "Error: City name cannot be empty." << endl;
                    }
                } while(memberRecord.memberCity.size() == 0);
                break;
            case 4:
                memberRecord.memberState = getline_format("Please enter the two character state id:", "cc", "Error: Please enter exactly two characters.");
                break;
            case 5:
                memberRecord.memberZip = getline_format("Please enter the member's zip code:", "iiiii", "Error: Please enter a 5 digit number.");
                break;
            case 6:
                updating_status = true;
                do
                {
                    cout << "Valid:     1" << endl;
                    cout << "Suspended: 2" << endl;
                    switch(stoi(getline_format("Please enter a number corresponding to the correct member status:", "i", "Error: please enter a number between 1 and 2.")))
                    {
                        case 1:
                            memberRecord.memberStatus = "Valid";
                            updating_status = false;
                            break;
                        case 2:
                            memberRecord.memberStatus = "Suspended";
                            updating_status = false;
                            break;
                        default:
                            cout << "Error: Please enter a number between 1 and 2." << endl;
                            break;
                    }
                } while(updating_status == true);
                break;
            case 7:
                updating = false;
                break;
            default:
                cout << "Error: Please enter a number between 1 and 7." << endl;
                break;
        }
    }
    return memberRecord;
}


bool update_member_record(void)
{
    /*
        Struct data and Member number pass as a parameter

        Search for record in Member_Records using mNum

        If Found
            Replace current member Data with updated data <--- THIS INFORMATION WILL BE THE FIRST THING IN THE FILE
            Seperate data with commas
                Name
                NOT NUMBER
                Street Address
                City
                State
                Zip
            Return true

        Else if NOT found
            return false
    */
	string directory = Build_Directory + "/Member_Records/";
    member_record mRecord = get_new_member_data();
	string path = directory + mRecord.memberNum + ".txt";
	string line;
    list<string> file_content;
    list<string>::iterator i;
    ifstream read(path);
    ofstream write;
    if(read)
    {
        while(getline(read, line))
        {
            file_content.push_back(line);
        }
        read.close();
        read.clear();
        i = file_content.begin();
        write.open(path);
        *i = mRecord.memberName;
        write << "Member Name: " << *i << endl;
        ++i;
        *i = mRecord.memberNum;
        write << "Member Number: " << *i << endl;
        ++i;
        *i = mRecord.memberAddress;
        write << "Member Street Address: " << *i << endl;
        ++i;
        *i = mRecord.memberCity;
        write << "Member City: " << *i << endl;
        ++i;
        *i = mRecord.memberState;
        write << "Member State: " << *i << endl;
        ++i;
        *i = mRecord.memberZip;
        write << "Member Zip Code: " << *i << endl;
        ++i;
        *i = mRecord.memberStatus;
        write << "Member Status: " << *i << endl;
        ++i;
        for(i; i != file_content.end(); ++i)
        {
            write << *i << endl;
        }
        write.close();
        write.clear();
    }
    else
    {
        cout << "Could not open file." << endl;
    }

    /*
	fstream mFile(path, ios::in | ios::out);

	if(mFile.is_open()){
		mFile << "Member Name: " << mRecord.memberName << endl <<
			"Member Number: " << mRecord.memberNum << endl <<
			"Member Street Address: " << mRecord.memberAddress << endl <<
			"Member City: " << mRecord.memberCity << endl <<
			"Member State: " << mRecord.memberState << endl <<
			"Member Zip Code: " << mRecord.memberZip << endl <<
            "Member Status: " << mRecord.memberStatus << endl;
		cout << "Member " << mRecord.memberNum << " file updated" << endl;
		mFile.close();
		return true;
	} else {
		cout << "Could not open file for member " << mRecord.memberNum << endl;
		return false;
	}
     */
}


static provider_record get_updated_provider_info()
{
    provider_record providerRecord;
    ofstream write;
    ifstream read;
    bool updating = true;
    bool updating_status = true;
    bool valid_id;
    do
    {
        providerRecord.providerNum = getline_format("Please enter the provider's 9 digit number:", "iiiiiiiii",
                                                "Error: Please enter a 9 digit number.");
        read.open(Build_Directory + "/Provider_Records/" + providerRecord.providerNum + ".txt");
        if(read)
        {
            valid_id = true;
            getline(read, providerRecord.providerName);
            providerRecord.providerName = providerRecord.providerName.substr(providerRecord.providerName.find(": ") + 2,
                                                                             providerRecord.providerName.size());
            getline(read, providerRecord.providerNum);
            providerRecord.providerNum = providerRecord.providerNum.substr(providerRecord.providerNum.find(": ") + 2,
                                                                           providerRecord.providerNum.size());
            getline(read, providerRecord.providerAddress);
            providerRecord.providerAddress = providerRecord.providerAddress.substr(
                    providerRecord.providerAddress.find(": ") + 2, providerRecord.providerAddress.size());
            getline(read, providerRecord.providerCity);
            providerRecord.providerCity = providerRecord.providerCity.substr(providerRecord.providerCity.find(": ") + 2,
                                                                             providerRecord.providerCity.size());
            getline(read, providerRecord.providerState);
            providerRecord.providerState = providerRecord.providerState.substr(
                    providerRecord.providerState.find(": ") + 2, providerRecord.providerState.size());
            getline(read, providerRecord.providerZip);
            providerRecord.providerZip = providerRecord.providerZip.substr(providerRecord.providerZip.find(": ") + 2,
                                                                           providerRecord.providerZip.size());
        }
        else
        {
            valid_id = false;
        }
        read.close();
        read.clear();
    } while(valid_id == false);
    while(updating == true)
    {
        cout << "Update Name:   1" << endl;
        cout << "Update Address:2" << endl;
        cout << "Update City:   3" << endl;
        cout << "Update State:  4" << endl;
        cout << "Update Zip:    5" << endl;
        cout << "Confirm Changes  6" << endl;
        switch(stoi(getline_format("Please enter a number corresponding to what you want to update:", "i", "Error: Please enter a 1 digit number between 1 and 6.")))
        {
            case 1:
                do
                {
                    providerRecord.providerName = getline_limit("Please enter a name up to 25 characters:", 25,
                                                            "Error: Please enter a name with up to 25 characters.");
                    if(providerRecord.providerName.size() == 0)
                    {
                        cout << "Error: Name cannot be empty." << endl;
                    }
                } while(providerRecord.providerName.size() == 0);
                break;
            case 2:
                do
                {
                    providerRecord.providerAddress = getline_limit("Please enter an address up to 25 characters:", 25, "Error: Please enter an address up to 25 characters.");
                    if(providerRecord.providerAddress.size() == 0)
                    {
                        cout << "Error: Address cannot be empty." << endl;
                    }
                } while(providerRecord.providerAddress.size() == 0);
                break;
            case 3:
                do
                {
                    providerRecord.providerCity = getline_limit("Please enter a city name up to 14 characters:", 14, "Error: Please enter a name up to 14 characters.");
                    if(providerRecord.providerCity.size() == 0)
                    {
                        cout << "Error: City name cannot be empty." << endl;
                    }
                } while(providerRecord.providerCity.size() == 0);
                break;
            case 4:
                providerRecord.providerState = getline_format("Please enter the two character state id:", "cc", "Error: Please enter exactly two characters.");
                break;
            case 5:
                providerRecord.providerZip = getline_format("Please enter the provider's zip code:", "iiiii", "Error: Please enter a 5 digit number.");
                break;
            case 6:
                updating = false;
                break;
            default:
                cout << "Error: Please enter a number between 1 and 6." << endl;
                break;
        }
    }
    return providerRecord;
}

bool update_provider_record(void)
{
    /*
        Struct data and Provider number pass as a parameter

        Search for record in Provider_Records using pNum

        If Found
            Replace current Provider Data with updated data   <-- THIS INFORMATION WILL BE THE FIRST THING IN THE FILE
            Seperate data with commas
                Name
                NOT NUMBER
                Street Address
                City
                State
                Zip
            Return true

        Else if NOT found
            return false
    */
	string directory = Build_Directory + "/Provider_Records/";
    provider_record pRecord = get_updated_provider_info();
	string path = directory + pRecord.providerNum + ".txt";
    string line;
    list<string> file_content;
    list<string>::iterator i;
    ifstream read(path);
    ofstream write;
    if(read)
    {
        while(getline(read, line))
        {
            file_content.push_back(line);
        }
        read.close();
        read.clear();
        i = file_content.begin();
        write.open(path);
        *i = pRecord.providerName;
        write << "Member Name: " << *i << endl;
        ++i;
        *i = pRecord.providerNum;
        write << "Member Number: " << *i << endl;
        ++i;
        *i = pRecord.providerAddress;
        write << "Member Street Address: " << *i << endl;
        ++i;
        *i = pRecord.providerCity;
        write << "Member City: " << *i << endl;
        ++i;
        *i = pRecord.providerState;
        write << "Member State: " << *i << endl;
        ++i;
        *i = pRecord.providerZip;
        write << "Member Zip Code: " << *i << endl;
        ++i;
        for(i; i != file_content.end(); ++i)
        {
            write << *i << endl;
        }
        write.close();
        write.clear();
    }
    else
    {
        cout << "Could not open file." << endl;
    }
    /*
	fstream pFile(path, ios::in | ios::out);

	if(pFile.is_open()){
		pFile << "Provider Name: " << pRecord.providerName << endl <<
			"Provider Number: " << pRecord.providerNum << endl <<
			"Provider Street Address: " << pRecord.providerAddress << endl <<
			"Provider City: " << pRecord.providerCity << endl <<
			"Provider State: " << pRecord.providerState << endl <<
			"Provider Zip Code: " << pRecord.providerZip << endl;
		cout << "Provider " << pRecord.providerNum << " file updated" << endl;
		pFile.close();
		return true;
	} else {
		cout << "Could not open file for provider " << pRecord.providerNum << endl;
		return false;
	}*/


}


bool delete_provider_record(void) {
    /*
        Provider Number is passed as a string

        Searches for Provider record:

        If found:
            Delete Provider Record
            Return True
        
        Else
            Return False
    */
    string pNum = getline_format("Please enter the provider's 9 digit number:", "iiiiiiiii", "Error: Please enter a number with 9 digits.");
	string path = Build_Directory + "/Provider_Records/" + pNum + ".txt";
	if(remove(path.c_str()) != 0){
		cout << "Error deleting file" << endl;
		return false;
	} else{
		cout << "Records for provider " << pNum << " deleted successfully" << endl;
		return true;
	}
}


bool delete_member_record(void) {
    /*
        Member Number is passed as a string

        Searches for Member record:

        If found:
            Delete Member Record
            Return True
        
        Else
            Return False
    */
    string mNum = getline_format("Please enter the member's 9 digit number:", "iiiiiiiii", "Error: Please enter a number with 9 digits.");
	string path = Build_Directory + "/Member_Records/" + mNum + ".txt";
	if(remove(path.c_str()) != 0){
		cout << "Error deleting file" << endl;
		return false;
	} else{
		cout << "Records for member " << mNum << " deleted successfully" << endl;
		return true;
	}


}


/***********************************************************************************************************************************
                                                  REPORTS TO BE RUN
***********************************************************************************************************************************/


static
int date_count(int m, int d, int y) {
    long total_days = 0;
    int counting_month = 12;
    for (int x = 2000; x <= y; x++) {
        if (x == y) counting_month = m - 1;
        for (int i = 1; i <= counting_month; i++) {
            if (i == 1 || i == 3 || i == 5 || i == 7 ||
                i == 8 || i == 10 || i == 12)
                total_days += 31;
            else if (i == 4 || i == 6 || i == 9 || i == 11)
                total_days += 30;
            else if (i == 2) total_days += 28;
        }
        if (x % 4 == 0) total_days += 1;
    }
    total_days += d;
    return total_days;
}
// this function reads in a string date and checks if the given date is within a period of the week from the current date
static
bool check_to_add(string date) { //
    duration current;
    duration record;
    char receive[25];
    time_t t = time(0);
    strftime(receive, 24, "%m", localtime(&t));
    current.month = stoi(receive);
    strftime(receive, 24, "%d", localtime(&t));
    current.date = stoi(receive);
    strftime(receive, 24, "%y", localtime(&t));
    current.year = stoi(receive) + 2000;

    size_t pos = date.find(':') + 2;
    string temp = date.substr(pos, date.size());
    pos = temp.find('-');
    record.month = stoi(temp.substr(0, pos));
    temp = temp.substr(pos + 1, temp.size());
    pos = temp.find('-');
    record.date = stoi(temp.substr(0, pos));
    temp = temp.substr(pos + 1, temp.size());
    pos = temp.find('-');
    record.year = stoi(temp.substr(0, pos));
    temp = temp.substr(pos + 1, temp.size());
    int record_date = date_count(record.month, record.date, record.year);
    int current_date = date_count(current.month, current.date, current.year);
    if (current_date - record_date <= 7) {
        return true;
    } else {
        return false;
    }
}

static
int service_fee(string serv_code) { // calculates service fee
    ifstream sinf;
    sinf.open("Provider_Directory//directory.txt");
    string serv_name, tserv_code, tfee;
    if (sinf) {
        while (!sinf.eof()) {
            getline(sinf, serv_name, ',');
            getline(sinf, tserv_code, ',');
            getline(sinf, tfee, '\n');
            if (serv_code == tserv_code) {
                int fee = stoi(tfee) * 100;
                sinf.close();
                return fee;
            }
        }
    }
    sinf.close();
    return -1;
}

static
string trim(string str) { //trims string
    string slim;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] != ' ') {
            slim.push_back(str[i]);
        }
    }
    return slim;
}

static
string file_name(string full_name) { //creates file name
    string tname = full_name;
    string filename;
    size_t pos = tname.find(':') + 2;
    string f_name = tname.substr(pos, tname.find(" "));
    tname = tname.substr(pos, tname.size());
    size_t lpos = tname.find(' ') + 1;
    string l_name = tname.substr(lpos, tname.size());
    char dos[25];
    time_t t = time(0);
    strftime(dos, 25, "%m-%d-%y", localtime(&t));
    string date = string(dos);
    filename = f_name + l_name + date;
    return trim(filename);
}

void member_report(void)
{
    string id = getline_format("Please enter the member's 9 digit number:", "iiiiiiiii", "Error: Please enter a 9 digit number.");
    run_member_report(id);
}

void run_member_report(string& id) {
    basic_info m_info;
    serv_info s_info;
    string to_open = Build_Directory + "/Member_Records/" + id + ".txt";
    ifstream minf;
    ofstream mp;
    minf.open(to_open); //
    if (minf) {//reading basic member information
        getline(minf, m_info.name, '\n');
        getline(minf, m_info.id, '\n');
        getline(minf, m_info.str_addr, '\n');
        getline(minf, m_info.city, '\n');
        getline(minf, m_info.state, '\n');
        getline(minf, m_info.zip, '\n');
        minf.ignore(200, '\n'); //Ignoring member status
        minf.ignore(200, '\n');
        //writing member info
        mp.open(Build_Directory + "/Member_Reports/" + file_name(m_info.name) + ".txt");
        mp << m_info.name << endl << m_info.id << endl <<
           m_info.str_addr << endl << m_info.city << endl <<
           m_info.state << endl << m_info.zip << endl;
        while (!minf.eof()) {
            getline(minf, s_info.date_of_service, '\n');
            getline(minf, s_info.provider_name, '\n');
            getline(minf, s_info.service_name, '\n');
            minf.ignore(200, '\n');
            //check if the service date is within a week from the current date.
            //if yes, write the services

            if (check_to_add(s_info.date_of_service)) {
                mp << endl << s_info.date_of_service << endl;
                mp << s_info.provider_name << endl;
                mp << s_info.service_name << endl;
            }
        }
    }
    else
    {
        cout << "Could not open file." << endl;
    }
    mp.close();
    minf.close();
}
void provider_report(void)
{
    string id = getline_format("Please enter the provider's 9 digit number:", "iiiiiiiii", "Error: Please enter a 9 digit number.");
    run_provider_report(id);
}

void run_provider_report(string& id) {
    basic_info p_info;
    int consultation = 0;
    float total_fee = 0;
    services s_info;
    string to_open = Build_Directory + "/Provider_Records/" + id + ".txt";
    ifstream inf;
    ofstream fp;
    inf.open(to_open);
    if (inf) {
        //provider info
        getline(inf, p_info.name, '\n');
        getline(inf, p_info.id, '\n');
        getline(inf, p_info.str_addr, '\n');
        getline(inf, p_info.city, '\n');
        getline(inf, p_info.state, '\n');
        getline(inf, p_info.zip, '\n');
        inf.ignore(200, '\n');

        //writing provider info to file
        fp.open(Build_Directory + "/Provider_Reports/" + file_name(p_info.name) + ".txt");
        fp << p_info.name << endl << p_info.id << endl <<
           p_info.str_addr << endl << p_info.city << endl <<
           p_info.state << endl << p_info.zip << endl;
        //reading services
        while (!inf.eof()) {
            getline(inf, s_info.date_of_service, '\n');
            getline(inf, s_info.time_received, '\n');
            getline(inf, s_info.member_name, '\n');
            getline(inf, s_info.member_id, '\n');
            getline(inf, s_info.service_code, '\n');
            getline(inf, s_info.fee, '\n');
            inf.ignore(200, '\n');
            //check if the services provided happened within a week from the current date
            //if yes, write
            if (check_to_add(s_info.date_of_service)) {
                fp << endl << s_info.date_of_service << endl;
                fp << s_info.time_received << endl;
                fp << s_info.member_name << endl;
                fp << s_info.member_id << endl;
                fp << s_info.service_code << endl;
                fp << s_info.fee << endl;
                consultation++;
                //truncation to get the service fee
                string temp = s_info.service_code;
                size_t pos = temp.find(':');
                temp = temp.substr(pos + 2, temp.size());
                total_fee += service_fee(temp);
            };
        }
        fp << endl << "Total number of consultations: " << consultation << endl;
        fp << "Total fee for the week: " << total_fee/100 << endl;
        fp.close();
        inf.close();
        /*Now runs the EFT report...*/
        const string str = Build_Directory + "/Provider_Reports/" + file_name(p_info.name) + ".txt"; //getting the provider report file name and then pass it to EFT report func
        run_eft_report(str);
    }
}


//Yihui Tian, 11/25/2022
//This function takes provider report file as arguments and then search the file to generate new eft report for the provider
//EFT report saved as (ProviderFirstName)(ProviderLastName)(Date of Report:DD-MM-YYYY).txt, contains provider name, provider number and Weekly fees to be paid.
bool run_eft_report(const string & pNum)
{
    //read the file and get the data: Name, Number, weekly fees.
    int total_fees;//holds the weekly fees
    char name[50];//holds provider name
    char num[50];//holds provider number
    ifstream in_file;
    in_file.open(pNum);
    if(!in_file)//check if file opened successfully
    {
        cout<<"Error: unable to open file and read "<<endl;
        return false;
    }
    else
    {
        while( !in_file.eof() ) //loop through every lines in the file to get the info
        {
            char tmp[50];
            in_file.get(tmp,50,':');//read in the key first
            in_file.ignore(50,':');
            if(strcmp(tmp,"Provider Name")==0)
            {
                in_file.get(name,50,'\n');
            }
            else if(strcmp(tmp,"Provider Number")==0)
            {
                in_file.get(num,50,'\n');
            }
            else if(strcmp(tmp,"Total fee for the week")==0)
            {   
                in_file>>total_fees;
            }
            in_file.ignore(50,'\n');
        }
        in_file.close();//Done read
    }
    //write in file, generate EFT report...
    string name_holds  = name;
    //split first and last name to generate report file name...
    char *tmp = strtok(name, " ");
    char *first = tmp;
    char *last;
    if(tmp){
        tmp = strtok(NULL, " ");
        last = tmp;
    }
    //convert char* to string
    string f(first);
    string l(last);
    //getting current date
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[50];
    tstruct = *localtime(&now);
    //format:DD-MM-YYYY
    strftime(buf,50, "%d-%m-%y", &tstruct);
    //convert char to string
    string st(buf);
    //getting/formatting the report file name
    const string t = f+l+st+".eft";//(ProviderFirstName)(ProviderLastName)(Date of Report).txt,
    fstream fin;
   // fin.open(t.c_str(), ios::out);
    fin.open(Build_Directory + "/Provider_Reports/" + t, ios::out);//write in the new file
    if (fin.is_open())
    {
        fin<<"Provider Name: "<<name_holds<<endl;
        fin<<"Provider Number: "<<num<<endl;
        fin<<"Weekly fees to be paid: "<<total_fees<<endl;
        cout<<"EFT report generated!"<<endl;
    }
    else
    {
        cout<<"Error: unable to open file and write EFT"<<endl;
        return false;
    }
    fin.close();
return true;
}




void unit_tests_chocancpp()
{
	string temp;
	string temp2;
	int temp3;
	cout << "Testing trim" << endl;
	temp = "Patrick Wood";
	temp2 = trim(temp);
	assert(temp2 == "PatrickWood");
	cout << "trim successful" << endl;
	cout << "Testing date_count" << endl;
	temp3 = date_count(11, 30, 2022);
	assert(temp3 == 8370);
	cout << "Testing verify_date" << endl;
	temp = "11-30-2022";
	assert(verify_date(temp));
	temp = "11-46-2022";
	assert(verify_date(temp) == false);
	cout << "verify_date successfull" << endl;
	cout << "All unit tests passed!" << endl;
}



void run_all_reports(void)
{
    string mem_dir = Build_Directory + "/Member_Records/";
    string prov_dir = Build_Directory + "/Provider_Records/";
    string line;
    for(const auto & entry : filesystem::directory_iterator(mem_dir))
    {
        line = entry.path().string();
        line = line.substr(line.find("/") + 1, line.size());
        line = line.substr(line.find("/") + 1, line.size());
        line = line.substr(0, line.find("."));
        run_member_report(line);
    }
    for(const auto & entry : filesystem::directory_iterator(prov_dir))
    {
        line = entry.path().string();
        line = line.substr(line.find("/") + 1, line.size());
        line = line.substr(line.find("/") + 1, line.size());
        line = line.substr(0, line.find("."));
        run_provider_report(line);
    }
}