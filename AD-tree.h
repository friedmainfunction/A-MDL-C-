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
	//孩子节点

	AD_Node(type_data& cache);
	//构造

	void make_vary_Node(vector<int>& variables, vector<float>& value);
	//对于AD_Node来说，任务是构造vary_Node

	void set_num(int num);
	//设置AD_Node值

	inline int get_num();
	//获取AD_Node计数
private:
	int num;
	//计数

	type_data& cache;
    vector<vector<float>>& my_data;
	//type_data当中的功能性对象
};

class vary_Node
{
public:
	vector<AD_Node*> children;
	//孩子

	vary_Node(type_data& cache);
	//构造

	int count_combine(vector<int>& variables, vector<float>& value);
	//对于特定组合，在数据集当中计数

	void make_AD_Node(vector<int>& variables, vector<float>& value);
	//对于vary_Node来说，任务是构造AD_Node

	inline int get_sum();
	//获取vary_Node所有子AD_Node计数之和

	inline int get_variable_now();
	//获取当前节点的变量

	int query(vector<int>& pa, float *value, int now);
	//给定leaf和PA以及取值，查询MDL

	float query(int leaf_val, vector<int>& pa, float* value);
	//入口函数

private:
	int sum;
	//所有子AD_Node的和
	int variable_now;
	//变量集合，最后一个值是当前节点的变量
	int upper_bound;
	type_data& cache;
	vector<vector<float>>& my_data;
	//type_data当中的功能性对象
};
#endif