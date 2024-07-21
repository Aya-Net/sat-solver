//
// Created by 郭修屹 on 2024/7/18.
//

#include "cnf.h"

namespace sat {

    void cnf::set_inactivity_(int i, int inactivity) {
        if (inactivity_[i] < 0) return; // origin clause is always active.
        int delta = (inactivity > CNF_IGNORE_RANGE) - (inactivity_[i] > CNF_IGNORE_RANGE);
        inactivity_count_ += delta;
        inactivity_[i] = inactivity;
    }

    void cnf::rebuild_() {
        std::vector<clause> new_clauses;
        std::vector<int> reindex(clauses_.size(), -1);
        for (int i = 0; i < clauses_.size(); i++) {
            if (inactivity_[i] < CNF_IGNORE_RANGE) {
                reindex[i] = (int) new_clauses.size();
                new_clauses.emplace_back(clauses_[i]);
            }
        }
        for (auto &v : history_) {
            for (auto &i : v) {
                i = reindex[i];
            }
        }
        for (int i = 0; i < clauses_.size(); i++) {
            if (reindex[i] >= 0) {
                satisfied_[reindex[i]] = satisfied_[i];
                inactivity_[reindex[i]] = inactivity_[i];
            }
        }
        clauses_ = new_clauses;
        satisfied_.resize(clauses_.size());
        inactivity_.resize(clauses_.size());
        inactivity_count_ = 0;
    }

    void cnf::add_clause(const clause &cl, int inactivity) {
        if (cl.empty() || cl.truth()) {
            return;
        }
        for (const auto& cl_: clauses_) {
            if (cl == cl_) {
                return;
            }
        }
        clauses_.emplace_back(cl);
        satisfied_.emplace_back(false);
        inactivity_.emplace_back(inactivity);
    }

    void cnf::add_clause(const clause &cl) {
        add_clause(cl, -1);
    }

    void cnf::add_history(const std::vector<int> &history) {
        history_.emplace_back(history);
        for (auto i: history) {
            satisfied_[i] = true;
        }
    }

    void cnf::backtrack() {
        if (history_.empty()) {
            return;
        }
        for (auto i: history_.back()) {
            if (i >= 0) satisfied_[i] = false;
        }
        history_.pop_back();
    }

    std::vector<analysis_result> cnf::analyze(const std::vector<byte> &decisions) {
        std::vector<int> history;
        std::vector<analysis_result> res;
        std::vector<bool> is_decided(num_vars_);
//        if (inactivity_count_ * CNF_REBUILD_RATIO > static_cast<double>(clauses_.size())) {
//            rebuild_();
//        }
        for (int i = 0; i < (int) clauses_.size(); i++) {
            if (satisfied_[i] || inactivity_[i] > CNF_IGNORE_RANGE)
                continue;
            auto result = clauses_[i].analyze(decisions);
            switch (result.type) {
                case CONFLICT:
                    set_inactivity_(i, 0);
                    add_history(history);
                    res.emplace_back(CONFLICT, i);
                    return std::move(res);
                case DECISION:
                    set_inactivity_(i, 0);
                    if (!is_decided[result.decision.id()]) {
                        add_history(history);
                        history.clear();
                        res.emplace_back(DECISION, result.decision, i);
                        is_decided[result.decision.id()] = true;
                    }
                    break;
                case SATISFIED:
                    set_inactivity_(i, inactivity_[i] + 1);
                    history.emplace_back(i);
                    break;
                case NONE:
                    break;
            }
        }
        if (res.empty()) add_history(history);
        return std::move(res);
    }

    void cnf::clear_history() {
        history_.clear();
        std::fill(satisfied_.begin(), satisfied_.end(), false);
    }

} // namespace sat

