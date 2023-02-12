//
// Created by pdwoo on 10/21/2022.
//
#include <iostream>
#include <limits>
#include "validate.h"
using namespace std;

//Checks if character is a number.
static bool is_num(const char input)
{
    if(input < 58 && input > 47)
    {
        return true;
    }
    return false;
}


string getline_format(const string& prompt, const string& format, const string& err)
{
    string input; //Holds user input.
    bool valid; //Notes if the string is valid.

    while(true)
    {
        valid = true; //Input will be valid until proven invalid.
        cout << prompt;
        getline(cin, input);
        if(input == "abort")
        {
            throw input;
        }
        //checks the size of input and format.
        if(input.size() != format.size())
        {
            valid = false;
        }
        //Goes through each character of input and format to make sure they match.
        for(size_t i = 0; valid && i < input.size(); ++i)
        {
            //All if statements present are acceptable cases, if it doesn't pass any of them the input becomes invalid.
            if(format[i] == 'i' && is_num(input[i]))
            {
                continue;
            }
            else if(format[i] == 'c' && !is_num(input[i]))
            {
                continue;
            }
            else if(format[i] == 'x')
            {
                continue;
            }
            else if(format[i] == input[i])
            {
                continue;
            }
            valid = false;
            break; //Exits for loop to save time if input is invalid.
        }
        if(!valid)
        {
            cout << err << endl;
        }
        else
        {
            break; //exits while loop
        }
    }
    return input;
}



string getline_limit(const string& prompt, size_t character_limit, const string& err)
{
    string input;
    while(true)
    {
        cout << prompt;
        getline(cin, input);
        if(input.size() > character_limit)
        {
            cout << err << endl;
            continue; //Goes back and lets the user reformat their input to fit the character limit.
        }
        break; //Exits the loop for valid input.
    }
    return input;
}



bool verify_date(string date)
{
    bool valid = true;
    if(stoi(date.substr(0, date.find("-"))) > 12)
    {
        valid = false;
    }
    date = date.substr(date.find("-") + 1, date.size());
    if(stoi(date.substr(0, date.find("-"))) > 31)
    {
        valid = false;
    }
    return valid;
}
