#include <lib_1.hpp>
#include "lib_2.hpp"

template<typename T>
const int my_function(T* param_one, const std::string& param_two)
{
    for (std::size_t i = 0; i < 10U; ++i)
    {
        param_one[i] += 2.0F;
    }
    return 0;
}

namespace my_namespace
{

class MyClass : public OtherClass
{
public

    MyClass(
            const long** param_one,
            bool param_two,
            std::string param_three="default")
        :
        m_attr_one  (*param_one),
        m_attr_two  (param_two),
        m_attr_three(param_three)
    {
        // this is a multi line function call
        my_function<long>(
                m_attr_one,
                m_attr_three
        );
    }

private:

    long*       m_attr_one;
    bool        m_attr_two;
    std::string m_attr_three;
};

} // namespace m_namespace
