//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_CNF_H
#define SAT_SOLVER_CNF_H

#include <vector>

#include "clause.h"

namespace sat {
    class cnf {
      private:
        size_t num_vars_ = 0;

        std::vector<clause> clauses_;

        std::vector<std::vector<int>> history_;

        std::vector<bool> satisfied_;

        int inactivity_count_ = 0;

        std::vector<int> inactivity_;

        void set_inactivity_(int i, int inactivity);

        void rebuild_();

      public:
        cnf() = default;

        ~cnf() = default;

        cnf(const cnf &cnf) = default;

        explicit cnf(size_t num_vars) : num_vars_(num_vars) {}

        void set_num_vars(size_t num_vars) {
            num_vars_ = num_vars;
        };

        [[nodiscard]] size_t num_vars() const { return num_vars_; }

        [[nodiscard]] const std::vector<clause> &clauses() const { return clauses_; }

        [[nodiscard]] size_t history_size() const { return history_.size(); }

        const clause &operator[](int i) { return clauses_[i]; }

        void add_clause(const clause &cl);

        void add_clause(const clause &cl, int inactivity);

        void add_history(const std::vector<int> &history);

        void clear_history() {
            history_.clear();
            satisfied_.clear();
            satisfied_.resize(clauses_.size());
        };

        void backtrack();

        std::vector<analysis_result> analyze(const std::vector<byte> &decisions);

    };
} // namespace sat

#endif //SAT_SOLVER_CNF_H
