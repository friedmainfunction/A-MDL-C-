#ifndef _NODE_H
#define _NODE_H
#include "define.h"
#include "varset.h"
class Node
{
public:
    Node (){}
    Node (float g, float h, varset variables, int leaf):variables(variables.get_count())
    {
        this -> g = g;
        this -> h = h;
        this -> variables = variables;
        this -> leaf = leaf;
        this -> is_in_closedlist = false;
        //赋初值
    }

    Node(const Node &another_Node)
    {
        this -> g = another_Node.g;
        this -> h = another_Node.h;
        this -> variables = another_Node.variables;
        this -> leaf = leaf;
        this -> is_in_closedlist = another_Node.is_in_closedlist;
        //拷贝赋初值
    }

    Node& operator = (const Node& another_Node)
    {
        this -> g = another_Node.g;
        this -> h = another_Node.h;
        this -> variables = another_Node.variables;
        this -> leaf = leaf;
        this -> is_in_closedlist = another_Node.is_in_closedlist;

        return *this;
        //赋值函数
    }

    float get_g()
    {
        return g;
        //当前节点的代价
    }

    float get_f() const
    {
        return h + g;
        //启发式值+当前代价
    }

    float get_h() const
    {
        return h;
    }

    byte get_leaf()
    {
        return leaf;
        //从哪里来到这个节点的
    }

    varset get_variables()
    {
        return variables;
        //获取当前节点对应的变量集合
    }

    void set_g(float g)
    {
        this -> g = g;
        //修改代价
    }

    void set_h(float h)
    {
        this -> h = h;
        //修改启发式函数值
    }

    void set_leaf(int leaf)
    {
        this -> leaf = leaf;
        //修改来的路
    }

    struct CompStr {
        bool operator()(Node* Node1, Node* Node2)
        {
            return Node1->get_f() > Node2->get_f();
        }
    };
    //用于小根堆

    void set_in_closedlist()
    {
        is_in_closedlist = true;
        //closedlist标志
    }

    bool check_in_closedlist()
    {
        return is_in_closedlist;
        //是否在closedlist当中
    }

    bool operator < (const Node *b)
    {
        return this -> get_f() > b -> get_f();
        //定义指针的比较，反向，方便使用STL小根堆
    }

private:
    float h, g;
    varset variables;
    //当前节点的变量集合
    bool is_in_closedlist;
    //是否在closedlist
    byte leaf;
    //来的路径
};


#endif