#include "command-processor.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <map.hpp>

namespace kizhin {
  void outGraphs(const GraphContainer&, std::ostream&);
  void outVertexes(const GraphContainer&, std::ostream&, std::istream&);
  void outOutbound(const GraphContainer&, std::ostream&, std::istream&);
  void outInbound(const GraphContainer&, std::ostream&, std::istream&);
  void bindEdge(GraphContainer&, std::istream&);
  void cutEdge(GraphContainer&, std::istream&);
  void create(GraphContainer&, std::istream&);
  void merge(GraphContainer&, std::istream&);
  void extract(GraphContainer&, std::istream&);
}

void kizhin::processCommands(GraphContainer& graphs, std::istream& in, std::ostream& out)
{
  using CmdContainer = Map< std::string, std::function< void(void) > >;
  const auto inRef = std::ref(in);
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "graphs", std::bind(outGraphs, std::cref(graphs), outRef) },
    { "vertexes", std::bind(outVertexes, std::cref(graphs), outRef, inRef) },
    { "outbound", std::bind(outOutbound, std::cref(graphs), outRef, inRef) },
    { "inbound", std::bind(outInbound, std::cref(graphs), outRef, inRef) },
    { "bind", std::bind(bindEdge, std::ref(graphs), inRef) },
    { "cut", std::bind(cutEdge, std::ref(graphs), inRef) },
    { "create", std::bind(create, std::ref(graphs), inRef) },
    { "merge", std::bind(merge, std::ref(graphs), inRef) },
    { "extract", std::bind(extract, std::ref(graphs), inRef) },
  };
  CmdContainer::key_type currKey;
  while (in >> currKey) {
    try {
      commands.at(currKey)();
    } catch (const std::logic_error&) {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      in.clear();
    }
  }
}

void kizhin::outGraphs(const GraphContainer& graphs, std::ostream& out)
{
  for (const auto& graph: graphs) {
    out << graph.first << '\n';
  }
}

void kizhin::outVertexes(const GraphContainer& graphs, std::ostream& out,
    std::istream& in)
{
  GraphContainer::key_type key{};
  if (!(in >> key)) {
    throw std::logic_error("Falied to input key");
  }
  VertexContainer vertexes = getVertices(graphs.at(key));
  for (const auto& vertex: vertexes) {
    out << vertex << '\n';
  }
}

void kizhin::outOutbound(const GraphContainer& graphs, std::ostream& out,
    std::istream& in)
{
  GraphContainer::key_type key{};
  Vertex vertex{};
  if (!(in >> key >> vertex)) {
    throw std::logic_error("Falied to input key");
  }
  Graph outbounds = getOutbound(graphs.at(key), vertex);
  for (const auto& edge: outbounds) {
    out << edge.to << ' ' << edge.weight << '\n';
  }
}

void kizhin::outInbound(const GraphContainer& graphs, std::ostream& out, std::istream& in)
{
  GraphContainer::key_type key{};
  Vertex vertex{};
  if (!(in >> key >> vertex)) {
    throw std::logic_error("Falied to input key");
  }
  Graph inbounds = getInbound(graphs.at(key), vertex);
  for (const auto& edge: inbounds) {
    out << edge.from << ' ' << edge.weight << '\n';
  }
}

void kizhin::bindEdge(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type key{};
  Vertex from{};
  Vertex to{};
  Weight weight{};
  if (!(in >> key >> from >> to >> weight)) {
    throw std::logic_error("Falied to input key");
  }
  graphs.at(key).insert({ from, to, weight });
}

void kizhin::cutEdge(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type key{};
  Vertex from{};
  Vertex to{};
  Weight weight{};
  if (!(in >> key >> from >> to >> weight)) {
    throw std::logic_error("Falied to input key");
  }
  GraphContainer::mapped_type& graph = graphs.at(key);
  auto pos = graph.find({ from, to, weight });
  if (pos == graph.end()) {
    throw std::logic_error("Failed to find edge");
  }
  graph.erase(pos);
}

void kizhin::create(GraphContainer& /*graphs*/, std::istream& /*in*/)
{
  //
}

void kizhin::merge(GraphContainer& graphs, std::istream& in)
{
  GraphContainer::key_type dest{};
  GraphContainer::key_type firstSrc{};
  GraphContainer::key_type secondSrc{};
  if (!(in >> dest >> firstSrc >> secondSrc)) {
    throw std::logic_error("Falied to input key");
  }
  if (!graphs.count(firstSrc) && !graphs.count(secondSrc) && graphs.count(dest)) {
    throw std::logic_error("Falied to perform merge");
  }
  GraphContainer::mapped_type result{};
  result.insert(graphs[firstSrc].begin(), graphs[firstSrc].end());
  result.insert(graphs[secondSrc].begin(), graphs[secondSrc].end());
  graphs[dest] = std::move(result);
}

void kizhin::extract(GraphContainer& /*graphs*/, std::istream& /*in*/)
{
  //
}

