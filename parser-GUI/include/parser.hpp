#pragma once

#include "TokenStream.hpp"
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
    ASTNode *parseTerm();
    ASTNode *parseFactor();
};
