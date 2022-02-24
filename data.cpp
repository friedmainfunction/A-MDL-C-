#include "data.h"
using namespace std;
inline void print_varset(varset & variables)
{
    if (variables.get_current_count() == 0)
    {
        cout << "NULL";
        return;
    }
    for (int i = 0; i < variables.get_count(); i++)
    {
        if (variables.test(i))
            cout << i << " ";
    }
}
//debug
type_data::type_data(string dataset_path)
{
    stringstream one_line;
    string original_line;
    fstream in;
    in.open(dataset_path, ios::in);
    //���ļ�
    if (!in)
    {
        printf("Cound Not Open File!\n");
        exit(0);
    }
    while (getline(in, original_line))//����һ�У�ֱ���ļ�β
    {
        one_line.clear();
        //���״̬
        one_line.str(original_line);
        //����ǰ�����д����ļ���
        my_data.push_back(vector<float>());
        //�½�һ��vector�浱ǰ��
        float one_data(0);
        //һ������ʵ��
        while (one_line >> one_data)
        {
            char divided_char;
            one_line >> divided_char;
            //�˵��ָ���
            my_data.back().push_back(one_data);
            //��ǰ���ݴ��뵱ǰvector
        }
    }
    variable_count = my_data.back().size();
    //һ�е��������Ǳ�������
    /*
    for (int i = 0; i < my_data.size(); i++)
    {
        for (auto k : my_data[i])
        {
            cout << k << " ";
        }
        cout << endl;
    }
    //debug
    */
    for (int i = 0; i < variable_count; i++)
    {
        one_property_type.push_back(set<float>());
        //����set
    }

    for (int i = 0; i < my_data.size(); i++)
        for (int j = 0; j < variable_count; j++)
        {
            one_property_type[j].insert(my_data[i][j]);
        }
    //������ȫ������setȥ��
    /*
    for (int i = 0; i < one_property_type.size(); i++)
    {
        cout << one_property_type[i].size() << endl;
        for (auto j = one_property_type[i].begin(); j != one_property_type[i].end(); j++)
        {
            cout << *j << " ";
        }
        cout << endl;
    }
    //debug
    */
}

byte type_data::get_variable_count()
{
    return variable_count;
}

