#ifndef _HEURISTIC_FUNCTION_H
#define _HEURISTIC_FUNCTION_H
#include "varset.h"
#include "define.h"
#include "data.h"
#include "Node.h"
using namespace std;
float heuristic_function(varset& variables, varset& all_variables, type_data &cache, Node *u, byte leaf);
//当前节点变量，所有变量，数据对象，父节点，路径
#endif
