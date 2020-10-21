#include "scanner.h"
#include "node.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <regex>
#include <iostream>

bool SplitTokenRead(std::string Token, std::vector<std::string>& tokenList)
{
    bool check = false;
    std::regex e("([\\(\\)\\{\\},~\\[\\]:!=])|([^\\(\\)\\{\\},~\\[\\]:!=]+)");  
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    while (rit != rend) 
    {
        tokenList.push_back(rit->str());
        ++rit;
        check = true;
    }
    return check;
}

bool IsSeparatorOrOperator(std::string Token)
{
    const char* arr[] = { "!", "[", "]", "(", ")", "{", "}", " ", ":", ";", ",", "~", "+", "-", "*", "/", "^", "=" };
    for (auto c : arr)
    {
        if (0 == Token.compare(c))
        {
            return true;
        }
    }
    return false;
}
bool IsReservedWord(std::string Token)
{
    const char* arr[] = { "int", "char", "string", "listof", "incase", "ifnot", "then", "gountil", "do", "input", "echo", "lt", "lte", "equals", "ne", "gt", "gte", "increment", "decrement" };
    for (auto c : arr)
    {
        if (0 == Token.compare(c))
        {
            return true;
        }
    }
    return false;
}

bool CheckIfProperNumericalValue(std::string Token)
{
    std::regex e("^[1-9][0-9]*$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfProperIdentifier(std::string Token)
{
    std::regex e("^[a-zA-Z][0-9a-zA-Z]*$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfIdentifierOrConst(std::string Token)
{
    return CheckIfProperIdentifier(Token) | CheckIfProperNumericalValue(Token);
}

void StartScanning(char* InputFilePath)
{
    std::vector<std::string> tokenList;
    std::string inputPath = InputFilePath;
    std::map<std::string, int> pif;

    std::vector<std::string> listOfTokens;
    std::string token;

    std::string stoutput;

    std::ifstream inputFile(inputPath);
    std::ofstream piffile("./PIF.out");
    std::ofstream stfile("./ST.out");

    HashTable hashTable;

    while (inputFile >> token)
    {
        if (!(SplitTokenRead(token, tokenList)))
        {
            tokenList.push_back(token);
        }
    }
    
    for (std::string singleToken : tokenList)
    {
        if (IsReservedWord(singleToken) || IsSeparatorOrOperator(singleToken))
        {
            pif.emplace(singleToken, -1);
        }
        else
        { 
            if (!(CheckIfIdentifierOrConst(singleToken)))
            {
                fprintf_s(stderr, "LEXICAL ERROR\n");
                return;
            }
            hashTable.insertNode(singleToken);
            pif.emplace(singleToken, hashTable.getPosition(singleToken));
        }
    }
    fprintf_s(stdout, "Lexically correct!\n");

    piffile << "PIF\n";
    for (auto c : pif)
    {
        piffile << c.first << " " << c.second << std::endl;
    }
    
    stoutput = hashTable.displayToString();
    stfile << "Symbol Table\n";
    stfile << stoutput;

    piffile.flush();
    piffile.close();
    stfile.flush();
    stfile.close();
}