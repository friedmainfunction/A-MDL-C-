#include "assistant_function.h"

vector<varset> reconstruct_solution(Node* goal,
									unordered_map<varset, Node*, varset::myOwnHash>& generated_Nodes,
                                    byte variable_count, 
                                    type_data &cache)
{
    vector<varset> optimal_parents;
    for (int i = 0; i < variable_count; i++)
    {
        optimal_parents.push_back(varset(variable_count));
    }
    //����һ��vector���±��Ǳ�����ţ������varset�����Ÿ���

    varset remaining_variables = goal -> get_variables();
    Node* current = goal;
    float score = 0;
    int count = remaining_variables.get_current_count();


    for (int i = 0; i < count; i++)
    {
        byte leaf = current -> get_leaf();
        //������������

        score += cache.best_MDL(leaf, remaining_variables);
        //�ܴ����ۼ�

        remaining_variables.remove(leaf);
        //ʣ�µı���ȥ�����ı��ٵ���

        varset parents (cache.get_optimal_parents(leaf, remaining_variables));
        //�õ������߶�Ӧ�����Ÿ���

        optimal_parents[leaf] = parents;
        //��ֵ

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
    //û�򿪱���
    for (int i = 1; i < optimal_parents.size(); i ++)
    {
        varset& current_varset = optimal_parents[i];
        for (int j = 1, k = 0; j < current_varset.get_count(); j++)
        {
            if (current_varset.test(j))
            {
                k++;
                out << j;
                if (k != current_varset.get_current_count())
                    out << ",";
            }
        }
        //������������Ÿ������б���
        if (optimal_parents[i].get_current_count() > 0)
        {
            out << " -> ";
            out << i;
        }
        //����������

        out << "\n";
    }
}