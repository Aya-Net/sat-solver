//
// Created by 郭修屹 on 2024/7/20.
//

#ifndef SAT_SOLVER_DECISION_H
#define SAT_SOLVER_DECISION_H

#include "../model/clause.h"

namespace sat {
    class decision {
      private:
        literal var_;

        clause reason_;

        int level_;

      public:

        decision() = delete;

        ~decision() = default;

        decision(const decision &decision) = default;

        decision(literal var, const clause &reason, int level) : var_(var), reason_(reason), level_(level) {}

        decision(literal var, int level) : var_(var), level_(level) {}

        [[nodiscard]] literal var() const { return var_; }

        [[nodiscard]] clause reason() const { return reason_; }

        [[nodiscard]] int level() const { return level_; }

        [[nodiscard]] bool has_reason() const { return !reason_.empty(); }

    };
} // namespace sat

#endif //SAT_SOLVER_DECISION_H
