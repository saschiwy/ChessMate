#pragma once
#include <string>
#include <vector>

namespace ChessNS
{
    /*!
     * \class   StringHelper
     *
     * \brief   A string helper class.
     */
    class StringHelper
    {
    public:

        /*!
         * \fn  static std::vector<std::string> StringHelper::split(const std::string& s, const char seperator)
         *
         * \brief   Splits a string by a seperator
         *
         * \param   s           A std::string to process.
         * \param   seperator   The seperator.
         *
         * \returns A std::vector with the splitted strings;
         */
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
    };
}
