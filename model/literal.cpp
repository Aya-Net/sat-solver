//
// Created by 郭修屹 on 2024/7/18.
//

#include "literal.h"

namespace sat {
    literal &literal::operator=(byte &value) {
        value_ = value;
        return *this;
    }

    bool literal::operator==(const literal &lit) const {
        return id_ == lit.id_ && value_ == lit.value_;
    }

    bool literal::operator<(const literal &lit) const {
        if (id_ == lit.id_)
            return value_ < lit.value_;
        return id_ < lit.id_;
    }

    std::ostream &operator<<(std::ostream &out, const literal &lit) {
        return out << lit.id_ * lit.value_;
    }

    std::istream &operator>>(std::istream &in, literal &lit) {
        int token;
        in >> token;
        lit = literal(token);
        return in;
    }

} // namespace sat