float type_data::best_MDL(byte leaf, varset& pa, bool* is_cut_by_superset)
{
    int upper_bound = (log(2 * my_data.size()) - log(log(my_data.size())));
    //upper_bound = INT_MAX;
    //debug
    //���㸸ͼ�����������Ͻ�
    bool is_cut_by_number = false, is_now_cut_by_superset = false;
    //ͨ������������֦��ͨ��������֦�ı�־
    /*if (!pa.get_current_count())
        return MAX_FLOAT;
        wrong?
    */

    if (optimal_parent_variables[leaf].count(pa))
    {
        //����Ѿ����������ô��һ���Ƿ񳬼����Լ�֦����������
        if (log(my_data.size()) * K(leaf - 1, pa) / 2 < optimal_parent_variables[leaf][pa].second)
            *is_cut_by_superset = true;
        //������֦
        return optimal_parent_variables[leaf][pa].second;
        //ֱ�ӷ���ֵ
    }

    optimal_parent_variables[leaf][pa] = make_pair(pa, MAX_FLOAT);
    //��û�м�������ȸ�һ�������ĳ�ֵ
    //Ȼ��ݹ�����Ӽ�MDL���پ����費��Ҫ���㵱ǰ����MDL
    if (upper_bound < pa.get_current_count())
    {
        is_cut_by_number = true;
        //�����ǰ���ϵ����������Ͻ磬����ͨ��������֦
    }
    //��ͼ�ڵ�Ԫ��������֦

    bool* is_now_cut_by_superset_flag = new bool;
    *is_now_cut_by_superset_flag = false;
    //��ǰ�Ƿ���Ҫͨ��������֦�������Ϊ��������ݹ�ĺ���
    for (int i = 1; i < pa.get_count(); i++)
    {
        //����ÿһ�������еı�����ȥ������Ȼ�����ݹ�
        if (pa.test(i))
        {
            float score = optimal_parent_variables[leaf][pa].second;
            //��¼֮ǰ�ķ���
            pa.remove(i);
            //ȥ�����ڵ�һ������
            float next_score = best_MDL(leaf, pa, is_now_cut_by_superset_flag);
            //����һ���ӷ���
            if (*is_now_cut_by_superset_flag)
                is_now_cut_by_superset = true;
            //������Լ�֦����ǣ����治����MDL
            varset& inner_pa = optimal_parent_variables[leaf][pa].first;
            //��¼һ�µ�ǰ���������Լ������µ����Ÿ���
            pa.set(i);
            //��ȥ���ı����Ż�
            if (next_score < score)
            {
                cout << "leaf: " << leaf << "i: " << i << endl;
                cout << "next_score: " << next_score << " score: " << score << endl;
                cout << "inner_pa: "; print_varset(inner_pa); cout << endl;
                cout << "pa: "; print_varset(pa); cout << endl;
                cout << endl;
                //debug
                optimal_parent_variables[leaf][pa] = make_pair(inner_pa, next_score);
                //�����ǰ����ķ������ţ�����ļ�¼
            }
        }
    }
    if (!is_cut_by_number && !is_now_cut_by_superset)
    {
        //�����֦�����ɹ�������㵱ǰ��MDL
        float current_pa_score = MDL(leaf, pa);
        if (current_pa_score < optimal_parent_variables[leaf][pa].second)
            optimal_parent_variables[leaf][pa] = make_pair(pa, current_pa_score);
        cout << endl << endl;
        cout << "leaf: " << leaf << "PA: "; print_varset(pa); cout << endl;
        cout << "MDL_score: " << current_pa_score << endl;
        cout << endl << endl;
        //debug
        //������ţ����¼
    }
    /*
    cout << "leaf: " << leaf << endl;
    cout << "PA: "; print_varset(pa); cout << endl;
    cout << "best_pa: "; print_varset(optimal_parent_variables[leaf][pa].first); cout << endl;
    */
    //debug
    if (log(my_data.size()) * K(leaf - 1, pa) / 2 < optimal_parent_variables[leaf][pa].second)
        *is_cut_by_superset = true;
    //����һ�±������ϳ����Ƿ���Լ�֦
    delete is_now_cut_by_superset_flag;
    //ɾ����ǰ����Ŀռ�
    return optimal_parent_variables[leaf][pa].second;
}

float type_data::best_MDL(byte leaf, varset& pa)
{
    //��ں�������ӱ���leaf����pa�����
    bool flag = false;
    if (pa.test(leaf))
    {
        pa.remove(leaf);
        flag = true;
    }
    bool* is_cut_by_superset = new bool;
    *is_cut_by_superset = false;
    //return best_MDL(leaf, pa, is_cut_by_superset);
    float answer = best_MDL(leaf, pa, is_cut_by_superset);
    if (flag)
        pa.set(leaf);
    delete is_cut_by_superset;
    //�ͷſռ�
    return answer;
}

varset type_data::get_optimal_parents(byte leaf, varset pa)
{
    return optimal_parent_variables[leaf][pa].first;
}

