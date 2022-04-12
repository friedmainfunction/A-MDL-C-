#include "AD-tree.h"

AD_Node::AD_Node(type_data& cache) :cache(cache), my_data(cache.get_my_data())
{
	num = 0;
	//��0
}

void AD_Node::make_vary_Node(vector<int>& variables, vector<float>& value)
{

	int variables_count = cache.get_variable_count();
	//�����ܼ���
	int base_count = 0;
	if (variables.size())
		base_count = variables.back();
	//base_count�ǵ�ǰ���ϵ������¼���ı���
	//������

	for (int i = base_count + 1; i <= variables_count; i++)
	{
		//�ӵ�ǰѡ����������һ����ʼö�٣�һֱ�����һ������
		children.push_back(NULL);
		//����º���
		vary_Node* vn = new vary_Node(cache);

		variables.push_back(i);
		//�����±���

		vn -> make_AD_Node(variables, value);
		//�²�����vary_Node�ݹ�ع���AD_Node
		if (vn -> get_sum())
			children.back() = vn;
		else
			delete vn;
		//Ϊ0�Ľڵ�ɾ����ֻ�����м����Ľڵ�

		variables.pop_back();
		//��֮ǰ����ı���ȡ������������
	}

	//if (variables.size() == 0)
		//system("pause");

	for (int i = 0; i < children.size(); i ++)
	{
		if (!children[i])
			continue;

		num = children[i]-> get_sum();
		break;
		//��ǰ��AD_Node������ĳһ����vary_Node�ļ���
	}
	/*
	if (num > 0)
	{
		cout << "mk vary node" << endl;
		for (int i = 0; i < variables.size(); i++)
			cout << variables[i] << " ";
		cout << endl;
		cout << "num: " << num << endl;
	}
	*/
	//debug
}


void AD_Node::set_num(int num)
{
	this -> num = num;
}

inline int AD_Node::get_num()
{
	return num;
}


vary_Node::vary_Node(type_data& cache) :cache(cache), my_data(cache.get_my_data())
{
	sum = 0;
	for (int i = 0; i < 2; i++)
	{
		children.push_back(NULL);
	}
	//��ʵ����ÿ������ֻ������ȡֵ�����vary_Nodeֻ�����ֶ���

	upper_bound = (log(2 * my_data.size()) - log(log(my_data.size())));
	//�����������Ͻ�
}


int vary_Node::count_combine(vector<int>& variables, vector<float>& value)
{
	if (variables.size() == 0)
		return 0;
	//�������Ϊ�ռ�����ô����0

	int n = 0;

	for (int i = 0; i < my_data.size(); i++)
	{
		bool is_n = true;
		for (int j = 0; j < variables.size(); j++)
		{
			if (my_data[i][variables[j] - 1] != value[j + 1])
			{
				//��AD-tree���������1��ʼ���
				//�����ݻ��浱�д�0��ʼ��ţ�����Ҫת��
				//value��Ϊ��ڻ��һ���ռ�ȡֵ����һ����������
				//����Ҫj + 1
				is_n = false;
				break;
			}
		}
		if (is_n)
		{
			n++;
		}
	}



	//if (n)
		//system("pause");
	//debug
	return n;
}

