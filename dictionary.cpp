#include <iostream> 
#include <vector>

class dictionary { 

    private:
        std::vector<std::string> optimizationVars;
        std::vector<std::string> slackVars;
        std::vector<std::vector<fraction>> dic;
        int length;
        int height;

        //Helper function for readInput that takes a string splits words and stores 
        //in a vector
        void tokenize (const std::string& line) {
            std::string word;
            for (const auto& c : line) {
                if (c == ' ' || c == '\t') {
                    if (word != "") 
                        dic.back().push_back(std::stod(word) * -1);
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
            dic.front().insert(dic.front().begin(),fraction{0});

            length = dic.front().size();
            height = dic.size();
            

            for (int i = 1; i < length; i++) {
                dic.front().at(i) = dic.at(0).at(i) * -1;               
            }

            for (int i = 1; i < height; i++) {
                dic.at(i).insert(dic.at(i).begin(), dic.at(i).back() * -1);
                dic.at(i).pop_back();
            }
        }

    public:
        dictionary() {
            readInput();
            for (int i = 1; i < length; i++) {
                optimizationVars.push_back('x' + std::to_string(i));
            }

            for (int i = 1; i < height; i++) {
                slackVars.push_back('w' + std::to_string(i));
            }
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

        //Returns true if dictionary is feasible
        bool isFeasible() {
            for (int i = 1; i < height; i++) {
                if (dic.at(i).front().getNumerator() < 0)
                    return false;  
                
            }
            return true; 
        }


        //Returns true if dictionary is optimal 
        bool isOptimal() {
            for (int i = 1; i < length; i++) {
                if (dic.front().at(i) > 0) {
                    return false;
                }
            }
            return true; 
        }

        
        


};