float type_data::MDL(byte leaf, varset pa)
{
    if (pa.test(14) && pa.test(13))
        system("pause");
    float h = 0, k = 0;
    //MDL��֤pa������leaf
    vector<int> variables;
    for (int i = 1; i < pa.get_count(); i++)
    {
        if (pa.test(i))
            variables.push_back(i - 1);
        //�������ı�����Ŵ���һ��vector
        //��������������б�����Ŵ�0��ʼ�������Ҫת��һ��
    }



    variables.push_back(leaf - 1);
    //leaf������vector�����
    int* variables_combine_size = new int;
    //��������ʵ����Ϻ����Ŀ
    float** variables_combine = combine(variables, 1, variables_combine_size);
    //�ݹ�������
    float* current_row = new float[variables.size()];
    //��������һ��
    for (int i = 0; i < *variables_combine_size; i++)
    {
        //ɨ��ÿһ�У�ȡ��
        for (int j = 0; j < variables.size(); j++)
        {
            current_row[j] = variables_combine[j][i];
            //cout << current_row[j] << " ";
            //debug
        }

        /*if (variables.size() == 2)
        {
            if (current_row[0] == 345)
                system("pause");
        }*/
        h += H(variables, current_row);
        //��ÿһ�ֿ��ܵ���ϼ���H
        /*
        cout << endl;
        cout << "h: " << h << endl;
        */
        //debug
    }
    /*
    for (auto i : variables)
        cout << i << " ";
    cout << endl;
    */
    //debug
    k = K(leaf - 1, pa);
    //����K
    delete variables_combine_size;
    for (int i = 0; i < variables.size(); i++)
        delete[] variables_combine[i];
    delete[] variables_combine;
    delete[] current_row;
    return log(my_data.size()) * k / 2 - h;
    //���ݹ�ʽ���
}
float** type_data::reconstruct_combine(const vector<int>& variables, float** before, int* total, int dimension)
{
    bool* flag = new bool[*total];
    //���ÿһ���ǲ��������ݼ�������ʵ����
    int count = 0;
    //��������ݼ�������ʵ�����е���Ŀ
    memset(flag, false, sizeof(bool) * (*total));
    for (int i = 0; i < *total; i++)
    {
        bool is_same = true;
        //��ǵ�ǰ���Ƿ������ݼ����д���
        //������ǰ��
        for (int j = 0; j < my_data.size(); j++)
        {
            //�������ݼ�ÿһ��
            //is_same = true;
            for (int k = variables.size() - dimension, m = 0; k < variables.size(); k++, m++)
            {
                if (before[m][i] != my_data[j][variables[k]])
                    break;
                
                if (k == variables.size() - 1)
                    flag[i] = true;
            }
            if (flag[i])
            {
                count++;
                
                //cout << "count ++!!!" << endl;
                //cout << variables.size() << endl;
                //debug
                break;
            }
            //���¼���
        }
        //flag[i] = is_same;
        //������
        
    }
    //����Ƿ����
    float** answer = new float* [dimension];
    for (int i = 0; i < dimension; i++)
        answer[i] = new float[count];
    //�����ڴ�

    for (int i = 0, answer_val = 0; i < *total; i++)//answer_val��¼�б�
    {
        if (flag[i])
        {
            for (int j = 0; j < dimension; j++)
            {
                answer[j][answer_val] = before[j][i];
                //cout << answer[j][answer_val - 1];
                //debug
            }

            answer_val ++;
        }
        //cout << endl;
        //debug
    }
    //ת�����õ�����
    for (int i = 0; i < dimension; i++)
    {
        delete[] before[i];
    }
    delete[] before;
    delete[] flag;
    //�ͷŵ�֮ǰ��
    *total = count;
    return answer;
}
float** type_data::combine(const vector<int>& variables, int depth, int* total_dimension_size)//depth��1��ʼ
{
    int dimension = variables.size() - depth + 1;
    //��¼��ǰ��ά����depth��1��ʼ
    float** answer = new float* [dimension];
    //����һ��ռ�
    int current_type_val = depth - 1;
    //��ǰ�ݹ���Ӧ�������±�
    if (dimension == 1)
    {
        //�ݹ��
        int current_dimension_size = one_property_type[variables[current_type_val]].size();
        //��¼��ǰά�ȱ�����ʵ������
        answer[0] = new float[current_dimension_size];
        int i = 0;
        for (auto current_value = one_property_type[variables[current_type_val]].cbegin(); current_value != one_property_type[variables[current_type_val]].cend(); current_value++)
        {
            answer[0][i++] = *current_value;
        }
        //��ֵ����������
        *total_dimension_size = current_dimension_size;
        //�ش���ǰά�ȵ�ʵ����Ŀ
        return answer;
    }

    int* pre_dimension_size = new int;
    //֮ǰ����ϵ���Ŀ
    float** pre_dimension = combine(variables, depth + 1, pre_dimension_size);
    //֮ǰ������
    int current_dimension_size = one_property_type[variables[current_type_val]].size();
    //��ǰ���Ӧ������ʵ����Ŀ
    *total_dimension_size = current_dimension_size * (*pre_dimension_size);
    //�������ǰ�㽫Ҫ��ϳ���ȫ������
    for (int i = 0; i < dimension; i++)
    {
        answer[i] = new float[*total_dimension_size];
    }
    //����ռ�

    for (int i = 1; i < dimension; i++)
    {
        //����ÿһ��ά��
        for (int m = 0; m < current_dimension_size; m++)
        {
            //���ڵ�ǰ��ÿһ������������������һ��֮ǰ������
            //֮�󽫵�ǰ��ÿһ��������䵽��һ��
            for (int j = m * (*pre_dimension_size); j < *pre_dimension_size * (m + 1); j++)
            {
                answer[i][j] = pre_dimension[i - 1][j - m * (*pre_dimension_size)];
                //cout << answer[i][j] << " ";
                //debug
            }
            //cout << endl;
            //debug
        }
    }
    int i = 0;
    for (auto current_value = one_property_type[variables[current_type_val]].cbegin(); current_value != one_property_type[variables[current_type_val]].end(); current_value++)
    {
        for (int j = i; j < *pre_dimension_size + i; j ++)
        {
            answer[0][j] = *current_value;
        }
        i += (*pre_dimension_size);
    }
    answer = reconstruct_combine(variables, answer, total_dimension_size, dimension);
    for (int i = 0; i < dimension - 1; i++)
        delete[] pre_dimension[i];
    delete[] pre_dimension;
    delete pre_dimension_size;
    return answer;
}

