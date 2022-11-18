#include <iostream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "fraction.cpp"
#include "dictionary.cpp" 

void simplexMethod (dictionary lp) {
    
}

void dualMethod (dictionary lp) {
    
}


int main() {

    dictionary lp {};

    lp.print();    
 
    if (lp.isFeasible()) {
        simplexMethod(lp);
    } else {
        dualMethod(lp);
    }

}
