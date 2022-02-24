#include "assistant_function.h"

vector<varset> reconstruct_solution(Node* goal,
									unordered_map<varset, Node*, myOwnHash>& generated_Nodes,
                                    byte variable_count, 
                                    type_data &cache)
{
    vector<varset> optimal_parents;
    for (int i = 0; i < variable_count; i++)
    {
        optimal_parents.push_back(varset(variable_count));
    }
    //创建一个vector，下标是变量编号，保存的varset是最优父集

    varset remaining_variables = goal -> get_variables();
    Node* current = goal;
    float score = 0;
    int count = remaining_variables.get_current_count();


    for (int i = 0; i < count; i++)
    {
        byte leaf = current -> get_leaf();
        //从哪条边来的
        score += cache.best_MDL(leaf, remaining_variables);
        //总代价累加
        varset parents = cache.get_optimal_parents(leaf, remaining_variables);
        //拿到这条边对应的最优父集
        optimal_parents[leaf] = parents;
        //赋值
        remaining_variables.remove(leaf);
        //剩下的变量去掉来的边再迭代
        current = generated_Nodes[remaining_variables];
    }

    return optimal_parents;
}

void write_network(vector<varset>& optimal_parents, string& network_path)
{
    fstream out;

    out.open(network_path.c_str(), ios::out);
    if (!out)
    {
        printf("Network_path_Error.\n");
        return;
    }
    //没打开报错
    for (int i = 1; i < optimal_parents.size(); i ++)
    {
        if (optimal_parents[i].get_current_count() > 0)
        {
            out << i;
            out << " : ";
        }
        //先输出变量序号
        varset &current_varset = optimal_parents[i];
        for (int j = 1; i < current_varset.get_count(); j ++)
        {
            if (current_varset.test(j))
            {
                out << j;
                out << " ";
            }
        }
        out << "\n";
        //再输出变量的最优父集当中变量
    }
}