void vary_Node::make_AD_Node(vector<int>& variables, vector<float>& value)
{
	if (variables.size())
		this -> variable_now = variables.back();
	else
		children.pop_back();
	//�ѵ�ǰ�ڵ����ӵı�����һ�� 
	//����ǵ�һ��vary_Node����ôɾ��һ�����ӣ���Ϊû������ȡֵ

	if (variables.size() && ((variables.size() == upper_bound + 1 || variables.back() == cache.get_variable_count())))
	{
		//������˵ݹ��
		//���˵ײ�

		for (int i = 0; i < 2; i++)
		{
			//ö�ٱ���ȡֵ0��1�������
			AD_Node *adn = new AD_Node(cache);
			value.push_back(i);
			//�γ�����
			int num = count_combine(variables, value);
			//����������

			value.pop_back();
			//��λ

			if (!num)
			{
				for (int i = 0; i < adn -> children.size(); i++)
					delete adn -> children[i];
				delete adn;
				continue;
				//�������Ϊ0��ɾ����NULL
			}

			adn -> set_num(num);
			children[i] = adn;
			//������Ϊ0������
		}

		for (auto child : children)
		{
			if (!child)
				continue;
			sum += child -> get_num();
		}
		//���µ�ǰvary_Node��sum

		/*
		if (sum > 0)
		{
			cout << "mk AD node" << endl;
			for (int i = 0; i < variables.size(); i++)
				cout << variables[i] << " ";
			cout << endl;
			cout << "sum: " << sum << endl;
		}
		*/
		return;
		
	}

	int variables_count = cache.get_variable_count();
	//��������Ŀ

	for (int i = 0; i < children.size(); i++)
	{
		//�Ե�ǰ����ö��0��1����ֵ

		AD_Node* adn = new AD_Node(cache);
		value.push_back(i);
		//�γ��µ�����

		adn -> make_vary_Node(variables, value);
		//AD_Node�ݹ�ع���vary_Node

		value.pop_back();
		//��λ

		if (!(adn -> get_num()))
		{
			for (int i = 0; i < adn -> children.size(); i++)
				delete adn -> children[i];
			delete adn;
			//�������Ϊ0��ɾ���ڵ㱣����ָ��
		}
		else
		{
			children[i] = adn;
			//��Ϊ�㣬����ڵ�
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		if (!children[i])
			continue;

		sum += children[i] -> get_num();
	}
	//���㵱ǰvary_Node��sumֵ
	/*
	if (variables.size() == 0 || (variables.back() + 1 <= cache.get_variable_count()))
	{
		//�������һ���AD_Node�������ط���Ҫö��
		for (int i = 0; i < children.size(); i ++)
		{
			AD_Node *now_i = children[i];
			if (!now_i)
				continue;
			for (int j = 0; j < now_i -> children.size(); j ++)
			{
				vary_Node *now_j = now_i->children[j];
				for (int k = 0; k < now_j -> children.size(); k ++)
				{
					AD_Node *now_k = now_j->children[k];
					if (now_k)
					{
						//if (variables.size() == 0 && now_j->get_variable_now() == 1)
							//system("pause");
						//debug
						cache.MDL(now_j -> get_variable_now(), variables, now_k->get_num(), now_i->get_num());
						delete now_k;
					}
				}
				delete now_j;
			}
		}
	}
	*/
	//��vary_Node���ʺ���AD_Node(i)�ٷ��ʺ���vary_Node(j)�ٷ��ʺ���AD_Node(k)
	//�ֱ�õ�n_pa��n_x_pa
	//�Լ�������leaf
	
}

inline int vary_Node::get_sum()
{
	return sum;
}

inline int vary_Node::get_variable_now()
{
	return variable_now;
}

float vary_Node::query(int leaf_val, vector<int>& pa, float* value)
{
	int n_x_pa = 0, n_pa = 0;
	if (children[0] && children[0]->children[pa[0]])
		n_x_pa = children[0]->children[pa[0]]->query(pa, value, 0);
	else
		n_x_pa = 0;
	pa[leaf_val] = -1;

	if (pa.size() == 1)
		n_pa = cache.get_my_data().size();
	else
	{
		if (leaf_val == 0)
			n_pa = children[0] -> children[pa[1]] -> query(pa, value, 1);
		else
		{
			pa[leaf_val] = -1;
			n_pa = children[0] -> children[pa[0]] -> query(pa, value, 0);
		}
	}
	if (n_x_pa == 0)
		return 0;
	else
		return n_x_pa * (log(n_x_pa) - log(n_pa));
}

int vary_Node::query(vector<int>& pa, float* value, int now)
{
	if (now == pa.size() - 1)
	{
		if (children[value[now]])
			return children[value[now]] -> get_num();
		else
			return 0;
	}
	
	if (pa[now + 1] == -1)
	{ 
		//�����һ��������ȥ���ˣ���ô�������������
		//1.��һ�������������һ����������ôֱ�ӷ��ص�ǰvary_Nodeѡ����Ӧֵ��num
		//2.��һ�������������һ����������ô������
		if (now + 2 == pa.size())
		{
			if (children[value[now]])
				return children[value[now]] -> get_num();
			else
				return 0;
		}
		else
		{
			if (children[value[now]] && children[value[now]] -> children[pa[now + 2] - pa[now] - 1])
				return children[value[now]] -> children[pa[now + 2] - pa[now] - 1] -> query(pa, value, now + 2);
			else
				return 0;
		}
	}
	else
	{
		if (children[value[now]] && children[value[now]] -> children[pa[now + 1] - pa[now] - 1])
			return children[value[now]] -> children[pa[now + 1] - pa[now] - 1] -> query(pa, value, now + 1);
		else
			return 0;
	}
	//�����һ������û�б�ȥ��
	//�Ȱѵ�ǰ������ֵѡ����Ȼ������һ��������Ӧ��vary_Node
}
