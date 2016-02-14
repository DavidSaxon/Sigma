/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_CALLBACK_HPP_
#define SIGMA_CORE_CALLBACK_HPP_

#include <cassert>
#include <map>
#include <memory>

#include <chaoscore/base/BaseExceptions.hpp>

// TODO:
#include <iostream>

namespace sigma
{
namespace core
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

struct CallbackReferenceCounter;
class ScopedCallback;

template<typename... function_parameters>
class CallbackHandler;

//------------------------------------------------------------------------------
//                                    CLASSES
//------------------------------------------------------------------------------

// hide from doxygen
#ifndef IN_DOXYGEN

/*!
 * \brief Abstract base class that CallbackInterface derives from.
 *
 * This is used so that ScopedCallbacks can hold a pointer to CallbackInterfaces
 * without needing to know their template type.
 */
class CallbackInterfaceBase
{
public:

    virtual ~CallbackInterfaceBase() {}

protected:

    friend class ScopedCallback;

    virtual bool has_reference_counter(chaos::uint32 id) const = 0;

    virtual void add_reference_counter(
            chaos::uint32 id,
            CallbackReferenceCounter* ref_counter) = 0;

    virtual void unregister_function(chaos::uint32 id) = 0;
};

/*!
 * \brief Stores data about a callback id and the interface that holds it.
 *
 * This object is used for returning callback id data after registering a
 * function through a CallbackInterface. It has been hidden as the caller should
 * never have to handle these objects, when returned they should be wrapped in
 * a ScopedCallback which will handle callback lifecycle.
 */
struct TransientCallbackID
{
    /*!
     * \brief The CallbackInterface that holds this callback.
     */
    CallbackInterfaceBase* interface;
    /*!
     * \brief The identifier of this callback.
     */
    chaos::uint32 id;
    /*!
     * \brief Constructor
     */
    TransientCallbackID(
            CallbackInterfaceBase* p_interface,
            chaos::uint32 p_id)
        :
        interface(p_interface),
        id       (p_id)
    {
    }
};

/*!
 *\brief Simple structure used by ScopedCallbacks to counter reference so that
 *       the last deference associated with a callback unregisters the callback.
 */
struct CallbackReferenceCounter
{
    /*!
     * \brief Signifies if the underlying callback has been deleted before the
     *        reference counter has reached zero.
     */
    bool early_unregister;
    /*!
     * \brief Counts the number of references to this callback.
     */
    chaos::uint32 count;
    /*!
     * \brief Default constructor.
     */
    CallbackReferenceCounter() : early_unregister(false), count(1) {}

};

#endif
// IN_DOXYGEN

/*!
 * \brief Object that holds the id of a callback function and aids in life
 *        cycle management of the callback.
 *
 * ScopedCallback's use reference counting to manage the life cycle of the
 * callback. Once all references to this callback go out of scope or are
 * destroyed the Callback will be unregistered.
 *
 * The ScopedCallback can be explicitly unregistered using the ``unregister``
 * function. Once explicitly unregistered other reference to this are safe to
 * go out of scope and have the ``unregister`` function called on them
 * (although this will only nullify the objects).
 */
class ScopedCallback
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default constructor.
     *
     * Constructs a new null ScopedCallback. Note an null ScopedCallback
     * cannot be copied. To assign an null ScopedCallback use the assignment
     * operator (=) with a TransientCallbackID as the right-hand operator.
     */
    ScopedCallback()
        :
        m_ref_counter(nullptr),
        m_interface  (nullptr),
        m_id         (0)
    {
    }

    /*!
     * \brief Constructs a new ScopedCallback from the given
     *        TransientCallbackID.
     *
     * \throws chaos::ex::IllegalActionError If a ScopedCallback has already
     *                                       been initialised for this callback
     *                                       id.
     */
    ScopedCallback(TransientCallbackID transient)
        :
        m_ref_counter(new CallbackReferenceCounter()),
        m_interface  (transient.interface),
        m_id         (transient.id)
    {
        init();
    }

