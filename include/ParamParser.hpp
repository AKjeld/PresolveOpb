#include "StdInclude.hpp"

std::pair<std::string, std::string> split(std::string str) { 
  std::vector<std::string> internal; 
  std::stringstream ss(str); // Turn the string into a stream. 
  std::string tok; 
 
  while(getline(ss, tok, ' ')) { 
    internal.push_back(tok); 
  } 
 
  return std::pair<std::string, std::string>(internal[0], internal[internal.size()-1]); 
}
namespace PresolveOpb {
struct ParamParser {
    static std::map <std::string, std::string> ParseParamFile(std::string filename) {
        std::map <std::string, std::string> paramList; 
        std::ifstream file(filename);
        if (file.fail()) {
            throw "Parameters not found. Path given: " + filename;
        }
        std::string s;

        while (std::getline(file, s)) {
            if (s.length() < 2 || s[0] == '#') continue;
            paramList.insert(split(s));
        }
        return paramList;
    }
};

}
