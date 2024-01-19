
#include "webserv.hpp"

TokenizeInput::TokenizeInputExceptions::TokenizeInputExceptions(const tokenization_errors &err , TokenizeInput *cln)
    : TException("Tokenization Error: ", err, cln)
{
    switch (err)
    {
        case E_FILE_NOT_FOUND:
            msg += "Config File Openning Error";
            break;
        case E_FILE_EMPTY:
            msg += "Empty Config File";
            break;
        case E_UNCLOSED_BRACKETS:
            msg += "Unclosed Brackets";
            break;
        default:
            msg += "Unknown Error";
    }
}

TokenizeInput::TokenizeInput(const char *path):tokens(NULL),brackets_count(0), tokens_received(false)
{
    ifile.open(path);
    if (!ifile.is_open())
        throw TokenizeInput::TokenizeInputExceptions(E_FILE_NOT_FOUND, NULL);
}

string    TokenizeInput::getToken(size_t &i, size_t token_start)
{
    i = line.find_first_of(SEPERATORS_TOKENS, i);
    if (i == string::npos)
        i = line.size();
    i--;
    return line.substr(token_start, i - token_start + 1);
}

void    TokenizeInput::insertTokens()
{
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == ' ' || line[i] == '\t')
            continue;
        else if (line[i] == '#')
            break;
        else if (line[i] == '{')
        {
            ++brackets_count;
            tokens->push_back("{");
        }
        else if (line[i] == '}')
        {
            --brackets_count;
            tokens->push_back("}");
        }
        else if (line[i] == ';')
            tokens->push_back(";");
        else
            tokens->push_back(getToken(i,i));
    }
}

list<string> *TokenizeInput::getTokensList() 
{
    tokens = new list<string>;
    while (getline(ifile, line))
        insertTokens();
    if (tokens->empty())
        throw TokenizeInputExceptions(E_FILE_EMPTY, this);
    if (brackets_count != 0)
        throw TokenizeInputExceptions(E_UNCLOSED_BRACKETS, this);
    tokens_received = true;
    return tokens;
}

TokenizeInput::~TokenizeInput()
{
    if (tokens && !tokens_received)
        delete tokens;
    if (ifile.is_open())
        ifile.close();
}
