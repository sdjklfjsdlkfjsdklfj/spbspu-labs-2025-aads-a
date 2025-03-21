#include <fstream>
#include <iostream>
#include <stdexcept>
#include "io-utils.hpp"
#include "postfix-expression.hpp"

int main(int argc, char** argv)
{
  using namespace kizhin;
  try {
    if (argc > 2) {
      std::cerr << "Usage: " << argv[0] << " [filename]\n";
      return 1;
    }
    std::ifstream fin;
    if (argc == 2) {
      std::string filePath = argv[1];
      fin.open(filePath);
      if (!fin) {
        throw std::logic_error("Failed to open file: " + filePath);
      }
    }
    std::istream& in = fin ? fin : std::cin;
    PostfixExpression expr = infixToPostfix(in);
    std::cout << expr.evaluate();
    std::cout << '\n';
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
