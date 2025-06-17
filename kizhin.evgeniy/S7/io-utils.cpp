#include "io-utils.hpp"
#include <iostream>

namespace kizhin {
  std::istream& operator>>(std::istream&, GraphContainer::mapped_type&);
}

std::istream& kizhin::operator>>(std::istream& in, GraphContainer& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  GraphContainer input{};
  while (in) {
    GraphContainer::key_type key{};
    GraphContainer::mapped_type graph{};
    if (in >> key >> graph) {
      input.emplace(std::move(key), std::move(graph));
    }
  }
  if (!input.empty()) {
    in.clear();
    dest = std::move(input);
  }
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, GraphContainer::mapped_type& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  GraphContainer::mapped_type input{};
  std::size_t count{};
  in >> count;
  while (in && count != input.size()) {
    Vertex from{};
    Vertex to{};
    Weight weight{};
    if (in >> from >> to >> weight) {
      input.insert({ std::move(from), std::move(to), weight });
    }
  }
  if (input.size() == count) {
    in.clear();
    dest = std::move(input);
  }
  return in;
}
