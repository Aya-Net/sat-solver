//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_DOM_TREE_H
#define SAT_SOLVER_DOM_TREE_H

#include <vector>
#include <stack>
#include <iostream>

namespace sat {
    class dom_tree {
      private:

        size_t size_, pre_size_;

        std::vector<std::vector<int>> graph_, inv_graph_;

        std::vector<int> topo_order_, depth_, idom_;

        std::vector<std::vector<int>> pre_;

        void topo_sort_();

        void build_();

      public:
        dom_tree() = default;

        ~dom_tree() = default;

        explicit dom_tree(const std::vector<std::vector<int>> &inv_graph);

        int lca(int u, int v);

        int dom(int u) { return idom_[u]; }

        [[nodiscard]] const std::vector<int> &topo_order() const { return topo_order_; }
    };

} // namespace sat
#endif //SAT_SOLVER_DOM_TREE_H
