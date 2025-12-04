#pragma once

#include "tokenStream.hpp"
#include "ASTNode.hpp"

class Parser
{
public:
    Parser(TokenStream &stream);

    ASTNode *parseProgram();

private:
    TokenStream &ts;

    ASTNode *parseStmtSeq();
    ASTNode *parseStatement();
    ASTNode *parseExpression();
    ASTNode *parseSimpleExpression();
    ASTNode *parseTerm();
    ASTNode *parseFactor();
    ASTNode *parseIfStatement();
    ASTNode *parseRepeatStatement();
    ASTNode *parseAssignStatement();
    ASTNode *parseReadStatement();
    ASTNode *parseWriteStatement();
};
