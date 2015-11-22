/*
Shader1
Copyright (c) 2015 Frank Richter


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
 * Generic code generation options container
 */
#ifndef __CODEGEN_OPTIONSBASE_H__
#define __CODEGEN_OPTIONSBASE_H__

#include "base/uc/String.h"

#include "boost/optional.hpp"
#include "boost/unordered_map.hpp"

#include <functional>

namespace s1
{
  namespace codegen
  {
    template<typename Derived>
    class OptionsBase
    {
    protected:
      /// Options declarations
      class Declarations
      {
      public:
        /// Data to access an option
        template<typename T>
        struct Option
        {
          /// Setter function
          std::function<void (OptionsBase*, T)> set;
          /// Getter function
          std::function<boost::optional<T> (const OptionsBase*)> get;
          /// Reset function
          std::function<void (OptionsBase*)> reset;
        };
        /// Data to access a flag option
        typedef Option<bool> FlagOption;
        /// Registered flag options
        boost::unordered_map<uc::String, FlagOption> flags;
      protected:
        /**
        * Register a 'flag' style options.
        * \param name Name of flag.
        * \param flagMember Pointer to member where the flag data is stored.
        */
        template<typename T>
        void RegisterFlag (const uc::String& name, boost::optional<bool> T::* flagMember)
        {
          flags[name] = {
            [=] (OptionsBase* instance, bool value) { static_cast<T*> (instance)->*flagMember = value; },
            [=] (const OptionsBase* instance) { return static_cast<const T*> (instance)->*flagMember; },
            [=] (OptionsBase* instance) { static_cast<T*> (instance)->*flagMember = boost::none; }
          };
        }
      };
      const Declarations& InternalGetDeclarations () const
      {
        return static_cast<const Derived*> (this)->GetDeclarations ();
      }
    public:
      /**
       * Set the value of a flag option.
       * \param name Name of the flag option.
       * \param value Value of the flag option.
       * \returns Whether setting the flag was successful. (Fails if flag is unknown.)
       */
      bool SetFlag (const uc::String& name, bool value)
      {
        const auto& flagsDecl = InternalGetDeclarations ().flags;
        const auto flagsIt = flagsDecl.find (name);
        if (flagsIt == flagsDecl.end ()) return false;
        flagsIt->second.set (this, value);
        return true;
      }

      /**
       * Set all unset flag and option values to their respective values in
       * \a other.
       */
      void SetAllUnsetFrom (const Derived& other)
      {
        {
          const auto& flagsDecl = InternalGetDeclarations ().flags;
          for (auto& flag : flagsDecl)
          {
            if (!flag.second.get (this))
            {
              auto other_flag = flag.second.get (&other);
              if (other_flag)
                flag.second.set (this, *other_flag);
            }
          }
        }
      }
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_OPTIONSBASE_H__
