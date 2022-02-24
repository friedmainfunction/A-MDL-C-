#include "heuristic_function.h"
float heuristic_function(varset &variables, varset &all_variables, type_data &cache, Node *u, byte leaf)
{
	if (u -> get_h())
		return u -> get_h() - cache.best_MDL(leaf, all_variables);
	//如果父亲已经算过，就直接减去
	float answer = 0;
	varset pa(all_variables);

	for (byte i = 0; i < all_variables.get_count(); i ++)
	{
		if (variables.test(i))
			pa.remove(i);
	}
	//得到当前集合和全集的差集

	//cout << pa.get_current_count();
	//debug

	for (byte i = 1; i < all_variables.get_count(); i++)
	{
		//枚举所有元素，如果元素在差集当中，就计算
		if (pa.test(i))
		{
			all_variables.remove(i);
			answer += cache.best_MDL(i, all_variables);
			all_variables.set(i);
			//从全集当中去掉当前元素，再放回
		}
	}
	return answer;
}