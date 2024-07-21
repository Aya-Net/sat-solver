//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_ANALYSIS_RESULT_H
#define SAT_SOLVER_ANALYSIS_RESULT_H

#include "../model/literal.h"
#include "type.h"

namespace sat {

    class analysis_result {
      public:
        analysis_result() = delete;

        ~analysis_result() = default;

        analysis_result(result r, literal d) : type(r), decision(d) {};

        analysis_result(result r, int cl) : type(r), id(cl) {};

        analysis_result(result r, literal d, int cl) : type(r), decision(d), id(cl) {};

        result type;
        literal decision = literal(0);
        int id = -1;
    };
} // namespace sat
#endif //SAT_SOLVER_ANALYSIS_RESULT_H
