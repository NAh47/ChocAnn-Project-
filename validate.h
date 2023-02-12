#pragma once
//
// Created by pdwoo on 10/21/2022.
//

#ifndef GROUP9_VALIDATE_H
#define GROUP9_VALIDATE_H
#include <string>
using namespace std;

/* Gets input from the user in the format specified
 * Parameters:
 *     prompt: Gets printed to the user before asking for input.
 *     format: A string the function checks to make sure the format is correct.
 *     err: A string the function prints if the input does not match the format.
 * Format: Special characters in format are 'c' and 'i'.
 *     i: Checks to see if the character at the same position in the input is a number.
 *     c: Checks to see if the character at the same position in the input is not a number.
 *     x: Any character works at the same position in the input.
 *     Anything else: Directly checks to see if the character in the input and the format are the same.
 * Example:
 *     input = getline_format("Please enter the date in MM-DD-YYYY format: ", "ii-ii-iiii", "Incorrect format");
 */
string getline_format(const string& prompt, const string& format, const string& err);

//Gets input from the user limiting the amount of characters they can input by character_limit
string getline_limit(const string& prompt, size_t character_limit, const string& err);

//Enter a date in the form ii-ii-iiii, and it will return whether that date could exist.
bool verify_date(string date);
#endif //GROUP9_VALIDATE_H
