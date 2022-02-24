#ifndef _ASSISTANT_FUNCTION_H
#define _ASSISTANT_FUNCTION_H
#include "varset.h"
#include "data.h"
#include "Node.h"
#include <vector>
#include <cstring>
#include <unordered_map>
using namespace std;

vector<varset> reconstruct_solution(Node* goal,//终点
                                    unordered_map<varset, Node*, myOwnHash>& generated_Nodes,//变量集合与生成节点的映射
                                    byte variable_count,//变量计数
                                    type_data& cache);//保存数据的对象

void write_network(vector<varset>& optimal_parents, string& network_path);//把一个变量对应的最优父集写入到文件
#endif