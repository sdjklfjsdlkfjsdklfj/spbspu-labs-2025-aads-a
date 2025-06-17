#include "graph.hpp"
#include <algorithm>
#include <tuple>

#if 0
#  include <boost/hash2/fnv1a.hpp>
#  include <boost/hash2/hash_append.hpp>

std::size_t std::hash< std::pair< std::string, std::string > >::operator()(
    const pair< string, string >& value) const
{
  namespace hash2 = boost::hash2;
  hash2::fnv1a_64 hasher;
  hash2::hash_append(hasher, {}, value);
  return hash2::get_integral_result< size_t >(hasher);
}
#endif

namespace kizhin {
  struct VertexEdgeFrom
  {
    Vertex lhs{};
    bool operator()(const Edge& rhs) const { return lhs == rhs.from; }
  };

  struct VertexEdgeTo
  {
    Vertex lhs{};
    bool operator()(const Edge& rhs) const { return lhs == rhs.to; }
  };
}

bool kizhin::operator==(const Edge& lhs, const Edge& rhs)
{
  return lhs.from == rhs.from && lhs.to == rhs.to && lhs.weight == rhs.weight;
}

bool kizhin::operator<(const Edge& lhs, const Edge& rhs)
{
  const auto lhsTuple = std::tie(lhs.from, lhs.to, lhs.weight);
  const auto rhsTuple = std::tie(rhs.from, rhs.to, rhs.weight);
  return lhsTuple < rhsTuple;
}

kizhin::VertexContainer kizhin::getVertices(const Graph& graph)
{
  VertexContainer result;
  for (const auto& edge: graph) {
    result.insert(edge.from);
    result.insert(edge.to);
  }
  return result;
}

kizhin::Graph kizhin::getOutbound(const Graph& graph, const Vertex& vertex)
{
  Graph result;
  auto out = std::inserter(result, result.end());
  std::copy_if(graph.begin(), graph.end(), out, VertexEdgeFrom{ vertex });
  return result;
}

kizhin::Graph kizhin::getInbound(const Graph& graph, const Vertex& vertex)
{
  Graph result;
  auto out = std::inserter(result, result.end());
  std::copy_if(graph.begin(), graph.end(), out, VertexEdgeTo{ vertex });
  return result;
}

