#pragma once

enum class ASTNodeType
{
    Program,
    StmtSeq,
    IfStmt,
    ReadStmt,
    WriteStmt,
    AssignStmt,
    RepeatStmt,
    Expression,
    Term,
    Factor,
    Identifier,
    Number
};
