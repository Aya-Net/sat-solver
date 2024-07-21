//
// Created by 郭修屹 on 2024/7/18.
//

#ifndef SAT_SOLVER_CNF_READER_H
#define SAT_SOLVER_CNF_READER_H

#include <iostream>

#include "../model/cnf.h"

namespace sat {
    class cnf_reader {
      private:
        std::istream &in_;
      public:
        cnf_reader() = delete;

        ~cnf_reader() = default;

        explicit cnf_reader(std::istream &in) : in_(in) {}

        std::shared_ptr<cnf> read();
    };

} // namespace sat

#endif //SAT_SOLVER_CNF_READER_H
