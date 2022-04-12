#ifndef _AD_TREE_H
#define _AD_TREE_H
#include "data.h"
#include <iostream>
#include <vector>
using namespace std;
class vary_Node;
class AD_Node
{
public:
	vector<vary_Node*> children;
	//���ӽڵ�

	AD_Node(type_data& cache);
	//����

	void make_vary_Node(vector<int>& variables, vector<float>& value);
	//����AD_Node��˵�������ǹ���vary_Node

	void set_num(int num);
	//����AD_Nodeֵ

	inline int get_num();
	//��ȡAD_Node����
private:
	int num;
	//����

	type_data& cache;
    vector<vector<float>>& my_data;
	//type_data���еĹ����Զ���
};

class vary_Node
{
public:
	vector<AD_Node*> children;
	//����

	vary_Node(type_data& cache);
	//����

	int count_combine(vector<int>& variables, vector<float>& value);
	//�����ض���ϣ������ݼ����м���

	void make_AD_Node(vector<int>& variables, vector<float>& value);
	//����vary_Node��˵�������ǹ���AD_Node

	inline int get_sum();
	//��ȡvary_Node������AD_Node����֮��

	inline int get_variable_now();
	//��ȡ��ǰ�ڵ�ı���

	int query(vector<int>& pa, float *value, int now);
	//����leaf��PA�Լ�ȡֵ����ѯMDL

	float query(int leaf_val, vector<int>& pa, float* value);
	//��ں���

private:
	int sum;
	//������AD_Node�ĺ�
	int variable_now;
	//�������ϣ����һ��ֵ�ǵ�ǰ�ڵ�ı���
	int upper_bound;
	type_data& cache;
	vector<vector<float>>& my_data;
	//type_data���еĹ����Զ���
};
#endif