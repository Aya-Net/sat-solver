cmake_minimum_required(VERSION 3.28)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")

add_executable(SAT_Solver main.cpp
        util/dom_tree.cpp
        util/dom_tree.h
        model/clause.cpp
        model/clause.h
        model/literal.cpp
        model/literal.h
        model/cnf.cpp
        model/cnf.h
        util/cnf_reader.cpp
        util/cnf_reader.h
        solver.cpp
        solver.h
        util/type.h
        util/analysis_result.h
        util/decision.h
        util/constant.h)
