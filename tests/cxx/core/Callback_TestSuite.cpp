#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.callback)

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

class NoParametersFixture : public chaos::test::Fixture
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

CHAOS_TEST_UNIT_FIXTURE(no_params, NoParametersFixture)
{
    // create the handler
    sigma::core::CallbackHandler<> handler;

    // add global callback
    sigma::core::ScopedCallback global_callback(
            handler.get_interface().register_function(global_func_void)
    );
    CHAOS_CHECK_TRUE(global_callback.is_registered());
    // check state
    handler.trigger();
    CHAOS_CHECK_TRUE (fixture->global_called);
    CHAOS_CHECK_FALSE(fixture->static_called);
    CHAOS_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    // add the static callback
    sigma::core::ScopedCallback static_callback(
            handler.get_interface().register_function(fixture->static_func)
    );
    CHAOS_CHECK_TRUE(static_callback.is_registered());
    // check state
    handler.trigger();
    CHAOS_CHECK_TRUE (fixture->global_called);
    CHAOS_CHECK_TRUE (fixture->static_called);
    CHAOS_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    {
        // add member callback
        sigma::core::ScopedCallback member_callback(
                handler.get_interface().register_member_function<
                        NoParametersFixture,
                        &NoParametersFixture::member_func
                >(fixture)
        );
        CHAOS_CHECK_TRUE(member_callback.is_registered());
        // check state
        handler.trigger();
        CHAOS_CHECK_TRUE (fixture->global_called);
        CHAOS_CHECK_TRUE (fixture->static_called);
        CHAOS_CHECK_TRUE (fixture->member_called);
        fixture->reset();
    } // member goes out of scope

    // check state
    handler.trigger();
    CHAOS_CHECK_TRUE (fixture->global_called);
    CHAOS_CHECK_TRUE (fixture->static_called);
    CHAOS_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    // unregister global
    global_callback.unregister();
    CHAOS_CHECK_FALSE(global_callback.is_registered());

    // check state
    handler.trigger();
    CHAOS_CHECK_FALSE(fixture->global_called);
    CHAOS_CHECK_TRUE (fixture->static_called);
    CHAOS_CHECK_FALSE(fixture->member_called);
    fixture->reset();

    // test for double unregistering
    global_callback.unregister();
    static_callback.unregister();
    CHAOS_CHECK_FALSE(global_callback.is_registered());
    CHAOS_CHECK_FALSE(static_callback.is_registered());

    // check state
    handler.trigger();
    CHAOS_CHECK_FALSE(fixture->global_called);
    CHAOS_CHECK_FALSE(fixture->static_called);
    CHAOS_CHECK_FALSE(fixture->member_called);
}

//------------------------------------------------------------------------------
//                                 INT PARAMETER
//------------------------------------------------------------------------------

class IntParameterFixture : public chaos::test::Fixture
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

CHAOS_TEST_UNIT_FIXTURE(int_param, IntParameterFixture)
{
    // create the handler as a pointer
    sigma::core::CallbackHandler<int>* handler =
            new sigma::core::CallbackHandler<int>();

    // add the static callback
    sigma::core::ScopedCallback static_callback(
            handler->get_interface().register_function(fixture->static_func)
    );
    CHAOS_CHECK_TRUE(static_callback.is_registered());
    // check state
    handler->trigger(12);
    CHAOS_CHECK_FALSE(fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_int, 0);
    CHAOS_CHECK_TRUE (fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_int, 12);
    CHAOS_CHECK_FALSE(fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_int, 0);
    fixture->reset();

    {
        // add member callback
        sigma::core::ScopedCallback member_callback(
                handler->get_interface().register_member_function<
                        IntParameterFixture,
                        &IntParameterFixture::member_func
                >(fixture)
        );
        CHAOS_CHECK_TRUE(member_callback.is_registered());
        // check state
        handler->trigger(-874);
        CHAOS_CHECK_FALSE(fixture->global_called);
        CHAOS_CHECK_EQUAL(fixture->global_int, 0);
        CHAOS_CHECK_TRUE (fixture->static_called);
        CHAOS_CHECK_EQUAL(fixture->static_int, -874);
        CHAOS_CHECK_TRUE (fixture->member_called);
        CHAOS_CHECK_EQUAL(fixture->member_int, -874);
        fixture->reset();

        {
            // copy member callback
            sigma::core::ScopedCallback member_copy(member_callback);
            CHAOS_CHECK_TRUE(member_callback.is_registered());
            CHAOS_CHECK_TRUE(member_copy.is_registered());
            // check state
            handler->trigger(9);
            CHAOS_CHECK_FALSE(fixture->global_called);
            CHAOS_CHECK_EQUAL(fixture->global_int, 0);
            CHAOS_CHECK_TRUE (fixture->static_called);
            CHAOS_CHECK_EQUAL(fixture->static_int, 9);
            CHAOS_CHECK_TRUE (fixture->member_called);
            CHAOS_CHECK_EQUAL(fixture->member_int, 9);
            fixture->reset();
        } // copy goes out of scope but shouldn't unregister
        CHAOS_CHECK_TRUE(member_callback.is_registered());

        // check state
        handler->trigger(-1);
        CHAOS_CHECK_FALSE(fixture->global_called);
        CHAOS_CHECK_EQUAL(fixture->global_int, 0);
        CHAOS_CHECK_TRUE (fixture->static_called);
        CHAOS_CHECK_EQUAL(fixture->static_int, -1);
        CHAOS_CHECK_TRUE (fixture->member_called);
        CHAOS_CHECK_EQUAL(fixture->member_int, -1);
        fixture->reset();
    } // member goes out of scope

    // check state
    handler->trigger(32235);
    CHAOS_CHECK_FALSE(fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_int, 0);
    CHAOS_CHECK_TRUE (fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_int, 32235);
    CHAOS_CHECK_FALSE(fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_int, 0);
    fixture->reset();

    // add global callback
    sigma::core::ScopedCallback global_callback(
            handler->get_interface().register_function(global_func_int)
    );
    CHAOS_CHECK_TRUE(global_callback.is_registered());

    // check state
    handler->trigger(-80);
    CHAOS_CHECK_TRUE (fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_int, -80);
    CHAOS_CHECK_TRUE (fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_int, -80);
    CHAOS_CHECK_FALSE(fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_int, 0);
    fixture->reset();

    // delete the handler
    delete handler;
    handler = nullptr;

    // check callbacks are unregistered
    CHAOS_CHECK_FALSE(global_callback.is_registered());
    CHAOS_CHECK_FALSE(static_callback.is_registered());

    // check unregistering does not alter state
    global_callback.unregister();
    static_callback.unregister();
    CHAOS_CHECK_FALSE(global_callback.is_registered());
    CHAOS_CHECK_FALSE(static_callback.is_registered());
}

