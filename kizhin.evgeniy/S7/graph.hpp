#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S7_GRAPH_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S7_GRAPH_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#if 0
namespace std {
  template <>
  struct hash< pair< string, string > >
  {
    size_t operator()(const pair< string, string >&) const;
  };
}
#endif

namespace kizhin {
  using Vertex = std::string;
  using Weight = std::size_t;

  struct Edge
  {
    Vertex from{};
    Vertex to{};
    Weight weight{};
  };
  bool operator==(const Edge&, const Edge&);
  bool operator<(const Edge&, const Edge&);

  using Graph = std::set< Edge >;
  using VertexContainer = std::set< Vertex >;

  VertexContainer getVertices(const Graph&);
  Graph getOutbound(const Graph&, const Vertex&);
  Graph getInbound(const Graph&, const Vertex&);
}

#endif

