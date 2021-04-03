/* File:    MinMaxNumber.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Returns the smallest and biggest number in a list 
   License: GNU GPLv3 (../LICENSE) 
*/
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "StringHandling.h"


int main() {
    // Define variables
    std::string rawInput;
    std::string lowerInput;
    std::vector<int> numList;


    // Get list of numbers from user
    int count = 1;
    std::cout << "Type 'done' when you are finished typing your list of numbers" << std::endl;
    do {
        std::cout << "Enter #";
        std::cout << count;
        std::cout << ": ";
        std::cin >> rawInput;
        lowerInput = ToLowerString(rawInput);

        // While user hasn't typed a number and hasn't typed 'done' throw error
        while (!IsStringNumber(lowerInput) && lowerInput != "done") {
            std::cout << "That is not a valid number, please try again: ";
            std::cin >> rawInput;
            lowerInput = ToLowerString(rawInput);
        }

        if (lowerInput != "done") {
            // If a valid number was given cast it to int and store in numList
            numList.push_back(std::stoi(lowerInput));
        }

        count++;
    } while (lowerInput != "done");


    // Find smallest and biggest numbers in numList
    int smallest = INT_MAX;
    int biggest = NULL;
    for (std::vector<int>::size_type i = 0; i < numList.size(); i++) {
        smallest = std::min(smallest, numList[i]);
        biggest = std::max(biggest, numList[i]);
    }


    // Output results
    std::cout << "The smallest number in your list is ";
    std::cout << smallest << std::endl;
    std::cout << "The biggest number in your list is ";
    std::cout << biggest << std::endl;

    // In windows, pause command gives user a chance to read results and quit
    system("pause");
    return 0;
}