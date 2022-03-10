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
        //����ֵ
    }

    Node(const Node &another_Node)
    {
        this -> g = another_Node.g;
        this -> h = another_Node.h;
        this -> variables = another_Node.variables;
        this -> leaf = leaf;
        this -> is_in_closedlist = another_Node.is_in_closedlist;
        //��������ֵ
    }

    Node& operator = (const Node& another_Node)
    {
        this -> g = another_Node.g;
        this -> h = another_Node.h;
        this -> variables = another_Node.variables;
        this -> leaf = leaf;
        this -> is_in_closedlist = another_Node.is_in_closedlist;

        return *this;
        //��ֵ����
    }

    float get_g()
    {
        return g;
        //��ǰ�ڵ�Ĵ���
    }

    float get_f() const
    {
        return h + g;
        //����ʽֵ+��ǰ����
    }

    float get_h() const
    {
        return h;
    }

    byte get_leaf()
    {
        return leaf;
        //��������������ڵ��
    }

    varset get_variables()
    {
        return variables;
        //��ȡ��ǰ�ڵ��Ӧ�ı�������
    }

    void set_g(float g)
    {
        this -> g = g;
        //�޸Ĵ���
    }

    void set_h(float h)
    {
        this -> h = h;
        //�޸�����ʽ����ֵ
    }

    void set_leaf(int leaf)
    {
        this -> leaf = leaf;
        //�޸�����·
    }

    struct CompStr {
        bool operator()(Node* Node1, Node* Node2)
        {
            return Node1->get_f() > Node2->get_f();
        }
    };
    //����С����

    void set_in_closedlist()
    {
        is_in_closedlist = true;
        //closedlist��־
    }

    bool check_in_closedlist()
    {
        return is_in_closedlist;
        //�Ƿ���closedlist����
    }

    bool operator < (const Node *b)
    {
        return this -> get_f() > b -> get_f();
        //����ָ��ıȽϣ����򣬷���ʹ��STLС����
    }

private:
    float h, g;
    varset variables;
    //��ǰ�ڵ�ı�������
    bool is_in_closedlist;
    //�Ƿ���closedlist
    byte leaf;
    //����·��
};


#endif