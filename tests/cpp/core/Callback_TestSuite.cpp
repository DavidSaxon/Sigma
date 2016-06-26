#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(core.Callback)

#include "sigma/core/Sigma.hpp"
#include <sigma/core/Callback.hpp>

namespace core_callback_tests
{

/*
 * Since the callback system is complex and the components are tightly coupled,
 * callback tests are a series of actions run on CallbackHandlers with different
 * function parameters.
 */

//------------------------------------------------------------------------------
//                                 NO PARAMETERS
//------------------------------------------------------------------------------

class NoParametersFixture : public arc::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    static bool global_called;
    static bool static_called;
    bool member_called;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        reset();
    }

    void reset()
    {
        global_called = false;
        static_called = false;
        member_called = false;
    }

    static void static_func()
    {
        static_called = true;
    }

    void member_func()
    {
        member_called = true;
    }
};
bool NoParametersFixture::global_called = false;
bool NoParametersFixture::static_called = false;

void global_func_void()
{
    NoParametersFixture::global_called = true;
}

ARC_TEST_UNIT_FIXTURE(no_params, NoParametersFixture)
{
    // create the handler
    sigma::core::CallbackHandler<> handler;

    // add global callback
    ARC_TEST_MESSAGE("Checking adding global callback");
    sigma::core::ScopedCallback global_callback(
            handler.get_interface().register_function(global_func_void)
    );
    ARC_CHECK_TRUE(global_callback.is_registered());
    ARC_TEST_MESSAGE("Checking state");
    handler.trigger();
    ARC_CHECK_TRUE (fixture->global_called);
    ARC_CHECK_FALSE(fixture->static_called);
    ARC_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    // add the static callback
    handler.get_interface().register_function(fixture->static_func);
    ARC_TEST_MESSAGE("Checking state");
    handler.trigger();
    ARC_CHECK_TRUE (fixture->global_called);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    {
        ARC_TEST_MESSAGE("Checking adding member callback");
        sigma::core::ScopedCallback member_callback(
                handler.get_interface().register_member_function<
                        NoParametersFixture,
                        &NoParametersFixture::member_func
                >(fixture)
        );
        ARC_CHECK_TRUE(member_callback.is_registered());
        ARC_TEST_MESSAGE("Checking state");
        handler.trigger();
        ARC_CHECK_TRUE (fixture->global_called);
        ARC_CHECK_TRUE (fixture->static_called);
        ARC_CHECK_TRUE (fixture->member_called);
        fixture->reset();
    } // member goes out of scope

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger();
    ARC_CHECK_TRUE (fixture->global_called);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    ARC_TEST_MESSAGE("Checking unregistering global callback");
    global_callback.unregister();
    ARC_CHECK_FALSE(global_callback.is_registered());

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger();
    ARC_CHECK_FALSE(fixture->global_called);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger();
    ARC_CHECK_FALSE(fixture->global_called);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_FALSE(fixture->member_called);
}

//------------------------------------------------------------------------------
//                                 INT PARAMETER
//------------------------------------------------------------------------------

class IntParameterFixture : public arc::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    static bool global_called;
    static int global_int;
    static bool static_called;
    static int static_int;
    bool member_called;
    int member_int;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        reset();
    }

    void reset()
    {
        global_called = false;
        global_int = 0;
        static_called = false;
        static_int = 0;
        member_called = false;
        member_int = 0;
    }

    static void static_func(int i)
    {
        static_called = true;
        static_int = i;
    }

    void member_func(int i)
    {
        member_called = true;
        member_int = i;
    }
};
bool IntParameterFixture::global_called = false;
int IntParameterFixture::global_int = 0;
bool IntParameterFixture::static_called = false;
int IntParameterFixture::static_int = 0;

void global_func_int(int i)
{
    IntParameterFixture::global_called = true;
    IntParameterFixture::global_int = i;
}

