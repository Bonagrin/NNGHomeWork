#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <tuple>
#include <map>

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
        return false;

    for (char c : s)
    {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

std::string trimQuotes(const std::string &s)
{
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);

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
            std::string scheme = trimQuotes(fields[20]);

            if (!strNamePart1.empty() && !strNamePart2.empty() && !scheme.empty())
            {
                Data segment;
                segment.StrName = strNamePart1 + " " + strNamePart2 + ":";
                segment.Scheme = scheme;
                segment.StarHouseNum = isNumber(trimQuotes(fields[21])) ? std::stoi(trimQuotes(fields[21])) : 0;
                segment.EndHouseNum = isNumber(trimQuotes(fields[22])) ? std::stoi(trimQuotes(fields[22])) : 0;

                segments.push_back(segment);
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

void printDuplicates(const std::vector<Data> &segments)
{
    std::map<std::tuple<std::string, std::string, int, int>, int> countMap;

    for (const auto &seg : segments)
    {
        auto key = std::make_tuple(seg.StrName, seg.Scheme, seg.StarHouseNum, seg.EndHouseNum);
        countMap[key]++;
    }

    std::cout << "Duplicate rows:" << std::endl;

    for (const auto &[key, count] : countMap)
    {
        if (count > 1)
        {
            const auto &[name, scheme, from, to] = key;
            std::cout<< name << " " << scheme << " " << from << " " << to << std::endl;
        }
    }
}

int main()
{
    std::string filename = "a.txt";
    std::vector<Data> segments = fileOpen(filename);

    if (segments.empty())
    {
        std::cout << "Data Read NOTOK" << std::endl;
        return 1;
    }

    std::cout << "Valid row count (excluding duplicates, invalid or empty rows): " << segments.size() << std::endl;

    printDuplicates(segments);

    return 0;
}
