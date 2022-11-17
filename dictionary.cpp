#include <iostream> 
#include <vector>

class dictionary { 

    private:
        std::vector<std::vector<fraction>> dic;

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
        void readInput () {  

            std::vector<std::string> strings {};
            
            std::string line;
            int i = 0;
            while (std::getline(std::cin,line)) {
                dic.push_back(std::vector<fraction>{});
                strings = tokenize(line + " ");                    
                for (const auto& word : strings) {
                    dic.at(i).push_back(std::stod(word));       
                }
                i++;
            }
            dic.at(0).insert(dic.at(0).begin(),fraction{0});
        }

    public:
        dictionary() {
            readInput();
        } 

        void print() {
            for (const auto& i: dic) {
                for (const auto& j: i) {
                    std::cout << j << " ";   
                }
                std::cout << std::endl;
            } 
        }


};