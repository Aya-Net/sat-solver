//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_LITERAL_H
#define SAT_SOLVER_LITERAL_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "../util/type.h"

namespace sat {

    class literal {
      private:
        int id_;
        byte value_;

      public:
        literal() = default;

        ~literal() = default;

        literal(const literal &lit) = default;

        explicit literal(const int token) : id_(std::abs(token)), value_(token > 0 ? 1 : -1) {}

        literal(int id, byte value) : id_(id), value_(value) {}

        [[nodiscard]] int id() const { return id_; }

        [[nodiscard]] byte value() const { return value_; }

        [[nodiscard]] bool assigned() const { return value_ == 0; }

        [[nodiscard]] bool valid() const { return id_ > 0; }

        literal operator-() const { return {id_, static_cast<byte>(-value_)}; }

        literal &operator=(byte &value);

        literal &operator=(int token) {
            *this = literal(token);
            return *this;
        }

        bool operator==(const literal &literal) const;

        bool operator<(const literal &literal) const;

        explicit operator int() const { return id_ * value_; };

        friend std::ostream &operator<<(std::ostream &out, const literal &lit);

        friend std::istream &operator>>(std::istream &in, literal &lit);

    };
} // namespace sat

#endif //SAT_SOLVER_LITERAL_H
