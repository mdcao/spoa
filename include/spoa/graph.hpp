/*!
 * @file graph.hpp
 *
 * @brief Graph class header file
 */

#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

namespace spoa {

class Node;
class Edge;

class Graph;
std::unique_ptr<Graph> createGraph();

using Alignment = std::vector<std::pair<int32_t, int32_t>>;

class Graph {
public:
    ~Graph();

    const std::vector<std::unique_ptr<Node>>& nodes() const {
        return nodes_;
    }

    const std::vector<uint32_t>& sorted_nodes_ids() const {
        return sorted_nodes_ids_;
    }

    uint32_t num_codes() const {
        return num_codes_;
    };

    uint8_t coder(uint8_t c) const {
        return coder_[c];
    }

    uint8_t decoder(uint8_t code) const {
        return decoder_[code];
    }

    void add_alignment(const Alignment& alignment, const std::string& sequence,
        uint32_t weight = 1);

    void add_alignment(const Alignment& alignment, const std::string& sequence,
        const std::string& quality);

    void add_alignment(const std::vector<std::pair<int32_t, int32_t>>& alignment,
        const std::string& sequence, const std::vector<uint32_t>& weights);

    void generate_multiple_sequence_alignment(std::vector<std::string>& dst,
        bool include_consensus = false);

    std::string generate_consensus();
    // returns coverages
    std::string generate_consensus(std::vector<uint32_t>& dst);

    std::unique_ptr<Graph> subgraph(uint32_t begin_node_id, uint32_t end_node_id,
        std::vector<int32_t>& subgraph_to_graph_mapping) const;

    void print_csv() const;

    friend std::unique_ptr<Graph> createGraph();
private:
    Graph();
    Graph(const Graph&) = delete;
    const Graph& operator=(const Graph&) = delete;

    static std::unique_ptr<Node> createNode(uint32_t id, uint32_t code);

    static std::unique_ptr<Edge> createEdge(uint32_t begin_node_id,
        uint32_t end_node_id, uint32_t label, uint32_t weight);

    uint32_t add_node(uint32_t code);

    void add_edge(uint32_t begin_node_id, uint32_t end_node_id, uint32_t weight);

    int32_t add_sequence(const std::string& sequence,
        const std::vector<uint32_t>& weights,
        uint32_t begin, uint32_t end);

    void topological_sort();

    bool is_topologically_sorted() const;

    void traverse_heaviest_bundle();

    uint32_t branch_completion(std::vector<int32_t>& scores,
        std::vector<int32_t>& predecessors,
        uint32_t rank);

    void check_multiple_sequence_alignment(const std::vector<std::string>& msa,
        const std::vector<std::string>& sequences,
        const std::vector<uint32_t>& indices) const;

    void extract_subgraph_nodes(std::vector<bool>& dst, uint32_t current_node_id,
        uint32_t end_node_id) const;

    uint32_t num_sequences_;
    uint32_t num_codes_;
    std::vector<int32_t> coder_;
    std::vector<int32_t> decoder_;
    std::vector<std::unique_ptr<Node>> nodes_;
    std::vector<uint32_t> sorted_nodes_ids_;
    std::vector<uint32_t> sequences_begin_nodes_ids_;
    std::vector<uint32_t> consensus_;
};

class Node {
public:
    ~Node();

    uint32_t id() const {
        return id_;
    }

    uint32_t code() const {
        return code_;
    }

    const std::vector<std::shared_ptr<Edge>>& in_edges() const {
        return in_edges_;
    }

    const std::vector<std::shared_ptr<Edge>>& out_edges() const {
        return out_edges_;
    }

    const std::vector<uint32_t>& aligned_nodes_ids() const {
        return aligned_nodes_ids_;
    }

    friend Graph;
private:
    Node(uint32_t id, uint32_t code);
    Node(const Node&) = delete;
    const Node& operator=(const Node&) = delete;

    uint32_t id_;
    uint32_t code_;
    std::vector<std::shared_ptr<Edge>> in_edges_;
    std::vector<std::shared_ptr<Edge>> out_edges_;
    std::vector<uint32_t> aligned_nodes_ids_;
};

class Edge {
public:
    ~Edge();

    uint32_t begin_node_id() const {
        return begin_node_id_;
    }

    uint32_t end_node_id() const {
        return end_node_id_;
    }

    friend Graph;
private:
    Edge(uint32_t begin_node_id, uint32_t end_node_id, uint32_t label,
        uint32_t weight);
    Edge(const Edge&) = delete;
    const Edge& operator=(const Edge&) = delete;

    void add_sequence(uint32_t label, uint32_t weight = 1);

    uint32_t begin_node_id_;
    uint32_t end_node_id_;
    std::vector<uint32_t> sequence_labels_;
    std::vector<int32_t> sequence_weights_;
    int32_t total_weight_;
};

}