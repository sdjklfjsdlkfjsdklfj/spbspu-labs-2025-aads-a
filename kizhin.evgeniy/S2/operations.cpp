#include "operations.hpp"
#include <stdexcept>

kizhin::BinaryOperation::number_type kizhin::Addition::eval(const number_type lhs,
    const number_type rhs) const
{
  return lhs + rhs;
}

kizhin::OperationPrecedence kizhin::Addition::precedence() const noexcept
{
  return OperationPrecedence::addition;
}

kizhin::Addition& kizhin::Addition::instance() noexcept
{
  static Addition instance;
  return instance;
}

kizhin::BinaryOperation::number_type kizhin::Subtraction::eval(const number_type lhs,
    const number_type rhs) const
{
  return lhs - rhs;
}

kizhin::OperationPrecedence kizhin::Subtraction::precedence() const noexcept
{
  return OperationPrecedence::subtraction;
}

kizhin::Subtraction& kizhin::Subtraction::instance() noexcept
{
  static Subtraction instance;
  return instance;
}

kizhin::BinaryOperation::number_type kizhin::Multiplication::eval(const number_type lhs,
    const number_type rhs) const
{
  return lhs * rhs;
}

kizhin::OperationPrecedence kizhin::Multiplication::precedence() const noexcept
{
  return OperationPrecedence::multiplication;
}

kizhin::Multiplication& kizhin::Multiplication::instance() noexcept
{
  static Multiplication instance;
  return instance;
}

kizhin::BinaryOperation::number_type kizhin::Division::eval(const number_type lhs,
    const number_type rhs) const
{
  if (rhs == 0) {
    throw std::logic_error("Division by zero");
  }
  return lhs / rhs;
}

kizhin::OperationPrecedence kizhin::Division::precedence() const noexcept
{
  return OperationPrecedence::division;
}

kizhin::Division& kizhin::Division::instance() noexcept
{
  static Division instance;
  return instance;
}

kizhin::BinaryOperation::number_type kizhin::Modulus::eval(const number_type lhs,
    const number_type rhs) const
{
  if (rhs == 0) {
    throw std::logic_error("Modulus by zero");
  }
  return lhs % rhs;
}

kizhin::OperationPrecedence kizhin::Modulus::precedence() const noexcept
{
  return OperationPrecedence::modulus;
}

kizhin::Modulus& kizhin::Modulus::instance() noexcept
{
  static Modulus instance;
  return instance;
}

