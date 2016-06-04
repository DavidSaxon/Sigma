/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSCORE_IO_SYS_FILEREADER_HPP_
#define CHAOSCORE_IO_SYS_FILEREADER_HPP_

#include <memory>

#include "chaoscore/io/sys/FileHandle.hpp"

class ifstream;

namespace chaos
{
namespace io
{
namespace sys
{

//------------------------------------------------------------------------------
//                             FORWARD DECELERATIONS
//------------------------------------------------------------------------------

struct NewlineChecker;

/*!
 * \brief Used for reading the contents of a file from disk.
 */
class FileReader : public chaos::io::sys::FileHandle
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN(FileReader);

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default constructor.
     *
     * Creates a new unopened FileReader with no file path yet defined.
     *
     * \param encoding Defines the encoding of the contents of the file to read.
     *                 If chaos::io::sys::FileHandle::ENCODING_DETECT is used
     *                 the FileReader will attempt to detect the encoding used
     *                 in the file at the time of opening. If the encoding
     *                 cannot be detected
     *                 chaos::io::sys::FileHandle::ENCODING_RAW will be used.
     *                 The detected encoding can be queried using get_encoding()
     *                 once the file has been opened.
     * \param newline The newline symbol used in the file to read.
     *                See set_newline().
     */
    FileReader(
            Encoding encoding = ENCODING_DETECT,
            Newline newline   = NEWLINE_UNIX);

    /*!
     * \brief Path constructor.
     *
     * Creates a new FileReader opened to the given path.
     *
     * \param path The path to the file to read from.
     * \param encoding Defines the encoding of the contents of the file to read.
     *                 If chaos::io::sys::FileHandle::ENCODING_DETECT is used
     *                 the FileReader will attempt to detect the encoding used
     *                 in the file at the time of opening. If the encoding
     *                 cannot be detected
     *                 chaos::io::sys::FileHandle::ENCODING_RAW will be used.
     *                 The detected encoding can be queried using get_encoding()
     *                 once the file has been opened.
     * \param newline The newline symbol used in the file to read.
     *                See set_newline().
     *
     * \throws chaos::ex::InvalidPathError If the path cannot be opened.
     */
    FileReader(
            const chaos::io::sys::Path& path,
            Encoding encoding = ENCODING_DETECT,
            Newline newline   = NEWLINE_UNIX);