    /*!
     * \brief Copy constructor.
     *
     * Makes a copy of the given ScopedCallback and increases the reference
     * counter.
     *
     * \throws chaos::ex::IllegalActionError If the other ScopedCallback to copy
     *                                       from is a null ScopedCallback.
     */
    ScopedCallback(const ScopedCallback& other)
        :
        m_ref_counter(other.m_ref_counter),
        m_interface  (other.m_interface),
        m_id         (other.m_id)
    {
        // are we trying to copy from a null callback?
        if (other.is_null())
        {
            throw chaos::ex::IllegalActionError(
                    "Cannot use ScopedCallback copy constructor to copy from a "
                    "null ScopedCallback."
            );
        }

        // increment the reference counter
        ++m_ref_counter->count;
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    // delete the default assignment operator
    ScopedCallback& operator=(const ScopedCallback& other) = delete;

    /*!
     * \brief TransientCallbackID assignment operator.
     *
     * Assigns the data from the given TransientCallbackID to this
     * ScopedCallback.
     *
     * \note This can only be performed on null ScopedCallbacks to "properly"
     *       initialise them.
     *
     * \throws chaos::ex::IllegalActionError If a ScopedCallback has already
     *                                       been initialised for this callback
     *                                       id. Or if this is not a null
     *                                       ScopedCallback.
     */
    ScopedCallback& operator=(TransientCallbackID transient)
    {
        // can only be performed on null ScopedCallbacks
        if (!is_null())
        {
            throw chaos::ex::IllegalActionError(
                    "Cannot assign to a non-null ScopedCallback.");
        }

        m_ref_counter = new CallbackReferenceCounter();
        m_interface = transient.interface;
        m_id = transient.id;

        init();
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~ScopedCallback()
    {
        // do nothing if this is null
        if (is_null())
        {
            return;
        }

        // reference counter should be non-zero
        assert(m_ref_counter->count > 0);

        // decrement the reference counter and check if this is the last
        // reference
        if (--m_ref_counter->count == 0)
        {
            // does the callback need unregistering?
            if (is_registered())
            {
                m_interface->unregister_function(m_id);
            }
            // clean up the reference counter
            delete m_ref_counter;
        }
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the id of this callback.
     */
    chaos::uint32 get_id() const
    {
        return m_id;
    }

    /*!
     * \brief Returns whether this is a null ScopedCallback.
     *
     * Null ScopedCallbacks cannot be copied and must be assigned using the
     * assignment operator with a TransientCallbackID as the right-hand value.
     */
    bool is_null() const
    {
        if (m_ref_counter == nullptr)
        {
            assert(m_interface == nullptr);
            return true;
        }
        assert(m_interface != nullptr);
        return false;
    }

    /*!
     * \brief Returns whether this contains a registered callback or not.
     *
     * A callback may return as unregistered because:
     * - ``unregister`` has been explicitly called.
     * - The ``CallbackHandler``/``CallbackInterface`` this was registered with
     *   has been destroyed.
     */
    bool is_registered() const
    {
        if (is_null())
        {
            return false;
        }
        return !m_ref_counter->early_unregister;
    }

    /*!
     * \brief Explicitly unregisters this callback.
     *
     * Once the callback has been unregistered the function this is associated
     * with will no longer be called when the CallbackHandler is triggered.
     * Once unregistering this will become a null ScopedCallback.
     *
     * \throws chaos::ex::IllegalActionError If this is called on a null
     *                                       ScopedCallbackId.
     */
    void unregister()
    {
        if (is_null())
        {
            throw chaos::ex::IllegalActionError(
                    "unregister cannot be called on null ScopedCallbacks");
        }

        // there should be at least one instance!!
        assert(m_ref_counter->count > 0);
        // unregister if it hasn't been done already
        if (is_registered())
        {
            m_interface->unregister_function(m_id);
            m_ref_counter->early_unregister = true;
        }
        // nullify this object
        m_ref_counter = nullptr;
        m_interface   = nullptr;
        m_id          = 0;
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief simple structure used for counting references.
     */
    CallbackReferenceCounter* m_ref_counter;
    /*!
     * \brief The CallbackInterface that owns this callback.
     */
    CallbackInterfaceBase* m_interface;
    /*!
     * \brief The global id of the callback this is handling.
     */
    chaos::uint32 m_id;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises this ScopedCallback id.
     *
     * \throws chaos::ex::IllegalActionError If a ScopedCallback has already
     *                                       been initialised for this callback
     *                                       id.
     */
    void init()
    {
        // is there already a ScopedCallback for this callback id?
        if (m_interface->has_reference_counter(m_id))
        {
            // nullify the callback
            m_ref_counter = nullptr;
            m_interface   = nullptr;
            m_id          = 0;
            throw chaos::ex::IllegalActionError(
                    "Cannot instantiate multiple ScopedCallbacks for the "
                    "same TransientCallbackID object."
            );
        }

        // pass the reference counter back to the interface
        m_interface->add_reference_counter(m_id, m_ref_counter);
    }
};

/*!
 * \brief Used to register global, static, and member functions as callbacks.
 *
 * This object will call all registered callback functions when triggered. The
 * callback routine may only be triggered by the owner of this object, see:
 * sigma::core::CallbackHandler.
 *
 * Registered functions can have their life cycle manged through a
 * ScopedCallback. The ``register_function`` and ``register_member_function``
 * methods return an object of type TransientCallbackID which can be wrapped
 * and handled by a ScopedCallback. The TransientCallbackID object should not
 * be interacted with directly or stored.
 *
 * ScopedCallbacks will unregister the wrapped callback when all references go
 * out of scope, or can be explicitly used to unregister the callback.

 * An example of registering both a global and a member function callback and
 * storing them with ScopedCallbacks:
 *
 * \code
 * // g_callback_interface is an object of type CallbackInterface<bool, int>
 *
 * void global_callback_func(bool a, int b)
 * {
 *     std::cout << "global called: " << a << ":" << b << std::endl;
 * }
 *
 * class MyClass
 * {
 * public:
 *
 *     void member_callback_function(bool a, int b)
 *     {
 *         std::cout << "member called: " << a << ":" << b << std::endl;
 *     }
 * }
 *
 * int main(int argc, char* argv[])
 * {
 *     MyClass my_class;
 *
 *     sigma::core::ScopedCallback callback_1(
 *             g_callback_interface.register_function(global_callback_func)
 *     );
 *     sigma::core::ScopedCallback callback_1(
 *             g_callback_interface.register_member_function<
 *                     <MyClass, &MyClass::member_callback_function>(&my_class)
 *     );
 *
 *     return 0;
 * }
 * \endcode
 *
 * Notice that the global function may be passed in directly to the
 * ``register_function method``, but the member function must be passed in using
 * templates. This is because member function pointers are treated differently
 * to function pointers in C++.
 * The ``register_member_function`` needs to know the member functions class
 * type (template parameter 1), the member function to call (template parameter
 * 2), and the object instance to call the member function on (function
 * parameter 1).
 *
 * \tparam function_parameters The types of the parameters of the function
 *                             this CallbackInterface is managing.
 *
 */
template<typename... function_parameters>
class CallbackInterface : public CallbackInterfaceBase
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN( CallbackInterface );

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class ScopedCallback;
    friend class CallbackHandler<function_parameters...>;

    //--------------------------------------------------------------------------
    //                             PRIVATE STRUCTURES
    //--------------------------------------------------------------------------

    /*!
     * \brief Union that holds either a global/static function or a member
     *        function pointer.
     */
    union PermissiveFunction
    {
        /*!
         *\brief Represents a standard global or static function which is not
         *       bound to an object.
         */
        void (*standard)(function_parameters...);
        /*!
         * \brief Represents a member function which is bound to it's owner
         *        object.
         */
        void (*member)(void*, function_parameters...);
    };

    /*!
     * \brief Data structure that holds a function and the function owner. If
     *        the function is a global or static function the owner should be
     *        null.
     */
    struct CallbackData
    {
        /*!
         * \brief The owner of this callback function. Will be null if this
         *        callback is not a member function.
         */
        void* owner;
        /*!
         * \brief The callback function.
         */
        PermissiveFunction function;
    };

public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~CallbackInterface()
    {
        // ensure that any remaining callbacks are alerted that this object
        // has been removed
        CHAOS_FOR_EACH(it, m_scope_refs)
        {
            it->second->early_unregister = true;
        }
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Registers a global or static function as a callback.
     *
     * Registered functions will be called when callback owner triggers the
     * CallbackHandler.
     *
     * \param callback_function The function to register as callback.
     *
     * \return A TransientCallbackID object which can be wrapped with a
     *         ScopedCallback but should not be interacted with directly or
     *         stored.
     */
    TransientCallbackID register_function(
            void (*callback_function)(function_parameters...))
    {
        // increment the global callback id and use it as this callback id
        chaos::uint32 id = m_next_id++;
        // this callback shouldn't be registered
        assert(m_callback_funcs.find(id) == m_callback_funcs.end());
        assert(m_scope_refs.find(id) == m_scope_refs.end());

        // store this function mapped under the id
        CallbackData f;
        f.owner = NULL;
        f.function.standard = callback_function;
        m_callback_funcs[id] = f;

        // return transient information about the callback
        return TransientCallbackID(this, id);
    }

    /*!
     * \brief Registers a member function as a callback.
     *
     * Registered functions will be called when callback owner triggers the
     * CallbackHandler.
     *
     * See sigma::core::CallbackInterface for a usage example.
     *
     * \tparam owner_type The class type that the member function is part of.
     * \tparam function_type The member function type to register as a callback.
     * \param owner Instance of the object that owns the member function to be
     *              used as the callback.
     *
     * \return A TransientCallbackID object which can be wrapped with a
     *         ScopedCallback but should not be interacted with directly or
     *         stored.
     */
    template<typename owner_type,
             void (owner_type::*function_type)(function_parameters...)>
    TransientCallbackID register_member_function(owner_type* owner)
    {
        // increment the global callback id and use it as this callback id
        chaos::uint32 id = m_next_id++;
        // this callback shouldn't be registered
        assert(m_callback_funcs.find(id) == m_callback_funcs.end());
        assert(m_scope_refs.find(id) == m_scope_refs.end());

        // store this function mapped under the id
        CallbackData f;
        f.owner = owner;
        f.function.member = member_wrapper<owner_type, function_type>;
        m_callback_funcs[id] = f;

        // return transient information about the callback
        return TransientCallbackID(this, id);
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Used to get an unused callback id and increment to the next
     *        callback id.
     */
    chaos::uint32 m_next_id;
    /*!
     * \brief Mapping which stores callback function ids to callback functions
     */
    std::map<chaos::uint32, CallbackData> m_callback_funcs;
    /*!
     * \brief Mapping which stores callback function ids to their internal
     *        reference counters.
     */
    std::map<chaos::uint32, CallbackReferenceCounter*> m_scope_refs;

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Static function used to wrap member functions so they can be
     *        handled as normal function pointer.
     */
    template<typename owner_type,
             void (owner_type::*member_function)(function_parameters...)>
    static void member_wrapper(void* owner, function_parameters... params)
    {
        (static_cast<owner_type*>(owner)->*member_function)(params...);
    }

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*
     * \brief Private default constructor.
     */
    CallbackInterface()
        :
        m_next_id(0)
    {
    }

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether this holds a reference counter for the given
     *        callback.
     */
    virtual bool has_reference_counter(chaos::uint32 id) const
    {
        return m_scope_refs.find(id) != m_scope_refs.end();
    }

    /*!
     * \brief Passes in a ScopedCallback's reference counter.
     *
     * The interface needs to hold the ScopedCallbacks reference counter so that
     * ScopedCallback can be alerted if this object is destroyed.
     */
    virtual void add_reference_counter(
        chaos::uint32 id,
        CallbackReferenceCounter* ref_counter)
    {
        // check the reference counter isn't registered
        assert(m_scope_refs.find(id) == m_scope_refs.end());
        // add to mapping
         m_scope_refs[id] = ref_counter;
    }

    /*!
     * \brief unregisters the callback with the given id from this
     *        CallbackInterface.
     *
     * This function is private since unregistering is handled by
     * sigma::core::ScopedCallback, which is a friend class.
     */
    virtual void unregister_function(chaos::uint32 id)
    {
        // ensure the id is associated with this interface
        assert(m_callback_funcs.find(id) != m_callback_funcs.end());
        assert(m_scope_refs.find(id) != m_scope_refs.end());

        // remove the id and function from the mappings
        m_callback_funcs.erase(id);
        m_scope_refs.erase(id);
    }

    /*!
     * \brief Calls all callback functions registered in this CallbackInterface.
     *
     * This function is private since trigger callbacks is done through
     * sigma::core::CallbackHandler.
     */
    void trigger(function_parameters... params)
    {
        // iterate over the attached functions and call them
        CHAOS_FOR_EACH(it, m_callback_funcs)
        {
            // member function?
            if (it->second.owner != NULL)
            {
                it->second.function.member(it->second.owner, params...);
            }
            // standard function
            else
            {
                it->second.function.standard(params...);
            }
        }
    }
};


/*!
 * \brief Object used to handle callbacks of a given function type.
 *
 * A CallbackHandler object should be used when data needs to be emitted to
 * observer objects. Observers register their callback functions through the
 * CallbackHandler's internal sigma::core::CallbackInterface. When this object
 * is trigged the functions registered with the CallbackInterface are called.
 *
 * This object can support callback functions with arbitrary parameters using
 * templates.
 *
 * \tparam function_parameters List of the types of the parameters of the
 *                             callback function type this object is being
 *                             instantiated for.
 *
 * Example use:
 *
 * \code
 * void my_func(const chaos::uni::UTF8String& s, int i)
 * {
 *     std::cout << "function called with: " << s << ":" << i << std::endl;
 * }
 *
 * int main(int argc, char* argv[])
 * {
 *     // create a new callback handler
 *     sigma::core::CallbackHandler<chaos::uni::UTF8String, int> handler;
 *     // add a function to this handler
 *     sigma::core::ScopedCallback callback(
 *             handler.get_interface().register_function(func)
 *     );
 *
 *     // the function 'my_func' is called here and will print:
 *     // "function called with: Hello World:10"
 *     handler.trigger("Hello World", 10);
 *
 *     // removes the callback from this handler's interface
 *     callback.unregister();
 *
 *     // no functions will be called this time
 *     handler.trigger(":(", 0);
 *
 *    return 0;
 * }
 * \endcode
 */
template<typename... function_parameters>
class CallbackHandler
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN( CallbackHandler );

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new CallbackHandler for callback functions with the
     *        template types as parameters.
     */
    CallbackHandler()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the sigma::core::CallbackInterface object associated
     *        with this handler.
     *
     * The CallbackInterface is used by observers to register callback
     * functions.
     */
    CallbackInterface<function_parameters...>& get_interface()
    {
        return m_interface;
    }

    /*!
     * \brief Calls all callback functions registered with this object's
     *        internal CallbackInterface.
     *
     * The callback functions will be called using the parameters passed into
     * this function.
     */
    void trigger(function_parameters... params)
    {
        m_interface.trigger(params...);
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal CallbackInterface
     */
    CallbackInterface<function_parameters...> m_interface;
};

} // namespace core
} // namespace sigma

#endif
