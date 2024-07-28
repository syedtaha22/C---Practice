#include <iostream>

class A
{
public:
    char type;

    A()
    {
        type = 'A';
    }

    virtual A *clone()
    {
        return new A(*this);
    }
};

class B : public A
{
public:
    B()
    {
        type = 'B';
    }

    virtual B *clone() override
    {
        return new B(*this);
    }
};

class C : public A
{
public:
    C()
    {
        type = 'C';
    }

    virtual C *clone() override
    {
        return new C(*this);
    }
};

class Container
{
    A *arr[6];

public:
    Container() {}

    // Deep Copy constructer
    Container(const Container &other)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (other.arr[i])
            {
                arr[i] = other.arr[i]->clone(); // Use the clone method to create a copy of the object
            }
            else
            {
                arr[i] = nullptr;
            }
        }
    }

    void init()
    {
        arr[0] = new B();
        arr[1] = new C();
        arr[2] = new B();
        arr[3] = new C();
        arr[4] = new B();
        arr[5] = new B();
    }

    void print()
    {
        for (const auto &obj : arr)
        {
            std::cout << obj->type << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    Container container1;
    container1.init();

    Container copy(container1);

    container1.print();
    copy.print();
}
