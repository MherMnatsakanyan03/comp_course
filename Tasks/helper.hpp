#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace Helper
{   
    template <typename T>
    void assertEqual(T actual, T expected, const std::string file, int line)
    {
        std::string fileName = file.substr(file.find_last_of("/\\") + 1);
        if (actual != expected)
        {
            std::cerr << "Assertion failed at " << fileName << ":" << line << ":"
                      << "\texpected " << expected << ", but got " << actual << std::endl;
            std::abort();
        }
    }

    std::string readFileIntoString(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return "";
        }
    
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}