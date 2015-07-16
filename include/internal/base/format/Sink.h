/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * String formatter sink type.
 */
#ifndef __BASE_FORMAT_SINK_H__
#define __BASE_FORMAT_SINK_H__

namespace s1
{
  namespace format
  {
    template<typename DestTypeT>
    class Sink
    {
    public:
      typedef DestTypeT DestType;
      typedef typename DestType::value_type value_type;
      Sink (DestType& dest, size_t output_size);

      void append (const value_type* s, size_t n);
      void append (const DestType& s);
      void push_back (value_type c);
    };
  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_SINK_H__
