#include <iostream> 
#include <vector>

class dictionary { 

    private:
        std::vector<std::string> optimizationVars;
        std::vector<std::string> slackVars;
        std::vector<std::vector<fraction>> dic;
        int Length;
        int Height;

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

            Length = dic.front().size();
            Height = dic.size();
            

            for (int i = 1; i < Length; i++) {
                dic.front().at(i) = dic.at(0).at(i) * -1;               
            }

            for (int i = 1; i < Height; i++) {
                dic.at(i).insert(dic.at(i).begin(), dic.at(i).back() * -1);
                dic.at(i).pop_back();
            }
        }

    public:
        dictionary() {
            readInput();
            for (int i = 1; i < Length; i++) {
                optimizationVars.push_back('x' + std::to_string(i));
            }

            for (int i = 1; i < Height; i++) {
                slackVars.push_back('w' + std::to_string(i));
            }
        } 

        
        //Prints out contents of dictionary
        void print() {
            std::cout << std::endl;
            for (const auto& i: dic) {
                for (const auto& j: i) {
                    std::cout << j << " ";   
                }
                std::cout << std::endl;
            }
        }

        //Returns true if dictionary is feasible
        bool isFeasible() {
            for (int i = 1; i < Height; i++) {
                if (dic.at(i).front().getNumerator() < 0)
                    return false;  
                
            }
            return true; 
        }


        //Returns true if dictionary is optimal 
        bool isOptimal() {
            for (int i = 1; i < Length; i++) {
                if (dic.front().at(i) > 0) {
                    return false;
                }
            }
            return true; 
        }

        void pivot(int enterVar, int exitVar){
            std::cout << "Pivot - Entering: " << optimizationVars[enterVar] << ", Exiting: " <<
                slackVars[exitVar] << std::endl;
            
            std::string temp = optimizationVars[enterVar];
            optimizationVars[enterVar] = slackVars[exitVar];
            slackVars[exitVar] = temp;

            fraction divisor = dic[exitVar+1][enterVar+1] * -1;
            for (auto& elem: dic[exitVar+1]){
                elem = elem/divisor;
            }
            dic[exitVar+1][enterVar+1] = fraction{-1}/divisor;
                    
            for (int i = 0; i < Height; i++) {
                if (i == exitVar+1) {
                    continue;
                }
                fraction multiplier = dic[i][enterVar+1];
                dic[i][enterVar+1] = 0;
                for (int j = 0; j < Length; j++){
                    dic[i][j] = dic[i][j] + (dic[exitVar+1][j] * multiplier);
                }
            }
            
        }

        int length () { 
            return Length;
        }

        int height () { 
            return Height;
        }
        
        std::vector<fraction>& operator[] (int i) {
            return dic.at(i);
        } 


};