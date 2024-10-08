//
// Created by 郭修屹 on 2024/7/18.
//


#include <cassert>
#include "solver.h"

namespace sat {
    bool solver::solve() {
        return search_();
    }

    void solver::gen_learnt_clause_(const clause &reason) {
        std::vector<literal> cl;
        std::unordered_map<int, int> converted_id;
        std::vector<std::vector<int>> inv_graph(1);
        std::unordered_map<int, std::vector<literal>> upper_literals;
        literal var(0);
        for (int i = (int) history_.size() - 1; i >= 0 && history_[i].level() == level_; i--) {
            if (levels_[history_[i].var().id()] == level_ && reason.contains(-history_[i].var())) {
                var = history_[i].var();
                break;
            }
        }
        converted_id[static_cast<int>(-var)] = 0;
        for (const auto &lit: reason.literals()) {
            int pre = static_cast<int>(-lit);
            if (lit != -var && levels_[lit.id()] == level_) {
                converted_id[pre] = static_cast<int>(converted_id.size());
                inv_graph[0].emplace_back(converted_id[pre]);
            }
        }
        inv_graph.resize(converted_id.size());
        for (int i = (int) history_.size() - 1; i >= 0 && history_[i].level() == level_; i--) {
            int now = static_cast<int>(history_[i].var());
            if (!converted_id.contains(now)) {
                converted_id[now] = static_cast<int>(converted_id.size());
            }
            for (const auto &lit: history_[i].reason().literals()) {
                int pre = static_cast<int>(lit);
                if (pre != now && levels_[lit.id()] == level_ && !converted_id.contains(-pre)) {
                    converted_id[-pre] = static_cast<int>(converted_id.size());
                }
            }
            inv_graph.resize(converted_id.size());
            for (const auto &lit: history_[i].reason().literals()) {
                int pre = static_cast<int>(lit);
                if (pre != now && levels_[lit.id()] == level_) {
                    inv_graph[converted_id[now]].emplace_back(converted_id[-pre]);
                }
                else if (levels_[lit.id()] < level_) {
                    upper_literals[now].emplace_back(-lit);
                }
            }
        }
        std::vector<int> origin_id(converted_id.size());

        for (auto &k : converted_id) {
            origin_id[k.second] = k.first;
        }
        dom_tree tree(inv_graph);
        int first_uip = tree.lca(converted_id[var.id()], converted_id[-var.id()]);
        auto &topo_order = tree.topo_order();

        bool flag = false;
        for (int id : topo_order) {
            if (flag) {
                cl.insert(cl.end(), upper_literals[origin_id[id]].begin(), upper_literals[origin_id[id]].end());
            }
            else if (id == first_uip) {
                cl.emplace_back(-origin_id[id]);
                flag = true;
            }
        }
        clause added_clause(cl);
        cnf_->add_clause(added_clause, 0);
    }

    void solver::decide_(literal var, const clause &reason) {
        level_ += reason.empty();
//        std::cout << "decide: " << var << ' ' << (int) decisions_[var.id()]<< std::endl;
//        assert(decisions_[var.id()] == 0);
        decisions_[var.id()] = var.value();
        levels_[var.id()] = level_;
        history_.emplace_back(var, reason, level_);
        num_decisions_++;
    }

    void solver::backtrack_() {
        if (history_.empty())
            return;
        auto decision = history_.back();
//        assert(history_.size()+1 == cnf_->history_size());
//        assert(history_.size() == num_decisions_);
        history_.pop_back();
        decisions_[decision.var().id()] = 0;
        levels_[decision.var().id()] = -1;
        cnf_->backtrack();
        num_decisions_--;
        level_ = history_.empty() ? 0 : history_.back().level();
//        std::cout << "backtrack: ";
//        for (auto k : decisions_) {
//            std::cout << (int) k << ' ';
//        }
//        std::cout << std::endl;
    }

    void solver::backtrack_by_conflict_(const clause &clause) {
        while (num_decisions_ > 0 && !clause.contains(-history_.back().var()))
            backtrack_();
        while (num_decisions_ > 0 && history_.back().has_reason())
            backtrack_();
    }

    void solver::backtrack_to_level_(int level) {
        while (num_decisions_ > 0 && level_ > level)
            backtrack_();
        while (num_decisions_ > 0 && history_.back().has_reason())
            backtrack_();
    }

    result solver::analyze_() {
        auto results = cnf_->analyze(decisions_);
        if (results.empty()) {
            if (num_decisions_ == cnf_->num_vars()) {
                return SATISFIED;
            }
            for (int i = 1; i <= cnf_->num_vars(); i++) {
                if (decisions_[order_[i]] == 0) {
                    //std::cout << "decide_0 ";
                    decide_(literal(order_[i]), clause());
                    break;
                }
            }
        }
        for (const auto &result : results) {
            if (result.type == DECISION) {
                auto cl = (*cnf_)[result.id];
                //std::cout << "decide_1 ";
                decide_(result.decision, cl);
            }
            else {
                const auto &cl = (*cnf_)[result.id];
                //std::cout << "conflict: " << cl << std::endl;
                //gen_learnt_clause_(cl);
                backtrack_by_conflict_(cl);
                //backtrack_to_level_(level_);
                while (num_decisions_ > 0 && (int) history_.back().var() < 0) {
                    backtrack_to_level_(level_ - 1);
                }
                if (num_decisions_ == 0) {
                    return CONFLICT;
                }
                auto token = history_.back();
                backtrack_();
                //std::cout << "decide_2 ";
                decide_(-token.var(), clause());
            }
        }
        return NONE;
    }

    bool solver::search_() {
        while (true) {
            result res = analyze_();
            if (res == CONFLICT)
                return false;
            if (res == SATISFIED)
                return true;
        }
    }

    void solver::order_by_frequency() {
        std::vector<int> freq(cnf_->num_vars() + 1, 0);
        for (const auto &cl: cnf_->clauses()) {
            for (const auto &lit: cl.literals()) {
                freq[lit.id()]++;
            }
        }
        std::sort(order_.begin() + 1, order_.end(), [&](int a, int b) {
            return freq[a] > freq[b];
        });
    }

    void solver::order_by_random(int seed) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        std::shuffle(order_.begin() + 1, order_.end(), rnd);
    }


} // namespace sat
