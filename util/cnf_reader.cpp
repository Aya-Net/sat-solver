//
// Created by 郭修屹 on 2024/7/18.
//

#include "cnf_reader.h"

#include <sstream>
#include <vector>
#include <memory>

namespace sat {
    std::shared_ptr<cnf> cnf_reader::read() {
        std::string line;
        int num_vars = 0, num_clauses = 0;
        std::shared_ptr<cnf> _cnf = std::make_shared<cnf>();
        while (std::getline(in_, line)) {
            std::istringstream iss(line);
            char prefix = static_cast<char>(iss.peek());
            if (prefix == 'c') {
                // Comment line, ignore
                continue;
            } else if (prefix == 'p') {
                // Problem line, format: p cnf num_vars num_clauses
                std::string p, format;
                iss >> p >> format >> num_vars >> num_clauses;
                _cnf->set_num_vars(num_vars);
            } else if (prefix == '%') {
                // End of file
                break;
            } else {
                // Clause line, ends with 0
                std::vector<literal> cl;
                int literal;
                while (iss >> literal && literal != 0) {
                    cl.emplace_back(literal);
                }
                _cnf->add_clause(clause(cl));
            }
        }

        return _cnf;
    }
} // namespace sat