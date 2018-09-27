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

#ifndef __INTERMEDIATE_T_COMMONCHECKS_H__
#define __INTERMEDIATE_T_COMMONCHECKS_H__

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

static inline void VerifyRegsAssignedOnce (const s1::intermediate::SequencePtr& seq)
{
  typedef std::unordered_map<s1::intermediate::RegisterPtr, size_t> AssignCountMap;
  AssignCountMap assignCount;
  
  for (size_t n = 0; n < seq->GetNumOps(); n++)
  {
    s1::intermediate::SequenceOpPtr op (seq->GetOp (n));
    s1::intermediate::RegisterSet writtenRegs (op->GetWrittenRegisters());
    for(const s1::intermediate::RegisterPtr& reg : writtenRegs)
    {
      ++(assignCount[reg]);
    }
  }
  
  for (const auto& assigned_count : assignCount)
  {
    std::string regName;
    assigned_count.first->GetName().toUTF8String (regName);
    BOOST_CHECK_MESSAGE(assigned_count.second == 1, regName << " assigned more than once (" << assigned_count.second << " time)");
  }
}

static inline void VerifyRegsWrittenBeforeRead (const s1::intermediate::SequencePtr& seq)
{
  s1::intermediate::RegisterSet allWrittenRegs;
  
  for (size_t n = 0; n < seq->GetNumOps(); n++)
  {
    s1::intermediate::SequenceOpPtr op (seq->GetOp (n));
    s1::intermediate::RegisterSet readRegs (op->GetReadRegisters());
    for(const s1::intermediate::RegisterPtr& reg : readRegs)
    {
      std::string regName;
      reg->GetName().toUTF8String (regName);
      
      s1::intermediate::RegisterSet::const_iterator writtenRegIt = allWrittenRegs.find (reg);
      BOOST_CHECK_MESSAGE(writtenRegIt != allWrittenRegs.end(), regName << " not written");
    }
    s1::intermediate::RegisterSet writtenRegs (op->GetWrittenRegisters());
    allWrittenRegs.insert (writtenRegs.begin(), writtenRegs.end());
  }
}

#endif // __INTERMEDIATE_T_COMMONCHECKS_H__
