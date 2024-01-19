
#pragma once
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include "../helpers/helpers.hpp"

#define SEPERATORS_TOKENS " \t{};"

using std::string;
using std::list;
using std::ifstream;
using std::getline;
using std::cout;
using std::endl;


typedef  enum TOKENIZE_ERR_CODES
{
    E_FILE_NOT_FOUND,
    E_FILE_EMPTY,
    E_UNCLOSED_BRACKETS,
} tokenization_errors;

class TokenizeInput
{
    private:
        ifstream ifile;
        string line;
        list<string> *tokens;
        int      brackets_count;
        bool        tokens_received;
        string  getToken(size_t &i, size_t token_start);
    public:
        class TokenizeInputExceptions : public TException<tokenization_errors, TokenizeInput>
        {
            public:
                TokenizeInputExceptions(const tokenization_errors &err, TokenizeInput *cln);
                virtual const char *what() const throw()
                {
                    return TException::what();
                };
                virtual ~TokenizeInputExceptions() throw() 
                {};
        };
        TokenizeInput(const char *path);
        void    insertTokens();
        list<string>    *getTokensList();
        ~TokenizeInput();
};

