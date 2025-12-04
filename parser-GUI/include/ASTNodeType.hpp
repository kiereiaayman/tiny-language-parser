#pragma once

enum class ASTNodeType {
  IfStmt,
  ReadStmt,
  WriteStmt,
  AssignStmt,
  RepeatStmt,
  OP,
  Identifier,
  Number
};