/*
float** combine(const vector<int>& variables, int* total_dimension_size)
{
    vector<vector<set<float>::iterator>> control(2);
    //0,1�ֱ��ʾ��ǰ��ź�����������
    vector<vector<int>> ans;
    //ans��ʾ�м���������
    for (int i : variables)
    {
        set<float> & current_set = one_property_type[i];
        control[0].push_back(current_set.cbegin());
        control[1].push_back(current_set.cend());
    }
    while (control[0][0] != control[1][0])
    {
        while ()
    }
}
*/
float type_data::H(const vector<int>& variables, float* current_combine)
{
    int n_x_pa = 0, n_pa = 0;
    for (int i = 0; i < my_data.size(); i++)
    {
        bool is_n_pa = true;
        for (int j = 0; j < variables.size() - 1; j++)
        {
            if (my_data[i][variables[j]] != current_combine[j])
            {
                is_n_pa = false;
            }
        }
        if (is_n_pa)
        {
            //���pa�Ƚ�ȫ��ͨ������ô�ٿ�x_pa
            n_pa++;
            /*cout << my_data[i][variables.back()] << " " << current_combine[variables.size() - 1] << endl;
            cout << current_combine[0] << endl;*/
            //debug
            if (my_data[i][variables.back()] == current_combine[variables.size() - 1])
                n_x_pa++;
        }
    }
    if (variables.size() == 1)
        n_pa = my_data.size();
    //���PAΪ�ռ���ôn_pa = N
    if (n_x_pa == 0)
        return 0;
    //�����ڵ���Ŀֱ������
    return n_x_pa * (log(n_x_pa) - log(n_pa));
}
/*
byte type_data::K(const vector<int>& variables, byte leaf, byte total_r)//���ε�ʱ��leaf�Ѿ���ȥ1
{
    total_r /= one_property_type[leaf].size();
    //if (variables.size() == 1)
    //    total_r = 1;
        
    return total_r * (one_property_type[leaf].size() - 1);
}
*/
byte type_data::K(byte leaf, const varset& pa)//���ε�ʱ��leaf�Ѿ���ȥ1
{
    vector<int> variables;
    for (int i = 1; i < pa.get_count(); i++)
    {
        if (pa.test(i) && i != leaf + 1)
            variables.push_back(i - 1);
    }
    //��Ҷ����ȫ�����浽vector
    float answer = one_property_type[leaf].size() - 1;
    for (int i = 0; i < variables.size(); i++)
    {
        answer *= one_property_type[variables[i]].size();
    }
    //����������������������
    return answer;
}



