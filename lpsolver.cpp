#include <iostream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fraction.cpp"
#include "dictionary.cpp" 


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
