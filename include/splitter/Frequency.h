#ifndef __SPLITTER_FREQUENCY_H__
#define __SPLITTER_FREQUENCY_H__

namespace s1
{
  namespace splitter
  {
    enum Frequency
    {
      freqVertex,
      freqFragment,
      
      freqNum,
      freqHighest = freqNum - 1
    };
    enum
    {
      freqFlagV = 1 << freqVertex,
      freqFlagF = 1 << freqFragment,
    };
  } // namespace splitter
} // namespace s1

#endif // __SPLITTER_FREQUENCY_H__
