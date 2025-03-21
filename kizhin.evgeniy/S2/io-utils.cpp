#include "io-utils.hpp"
#include <cassert>
#include <iostream>
#include "../common/queue.hpp"
#include "../common/stack.hpp"

namespace kizhin {
  Token constructToken(char);
}

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
  if (std::isdigit(in.peek())) {
    Token::number_type value = 0;
    if (in >> value) {
      token = Token(value);
    }
    return in;
  }
  const char current = in.get(); /* TODO: what will happen if in is empty? */
  if (current == '(' && in >> std::ws && in.peek() == '-') {
    Token::number_type value = 0;
    if (in >> value && in >> std::ws && in.get() == ')') {
      token = Token(value);
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  Token tmp = constructToken(current);
  if (tmp.type() == TokenType::unknown) {
    in.setstate(std::ios::failbit);
  } else {
    token = tmp;
  }
  return in;
}

kizhin::PostfixExpression kizhin::infixToPostfix(std::istream& input)
{
  /* TODO Rename and refactor func */
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

kizhin::Token kizhin::constructToken(const char symbol)
{
  switch (symbol) {
  case '+':
    return Token(std::addressof(Addition::instance()));
  case '-':
    return Token(std::addressof(Subtraction::instance()));
  case '*':
    return Token(std::addressof(Multiplication::instance()));
  case '/':
    return Token(std::addressof(Division::instance()));
  case '%':
    return Token(std::addressof(Modulus::instance()));
  case '(':
    return Token(BracketType::opening);
  case ')':
    return Token(BracketType::closing);
  default:
    return Token();
  }
}
