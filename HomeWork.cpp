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
    using Key = std::pair<std::string, std::string>;

    std::map<Key, std::map<int, int>> houseCounts;

    for (const auto &seg : segments)
    {
        Key key = {seg.StrName, seg.Scheme};
        int from = seg.StarHouseNum;
        int to = seg.EndHouseNum;

        if (from > to)
        {
            std::swap(from, to);
        }

        int step = 1;

        if (seg.Scheme == "O")
        {
            step = 2, from = (from % 2 == 0 ? from + 1 : from);
        }

        else if (seg.Scheme == "E")
        {
            step = 2, from = (from % 2 == 0 ? from : from + 1);
        }

        for (int i = from; i <= to; i += step)
        {
            houseCounts[key][i]++;
        }
    }

    std::cout << "\nDuplicated house numbers:\n";

    for (const auto &[key, counts] : houseCounts)
    {
        const auto &[street, scheme] = key;
        std::vector<std::pair<int, int>> ranges;

        int start = -1, prev = -1;

        for (const auto &[hnum, count] : counts)
        {
            if (count < 2)
            {
                continue;
            }

            if (start == -1)
            {
                start = prev = hnum;
            }
            else
            {
                int diff = hnum - prev;
                bool isSequential = (scheme == "M" && diff == 1) || ((scheme == "O" || scheme == "E") && diff == 2);

                if (isSequential)
                {
                    prev = hnum;
                }

                else
                {
                    ranges.emplace_back(start, prev);
                    start = prev = hnum;
                }
            }
        }

        if (start != -1)
        {
            ranges.emplace_back(start, prev);
        }

        for (const auto &[s, e] : ranges)
        {
            if (s == e)
            {
                std::cout << street << " " << scheme << " " << s << std::endl;
            }
            else
            {
                std::cout << street << " " << scheme << " " << s << " " << e << std::endl;
            }
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

    std::cout << "Valid row count (excluding invalid or empty rows): " << segments.size() << std::endl;

    printDuplicates(segments);

    return 0;
}