ARC_TEST_UNIT_FIXTURE(int_param, IntParameterFixture)
{
    // create the handler as a pointer
    sigma::core::CallbackHandler<int>* handler =
            new sigma::core::CallbackHandler<int>();

    ARC_TEST_MESSAGE("Checking adding static callback");
    sigma::core::ScopedCallback static_callback(
            handler->get_interface().register_function(fixture->static_func)
    );
    ARC_CHECK_TRUE(static_callback.is_registered());
    ARC_TEST_MESSAGE("Checking state");
    handler->trigger(12);
    ARC_CHECK_FALSE(fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_int, 0);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_int, 12);
    ARC_CHECK_FALSE(fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_int, 0);
    fixture->reset();

    {
        ARC_TEST_MESSAGE("Checking adding member callback");
        sigma::core::ScopedCallback member_callback(
                handler->get_interface().register_member_function<
                        IntParameterFixture,
                        &IntParameterFixture::member_func
                >(fixture)
        );
        ARC_CHECK_TRUE(member_callback.is_registered());
        ARC_TEST_MESSAGE("Checking state");
        handler->trigger(-874);
        ARC_CHECK_FALSE(fixture->global_called);
        ARC_CHECK_EQUAL(fixture->global_int, 0);
        ARC_CHECK_TRUE (fixture->static_called);
        ARC_CHECK_EQUAL(fixture->static_int, -874);
        ARC_CHECK_TRUE (fixture->member_called);
        ARC_CHECK_EQUAL(fixture->member_int, -874);
        fixture->reset();

        {
            // copy member callback
            ARC_TEST_MESSAGE("Checking copying callback");
            sigma::core::ScopedCallback member_copy(member_callback);
            ARC_CHECK_TRUE(member_callback.is_registered());
            ARC_CHECK_TRUE(member_copy.is_registered());
            ARC_TEST_MESSAGE("Checking state");
            handler->trigger(9);
            ARC_CHECK_FALSE(fixture->global_called);
            ARC_CHECK_EQUAL(fixture->global_int, 0);
            ARC_CHECK_TRUE (fixture->static_called);
            ARC_CHECK_EQUAL(fixture->static_int, 9);
            ARC_CHECK_TRUE (fixture->member_called);
            ARC_CHECK_EQUAL(fixture->member_int, 9);
            fixture->reset();
        } // copy goes out of scope but shouldn't unregister
        ARC_TEST_MESSAGE("Checking copy going out of scope");
        ARC_CHECK_TRUE(member_callback.is_registered());

        ARC_TEST_MESSAGE("Checking state");
        handler->trigger(-1);
        ARC_CHECK_FALSE(fixture->global_called);
        ARC_CHECK_EQUAL(fixture->global_int, 0);
        ARC_CHECK_TRUE (fixture->static_called);
        ARC_CHECK_EQUAL(fixture->static_int, -1);
        ARC_CHECK_TRUE (fixture->member_called);
        ARC_CHECK_EQUAL(fixture->member_int, -1);
        fixture->reset();
    } // member goes out of scope

    ARC_TEST_MESSAGE("Checking state");
    handler->trigger(32235);
    ARC_CHECK_FALSE(fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_int, 0);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_int, 32235);
    ARC_CHECK_FALSE(fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_int, 0);
    fixture->reset();

    ARC_TEST_MESSAGE("Checking adding global callback");
    sigma::core::ScopedCallback global_callback(
            handler->get_interface().register_function(global_func_int)
    );
    ARC_CHECK_TRUE(global_callback.is_registered());

    ARC_TEST_MESSAGE("Checking state");
    handler->trigger(-80);
    ARC_CHECK_TRUE (fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_int, -80);
    ARC_CHECK_TRUE (fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_int, -80);
    ARC_CHECK_FALSE(fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_int, 0);
    fixture->reset();

    ARC_TEST_MESSAGE("Checking deleting callback handler");
    delete handler;
    handler = nullptr;

    // check callbacks are unregistered
    ARC_CHECK_FALSE(global_callback.is_registered());
    ARC_CHECK_FALSE(static_callback.is_registered());

    ARC_TEST_MESSAGE("Checking unregistering after handle delete");
    global_callback.unregister();
    static_callback.unregister();
    ARC_CHECK_FALSE(global_callback.is_registered());
    ARC_CHECK_FALSE(static_callback.is_registered());
}

//------------------------------------------------------------------------------
//                           CHAR AND STRING PARAMETERS
//------------------------------------------------------------------------------

