//
// Created by 郭修屹 on 2024/7/18.
//

#include "dom_tree.h"

namespace sat {

    void dom_tree::topo_sort_() {
        std::stack<int> sta;
        std::vector<int> in;
        topo_order_.clear();

        for (int i = 0; i < (int) size_; ++i) {
            in.emplace_back(inv_graph_[i].size());
            if (inv_graph_[i].empty()) {
                sta.push(i);
            }
        }
        while (!sta.empty()) {
            int u = sta.top();
            sta.pop();
            if (u >= 0)
                topo_order_.emplace_back(u);
            for (int v: graph_[u]) {
                --in[v];
                if (!in[v]) {
                    sta.push(v);
                }
            }
        }
    }

    void dom_tree::build_() {
        topo_sort_();
        for (int i = 1; i < size_; ++i) {
            int u = topo_order_[i], v = inv_graph_[u][0];
            for (int j = 1; j < (int) inv_graph_[u].size(); ++j) {
                v = lca(v, inv_graph_[u][j]);
            }
            idom_[u] = v;
            pre_[u][0] = v;
            depth_[u] = depth_[v] + 1;
            for (int d = 1; d < pre_size_; ++d) {
                if (pre_[u][d - 1] == -1) {
                    break;
                }
                pre_[u][d] = pre_[pre_[u][d - 1]][d - 1];
            }
        }
    }

    int dom_tree::lca(int u, int v) {
        if (depth_[u] < depth_[v]) {
            std::swap(u, v);
        }
        for (int i = (int) pre_size_ - 1; i >= 0; --i) {
            if (pre_[u][i] > -1 && depth_[pre_[u][i]] >= depth_[v]) {
                u = pre_[u][i];
            }
        }
        if (u == v) {
            return u;
        }
        for (int i = (int) pre_size_ - 1; i >= 0; --i) {
            if (pre_[u][i] != pre_[v][i]) {
                u = pre_[u][i];
                v = pre_[v][i];
            }
        }
        return pre_[u][0];

    }

    dom_tree::dom_tree(const std::vector<std::vector<int>> &inv_graph) : inv_graph_(inv_graph),
                                                                         size_(inv_graph.size()),
                                                                         graph_(size_),
                                                                         depth_(size_),
                                                                         idom_(size_, -1),
                                                                         pre_size_((int) std::log(size_) + 1),
                                                                         pre_(size_,
                                                                              std::vector<int>(pre_size_, -1)) {
        for (int i = 0; i < size_; i++) {
            for (const int &j: inv_graph_[i]) {
                graph_[j].push_back(i);
            }
        }
        build_();
    }

} // namespace sat