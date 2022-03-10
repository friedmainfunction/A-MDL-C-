#ifndef _DATA_H
#define _DATA_H
#include "define.h"
#include "set"
#include "varset.h"
#include "define.h"
#include <algorithm>
#include <locale.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;
class vector_int_Hash {
public:
   
    size_t operator()(std::vector<int> const& vec) const {
        std::size_t seed = vec.size();
        for (auto& i : vec) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
//vector哈希

class type_data
{
public:
    type_data(string dataset_path);
   
    byte get_variable_count();
    //获取当前数据集变量个数

    float best_MDL(byte leaf, varset& pa, bool* is_cut_by_superset);
    //递归计算BestMDL

    float best_MDL(byte leaf, varset& pa);
    //BestMDL入口函数

    varset get_optimal_parents(byte leaf, varset& pa);
    //获取给定一个变量和一个集合的最优父集

    float MDL(byte leaf, varset pa);
    //计算给定变量和集合的MDL
    //MDL保证pa不包含leaf

    void MDL(byte leaf, vector<int>& variables, int n_x_pa, int n_pa);
    //直接计算的方法

    float** reconstruct_combine(const vector<int>& variables, float** before, int* total, int dimension);
    //重新整合变量实例排列组合后的结果，去除不存在的向量

    float** combine(const vector<int>& variables, int depth, int* total_dimension_size);//depth从1开始
    //递归对给定变量的所有实例进行排列组合，得到存在向量的所有组合

    float** combine(const vector<int>& variables);
    //入口函数

    float H(const vector<int>& variables, float* current_combine);
    //计算针对当前变量实例组合的H

    //byte K(const vector<int>& variables, byte leaf, byte total_r);//传参的时候leaf已经减去1
    //计算针对当前变量实例组合的K

    byte K(byte leaf, const varset& pa);//传参的时候leaf已经减去1
    //方便调用K的入口重载

    vector<vector<float>>& get_my_data();
    //把数据集取出

    vector <set<float>>& get_one_property_type();
    //把变量的实例取出

    unordered_map<int, unordered_map<vector<int>, float, vector_int_Hash>>& get_MDL_score();
    //取出MDL分数的映射

private:
    int upper_bound;
    //父图节点数目上界

    byte variable_count;
    //当前数据集的变量数目

    vector<set<float>> one_property_type;
    //保存每一个变量对应的实例

    vector<vector<float>> my_data;
    //把数据集原封不动地拷贝过来

    unordered_map<int, unordered_map<varset, pair<varset, float>, varset::myOwnHash>> optimal_parent_variables;
    //每个变量对应一张父图，一张父图只有一个最优父集，最优父集又对应了一个分数

    unordered_map<int, unordered_map<vector<int>, float, vector_int_Hash>> MDL_score;
    //缓存MDL

    unordered_map<int, unordered_map<varset, bool, varset::myOwnHash>> is_cut_superset;
    //是否可以用超集剪枝
};
#endif