class CharStringParametersFixture : public arc::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    static bool global_called;
    static char global_char;
    static std::string global_string;
    static bool static_called;
    static char static_char;
    static std::string static_string;
    bool member_called;
    char member_char;
    std::string member_string;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        reset();
    }

    void reset()
    {
        global_called = false;
        global_char = '\0';
        global_string = "";
        static_called = false;
        static_char = '\0';
        static_string = "";
        member_called = false;
        member_char = '\0';
        member_string = "";
    }

    static void static_func(char c, const std::string& s)
    {
        static_called = true;
        static_char = c;
        static_string = s;
    }

    void member_func(char c, const std::string& s)
    {
        member_called = true;
        member_char = c;
        member_string = s;
    }
};
bool CharStringParametersFixture::global_called = false;
char CharStringParametersFixture::global_char = '\0';
std::string CharStringParametersFixture::global_string("");
bool CharStringParametersFixture::static_called = false;
char CharStringParametersFixture::static_char = '\0';
std::string CharStringParametersFixture::static_string("");

void global_func_char_str(char c, const std::string& s)
{
    CharStringParametersFixture::global_called = true;
    CharStringParametersFixture::global_char = c;
    CharStringParametersFixture::global_string = s;
}

ARC_TEST_UNIT_FIXTURE(char_string_params, CharStringParametersFixture)
{
    // create the handler
    sigma::core::CallbackHandler<char, const std::string&> handler;

    ARC_TEST_MESSAGE("Checking adding member callback");
    sigma::core::TransientCallbackID member_id =
            handler.get_interface().register_member_function<
                    CharStringParametersFixture,
                    &CharStringParametersFixture::member_func
            >(fixture);

    // create a scoped callback
    sigma::core::ScopedCallback member_callback(member_id);
    ARC_CHECK_TRUE(member_callback.is_registered());

    ARC_TEST_MESSAGE(
            "Checking that another ScopedCallback cannot be constructed from "
            "the same TransientCallbackID"
    );
    ARC_CHECK_THROW(
            sigma::core::ScopedCallback(member_id),
            arc::ex::IllegalActionError
    );

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger('a', "Hello");
    ARC_CHECK_FALSE(fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_char, '\0');
    ARC_CHECK_EQUAL(fixture->global_string, "");
    ARC_CHECK_FALSE(fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_char, '\0');
    ARC_CHECK_EQUAL(fixture->static_string, "");
    ARC_CHECK_TRUE (fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_char, 'a');
    ARC_CHECK_EQUAL(fixture->member_string, "Hello");
    fixture->reset();

    ARC_TEST_MESSAGE("Checking adding global callback");
    sigma::core::ScopedCallback global_callback(
            handler.get_interface().register_function(global_func_char_str)
    );
    ARC_CHECK_TRUE(global_callback.is_registered());

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger('\t', "World");
    ARC_CHECK_TRUE (fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_char, '\t');
    ARC_CHECK_EQUAL(fixture->global_string, "World");
    ARC_CHECK_FALSE(fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_char, '\0');
    ARC_CHECK_EQUAL(fixture->static_string, "");
    ARC_CHECK_TRUE (fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_char, '\t');
    ARC_CHECK_EQUAL(fixture->member_string, "World");
    fixture->reset();

    {
        ARC_TEST_MESSAGE("Checking adding static callback");
        sigma::core::ScopedCallback static_callback(
                handler.get_interface().register_function(fixture->static_func)
        );
        ARC_CHECK_TRUE(static_callback.is_registered());

        ARC_TEST_MESSAGE("Checking state");
        handler.trigger('6', "...");
        ARC_CHECK_TRUE (fixture->global_called);
        ARC_CHECK_EQUAL(fixture->global_char, '6');
        ARC_CHECK_EQUAL(fixture->global_string, "...");
        ARC_CHECK_TRUE (fixture->static_called);
        ARC_CHECK_EQUAL(fixture->static_char, '6');
        ARC_CHECK_EQUAL(fixture->static_string, "...");
        ARC_CHECK_TRUE (fixture->member_called);
        ARC_CHECK_EQUAL(fixture->member_char, '6');
        ARC_CHECK_EQUAL(fixture->member_string, "...");
        fixture->reset();

        ARC_TEST_MESSAGE("Checking unregistering member callback");
        member_callback.unregister();
        ARC_CHECK_FALSE(member_callback.is_registered());

        ARC_TEST_MESSAGE("Checking state");
        handler.trigger('J', " ");
        ARC_CHECK_TRUE (fixture->global_called);
        ARC_CHECK_EQUAL(fixture->global_char, 'J');
        ARC_CHECK_EQUAL(fixture->global_string, " ");
        ARC_CHECK_TRUE (fixture->static_called);
        ARC_CHECK_EQUAL(fixture->static_char, 'J');
        ARC_CHECK_EQUAL(fixture->static_string, " ");
        ARC_CHECK_FALSE(fixture->member_called);
        ARC_CHECK_EQUAL(fixture->member_char, '\0');
        ARC_CHECK_EQUAL(fixture->member_string, "");
        fixture->reset();
    } // static callback goes out of scope

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger('z', "___o__O__");
    ARC_CHECK_TRUE (fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_char, 'z');
    ARC_CHECK_EQUAL(fixture->global_string, "___o__O__");
    ARC_CHECK_FALSE(fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_char, '\0');
    ARC_CHECK_EQUAL(fixture->static_string, "");
    ARC_CHECK_FALSE(fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_char, '\0');
    ARC_CHECK_EQUAL(fixture->member_string, "");
    fixture->reset();

    ARC_TEST_MESSAGE("Checking unregistering global callback");
    global_callback.unregister();
    ARC_CHECK_FALSE(global_callback.is_registered());

    ARC_TEST_MESSAGE("Checking state");
    handler.trigger(' ', "nope");
    ARC_CHECK_FALSE(fixture->global_called);
    ARC_CHECK_EQUAL(fixture->global_char, '\0');
    ARC_CHECK_EQUAL(fixture->global_string, "");
    ARC_CHECK_FALSE(fixture->static_called);
    ARC_CHECK_EQUAL(fixture->static_char, '\0');
    ARC_CHECK_EQUAL(fixture->static_string, "");
    ARC_CHECK_FALSE(fixture->member_called);
    ARC_CHECK_EQUAL(fixture->member_char, '\0');
    ARC_CHECK_EQUAL(fixture->member_string, "");
    fixture->reset();
}

