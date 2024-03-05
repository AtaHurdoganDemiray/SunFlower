#include "stdafx.h"
#include <string>
#include <iostream>

using namespace std;

bool ModifyString(const std::string& orginaltext, const std::string& findtext, std::string& replacetext, std::string& output);


int main()
{
 
    std::string orginaltext;
    std::string findtext;
    std::string replacetext;
    std::string output;

    std::cout << "Orginal Text =" ;
    std::cin >> orginaltext;
    std::cout << endl;
    std::cout << "Find Text = " ;
    std::cin >> findtext;
    std::cout << endl;
   
   
    ModifyString(orginaltext,findtext,replacetext,output);


   
}
bool ModifyString(const std::string& orginaltext, const std::string& findtext,  std::string& replacetext, std::string& output)
{
    int offset = 0, a = 0, b = 0;

    a = orginaltext.find(findtext, offset);
    if (a != -1)
    {
        std::cout << "Replace Text= ";
        std::cin >> replacetext;
        std::cout << endl;

        while (a != 1)
        {
            for (int i = offset; i < a; ++i)
            {
                output += orginaltext[i];
            }
            output += replacetext;
            offset = a + findtext.length();
            a = orginaltext.find(findtext, offset);
        }
        b = orginaltext.length();
        for (int i = offset; i < b; ++i)
        {
            output += orginaltext[i];
        }
        std::cout << output << endl;
        return true;

    }
    else
    {
        std::cout << "Find Text Not Found ";
        return false;
    }
}
