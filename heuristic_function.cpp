#include "heuristic_function.h"
float heuristic_function(varset &variables, varset &all_variables, type_data &cache, Node *u, byte leaf)
{
	if (u -> get_h())
		return u -> get_h() - cache.best_MDL(leaf, all_variables);
	//��������Ѿ��������ֱ�Ӽ�ȥ
	float answer = 0;
	varset pa(all_variables);

	for (byte i = 0; i < all_variables.get_count(); i ++)
	{
		if (variables.test(i))
			pa.remove(i);
	}
	//�õ���ǰ���Ϻ�ȫ���Ĳ

	//cout << pa.get_current_count();
	//debug

	for (byte i = 1; i < all_variables.get_count(); i++)
	{
		//ö������Ԫ�أ����Ԫ���ڲ���У��ͼ���
		if (pa.test(i))
		{
			all_variables.remove(i);
			answer += cache.best_MDL(i, all_variables);
			all_variables.set(i);
			//��ȫ������ȥ����ǰԪ�أ��ٷŻ�
		}
	}
	return answer;
}