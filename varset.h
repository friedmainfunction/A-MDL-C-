#ifndef _VARSET_H
#define _VARSET_H
#include "define.h"
#include <xstring>
#include <string>
#include <iostream>
using namespace std;
class varset
{
public:
    byte get_count() const //返回变量集合的总长度
    {
        return lenth;
    }
    varset() : current_count(0), lenth(0)
    {
        variables = new bool;
    }
    varset(byte variable_count)//由于变量编号从1开始，调用的时候自行将长度 + 1
    {
        lenth = variable_count;
        variables = new bool[lenth];
        current_count = 0;
        memset(variables, false, sizeof(bool) * lenth);
    }

    varset(const varset &another_varset) //拷贝构造
    {
        this -> variables = new bool[another_varset.get_count()];
        this -> lenth = another_varset.get_count();
        this -> current_count = another_varset.get_current_count();
        memcpy(this -> variables, another_varset.variables, sizeof(bool) * lenth);
    }

    byte get_current_count() const //获取当前集合当中变量个数
    {
        return current_count;
    }

    varset &operator = (const varset &another_varset) //赋值函数
    {
        delete this -> variables;
        this -> variables = new bool[another_varset.get_count()];
        this -> lenth = another_varset.get_count();
        this -> current_count = another_varset.get_current_count();
        memcpy(this -> variables, another_varset.variables, sizeof(bool) * lenth);
        return *this;
    }

    void set(byte variable)
    {
        if (variable + 1 > this -> get_count())
            return;

        if (!variables[variable])
            current_count ++;

        variables[variable] = true;
    }

    void set_first_n(byte n)
    {
        for (int i = 1; i <= n; i++)
            this -> set(i);
    }

    void remove(byte variable)
    {
        if (variables[variable])
            current_count --;

        variables[variable] = false;
    }

    bool test(byte variable) const
    {

        return variables[variable];
    }

    varset operator | (const varset another_varset)
    {
        varset answer(this -> lenth);

        for (int i = 0; i < this->lenth; i++)
        {
            if (this -> variables[i] || another_varset.variables[i])
            {
                answer.set(i);
            }


        }

        return answer;
    }

    bool operator == (const varset& another_varset) const
    {
        
        byte min_lenth = min(this -> lenth, another_varset.lenth);
        /*
        for (int i = 0; i < min_lenth; i++)
        {
            cout << int(this->variables[i]) << " " << int(another_varset.variables[i]);
            cout << endl;
        }
        */
        //debug
        for (int i = 0; i < min_lenth; i ++)
        {
            if (this -> test(i) != another_varset.test(i))
                return false;
        }

        if (min_lenth || (this -> lenth == 0 && another_varset.lenth == 0))
            return true;
    }
private:
    byte lenth;
    bool *variables;
    byte current_count;
};

struct myOwnHash
{
    size_t operator() (const varset& s) const noexcept
    {
        hash<string> myhash;
        string str;
        for (int i = 0; i < s.get_count(); i++)
        {
            if (s.test(i))
                str += to_string(i);
        }
        return myhash(str);
    }
    //自定类必须自定哈希函数
};
#endif