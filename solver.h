//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#include <vector>
#include <numeric>
#include <random>
#include <unordered_map>

#include "model/cnf.h"
#include "util/decision.h"
#include "util/dom_tree.h"

namespace sat {
    class solver {
      private:

        std::shared_ptr<cnf> cnf_;

        std::vector<byte> decisions_;

        std::vector<int> levels_;

        std::vector<decision> history_;

        std::vector<int> order_;

        int num_decisions_ = 0;

        int level_ = 0;

        literal next_decision_;

        bool search_();

        void backtrack_();

        void backtrack_by_conflict_(const clause& clause);

        void backtrack_to_level_(int level);

        void gen_learnt_clause_(const clause& reason);

        void decide_(literal token, const clause& reason);

        result analyze_();

      public:
        solver() = default;

        ~solver() = default;

        solver(const solver& solver) = default;

        [[nodiscard]] std::vector<byte> decisions() const { return decisions_; }

        explicit solver(auto cnf) : cnf_(cnf) {
            decisions_.resize(cnf->num_vars() + 1);
            levels_.resize(cnf->num_vars() + 1, -1);
            order_.resize(cnf_->num_vars() + 1);
            std::iota(order_.begin() + 1, order_.end(), 1);
        }

        void order_by_frequency();

        void order_by_random(int seed);

        bool solve();

    };
} // namespace sat

#endif //SAT_SOLVER_SOLVER_H
