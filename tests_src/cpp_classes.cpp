#include <iostream>

class B
{
public:
  virtual void bar();
  virtual void qux();
};

void B::bar()
{
  std::cout << "This is B's implementation of bar" << std::endl;
}
void B::qux()
{
  std::cout << "This is B's implementation of qux" << std::endl;
}

class A
{
public:
  virtual void lam()
  {
    std::cout<<"This is A's imp of lam"<<std::endl;
  }
  virtual void tool()
  {
    std::cout<<"This is A's imp of tool"<<std::endl;
  }
};


class C : public B, public A
{
public:
  void bar() override;
  void tool()
  {
	  std::cout<<"This is C's imp of tool"<<std::endl;
  }
  int k=0;

};

void C::bar()
{
  std::cout << "This is C's implementation of bar" << std::endl;
}

int main()
{
	B* b = new C();
	A* a = new C();
	C k;
	k.bar();
	b->bar();
	b->qux();
	a->tool();

	return 0;
}

