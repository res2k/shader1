#ifndef __BASE_UNICODESTREAM_H__
#define __BASE_UNICODESTREAM_H__

#include <unicode/errorcode.h>
#include <unicode/utypes.h>

#include <istream>

struct UConverter;

namespace s1
{
  /**
   * Stream for Unicode characters from some encoded source.
   */
  class UnicodeStream
  {
  protected:
    class ICUError : public U_NAMESPACE_QUALIFIER ErrorCode
    {
    public:
      virtual void handleFailure() const;
    };
    
    std::istream& inStream;
    /// ICU converter object
    UConverter* uconv;
    
    enum
    {
      /// Size of internal unicode characters buffer
      UCBufferSize = 1024
    };
    UChar ucBuffer[UCBufferSize];
    
    const UChar* ucBufferPtr;
    unsigned int ucBufferRemaining;
    /**
     * ICU error that occured during filling the buffer.
     * The buffer is filled up until an error occured. So the error needs to
     * be propagated only once we reach the end of the internal buffer.
     */
    UErrorCode ucBufferEndError;
    
    /**
     * Internal buffer for data from input stream.
     * Stored to allow resuming after ICU reports an error.
     */
    char streamInBuffer[UCBufferSize];
    const char* streamInBufferPtr;
    size_t streamInBufferRemaining;
    
    UChar32 currentChar;
    UErrorCode currentError;
    enum
    {
      errorCharacter = 0xFFFF,
      noCharacter = -1
    };
    
    /// Get next UTF-16 character from buffer, refill if necessary
    UChar GetNextUChar ();
    /// Refill unicode buffer
    void RefillUCBuffer ();
  public:
    /**
     * Constructor.
     * \param inStream Input byte stream.
     * \param encoding ICU encoding name of the input.
     */
    UnicodeStream (std::istream& inStream, const char* encoding);
    ~UnicodeStream();
    
    /// Returns whether more characters are available
    operator bool() const throw();
    bool operator!() const throw() { return !(bool)(*this); }

    /// Advance stream
    UnicodeStream& operator++() throw();
    
    /// Return current character
    UChar32 operator* () const;
  };
  
} // namespace s1

#endif // __BASE_UNICODESTREAM_H__
