#ifndef __INTERMEDIATE_T_COMMONCHECKS_H__
#define __INTERMEDIATE_T_COMMONCHECKS_H__

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

static void VerifyRegsAssignedOnce (const s1::intermediate::SequencePtr& seq)
{
  typedef boost::unordered_map<s1::intermediate::RegisterPtr, size_t> AssignCountMap;
  AssignCountMap assignCount;
  
  for (size_t n = 0; n < seq->GetNumOps(); n++)
  {
    s1::intermediate::SequenceOpPtr op (seq->GetOp (n));
    s1::intermediate::RegisterSet writtenRegs (op->GetWrittenRegisters());
    BOOST_FOREACH(const s1::intermediate::RegisterPtr& reg, writtenRegs)
    {
      ++(assignCount[reg]);
    }
  }
  
  for (AssignCountMap::const_iterator it = assignCount.begin();
	it != assignCount.end();
	++it)
  {
    std::string regName;
    it->first->GetName().toUTF8String (regName);
    TSM_ASSERT_EQUALS (regName, it->second, 1);
  }
}

static void VerifyRegsWrittenBeforeRead (const s1::intermediate::SequencePtr& seq)
{
  s1::intermediate::RegisterSet allWrittenRegs;
  
  for (size_t n = 0; n < seq->GetNumOps(); n++)
  {
    s1::intermediate::SequenceOpPtr op (seq->GetOp (n));
    s1::intermediate::RegisterSet readRegs (op->GetReadRegisters());
    BOOST_FOREACH(const s1::intermediate::RegisterPtr& reg, readRegs)
    {
      std::string regName;
      reg->GetName().toUTF8String (regName);
      
      s1::intermediate::RegisterSet::const_iterator writtenRegIt = allWrittenRegs.find (reg);
      TSM_ASSERT_DIFFERS (regName, writtenRegIt, allWrittenRegs.end());
    }
    s1::intermediate::RegisterSet writtenRegs (op->GetWrittenRegisters());
    allWrittenRegs.insert (writtenRegs.begin(), writtenRegs.end());
  }
}

#endif // __INTERMEDIATE_T_COMMONCHECKS_H__