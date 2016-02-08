#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.callback.callback_manager)

#include "sigma/core/Sigma.hpp"
#include <sigma/core/callback/CallbackManager.hpp>
#include <sigma/core/callback/CallbackMemberFunction.hpp>

namespace core_callback_manager_tests
{

class ProtoFixture : public chaos::test::Fixture
{
public:

    ProtoFixture()
        :
        Fixture(),
        m_test_callback(m_test_callback_trigger)
    {
    }

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
        m_test_callback.register_function(static_func);
        m_test_callback.register_member_function
                <ProtoFixture, &ProtoFixture::not_static>(this);
        m_test_callback_trigger.fire(false, 12);
    }

private:

    sigma::core::CallbackManager<bool, int>::Trigger m_test_callback_trigger;
    sigma::core::CallbackManager<bool, int> m_test_callback;
};


CHAOS_TEST_UNIT_FIXTURE(proto, ProtoFixture)
{
    sigma::core::init();
}

} // namespace core_callback_manager_tests
