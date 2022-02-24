#ifndef _ASSISTANT_FUNCTION_H
#define _ASSISTANT_FUNCTION_H
#include "varset.h"
#include "data.h"
#include "Node.h"
#include <vector>
#include <cstring>
#include <unordered_map>
using namespace std;

vector<varset> reconstruct_solution(Node* goal,//�յ�
                                    unordered_map<varset, Node*, myOwnHash>& generated_Nodes,//�������������ɽڵ��ӳ��
                                    byte variable_count,//��������
                                    type_data& cache);//�������ݵĶ���

void write_network(vector<varset>& optimal_parents, string& network_path);//��һ��������Ӧ�����Ÿ���д�뵽�ļ�
#endif