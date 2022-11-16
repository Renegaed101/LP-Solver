#include <iostream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fraction.cpp" 


std::vector<std::string> optimizationVars;
std::vector<std::string> slackVars;

//Prints out the elements in a vector
template <typename T> 
void printVector (const std::vector<T>& v) {
    for (const auto& i:v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

//Prints out elements in a 2D vector
template <typename T> 
void print2DVector (const std::vector<T>& v) {
    for (const auto& i:v) {
        printVector(i); 
    }
}

bool checkInitiallyFeasible (const std::vector<std::vector<fraction>>& input) {
    for (int i = 0; i < input.size(); i++) {
        if (i==0)
            continue;
        else {
            if (input.at(0).at(0).getNumerator() < 0)
                return false;  
        }
    }
    return true; 
 }

//Helper function for readInput that takes a string splits words and stores 
//in a vector
std::vector<std::string> tokenize (const std::string& line) {
    std::vector<std::string> result {};
    std::string word;
    for (const auto& c : line) {
        if (c == ' ' || c == '\t') {
            if (word != "") 
                result.push_back(word);
            word = "";
        }
        else {
            word = word + c;
        }
    }
    return result;
}


//Reads in input from stdin and stores in dictionary form
void readInput (std::vector<std::vector<fraction>>& input) {  

    std::vector<std::string> strings {};
    
    std::string line;
    int i = 0;
    while (std::getline(std::cin,line)) {
        input.push_back(std::vector<fraction>{});
        strings = tokenize(line + " ");                    
        for (const auto& word : strings) {
            input.at(i).push_back(std::stod(word));       
        }
        i++;
    }
    input.at(0).insert(input.at(0).begin(),fraction{0});
}

int main() {
    
    std::vector<std::vector<fraction>> initDic;
    readInput(initDic);
    print2DVector(initDic);      
    std::cout << "The given dictionary is initally feasible: " << checkInitiallyFeasible(initDic) << std::endl;

    
   
}
