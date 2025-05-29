#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

struct Data
{
    std::string StrName;
    std::string Scheme;
    int StarHouseNum;
    int EndHouseNum;
};

bool isNumber(const std::string &s)
{
    if (s.empty())
    {
        return false;
    }

    for (char c : s)
    {
        if (!std::isdigit(c))
        {
            return false;
        }
    }

    return true;
}

std::string trimQuotes(const std::string &s)
{
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
    {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

std::vector<Data> fileOpen(const std::string &filename)
{
    std::ifstream inputFile(filename);
    std::vector<Data> segments;

    if (!inputFile.is_open())
    {
        std::cout << "File open fail: " << filename << std::endl;
        return segments;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(inputFile, line))
    {
        lineNum++;

        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ','))
        {
            fields.push_back(field);
        }

        if (fields.size() < 23)
        {
            std::cout << "Row #" << lineNum << " fail: not enough data" << std::endl;
            continue;
        }

        try
        {
            std::string strNamePart1 = trimQuotes(fields[16]);
            std::string strNamePart2 = trimQuotes(fields[17]);
            std::string Scheme = trimQuotes(fields[20]);

            if (!strNamePart1.empty() && !strNamePart2.empty() && !Scheme.empty())
            {
                Data segment;
                segment.StrName = strNamePart1 + " " + strNamePart2 + ":";
                segment.Scheme = Scheme;
                segment.StarHouseNum = isNumber(trimQuotes(fields[21])) ? std::stoi(trimQuotes(fields[21])) : 0;
                segment.EndHouseNum = isNumber(trimQuotes(fields[22])) ? std::stoi(trimQuotes(fields[22])) : 0;

                segments.push_back(segment);
            }
            else
            {
                continue;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Row #" << lineNum << " irrelevant: " << e.what() << std::endl;
        }
    }

    inputFile.close();
    return segments;
}

int main()
{
    std::string filename = "a.txt";
    //std::string filename = "network.mid";

    std::vector<Data> segments = fileOpen(filename);

    if (segments.empty())
    {
        std::cout << "Data Read NOTOK" << std::endl;
        return 1;
    }

    std::cout << "Row numbers: " << segments.size() << std::endl;

    for (const auto &segment : segments)
    {
        std::cout << segment.StrName << "\t"
                  << segment.Scheme << "\t"
                  << segment.StarHouseNum << "\t"
                  << segment.EndHouseNum << std::endl;
    }

    return 0;
}
