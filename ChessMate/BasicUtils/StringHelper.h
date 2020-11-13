#pragma once
#include <string>
#include <vector>

namespace ChessNS
{
    class StringHelper
    {
    public:
        static std::vector<std::string> split(const std::string& s, const char seperator)
        {
            std::vector<std::string> output;
            std::string::size_type   prevPos = 0, pos = 0;

            while ((pos = s.find(seperator, pos)) != std::string::npos)
            {
                std::string substring(s.substr(prevPos, pos - prevPos));
                output.push_back(substring);
                prevPos = ++pos;
            }

            output.push_back(s.substr(prevPos, pos - prevPos)); // Last word
            return output;
        }

    private:
    };
}
