#ifndef csr_H
#define csr_H

#include <vector>
#include <cmath>
#include <limits>
#include <set>
#include <algorithm>

class csr {

    public:
        std::vector<double> AA;
        std::vector<size_t> JA;
        std::vector<size_t> IA;

        csr() = default;
        csr(std::initializer_list<std::initializer_list<double>> matrix) {
            for (auto& row : matrix) {
                size_t ia_index_begin = AA.size();
                size_t column = 0; 
                for (auto el : row) {
                    if (std::abs(el) > std::numeric_limits<double>::epsilon()) {
                        AA.push_back(el);
                        JA.push_back(column);
                    }
                    column++;
                }
                IA.push_back(ia_index_begin);
            } 
            IA.push_back(AA.size());
        }

        double x;
        double operator()(size_t i, size_t j) {
            size_t ind_begin = IA[i];
            size_t ind_end = IA[i + 1];

            //std::cout << ind_begin << " " << ind_end << std::endl;

            auto it_begin = std::next(JA.cbegin(), ind_begin);
            auto it_end = std::next(JA.cbegin(), ind_end);
            auto it = std::find_if(it_begin, it_end, [j](auto ind){ return j == ind;});

            return it != it_end ? AA[ind_begin + std::distance(it_begin, it)] : 0;
        }

        double operator()(size_t i, size_t j) const {
            size_t ind_begin = IA[i];
            size_t ind_end = IA[i + 1];

            //std::cout << ind_begin << " " << ind_end << std::endl;

            auto it_begin = std::next(JA.cbegin(), ind_begin);
            auto it_end = std::next(JA.cbegin(), ind_end);
            auto it = std::find_if(it_begin, it_end, [j](auto ind){ return j == ind;});

            return it != it_end ? AA[ind_begin + std::distance(it_begin, it)] : 0;
        }

        const std::vector<double>& get_aa() const {
            return AA;
        }

        const std::vector<size_t>& get_ja() const {
            return JA;
        }

        const std::vector<size_t>& get_ia() const {
            return IA;
        }

        friend csr operator+(const csr&, const csr&);
};

csr operator+(const csr& left, const csr& right) {
    csr addition_result;
 
    for (size_t row_id = 0; row_id < left.IA.size() - 1; ++row_id) {
        const size_t begin1 = left.IA[row_id];
        const size_t end1 = left.IA[row_id + 1];
        
        const size_t begin2 = right.IA[row_id];
        const size_t end2 = right.IA[row_id + 1];

        std::set<size_t> row1_set(left.JA.begin() + begin1, left.JA.begin() + end1);
        std::set<size_t> row2_set(right.JA.begin() + begin2, right.JA.begin() + end2);
        std::set<size_t> row_add_set;

        row_add_set.insert(row1_set.begin(), row1_set.end());
        row_add_set.insert(row2_set.begin(), row2_set.end());    

        size_t ia_index_begin = addition_result.AA.size();

        for (auto pos : row_add_set) {
            double val = 0;
            size_t cur_ja_pos = 0;
            if (row1_set.find(pos) != row1_set.end()) {
                auto it = row1_set.find(pos);
                auto it_aa_ja = begin1 + std::distance(row1_set.cbegin(), it); 
                val += left.AA[it_aa_ja];
                cur_ja_pos = left.JA[it_aa_ja];
            }
            if (row2_set.find(pos) != row2_set.end()) {
                auto it = row2_set.find(pos);
                auto it_aa_ja = begin2 + std::distance(row2_set.cbegin(), it);
                val += right.AA[it_aa_ja];
                cur_ja_pos = right.JA[it_aa_ja];
            }

            addition_result.AA.push_back(val);
            addition_result.JA.push_back(cur_ja_pos);        
        }
        
        addition_result.IA.push_back(ia_index_begin);
    }
    addition_result.IA.push_back(addition_result.AA.size());

    return addition_result;
}

#endif