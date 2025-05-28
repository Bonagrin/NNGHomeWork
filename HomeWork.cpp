#include <iostream>
#include <fstream>
#include <string>

bool FileOpen()
{

    std::ifstream inputFile("network.mid");

    if (inputFile.is_open())
    {
        std::string line;

        while (getline(inputFile, line))
        {
            std::cout << line << std::endl;
        }

        inputFile.close();

        return 1;
    }

    else
    {
        std::cout << "File open fail" << std::endl;
        return 0;
    }
}


int main()
{
    bool open_process = FileOpen();

    if (!open_process)
    {
        std::cout << "NOTOK Runtime" << std::endl;
        //...
    }

    else
    {
        std::cout << "OK Runtime" << std::endl;
        //...
    }

    return 0;
}