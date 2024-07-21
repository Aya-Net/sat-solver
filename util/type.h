//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_TYPE_H
#define SAT_SOLVER_TYPE_H

#include <vector>

namespace sat {

    typedef char byte;

    enum result {
        CONFLICT, NONE, SATISFIED, DECISION
    };

} // namespace sat

#endif //SAT_SOLVER_TYPE_H
