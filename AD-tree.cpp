#include "AD-tree.h"

AD_Node::AD_Node(type_data& cache) :cache(cache), my_data(cache.get_my_data())
{
	num = 0;
	//归0
}

void AD_Node::make_vary_Node(vector<int>& variables, vector<float>& value)
{

	int variables_count = cache.get_variable_count();
	//变量总计数
	int base_count = 0;
	if (variables.size())
		base_count = variables.back();
	//base_count是当前集合当中最新加入的变量
	//编号最大

	for (int i = base_count + 1; i <= variables_count; i++)
	{
		//从当前选定变量的下一个开始枚举，一直到最后一个变量
		children.push_back(NULL);
		//添加新孩子
		vary_Node* vn = new vary_Node(cache);

		variables.push_back(i);
		//加入新变量

		vn -> make_AD_Node(variables, value);
		//新产生的vary_Node递归地构造AD_Node
		if (vn -> get_sum())
			children.back() = vn;
		else
			delete vn;
		//为0的节点删掉，只保留有计数的节点

		variables.pop_back();
		//把之前加入的变量取出，继续迭代
	}

	//if (variables.size() == 0)
		//system("pause");

	for (int i = 0; i < children.size(); i ++)
	{
		if (!children[i])
			continue;

		num = children[i]-> get_sum();
		break;
		//当前的AD_Node随便等于某一个子vary_Node的计数
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
	//本实验中每个变量只有两种取值，因此vary_Node只有两种儿子

	upper_bound = (log(2 * my_data.size()) - log(log(my_data.size())));
	//变量个数的上界
}


int vary_Node::count_combine(vector<int>& variables, vector<float>& value)
{
	if (variables.size() == 0)
		return 0;
	//如果集合为空集，那么返回0

	int n = 0;

	for (int i = 0; i < my_data.size(); i++)
	{
		bool is_n = true;
		for (int j = 0; j < variables.size(); j++)
		{
			if (my_data[i][variables[j] - 1] != value[j + 1])
			{
				//在AD-tree里面变量从1开始编号
				//在数据缓存当中从0开始编号，所以要转换
				//value因为入口会计一个空集取值，第一个数字无用
				//所以要j + 1
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
	//把当前节点增加的变量存一下 
	//如果是第一个vary_Node，那么删掉一个儿子，因为没有两种取值

	if (variables.size() && ((variables.size() == upper_bound + 1 || variables.back() == cache.get_variable_count())))
	{
		//如果到了递归基
		//到了底层

		for (int i = 0; i < 2; i++)
		{
			//枚举变量取值0，1两种情况
			AD_Node *adn = new AD_Node(cache);
			value.push_back(i);
			//形成向量
			int num = count_combine(variables, value);
			//对向量计数

			value.pop_back();
			//归位

			if (!num)
			{
				for (int i = 0; i < adn -> children.size(); i++)
					delete adn -> children[i];
				delete adn;
				continue;
				//如果计数为0，删掉存NULL
			}

			adn -> set_num(num);
			children[i] = adn;
			//计数不为0，存下
		}

		for (auto child : children)
		{
			if (!child)
				continue;
			sum += child -> get_num();
		}
		//更新当前vary_Node的sum

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
	//变量的数目

	for (int i = 0; i < children.size(); i++)
	{
		//对当前变量枚举0，1两种值

		AD_Node* adn = new AD_Node(cache);
		value.push_back(i);
		//形成新的向量

		adn -> make_vary_Node(variables, value);
		//AD_Node递归地构造vary_Node

		value.pop_back();
		//复位

		if (!(adn -> get_num()))
		{
			for (int i = 0; i < adn -> children.size(); i++)
				delete adn -> children[i];
			delete adn;
			//如果计数为0，删除节点保留空指针
		}
		else
		{
			children[i] = adn;
			//不为零，保存节点
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		if (!children[i])
			continue;

		sum += children[i] -> get_num();
	}
	//计算当前vary_Node的sum值
	/*
	if (variables.size() == 0 || (variables.back() + 1 <= cache.get_variable_count()))
	{
		//除了最后一层的AD_Node，其它地方都要枚举
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
	//从vary_Node访问孩子AD_Node(i)再访问孩子vary_Node(j)再访问孩子AD_Node(k)
	//分别得到n_pa和n_x_pa
	//以及父集和leaf
	
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
		//如果下一个变量被去掉了，那么考虑两种情况：
		//1.下一个变量就是最后一个变量，那么直接返回当前vary_Node选定对应值的num
		//2.下一个变量不是最后一个变量，那么跳过它
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
	//如果下一个变量没有被去掉
	//先把当前变量的值选定，然后找下一个变量对应的vary_Node
}
