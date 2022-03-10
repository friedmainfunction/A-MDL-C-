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
//vector��ϣ

class type_data
{
public:
    type_data(string dataset_path);
   
    byte get_variable_count();
    //��ȡ��ǰ���ݼ���������

    float best_MDL(byte leaf, varset& pa, bool* is_cut_by_superset);
    //�ݹ����BestMDL

    float best_MDL(byte leaf, varset& pa);
    //BestMDL��ں���

    varset get_optimal_parents(byte leaf, varset& pa);
    //��ȡ����һ��������һ�����ϵ����Ÿ���

    float MDL(byte leaf, varset pa);
    //������������ͼ��ϵ�MDL
    //MDL��֤pa������leaf

    void MDL(byte leaf, vector<int>& variables, int n_x_pa, int n_pa);
    //ֱ�Ӽ���ķ���

    float** reconstruct_combine(const vector<int>& variables, float** before, int* total, int dimension);
    //�������ϱ���ʵ��������Ϻ�Ľ����ȥ�������ڵ�����

    float** combine(const vector<int>& variables, int depth, int* total_dimension_size);//depth��1��ʼ
    //�ݹ�Ը�������������ʵ������������ϣ��õ������������������

    float** combine(const vector<int>& variables);
    //��ں���

    float H(const vector<int>& variables, float* current_combine);
    //������Ե�ǰ����ʵ����ϵ�H

    //byte K(const vector<int>& variables, byte leaf, byte total_r);//���ε�ʱ��leaf�Ѿ���ȥ1
    //������Ե�ǰ����ʵ����ϵ�K

    byte K(byte leaf, const varset& pa);//���ε�ʱ��leaf�Ѿ���ȥ1
    //�������K���������

    vector<vector<float>>& get_my_data();
    //�����ݼ�ȡ��

    vector <set<float>>& get_one_property_type();
    //�ѱ�����ʵ��ȡ��

    unordered_map<int, unordered_map<vector<int>, float, vector_int_Hash>>& get_MDL_score();
    //ȡ��MDL������ӳ��

private:
    int upper_bound;
    //��ͼ�ڵ���Ŀ�Ͻ�

    byte variable_count;
    //��ǰ���ݼ��ı�����Ŀ

    vector<set<float>> one_property_type;
    //����ÿһ��������Ӧ��ʵ��

    vector<vector<float>> my_data;
    //�����ݼ�ԭ�ⲻ���ؿ�������

    unordered_map<int, unordered_map<varset, pair<varset, float>, varset::myOwnHash>> optimal_parent_variables;
    //ÿ��������Ӧһ�Ÿ�ͼ��һ�Ÿ�ͼֻ��һ�����Ÿ��������Ÿ����ֶ�Ӧ��һ������

    unordered_map<int, unordered_map<vector<int>, float, vector_int_Hash>> MDL_score;
    //����MDL

    unordered_map<int, unordered_map<varset, bool, varset::myOwnHash>> is_cut_superset;
    //�Ƿ�����ó�����֦
};
#endif