    /*!
     * \brief Move constructor.
     *
     * \param other The FileReader to move resources from.
     */
    FileReader(FileReader&& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~FileReader();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Opens this FileReader to the internal path.
     *
     * \throws chaos::ex::StateError If this FileReader is already open.
     * \throws chaos::ex::InvalidPathError If the path cannot be opened.
     */
    virtual void open();

    // override to avoid C++ function hiding
    virtual void open(const chaos::io::sys::Path& path);

    /*!
     * \brief Closes this FileReader.
     */
    virtual void close();

    /*!
     * \brief Returns the size of the file being read in bytes.
     *
     * \throws chaos::ex::StateError If the FileReader is not open.
     */
    virtual chaos::int64 get_size() const;

    /*!
     * \brief Returns the index of the byte the file position indicator is
     *        currently at.
     *
     * \throws chaos::ex::StateError If the FileReader is not open.
     */
    virtual chaos::int64 tell() const;

    /*!
     * \brief Sets the file position indicator to the given byte index.
     *
     * \throws chaos::ex::StateError If the FileReader is not open.
     * \throws chaos::ex::IndexOutOfBoundsError If the given byte index is
     *                                          greater than the number of bytes
     *                                          in the file or is less than 0.
     */

    virtual void seek(chaos::int64 index);

    /*!
     * \brief Returns whether file position indicated is at the End of File.
     *
     * \throws chaos::ex::StateError If this FileReader is not open.
     */
    bool eof() const;

    /*!
     * \brief Returns whether this file starts with a Unicode Byte Order Marker.
     *
     * \note This function will only return ```true``` if the file starts with
     *       a BOM that matches the file's encoding, see get_encoding().
     *
     * \warning This function will cause a read into the file. The file position
     *          indicator will be moved from its current position to the
     *          beginning of the file to check for the BOM, then moved back to
     *          its initial position before this function was called.
     *
     * \throws chaos::ex::StateError If this FileReader is not open.
     */
    bool has_bom();

    /*!
     * \brief Sets the file position indicator to the start of the actual file
     *        data.
     *
     * This function checks if the file has a Byte Order Marker and if so, the
     * file position indicator is set to the next character after the BOM. If
     * the file does not have a BOM the file position indicator is set to the
     * start of the file.
     *
     * \returns The file position indicator after this action has been applied.
     */
    chaos::int64 seek_to_data_start();

    /*!
     * \brief Reads a block of data from the file and moves the position
     *        indicator to the index beyond the last read character in the file.
     *
     * This function is a pure copy of data, the copied data will include the
     * Unicode BOM (if the file has one). To read the data starting after the
     * BOM the seek_to_data_start() function can be used.
     *
     * \param data Character array that file data will be copied into.
     * \param length The number of characters to read from the file. If this is
     *               greater than the number of characters in the file this
     *               function will read the remaining characters up to the end
     *               of the file.
     *
     * \throws chaos::ex::StateError If this FileReader is not open.
     * \throws chaos::ex::EOFError If the End of File Marker has been reached.
     */
    void read(char* data, chaos::int64 length);

    /*!
     * \brief Reads a block of data from the file and returns it (converting
     *        the data encoding if needed) represented as a
     *        chaos::str::UTF8String.
     *
     * This function will return data from the file ensuring that it is UTF-8
     * encoded. This function does not affect the newlines of the data, they
     * will be returned as they are in the file. If the file has a BOM it will
     * not be read into the returned data, however the bytes of the BOM will be
     * counted towards the length of data to read.
     *
     * \param data String that the file data will be read into. This
     *             function will remove any existing data contained within the
     *             UTF8String.
     * \param length The number of bytes to read from the file. If ```-1``` is
     *               provided this function will read from file position
     *               indicator to the end of the file.
     *
     * \throws chaos::ex::StateError If this FileReader is not open.
     * \throws chaos::ex::EOFError If the End of File Marker has been reached.
     */
    void read(chaos::str::UTF8String& data, chaos::int64 length = -1);

    /*!
     * \brief Reads a line of data from the file, allocates the memory to hold
     *        the contents of the line and copies the line data to be returned.
     *
     * This function will return the raw data from the next line in the file
     * from the file position indicator. The encoding of the data will not be
     * modified and the data will not be null terminated. However the newline
     * symbols will not be included in the returned data. Once the data has been
     * read the file position indicator will be moved to the start of the next
     * line.
     *
     * \note If the file has a Unicode BOM the data representing it will be
     *       ignored by this function.
     *
     * \param data Returns allocated data for a line that has been read from
     *             the file. The allocated data will be owned by the callee so
     *             must be deleted.
     * \returns The number of bytes that have been allocated and returned via
     *          the data parameter.
     *
     * \throws chaos::ex::StateError If this FileReader is not open. If this
     *                               exception is thrown no data will be
     *                               allocated.
     * \throws chaos::ex::EOFError If the End of File Marker has been reached.
     *                             If this exception is thrown no data will be
     *                             allocated.
     */
    std::size_t read_line(char** data);

    /*!
     * \brief Reads a line of data from the file and returns it (converting the
     *        data encoding if needed) represented as a chaos::str::UTF8String.
     *
     * This function will return the next line in the file from the file
     * position indicator and ensure that it is UTF-8 encoded. The newline
     * symbols will not be included in the returned string. Once the data has
     * been read the file position indicator will be moved to the start of the
     * next line.
     *
     * \note If the file has a Unicode BOM the data representing it will be
     *       ignored by this function.
     *
     * \param data String that the next line in the file will be read into. This
     *             function will remove any existing data in the UTF8String.
     *
     * \throws chaos::ex::StateError If this FileReader is not open. If this
     *                               exception is thrown no data will be
     *                               allocated.
     * \throws chaos::ex::EOFError If the End of File Marker has been reached.
     *                             If this exception is thrown no data will be
     *                             allocated.
     */
    void read_line(chaos::str::UTF8String& data);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The input stream used to read the file.
     */
    std::ifstream* m_stream;

    /*!
     * \brief The size of the file in bytes.
     */
    chaos::int64 m_size;

    /*!
     * \brief Whether the current static newline checker is valid for the
     *        FileReader's encoding and newline symbol.
     */
    bool m_newline_checker_valid;
    /*!
     * \brief The newline checker being used by this file handle.
     *
     * \note This should be accessed through get_newline_checker().
     */
    std::unique_ptr<NewlineChecker> m_newline_checker;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Retrieves a the newline checker for this FileReader's encoding and
     *        newline symbols.
     *
     * This function will initialise the checker if need be.
     */
    NewlineChecker* get_newline_checker();

    /*!
     * \brief Checks and throws common exception as to whether the file can be
     *        read from.
     */
    void check_can_read();
};

} // namespace sys
} // namespace io
} // namespace chaos

#endif
