#include "../include/scanner.hpp"
#include <fstream>
#include <iostream>

static const unordered_map<string, TokenType> reservedWords = {
    {"if", TokenType::IF},
    {"then", TokenType::THEN},
    {"end", TokenType::END},
    {"else", TokenType::ELSE},
    {"repeat", TokenType::REPEAT},
    {"until", TokenType::UNTIL},
    {"read", TokenType::READ},
    {"write", TokenType::WRITE}};

static const unordered_map<char, TokenType> singleCharTokens = {
    {'<', TokenType::LESSTHAN},
    {'=', TokenType::EQUAL},
    {'+', TokenType::PLUS},
    {'-', TokenType::MINUS},
    {'*', TokenType::MULT},
    {'/', TokenType::DIV},
    {'(', TokenType::OPENBRACKET},
    {')', TokenType::CLOSEDBRACKET},
    {';', TokenType::SEMICOLON}};

string tokenToString(TokenType t)
{
    switch (t)
    {
    case TokenType::IF:
        return "IF";
    case TokenType::THEN:
        return "THEN";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::END:
        return "END";
    case TokenType::REPEAT:
        return "REPEAT";
    case TokenType::UNTIL:
        return "UNTIL";
    case TokenType::READ:
        return "READ";
    case TokenType::WRITE:
        return "WRITE";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::LESSTHAN:
        return "LESSTHAN";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::MULT:
        return "MULT";
    case TokenType::DIV:
        return "DIV";
    case TokenType::OPENBRACKET:
        return "OPENBRACKET";
    case TokenType::CLOSEDBRACKET:
        return "CLOSEDBRACKET";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::ERROR:
        return "ERROR";
    case TokenType::ENDFILE:
        return "ENDFILE";
    }
    return "UNKNOWN";
}

TokenType getToken(ifstream &source, string &tokenString, int &lineNumber)
{
    char c;
    tokenString.clear();

    while (source.get(c))
    {
        if (c == '\n')
        {
            ++lineNumber;
            continue;
        }
        if (isspace(c))
            continue;

        // Identifier or keyword
        if (isalpha(c))
        {
            tokenString += c;
            while (source.get(c) && isalpha(c))
                tokenString += c;

            // if we stopped because we hit a digit, that's an error
            if (isdigit(c))
            {
                tokenString += c;
                return TokenType::ERROR;
            }

            if (source)
                source.unget();

            auto it = reservedWords.find(tokenString);
            return (it != reservedWords.end()) ? it->second : TokenType::IDENTIFIER;
        }

        // Number
        if (isdigit(c))
        {
            tokenString += c;
            while (source.get(c))
            {
                if (isdigit(c))
                    tokenString += c;
                else if (isalpha(c))
                {
                    tokenString += c;
                    return TokenType::ERROR;
                }
                else
                {
                    source.unget();
                    break;
                }
            }
            return TokenType::NUMBER;
        }

        // Assignment :=
        if (c == ':')
        {
            if (source.get(c))
            {
                if (c == '=')
                {
                    tokenString = ":=";
                    return TokenType::ASSIGN;
                }
                else
                {
                    source.unget();
                    tokenString = ":";
                    return TokenType::ERROR;
                }
            }
            else
            {
                tokenString = ":";
                return TokenType::ERROR;
            }
        }

        // Comments { ... }
        if (c == '{')
        {
            bool closed = false;
            int commentStartLine = lineNumber;
            while (source.get(c))
            {
                if (c == '\n')
                    ++lineNumber;
                if (c == '}')
                {
                    closed = true;
                    break;
                }
            }
            if (!closed)
            {
                cerr << "Error: Unterminated comment starting at line "
                     << commentStartLine << "\n";
                tokenString = "{";
                return TokenType::ERROR;
            }
            continue; // skip comment
        }

        // Single-character token
        auto it = singleCharTokens.find(c);
        if (it != singleCharTokens.end())
        {
            tokenString = c;
            return it->second;
        }

        // Unrecognized symbol
        tokenString = c;
        return TokenType::ERROR;
    }

    return TokenType::ENDFILE;
}
