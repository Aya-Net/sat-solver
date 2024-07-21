//
// Created by 郭修屹 on 2024/7/18.
//

#include "clause.h"

namespace sat {

    clause::clause(std::vector<literal> &literals) : literals_(std::move(literals)) {
        std::sort(literals_.begin(), literals_.end());
        literals_.erase(std::unique(literals_.begin(), literals_.end()), literals_.end());
    }

    analysis_result clause::analyze(const std::vector<byte> &decisions) const {
        int cnt = 0;
        literal decision(0);
        for (const auto &literal: literals_) {
            if (!decisions[literal.id()])
                decision = literal;
            else if (decisions[literal.id()] != literal.value())
                cnt++;
            else
                return {SATISFIED, {0, 0}};
        }
        if (cnt == size() - 1) { // fake decision
            return {DECISION, decision};
        }
        if (cnt == size()) { // conflict
            return {CONFLICT, {0, 0}};
        }
        return {NONE, {0, 0}}; // no conflict
    }

    bool clause::truth() const {
        return std::adjacent_find(literals_.begin(), literals_.end(),
                                  [](const literal &a, const literal &b) { return a == -b; })
               != literals_.end();
    }

    std::ostream &operator<<(std::ostream &out, const clause &clause) {
        for (const auto &lit: clause.literals_) {
            out << lit << (&lit != &clause.literals_.back() ? " " : "");
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, clause &cl) {
        int token;
        std::vector<literal> literals;
        while (in >> token) {
            if (token == 0)
                break;
            literals.emplace_back(token);
        }
        cl = clause(literals);
        return in;
    }
} // namespace sat