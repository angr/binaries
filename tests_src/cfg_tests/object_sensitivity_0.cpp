#include <iostream>
#include <string>

#include <stdlib.h>
#include <stdio.h>

// Definitions of some classes
class Real
{
    public:
        Real();
        Real operator + (const Real& b);
        std::string _pp();
        std::string pp();
};

class Integer : public Real
{
    public:
        Integer();
        Integer(const std::string& s);
        Integer operator + (const Integer& b);
        std::string pp();

    private:
        int val;
};

class Float : public Real
{
    public:
        Float();
        Float(const std::string& s);
        Float operator + (const Float& b);
        std::string pp();

    private:
        float val;
};

// Implementations

Real::Real()
{

}

Real Real::operator + (const Real& b)
{
    return Real();
}

std::string Real::_pp()
{
    return std::string("The sum is ");
}

std::string Real::pp()
{
    return std::string();
}

Integer::Integer()
{
    this->val = 0;
}

Integer::Integer(const std::string& s)
{
    this->val = atoi(s.c_str());
}

Integer Integer::operator + (const Integer& b)
{
    Integer sum;
    sum.val = this->val + b.val;

    return sum;
}

std::string Integer::pp()
{
    char buf[4];
    std::string s;

    sprintf(buf, "%d", this->val);
    s = this->_pp() + std::string(buf);

    return s;
}

Float::Float()
{
    this->val = 0.0f;
}

Float::Float(const std::string& s)
{
    this->val = (float)atof(s.c_str());
}

Float Float::operator + (const Float& b)
{
    Float sum;

    sum.val = this->val + b.val;

    return sum;
}

std::string Float::pp()
{
    char s[10];
    sprintf(s, "%f", this->val);
    return this->_pp() + std::string(s);
}

int main(int argc, char** argv)
{
    std::string choice;

    std::cout << "integer or float?" << std::endl;
    std::cin >> choice;
    if (choice == "integer")
    {
        Integer a, b;
        std::string s_0, s_1;
        std::cin >> s_0 >> s_1;
        a = Integer(s_0);
        b = Integer(s_1);
        std::cout << (a + b).pp() << std::endl;
    }
    else
    {
        Float a, b;
        std::string s_0, s_1;
        std::cin >> s_0 >> s_1;
        a = Float(s_0);
        b = Float(s_1);
        std::cout << (a + b).pp() << std::endl;
    }
}