//------------------------------------------------------------------------------
//                                 NULL CALLBACK
//------------------------------------------------------------------------------

class NullCallbackFixture : public arc::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    bool callback_called;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        callback_called = false;
    }

    void on_callback(float f)
    {
        callback_called = true;
    }
};

ARC_TEST_UNIT_FIXTURE(null_callback, NullCallbackFixture)
{
    // create the handler
    sigma::core::CallbackHandler<float> handler;

    // create the null callback
    ARC_TEST_MESSAGE("Checking default constructor callback is null");
    sigma::core::ScopedCallback callback;
    ARC_CHECK_TRUE(callback.is_null());

    ARC_TEST_MESSAGE("Checking that null callback cannot be copied");
    ARC_CHECK_THROW(
            sigma::core::ScopedCallback(callback),
            arc::ex::IllegalActionError
    );

    // assign to the callback
    ARC_TEST_MESSAGE("Checking callback is no longer null after assignment");
    callback = handler.get_interface().register_member_function<
            NullCallbackFixture,
            &NullCallbackFixture::on_callback>(fixture);
    ARC_CHECK_FALSE(callback.is_null());

    ARC_TEST_MESSAGE("Checking non-null callback can now be copied");
    sigma::core::ScopedCallback callback_copy(callback);
    ARC_CHECK_EQUAL(callback.get_id(), callback_copy.get_id());

    ARC_TEST_MESSAGE("Checking non-null callback cannot be assigned");
    ARC_CHECK_THROW(
            ( callback = handler.get_interface().register_member_function<
                    NullCallbackFixture,
                    &NullCallbackFixture::on_callback>(fixture) ),
            arc::ex::IllegalActionError
    );

    // check that two callbacks can't assign to the same id
    ARC_TEST_MESSAGE("Checking callbacks cannot assign to the same id");
    sigma::core::TransientCallbackID t_id =
            handler.get_interface().register_member_function<
                    NullCallbackFixture,
                    &NullCallbackFixture::on_callback>(fixture);
    sigma::core::ScopedCallback callback_1;
    sigma::core::ScopedCallback callback_2;
    callback_1 = t_id;
    ARC_CHECK_THROW(
            (callback_2 = t_id),
            arc::ex::IllegalActionError
    );

    ARC_TEST_MESSAGE("Checking unregister callback nullifies it");
    callback.unregister();
    ARC_CHECK_TRUE(callback.is_null());

    ARC_TEST_MESSAGE("Checking that null callback cannot be unregistered");
    ARC_CHECK_THROW(
            callback.unregister(),
            arc::ex::IllegalActionError
    );
}

} // namespace core_callback_tests
