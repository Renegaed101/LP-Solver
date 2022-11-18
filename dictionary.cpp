#include <iostream> 
#include <vector>

class dictionary { 

    private:
        std::vector<std::string> optimizationVars;
        std::vector<std::string> slackVars;
        std::vector<std::vector<fraction>> dic;

        //Helper function for readInput that takes a string splits words and stores 
        //in a vector
        void tokenize (const std::string& line) {
            std::string word;
            for (const auto& c : line) {
                if (c == ' ' || c == '\t') {
                    if (word != "") 
                        dic.back().push_back(std::stod(word));
                    word = "";
                }
                else {
                    word = word + c;
                }
            }
        }

        //Reads in input from stdin and stores in dictionary form
        void readInput () {  
            
            std::string line;
            while (std::getline(std::cin,line)) {
                dic.push_back(std::vector<fraction>{});
                tokenize(line + " ");                    
            }
            dic.at(0).insert(dic.at(0).begin(),fraction{0});
        }

    public:
        dictionary() {
            readInput();
        } 

        
        //Prints out contents of dictionary
        void print() {
            for (const auto& i: dic) {
                for (const auto& j: i) {
                    std::cout << j << " ";   
                }
                std::cout << std::endl;
            } 
        }

        bool isFeasible() {
            for (int i = 0; i < dic.size(); i++) {
                if (i==0)
                    continue;
                else {
                    if (dic.at(0).at(0).getNumerator() < 0)
                        return false;  
                }
            }
            return true; 
        }


};