//------------------------------------------------------------------------------
//                           CHAR AND STRING PARAMETERS
//------------------------------------------------------------------------------

class CharStringParametersFixture : public chaos::test::Fixture
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

CHAOS_TEST_UNIT_FIXTURE(char_string_params, CharStringParametersFixture)
{
    // create the handler
    sigma::core::CallbackHandler<char, const std::string&> handler;

    // add member callback
    sigma::core::ScopedCallback member_callback(
            handler.get_interface().register_member_function<
                    CharStringParametersFixture,
                    &CharStringParametersFixture::member_func
            >(fixture)
    );
    CHAOS_CHECK_TRUE(member_callback.is_registered());

    // check state
    handler.trigger('a', "Hello");
    CHAOS_CHECK_FALSE(fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->global_string, "");
    CHAOS_CHECK_FALSE(fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->static_string, "");
    CHAOS_CHECK_TRUE (fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_char, 'a');
    CHAOS_CHECK_EQUAL(fixture->member_string, "Hello");
    fixture->reset();

    // add global callback
    sigma::core::ScopedCallback global_callback(
            handler.get_interface().register_function(global_func_char_str)
    );
    CHAOS_CHECK_TRUE(global_callback.is_registered());

    // check state
    handler.trigger('\t', "World");
    CHAOS_CHECK_TRUE (fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_char, '\t');
    CHAOS_CHECK_EQUAL(fixture->global_string, "World");
    CHAOS_CHECK_FALSE(fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->static_string, "");
    CHAOS_CHECK_TRUE (fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_char, '\t');
    CHAOS_CHECK_EQUAL(fixture->member_string, "World");
    fixture->reset();

    {
        // add static callback
        sigma::core::ScopedCallback static_callback(
                handler.get_interface().register_function(fixture->static_func)
        );
        CHAOS_CHECK_TRUE(static_callback.is_registered());

        // check state
        handler.trigger('6', "...");
        CHAOS_CHECK_TRUE (fixture->global_called);
        CHAOS_CHECK_EQUAL(fixture->global_char, '6');
        CHAOS_CHECK_EQUAL(fixture->global_string, "...");
        CHAOS_CHECK_TRUE (fixture->static_called);
        CHAOS_CHECK_EQUAL(fixture->static_char, '6');
        CHAOS_CHECK_EQUAL(fixture->static_string, "...");
        CHAOS_CHECK_TRUE (fixture->member_called);
        CHAOS_CHECK_EQUAL(fixture->member_char, '6');
        CHAOS_CHECK_EQUAL(fixture->member_string, "...");
        fixture->reset();

        // unregister member callback
        member_callback.unregister();
        CHAOS_CHECK_FALSE(member_callback.is_registered());

        // check state
        handler.trigger('J', " ");
        CHAOS_CHECK_TRUE (fixture->global_called);
        CHAOS_CHECK_EQUAL(fixture->global_char, 'J');
        CHAOS_CHECK_EQUAL(fixture->global_string, " ");
        CHAOS_CHECK_TRUE (fixture->static_called);
        CHAOS_CHECK_EQUAL(fixture->static_char, 'J');
        CHAOS_CHECK_EQUAL(fixture->static_string, " ");
        CHAOS_CHECK_FALSE(fixture->member_called);
        CHAOS_CHECK_EQUAL(fixture->member_char, '\0');
        CHAOS_CHECK_EQUAL(fixture->member_string, "");
        fixture->reset();
    } // static callback goes out of scope

    // check state
    handler.trigger('z', "___o__O__");
    CHAOS_CHECK_TRUE (fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_char, 'z');
    CHAOS_CHECK_EQUAL(fixture->global_string, "___o__O__");
    CHAOS_CHECK_FALSE(fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->static_string, "");
    CHAOS_CHECK_FALSE(fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->member_string, "");
    fixture->reset();

    // unregister global callback
    global_callback.unregister();
    CHAOS_CHECK_FALSE(global_callback.is_registered());

    // check state
    handler.trigger(' ', "nope");
    CHAOS_CHECK_FALSE(fixture->global_called);
    CHAOS_CHECK_EQUAL(fixture->global_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->global_string, "");
    CHAOS_CHECK_FALSE(fixture->static_called);
    CHAOS_CHECK_EQUAL(fixture->static_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->static_string, "");
    CHAOS_CHECK_FALSE(fixture->member_called);
    CHAOS_CHECK_EQUAL(fixture->member_char, '\0');
    CHAOS_CHECK_EQUAL(fixture->member_string, "");
    fixture->reset();
}

} // namespace core_callback_tests
