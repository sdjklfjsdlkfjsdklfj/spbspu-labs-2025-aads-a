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
    bool operator()(Graph::const_reference rhs) const
    {
      const VertexPair& vertexes = rhs.first;
      return lhs == vertexes.from;
    }
  };

  struct VertexEdgeTo
  {
    Vertex lhs{};
    bool operator()(Graph::const_reference rhs) const
    {
      const VertexPair& vertexes = rhs.first;
      return lhs == vertexes.to;
    }
  };
}

#if 0
bool kizhin::operator==(const Edge& lhs, const Edge& rhs)
{
  return lhs.from == rhs.from && lhs.to == rhs.to && lhs.weight == rhs.weight;
}
#endif

bool kizhin::operator<(const VertexPair& lhs, const VertexPair& rhs)
{
  const auto lhsTuple = std::tie(lhs.from, lhs.to);
  const auto rhsTuple = std::tie(rhs.from, rhs.to);
  return lhsTuple < rhsTuple;
}

kizhin::VertexContainer kizhin::getVertices(const Graph& graph)
{
  VertexContainer result;
  for (const auto& edge: graph) {
    const VertexPair& vertexes = edge.first;
    result.insert(vertexes.from);
    result.insert(vertexes.to);
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

