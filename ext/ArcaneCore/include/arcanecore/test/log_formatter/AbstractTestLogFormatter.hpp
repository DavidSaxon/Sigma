/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_TEST_LOGFORMATTER_ABSTRACTTESTLOGFORMATTER_HPP_
#define ARCANECORE_TEST_LOGFORMATTER_ABSTRACTTESTLOGFORMATTER_HPP_

// hide from doxygen
#ifndef IN_DOXYGEN

#include <iosfwd>
#include <map>
#include <vector>

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{
namespace test
{
namespace log_formatter
{

/*!
 * \brief Abstract base class that all other Test Log Formatters will inherit
 *        from.
 *
 * Test Log Formatters provided functions for formatting data to log and then
 * writing it to it's output stream.
 */
class AbstractTestLogFormatter
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor for Test Log Formatters.
     *
     * \param verbosity The level of verbosity of the formatter.
     * \param stream The stream to write logs to.
     */
    AbstractTestLogFormatter(arc::uint16 verbosity, std::ostream* stream);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractTestLogFormatter()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void open_log() = 0;

    virtual void close_log(
            arc::int32 units_passed,
            arc::int32 units_failed,
            arc::int32 units_errored,
            arc::uint64 checks_passed,
            arc::uint64 checks_failed) = 0;

    virtual void open_test(
            const arc::str::UTF8String& path,
            const arc::str::UTF8String& id) = 0;

    virtual void close_test() = 0;

    virtual void report_crash(const arc::str::UTF8String& info) = 0;

    virtual void report_check_pass(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& file,
            arc::int32 line) = 0;

    virtual void report_check_fail(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& file,
            arc::int32 line,
            const arc::str::UTF8String& message) = 0;

    virtual void write_message(const arc::str::UTF8String& message) = 0;

    virtual void finialise_test_report(
            arc::uint64 checks_passed,
            arc::uint64 checks_failed) = 0;

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The verbosity level of the formatter.
     */
    arc::uint16 m_verbosity;
    /*!
     * \brief The stream this logger is writing to.
     */
    std::ostream* m_stream;
    /*!
     * \brief mapping from log message to the number of times they've occurred.
     */
    std::map<arc::str::UTF8String, arc::uint64> m_occurrence_map;
    /*!
     * \brief holds the order for the occurrence mapping.
     */
    std::vector<arc::str::UTF8String> m_occurrence_order;

    /*!
     * \brief Stores a log entry and the number of times it has consecutively
     *        occurred.
     */
    struct LogOccurence
    {
        arc::str::UTF8String entry;
        arc::uint32 count;
    };
    /*!
     * \brief Stores log occurrences
     */
    std::vector<LogOccurence> m_occurrences;

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Adds an occurrence of a log entry to the occurrence map.
     */
    void add_occurrence(const arc::str::UTF8String& entry);
};

} // namespace log_formatter
} // namespace test
} // namespace arc

#endif

#endif
