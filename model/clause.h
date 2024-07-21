//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <utility>
#include <vector>
#include <iostream>

#include "literal.h"
#include "../util/analysis_result.h"
#include "../util/constant.h"

namespace sat {
    class clause {
      private:
        std::vector<literal> literals_;

      public:
        clause() = default;

        ~clause() = default;

        explicit clause(std::vector<literal> &literals);

        [[nodiscard]] size_t size() const { return literals_.size(); }

        [[nodiscard]] bool truth() const;

        [[nodiscard]] bool empty() const { return literals_.empty(); }

        [[nodiscard]] std::vector<literal> literals() const { return literals_; }

        [[nodiscard]] literal &operator[](size_t index) { return literals_[index]; }

        [[nodiscard]] bool contains(const literal &lit) const {
            return literals_.end() !=
                   std::find(literals_.begin(), literals_.end(),
                             lit);
        };

        analysis_result analyze(const std::vector<byte> &decisions) const;

        friend std::ostream &operator<<(std::ostream &out, const clause &cl);

        friend std::istream &operator>>(std::istream &in, clause &cl);

        bool operator==(const clause &cl) const { return literals_ == cl.literals_; }
    };
} // namespace sat

#endif //SAT_SOLVER_CLAUSE_H
