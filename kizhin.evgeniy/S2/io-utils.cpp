#include "io-utils.hpp"
#include <cassert>
#include <iostream>
#include "../common/queue.hpp"
#include "../common/stack.hpp"

kizhin::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  s_(s),
  fill_(s.fill()),
  precision_(s.precision()),
  width_(s.width()),
  flags_(s.flags())
{}

kizhin::StreamGuard::~StreamGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(flags_);
}

std::istream& kizhin::operator>>(std::istream& in, Token& token)
{
  StreamGuard guard(in);
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  in >> std::skipws;
  const char current = in.get();
  if (std::isdigit(current)) {
    in.unget();
    Token::number_type value = 0;
    if (in >> value) {
      token = Token(value);
    }
    return in;
  }
  switch (current) {
  case '+':
    token = Token(std::addressof(Addition::instance()));
    break;
  case '-':
    token = Token(std::addressof(Subtraction::instance()));
    break;
  case '*':
    token = Token(std::addressof(Multiplication::instance()));
    break;
  case '/':
    token = Token(std::addressof(Division::instance()));
    break;
  case '%':
    token = Token(std::addressof(Modulus::instance()));
    break;
  case '(':
    if (in.peek() == '-') {
      Token::number_type value = 0;
      if (in >> value && in.get() == ')') {
        token = Token(value);
      } else {
        in.setstate(std::ios::failbit);
      }
      return in;
    } else {
      token = Token(BracketType::opening);
    }
    break;
  case ')':
    token = Token(BracketType::closing);
    break;
  default:
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

kizhin::PostfixExpression kizhin::infixToPostfix(std::istream& input)
{
  Stack< Token > opStack;
  Queue< Token > outputQueue;
  Token token;
  while (input >> token) {
    if (token.type() == TokenType::number) {
      outputQueue.push(token);
    } else if (token.type() == TokenType::bracket) {
      BracketType br = token.bracket();
      if (br == BracketType::opening) {
        opStack.push(token);
      } else {
        while (!opStack.empty() && opStack.top().type() != TokenType::bracket) {
          outputQueue.push(opStack.top());
          opStack.pop();
        }
        if (!opStack.empty()) {
          opStack.pop();
        }
      }
    } else if (token.type() == TokenType::operation) {
      const BinaryOperation* currOp = token.operation();
      while (!opStack.empty() && opStack.top().type() == TokenType::operation) {
        const BinaryOperation* stackOp = opStack.top().operation();
        if (stackOp->precedence() >= currOp->precedence()) {
          outputQueue.push(opStack.top());
          opStack.pop();
        } else {
          break;
        }
      }
      opStack.push(token);
    }
  }
  while (!opStack.empty()) {
    outputQueue.push(opStack.top());
    opStack.pop();
  }
  return PostfixExpression(outputQueue);
}
