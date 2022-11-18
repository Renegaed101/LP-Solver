#include <iostream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fraction.cpp"
#include "dictionary.cpp" 


std::vector<std::string> optimizationVars;
std::vector<std::string> slackVars;


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


int main() {

    dictionary x {};

    x.print();    
 
    /*
    for (int i = 1; i < initDic.at(0).size(); i++) {
        optimizationVars.push_back('x' + std::to_string(i));
    }

    for (int i = 1; i < initDic.size(); i++) {
        slackVars.push_back('w' + std::to_string(i));
    }

    
    if (checkInitiallyFeasible(initDic)) {
        simplexMethod(initDic);
    }
    else {
        dualMethod(initDic);
    }
   */
}
