#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.callback)

#include "sigma/core/Sigma.hpp"
#include <sigma/core/Callback.hpp>

namespace core_callback_tests
{

class ProtoFixture : public chaos::test::Fixture
{
public:

    static void static_func(bool a, int b)
    {
        std::cout << "static_func called: " << a << " : " << b << std::endl;
    }

    void not_static(bool a, int b)
    {
        std::cout << "not_static called: " << a << " : " << b << std::endl;
    }

    virtual void setup()
    {
        sigma::core::ScopedCallback callback_1(
                m_test_callback.get_interface().register_function(static_func)
        );
        sigma::core::ScopedCallback callback_2(
                m_test_callback.get_interface().register_member_function
                        <ProtoFixture, &ProtoFixture::not_static>(this)
        );
        m_test_callback.emit(false, 12);
    }

private:

    // sigma::core::CallbackManager<bool, int>::Trigger m_test_callback_trigger;
    // sigma::core::CallbackInterface<bool, int> m_test_callback;
    sigma::core::CallbackHandler<bool, int> m_test_callback;
};


CHAOS_TEST_UNIT_FIXTURE(callback_proto, ProtoFixture)
{
    sigma::core::init();
}

} // namespace core_callback_tests
