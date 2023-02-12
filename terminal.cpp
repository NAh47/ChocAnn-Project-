#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "memberStruct.h"
#include "providerStruct.h"
#include "providerServiceStruct.h"
#include "chocan.h"
#include "validate.h"
#define Build_Directory ".."

using namespace std;




void terminal_selection(void)
{
    int selection = 9;


    while(selection != 0)
    {
        cout << "Welcome to the simulated ChocAn terminal, please make your selection below:\n1) ChocAn Provider Terminal\n2) ChocAn Operator Terminal\n0) Quit simulation\nPlease enter your selection: ";
        cin >> selection;

        while(selection != 1 && selection != 2 && selection != 0)
        {
            cout << "Sorry, that input wasn't recognized, please re-enter your selection: ";
            cin >> selection;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(selection == 1)
        {
            provider_terminal();
        }
        else if(selection == 2)
        {
            operator_terminal();
        }
        else
        {
            break; // Formal break;
        }
    }

    cout << "Exiting program. Thank you for using our simulation.\n";
    
    exit(0);
}



void provider_terminal(void)
{
    string user_input;
    provider_Service_Record session_data;
    bool valid_provider;
    string member_status;
    string file_data;

    /*
        Terminal "switched on" and activated right away:
    */

    cout
            << "**********************************\n***Provider Terminal Activating***\n**********************************\n\n\n";
    while (true) //Endless loop for provider terminal log in.
    {
        //Provider log in
        try
        {
            do
            {
                session_data.providerNumber = getline_format("Please enter your 9 digit provider number: ",
                                                                 "iiiiiiiii",
                                                                 "Error: Please enter a number with 9 digits.");
                valid_provider = check_provider_num(session_data.providerNumber);
                if (valid_provider == false)
                {
                    cout << "Error: Provider number invalid." << endl;
                }
            } while (valid_provider == false);
        }
        catch (string& err)
        {
            break;
        }
        //Transfer into main provider terminal functionality after provider logs in.
        while (true) //Endless loop for asking member numbers.
        {
            try
            {
                session_data.memberNumber = getline_format("Please enter the clients 9 digit member number:",
                                                           "iiiiiiiii", "Error: Please enter a number with 9 digits.");
            }
            catch (string &err)
            {
                break; //Logs out provider.
            }
            member_status = check_member_num(session_data.memberNumber);
            cout << member_status << endl;
            if (member_status == "Member Valid")
            {
                do
                {
                    try
                    {
                        user_input = getline_format("Would you like to make a report? y/n:", "c",
                                                    "Error: Please enter y or n.");
                    }
                    catch (string &err)
                    {
                        break;
                    }
                    if (user_input != "y" && user_input != "n")
                    {
                        cout << "Error: Please enter y or n." << endl;
                    }
                } while (user_input != "y" && user_input != "n");
                if (user_input == "y")
                {
                    record_member_session(session_data);
                }
            }
        }
    }
};






void operator_terminal(void)
{
    cout << "*************************************************\n******** ChocAn Operator Terminal Activating ****\n*************************************************\n\n";

    string user_input;
    unsigned int option;

    while(true) //Endless loop for operator terminal
    {
        try
        {
            do
            {
                cout << "Create new provider:   1" << endl;
                cout << "Create new member:     2" << endl;
                cout << "Update provider info:  3" << endl;
                cout << "Update member info:    4" << endl;
                cout << "Delete provider:       5" << endl;
                cout << "Delete member:         6" << endl;
                cout << "Run member report:     7" << endl;
                cout << "Run provider report:   8" << endl;
                cout << "Run all reports:       9" << endl;
                user_input = getline_format("Please choose one of these options:", "i",
                                            "Please enter a one digit number between 1 and 9.");
                option = stoi(user_input);
                if(option < 1 || option > 9)
                {
                    cout << "Please enter a one digit number between 1 and 9." << endl;
                }
            } while(option < 1 || option > 9);
            try
            {
                switch (option)
                {
                    case 1:
                        create_provider_record();
                        break;
                    case 2:
                        create_member_record();
                        break;
                    case 3:
                        update_provider_record();
                        break;
                    case 4:
                        update_member_record();
                        break;
                    case 5:
                        delete_provider_record();
                        break;
                    case 6:
                        delete_member_record();
                        break;
                    case 7:
                        member_report();
                        break;
                    case 8:
                        provider_report();
                        break;
                    case 9:
                        run_all_reports();
                    default:
                        cout << "Under Construction" << endl;
                        break;
                }
            }
            catch(string& err)
            {}
        }
        catch (string& err)
        {
            break;
        }
    }
    /*

    Create a loop, give an option to exit
    {

        CREATE MEMBER:
            Create strings:

            Get from 'operator:
                • Member name (25 characters).
                • Member number (9 digits).
                • Member street address (25 characters).
                • Member city (14 characters).
                • Member state (2 letters).
                • Member zip code (5 digits).

                MEMBER STATUS SET TO 'Validated" automatically
 
            Create member_record struct, initializing it with strings
                When saving data, append the strings to these prefixes:
                    "Member Name: " 
                    "Member Number: " 
                    "Member Street Address: "
                    "Member City: "
                    "Member State: "
                    "Member Zip Code: "

            create_member_record(memberNumber, member_record);                                

            If true returned
                Success

            If false
                Catastrophic failure



        UPDATE MEMBER:
            Create strings:

            Get from 'operator:
                • Member name (25 characters).
                • Member number (9 digits). <--- Saved in seperate string variable
                • Member street address (25 characters).
                • Member city (14 characters).
                • Member state (2 letters).
                • Member zip code (5 digits).

            Create member_record struct, initializing it with strings
                When saving data, append the strings to these prefixes:
                    "Member Name: "
                    "Member Number: " 
                    "Member Street Address: "
                    "Member City: "
                    "Member State: "
                    "Member Zip Code: "

            update_member_record(memberNum, member_record_struct);
                If true returned
                    Success
                If false
                    Member doesnt exist or Catastrophic failure




        DELETE MEMBER:
            Create string variable

             Get member number from operator

            delete_member_record(string);
                if True returned
                    Success
                else if false
                    Member doesnt exit or failure



        CREATE PROVIDER:
            Create strings:

            Get from 'operator:
                • Provider name (25 characters).
                • Provider number (9 digits).
                • Provider street address (25 characters).
                • Provider city (14 characters).
                • Provider state (2 letters).
                • Provider zip code (5 digits).

            Create provider_record struct, initializing it with strings
                When saving data, append the strings to these prefixes:
                    "Provider Name: "
                    "Provider Number: " 
                    "Provider Street Address: "
                    "Provider City: "
                    "Provider State: "
                    "Provider Zip Code: "

                Initialize provider_record struct variable (Let's call it X), passing strings as a parameter.

                create_provider_record(providerNumberVariable, X);                                

                If true returned
                    Success

                If false
                    Catastrophic failure


        UPDATE PROVIDER:
            Create strings:

            Get from 'operator:
                • Provider name (25 characters).
                • Provider number (9 digits). <--- Saved in seperate string variable
                • Provider street address (25 characters).
                • Provider city (14 characters).
                • Provider state (2 letters).
                • Provider zip code (5 digits).

            Create provider_record struct, initializing it with strings
                When saving data, append the strings to these prefixes:
                    "Provider Name: "
                    "Provider Number: " 
                    "Provider Street Address: "
                    "Provider City: "
                    "Provider State: "
                    "Provider Zip Code: "

            update_provider_record(providerNumber, member_record_struct);
                If true returned
                    Success
                If false
                    Member doesnt exist or Catastrophic failure




        DELETE PROVIDER:
            Create string variable

            Get provider number from operator

            delete_provider_record(string);
                if True returned
                    Success
                else if false
                    Member doesnt exit or failure




        RUN MEMBER REPORT:
            run_member_report();




        RUN PROVIDER REPORT:
            run_provider_report();



        RUN EFT REPORT:
            run_eft_report();



        To Exit:
            Hit X or something to get out of loop

    }

    */

};
