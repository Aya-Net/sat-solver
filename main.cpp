#include <iostream>
#include <fstream>
#include <vector>

#include "model/clause.h"
#include "util/cnf_reader.h"
#include "solver.h"
#include "util/dom_tree.h"

using namespace sat;

void fun_1() {
    int n = 1000;
    bool output_detail = false;
    //output_detail = true;
    std::ofstream file_out;
    if (output_detail) file_out.open("./output.txt");
    int tot_time = 0, min_time = 1000000000, max_time = 0;
    int min_id = 0, max_id = 0;
    for (int i = 1; i <= n; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        //std::ifstream file("../cnf/sample/sample-0"+std::to_string(i)+".cnf");
        std::ifstream file("../cnf/uf20-91/uf20-0"+std::to_string(i)+".cnf");
        //std::ifstream file("../cnf/uf50-218/uf50-0"+std::to_string(i)+".cnf");
        //std::ifstream file("../cnf/UUF50.218.1000/uuf50-0"+std::to_string(i)+".cnf");
        //std::ifstream file("../cnf/uf100-430/uf100-0" + std::to_string(i) + ".cnf");
        //std::ifstream file("../cnf/UUF100.430.1000/uuf100-0"+std::to_string(i)+".cnf");
        //std::ifstream file("../cnf/uf200-860/uf200-0"+std::to_string(i)+".cnf");
        auto cnf = cnf_reader(file).read();
        auto read = std::chrono::high_resolution_clock::now();
        file.close();
        solver sat_solver(cnf);
        sat_solver.order_by_frequency();
        //sat_solver.order_by_random(10);
        bool res = sat_solver.solve();
        auto end = std::chrono::high_resolution_clock::now();
        if (res) {
            cnf->clear_history();
            auto result = cnf->analyze(sat_solver.decisions());
            if (!result.empty()) std::cout << "Error: " << i << std::endl;
//            for (int j = 1; j <= cnf->num_vars(); j++) {
//                file_out << (int) sat_solver.decisions()[j] << " ";
//            }
//            file_out << std::endl;
            if (output_detail)
                file_out << (result.empty() ? "Satisfied" : "Incorrect");
        } else {
            if (output_detail)
                file_out << "Unsatisfied";
        }
        int read_time = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(read - start).count());
        int solve_time = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(end - read).count());
        tot_time += solve_time;
        if (solve_time < min_time) {
            min_time = solve_time;
            min_id = i;
        }
        if (solve_time > max_time) {
            max_time = solve_time;
            max_id = i;
        }


        if (output_detail)
            file_out << ", read time: " << (double) read_time / 1000
                      << "ms, solve time: " << (double) solve_time / 1000 << "ms\n";
    }
    std::cout << "Total time: " << (double) tot_time / 1000 << "ms";
    std::cout << "\nMin time(" + std::to_string(min_id) + "): " << (double) min_time / 1000 << "ms";
    std::cout << "\nMax time(" + std::to_string(max_id) + "): " << (double) max_time / 1000 << "ms";
    std::cout << "\nAverage time: " << (double) tot_time / 1000 / n << "ms";

    fclose(stdout);
}

int main() {
//    std::vector<std::vector<int>> inv_graph(5);
//    inv_graph[0] = {1, 2};
//    inv_graph[1] = {2};
//    inv_graph[2] = {};
//    inv_graph[3] = {1};
//    inv_graph[4] = {2};
//    dom_tree dom(inv_graph);
//    std::cout << dom.lca(0, 3) << std::endl;
//    for (int i = 0; i < 5; i++) {
//        std::cout << dom.dom(i) << std::endl;
//    }
    fun_1();
    return 0;
}
