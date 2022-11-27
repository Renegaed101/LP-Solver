#include <iostream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fraction.cpp"
#include "dictionary.cpp" 
#include <utility>

struct pivotDetails {
    fraction amount {0};
    int enteringVarIndex {};
    int exitingVarIndex {};
    
};

struct UnboundedLPException {};

void dualMethod (dictionary lp) {
    
}


//Function that calculates the exiting variable for a choice of entering variable, returns 0 
std::pair<fraction,int> exitingVariable(dictionary lp, int j) {
    bool unbounded {true};   
    fraction candidateValue {0,1};
    int exitingVariableIndex;

    int i = 1;
    for (;i < lp.height();i++) {
        if (lp[i][j] < 0) {
            candidateValue = (lp[i][0]/lp[i][j]);
            exitingVariableIndex = i-1;
            unbounded = false;   
            break;  
        }
    }

    if (unbounded) {
        throw UnboundedLPException {};
    }
    
    for (;i < lp.height(); i++) {
        if (lp[i][j] < 0) {
            if (lp[i][0]/lp[i][j] > candidateValue) {
                candidateValue = (lp[i][0]/lp[i][j]);
                exitingVariableIndex = i-1;
            }
        }
    }

    return std::pair<fraction,int>{candidateValue,exitingVariableIndex};
}

 pivotDetails largestIncreaseRule(dictionary lp) {
    pivotDetails result;
    for (int j = 1; j < lp.length(); j++) {
        if (lp[0][j] > 0) {
            std::pair<fraction,int> exitVariable = exitingVariable(lp,j);
            fraction currentIncrease = lp[0][j] * exitVariable.first;
            if (currentIncrease >= result.amount) {
                result.amount = currentIncrease;
                result.enteringVarIndex = j-1;
                result.exitingVarIndex = exitVariable.second;
            }
        }
    }
    return result; 
}

void simplexMethod (dictionary lp) {
    while (true) {
        if (lp.isOptimal()) {
            std::cout << "optimal\n" << lp[0][0].toReal() << std::endl;
            exit(0);
        }
        auto [increase,enterVar,exitVar] = largestIncreaseRule(lp);
    }     
}



int main() {

    dictionary lp {};

    lp.print();    

    try {
        if (lp.isFeasible()) {
            simplexMethod(lp);
        } else {
            dualMethod(lp);
        }
    } catch (UnboundedLPException& e) {
        std::cout << "unbounded" << std::endl; 
    }

    std::cout << "Feasible: " << lp.isFeasible() << std::endl; 
    std::cout << "Optimal: " << lp.isOptimal() << std::endl;

}
