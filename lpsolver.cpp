#include <iostream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fraction.cpp"
#include "dictionary.cpp" 

struct pivotDetails {
    fraction amount {0};
    int enteringVarIndex {};
    int exitingVarIndex {};
    
};


void simplexMethod (dictionary lp) {
    
}

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
        //Throw unbounded exception here, remember to catch .
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


int main() {

    dictionary lp {};

    lp.print();    
 
    if (lp.isFeasible()) {
        simplexMethod(lp);
    } else {
        dualMethod(lp);
    }

    std::cout << "Feasible: " << lp.isFeasible() << std::endl; 
    std::cout << "Optimal: " << lp.isOptimal() << std::endl;

}
