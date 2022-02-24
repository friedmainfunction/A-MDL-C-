#include "Node.h"
#include "varset.h"
#include "define.h"
#include "data.h"
#include "heuristic_function.h"
#include "assistant_function.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <queue>

using namespace std;

string dataset_path("wine.data");
//数据集

string network_path("C:\\Users\\hasee\\Desktop\\test.txt");
//最优网络写入的路径



unordered_map<varset, Node*, myOwnHash> generated_Nodes;
//隐式的顺序图

int Node_expended;
//节点数量

bool out_of_time;
//超时标记

byte variable_count;

varset all_variables(1);

int main()
{
    type_data cache(dataset_path);
    //读入数据集
    byte variable_count = cache.get_variable_count();
    //随机变量数目
    varset ancestors(variable_count + 1);
    varset scc(ancestors);
    //初始化两个集合
    priority_queue<Node*> openlist;
    
    scc.set_first_n(variable_count);
    //本情形，默认前n个变量都考虑
    //变量编号从1开始，0不算
    byte leaf(0);
    Node *root = new Node(0.0f, 0.0f, ancestors, leaf);
    //初始化根节点
    openlist.push(root);

    Node *goal = NULL;
    //找到目标的标记
    all_variables = ancestors;
    all_variables = all_variables | scc;
    //得到全集
    float upperbound = MAX_FLOAT;
    //路径长度的上界
    bool complete = false;
    //是否完成
    Node_expended = 0;
    out_of_time = false;
    //尚未拓展节点
    while (openlist.size() > 0 || !out_of_time)
    {
        //若openlist空或超时则跳出

        Node* u = openlist.top();
        //拓展节点
        openlist.pop();

        Node_expended ++;
        //计数
        varset variables = u -> get_variables();
        //取出当前节点集合
        if (variables == all_variables)
        {
            goal = u;
            break;
            //若已经到达终点，标记，退出
        }

        if (u -> get_f() > upperbound)
        {
            break;
            //若溢出，退出
        }

        u -> set_in_closedlist();
        //已经拓展，将其放入closedlist
        for (byte leaf = 0; leaf <= cache.get_variable_count(); leaf ++)
        {
            //枚举变量，看是否已经被包含
            if (variables.test(leaf))
                continue;
            //如果已经包含，跳过
            if (!scc.test(leaf))
                continue;
            //如果不需要考虑，跳过
            varset new_variables(variables);
            new_variables.set(leaf);
            //否则考虑并生成新集合进行标记
            Node *succ = generated_Nodes[new_variables];
            //查看是否已经生成过
            if (succ == NULL)
            {
                //尚未生成则直接生成
                float g = u -> get_g() + cache.best_MDL(leaf, new_variables);//to do
                //计算当前代价
                complete = false;
                //标记未完成
                float h = heuristic_function(new_variables, all_variables, cache, u, leaf);//to do
                //计算启发式函数值
                succ = new Node(g, h, new_variables, leaf);
                //生成该后继
                openlist.push(succ);
                //放入openlist
                generated_Nodes[new_variables] = succ;
                //建立映射关系
                continue;
            }

            if (succ -> check_in_closedlist())
            {
                //如果已经拓展完毕，略过
                continue;
            }

            float g = u -> get_g() + cache.best_MDL(leaf, new_variables);
            //已经生成但并未拓展，那么就计算一下当前这种访问方式的代价
            if (g < succ -> get_g())
            {
                //如果当前代价更低，那么替换当前路径
                succ -> set_leaf(leaf);
                succ -> set_g(g);

                openlist.push(succ);
            }
        }
    }
    //跳出循环之后
    if (goal != NULL)
    {
        //若找到目标
        printf("Found Solution: %f\n", goal -> get_f());
        //打印代价
        if (network_path.length() > 0)
        {
        //将最优网络写入文件存储
            vector<varset> optimal_parents = reconstruct_solution(goal, generated_Nodes, all_variables.get_count(), cache);
            write_network(optimal_parents, network_path);
        }
    }
    else
    {
        //若没有找到目标，打印代价的下界
        Node* u = openlist.top();
        printf("No Solution Found.\n");
        printf("Low Bound:%f\n", u -> get_f());
    }

    for (auto pair : generated_Nodes)
    {
        delete pair.second;
    }
    //释放空间
    return 0;
}