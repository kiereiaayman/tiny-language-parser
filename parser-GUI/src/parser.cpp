#include "../include/parser.hpp"

ASTNode *Parser::parseProgram() { return parseStmtSeq(); }

Parser::Parser(TokenStream &stream) : ts(stream) {}

ASTNode *Parser::parseStmtSeq() {
  ASTNode *statementSequence = parseStatement();

  while (ts.match(TokenType::SEMICOLON)) {
    // Prevent empty statements like ";;"
    if (ts.peek().type == TokenType::SEMICOLON)
      throw std::runtime_error("Unexpected ';' - empty statement");

    statementSequence->sibling.push_back(parseStatement());
  }
  return statementSequence;
}

// const Token &peek(int offset = 0) const;
ASTNode *Parser::parseStatement() {
  Token currToken = ts.peek(0);
  switch (currToken.type) {
  case TokenType::IF:
    return parseIfStatement();
    break;

  case TokenType::REPEAT:
    return parseRepeatStatement();
    break;

  case TokenType::WRITE:
    return parseWriteStatement();
    break;

  case TokenType::READ:
    return parseReadStatement();
    break;

  case TokenType::IDENTIFIER:
    return parseAssignStatement();
    break;
  default:
    throw std::runtime_error("Unexpected token in statement at line " +
                             std::to_string(currToken.line));
  }
}

ASTNode *Parser::parseIfStatement() {
  ts.expect(TokenType::IF);

  ASTNode *ifNode = new ASTNode(ASTNodeType::IfStmt, "if");

  ASTNode *cond = parseExpression();
  ifNode->children.push_back(cond);

  ts.expect(TokenType::THEN);

  ASTNode *thenBlock = parseStmtSeq();
  ifNode->children.push_back(thenBlock);

  if (ts.peek().type == TokenType::ELSE) {
    ts.advance();
    ASTNode *elseBlock = parseStmtSeq();
    ifNode->children.push_back(elseBlock);
  }

  ts.expect(TokenType::END);
  return ifNode;
}

ASTNode *Parser::parseRepeatStatement() {
  ts.expect(TokenType::REPEAT);

  ASTNode *repeatNode = new ASTNode(ASTNodeType::RepeatStmt, "repeatStmt");

  ASTNode *stmtSeq = parseStmtSeq();
  repeatNode->children.push_back(stmtSeq);

  ts.expect(TokenType::UNTIL);

  ASTNode *exp = parseExpression();
  repeatNode->children.push_back(exp);

  return repeatNode;
}

ASTNode *Parser::parseAssignStatement() {
  Token identifier = ts.peek(0);
  ts.expect(TokenType::IDENTIFIER);

  ASTNode *AssignNode = new ASTNode(ASTNodeType::AssignStmt, identifier.value);

  ts.expect(TokenType::ASSIGN);

  ASTNode *exp = parseExpression();
  AssignNode->children.push_back(exp);

  return AssignNode;
}

ASTNode *Parser::parseReadStatement() {
  ts.expect(TokenType::READ);
  Token identifier = ts.peek(0);
  ts.expect(TokenType::IDENTIFIER);
  ASTNode *readNode = new ASTNode(ASTNodeType::ReadStmt, identifier.value);
  return readNode;
}

ASTNode *Parser::parseWriteStatement() {
  ts.expect(TokenType::WRITE);
  ASTNode *WriteNode = new ASTNode(ASTNodeType::WriteStmt, "writeStmt");
  WriteNode->children.push_back(parseExpression());
  return WriteNode;
}

ASTNode *Parser::parseExpression() {
  ASTNode *left = parseSimpleExpression();

  if (ts.match(TokenType::LESSTHAN) || ts.match(TokenType::EQUAL)) {
    Token op = ts.peek(-1); // last consumed token
    ASTNode *right = parseSimpleExpression();
    ASTNode *node = new ASTNode(ASTNodeType::OP, op.value);
    node->children.push_back(left);
    node->children.push_back(right);
    return node;
  }

  return left;
}

ASTNode *Parser::parseSimpleExpression() {
  ASTNode *left = parseTerm();

  while (ts.match(TokenType::PLUS) || ts.match(TokenType::MINUS)) {
    Token op = ts.peek(-1);
    ASTNode *right = parseTerm();
    ASTNode *node = new ASTNode(ASTNodeType::OP, op.value);
    node->children.push_back(left);
    node->children.push_back(right);
    left = node;
  }

  return left;
}

ASTNode *Parser::parseTerm() {
  ASTNode *left = parseFactor();

  while (ts.match(TokenType::MULT) || ts.match(TokenType::DIV)) {
    Token op = ts.peek(-1);
    ASTNode *right = parseFactor();
    ASTNode *node = new ASTNode(ASTNodeType::OP, op.value);
    node->children.push_back(left);
    node->children.push_back(right);
    left = node;
  }

  return left;
}

ASTNode *Parser::parseFactor() {
  Token t = ts.peek();

  if (ts.match(TokenType::NUMBER)) {
    return new ASTNode(ASTNodeType::Number, t.value);
  } else if (ts.match(TokenType::IDENTIFIER)) {
    return new ASTNode(ASTNodeType::Identifier, t.value);
  } else if (ts.match(TokenType::OPENBRACKET)) {
    ASTNode *node = parseExpression();
    ts.expect(TokenType::CLOSEDBRACKET);
    return node;
  }
  throw std::runtime_error("Unexpected token in factor: " + t.value);
}