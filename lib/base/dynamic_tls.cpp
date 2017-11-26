/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif

#include "base/common.h"

#include "base/dynamic_tls.h"

#include "base/bit_scan.h"

#include <malloc.h>
#include <string.h>

#include <algorithm>
#include <atomic>
#include <mutex>
#include <new>
#include <unordered_set>

#include <boost/dynamic_bitset.hpp>
#include <boost/optional.hpp>

namespace s1
{
  namespace dynamic_tls
  {
    /* This structure is a mix of array and tree.
     * Data storage is organised in blocks.
     * At the first level, the block simply contains a flat array with
     * the elements.
     * However, if more elements than a certain threshold are required,
     * the last element actually points to another block. That block
     * again contains a flat array of pointers, but these blocks point
     * to "leaf" blocks, which actually contain the data.
     * Again, if more elements than a certain threshold are required,
     * this "node block" again contains a pointer to another block;
     * from below that block there's no two levels of blocks (including
     * leafs), above that there may be another block...
     * (This is the tree-like part of the structure.)
     */
    namespace pointer_tree
    {
      static const size_t blockSize = 1024;
      static const size_t blockEntryCount = blockSize / sizeof(std::atomic<void*>);
      static const size_t blockDataEntries = blockEntryCount - 1;

      struct Block
      {
        std::atomic<void*> entries[blockEntryCount];
      };

      namespace detail
      {
        static std::atomic<void*>* locate_in_full (const Block* b, size_t index, size_t div)
        {
          if (!b) return nullptr;

          if (div == 1)
          {
            return const_cast<std::atomic<void*>*> (&(b->entries[index]));
          }
          else
          {
            size_t blockIndex = index / div;
            size_t subIndex = index % div;
            return locate_in_full (reinterpret_cast<const Block*> (b->entries[blockIndex].load (std::memory_order_relaxed)),
                                   subIndex, div / blockEntryCount);
          }
        }

        static std::atomic<void*>* locate (const Block* b, size_t index)
        {
          if (!b) return nullptr;

          const Block* currentBlock = b;
          size_t currentIndex = index;
          size_t indexDiv = 1;
          size_t currentOffset = 0;
          // First, go up...
          while (currentIndex >= blockDataEntries)
          {
            currentBlock = reinterpret_cast<Block*> (currentBlock->entries[blockDataEntries].load (std::memory_order_relaxed));
            currentIndex = (currentIndex - blockDataEntries) / blockEntryCount;
            currentOffset = currentOffset * blockEntryCount + blockDataEntries;
            indexDiv *= blockEntryCount;

            if (!currentBlock) return nullptr;
          }
          // ... then go down.
          return detail::locate_in_full (currentBlock, index - currentOffset, indexDiv);
        }

        static Block* alloc_block ()
        {
          Block* b = reinterpret_cast<Block*> (malloc (sizeof (Block)));
          if (!b) throw std::bad_alloc ();
          memset (b, 0, sizeof (Block));
          return b;
        }

        static void discard_full (Block* b, size_t depth)
        {
          if (!b) return;

          if (depth > 0)
          {
            for (size_t i = 0; i < blockEntryCount; i++)
            {
              discard_full (reinterpret_cast<Block*> (b->entries[i].load (std::memory_order_relaxed)), depth-1);
            }
          }

          free (b);
        }

        static void discard_up (Block* b, size_t depth)
        {
          if (!b) return;
          Block* up = reinterpret_cast<Block*> (b->entries[blockDataEntries].load (std::memory_order_relaxed));
          if (up) discard_up (up, depth+1);

          if (depth > 0)
          {
            for (size_t i = 0; i < blockDataEntries; i++)
            {
              discard_full (reinterpret_cast<Block*> (b->entries[i].load (std::memory_order_relaxed)), depth-1);
            }
          }

          free (b);
        }

        static void resize_full (std::atomic<void*>& b, size_t& remaining_size, size_t depth)
        {
          auto block_p = reinterpret_cast<Block*> (b.load (std::memory_order_relaxed));
          if (depth > 0)
          {
            if (remaining_size == 0)
            {
              discard_full (block_p, depth);
              b.store (nullptr, std::memory_order_relaxed);
              return;
            }

            if (!block_p)
            {
              block_p = alloc_block ();
              b.store (block_p, std::memory_order_relaxed);;
            }

            for (size_t i = 0; i < blockEntryCount; i++)
            {
              resize_full (block_p->entries[i], remaining_size, depth-1);
            }
          }
          else
          {
            remaining_size -= std::min<size_t> (remaining_size, 1);
          }
        }

        static void resize_up (std::atomic<void*>& b, size_t size, size_t depth)
        {
          auto block_p = reinterpret_cast<Block*> (b.load (std::memory_order_relaxed));
          if (size == 0)
          {
            detail::discard_up (block_p, depth);
            b.store (nullptr, std::memory_order_relaxed);
          }
          else
          {
            if (!block_p)
            {
              block_p = alloc_block ();
              b.store (block_p, std::memory_order_relaxed);;
            }

            size_t remaining_size = size;
            for (size_t i = 0; i < blockDataEntries; i++)
            {
              resize_full (block_p->entries[i], remaining_size, depth);
            }
            resize_up (block_p->entries[blockDataEntries], remaining_size, depth + 1);
          }
        }
      } // namespace detail

      static void discard (Block* b)
      {
        detail::discard_up (b, 0);
      }

      static boost::optional<void*> get (const Block* b, size_t index)
      {
        if (auto p = detail::locate (b, index))
        {
          return p->load (std::memory_order_acquire);
        }
        return boost::none;
      }

      static bool set (Block* b, size_t index, void* value)
      {
        if (auto p = detail::locate (b, index))
        {
          p->store (value, std::memory_order_release);
          return true;
        }
        return false;
      }

      static boost::optional<void*> exchange (const Block* b, size_t index, void* value)
      {
        if (auto p = detail::locate (b, index))
        {
          return p->exchange (value, std::memory_order_acq_rel);
        }
        return boost::none;
      }

      static void resize (Block*& b, size_t size)
      {
        std::atomic<void*> block_a;
        block_a.store (b, std::memory_order_relaxed);
        detail::resize_up (block_a, size, 0);
        b = reinterpret_cast<Block*> (block_a.load (std::memory_order_relaxed));
      }
    } // namespace pointer_tree

    struct Dtor
    {
      DestructorFunc func;
      uintptr_t context;
    };

    struct RegisteredSlot
    {
      /// On-thread destructor function
      Dtor threadDtor;
    };

    struct PerThreadData;
    typedef std::shared_ptr<PerThreadData> PerThreadDataPtr;

    /**
     * Dynamic bit array.
     * Basically same as boost::dynamic_bitset<>, but with a different, bespoke
     * feature set.
     */
    class bitset
    {
    public:
      typedef unsigned long block_type;
      static const size_t npos = static_cast<size_t>(-1);

      ~bitset ()
      {
        resize (0);
      }

      //size_t num_blocks () const;
      size_t size () const { return bitCount; }

      void resize (size_t new_size)
      {
        auto prevBlockCount = blockCount (bitCount);
        auto newBlockCount = blockCount (new_size);
        if (prevBlockCount != newBlockCount)
        {
          const auto prevBlocksPtr = GetBlocksPtr();
          block_type* newBlocksPtr = nullptr;
          bool prevInline = prevBlockCount <= (sizeof (block_type*) / sizeof (block_type));
          bool newInline = newBlockCount <= (sizeof (block_type*) / sizeof (block_type));
          if (newInline)
          {
            // Store new data inline
            newBlocksPtr = reinterpret_cast<block_type*> (&blocksData);
          }
          else
          {
            if (prevInline)
            {
              // Allocate new block
              newBlocksPtr = reinterpret_cast<block_type*> (malloc (newBlockCount * sizeof (block_type)));
            }
            else
            {
              // Reallocate block
              newBlocksPtr = reinterpret_cast<block_type*> (realloc (prevBlocksPtr, newBlockCount * sizeof (block_type)));
            }
          }

          if (newBlocksPtr != prevBlocksPtr)
          {
            auto copyBlockCount = std::min (prevBlockCount, newBlockCount);
            memcpy (newBlocksPtr, prevBlocksPtr, copyBlockCount * sizeof (block_type));
            if (!prevInline && newInline)
            {
              // Free old block
              free (prevBlocksPtr);
            }
          }

          if (!newInline)
            blocksData = newBlocksPtr;
        }

        auto old_count = bitCount;
        bitCount = new_size;
        // Zero out all new bits, bits beyond last
        zero_bits (std::min (old_count, new_size), newBlockCount * blockBits);
      }

      //const block_type* data () const;

      void set (size_t index, bool flag = true)
      {
        checkIndex (index);
        auto& block = GetBlocksPtr ()[index / blockBits];
        auto bit = index % blockBits;
        if (flag)
          block |= 1 << bit;
        else
          block &= ~(1 << bit);
      }
      void reset (size_t index)
      { set (index, false); }
      bool test (size_t index) const
      {
        checkIndex (index);
        auto& block = GetBlocksPtr ()[index / blockBits];
        auto bit = index % blockBits;
        return (block & (1 << bit)) != 0;
      }

      size_t find_first_unset () const
      {
        auto num_blocks = blockCount (bitCount);
        const auto blocks = GetBlocksPtr ();
        for (size_t b = 0; b < num_blocks; b++)
        {
          auto block_val = blocks[b];
          if (block_val == ~block_type (0)) continue;
          return bit_scan::lsb (~block_val) + b * blockBits;
        }

        return npos;
      }

      size_t find_last () const
      {
        auto num_blocks = blockCount (bitCount);
        const auto blocks = GetBlocksPtr ();
        for (size_t b = num_blocks; b-- > 0; )
        {
          auto block_val = blocks[b];
          if (block_val == block_type (0)) continue;
          return bit_scan::msb (block_val) + b * blockBits;
        }

        return npos;
      }

    protected:
      static const size_t blockBits = CHAR_BIT * sizeof (block_type);

      static size_t blockCount (size_t bitCount)
      { return (bitCount / blockBits) + ((bitCount % blockBits) != 0 ? 1 : 0); }

      size_t bitCount = 0;
      block_type* blocksData = 0;

      block_type* GetBlocksPtr ()
      {
        auto block_count = blockCount (bitCount);
        if (block_count == 0)
          return nullptr;
        else if (block_count <= (sizeof (block_type*) / sizeof (block_type)))
          return reinterpret_cast<block_type*> (&blocksData);
        else
          return blocksData;
      }

      const block_type* GetBlocksPtr () const
      {
        auto block_count = blockCount (bitCount);
        if (block_count == 0)
          return nullptr;
        else if (block_count <= (sizeof (block_type*) / sizeof (block_type)))
          return reinterpret_cast<const block_type*> (&blocksData);
        else
          return blocksData;
      }

      void checkIndex (size_t index) const
      {
        if (index >= bitCount) throw std::out_of_range ("bit index out of range");
      }

      /// Zero bits with indices first (incl.) to last (excl.)
      void zero_bits (size_t first, size_t last)
      {
        auto blocks = GetBlocksPtr();
        // Zero complete blocks
        auto firstComplete = blockCount (first);
        auto lastComplete = last / blockBits;
        for (size_t b = firstComplete; b < lastComplete; b++)
        {
          blocks[b] = 0;
        }

        // Partial zeroing: bits from 'first' until first complete block
        if ((first % blockBits) != 0)
        {
          auto firstBlock = first / blockBits;
          auto firstMask = ~((1 << (first % blockBits)) - 1);
          blocks[firstBlock] &= firstMask;
        }

        // Partial zeroing: bits from last complete block until 'last'
        if ((last % blockBits) != 0)
        {
          auto lastBlock = blockCount (last);
          auto lastMask = ((1 << (last % blockBits)) - 1);
          blocks[lastBlock] &= lastMask;
        }
      }
    };

    struct GlobalData
    {
      std::mutex mutex;
      /// Total number of slots required in per-thread data
      std::atomic<size_t> requiredSlotCount;
      /// Bits indicating used slots
      bitset usedSlots;
      /// Information for registered slots
      std::vector<RegisteredSlot> slotInfo;

      /// Known threads
      std::unordered_set<PerThreadData*> threads;
    };
    static GlobalData globalData;

    struct PerThreadData
    {
      /// Actual TLS slot values. Indexed by handle
      pointer_tree::Block* values = nullptr;
      /// Number of entries in 'values'
      size_t valuesSize = 0;

      PerThreadData ()
      {
        // Store ourselves in global set of known threads
        std::lock_guard<std::mutex> lockGlobal (globalData.mutex);
        globalData.threads.insert (this);
      }
      ~PerThreadData ()
      {
        typedef std::tuple<void*, Dtor> ValueAndDtor;
        ValueAndDtor* threadValues = reinterpret_cast<ValueAndDtor*> (alloca (valuesSize * sizeof (ValueAndDtor)));
        size_t n = 0;

        {
          std::lock_guard<std::mutex> lockGlobal (globalData.mutex);
          // For each seen slot:
          size_t num_slots = std::min (valuesSize, globalData.slotInfo.size());
          for (size_t i = 0; i < num_slots; i++)
          {
            if (!globalData.usedSlots.test (i)) continue;
            // Get slot information
            const auto& slotInfo = globalData.slotInfo[i];
            // Collect destructor function
            if (auto value_p = pointer_tree::get (values, i))
            {
              threadValues[n++] = std::make_tuple (*value_p, slotInfo.threadDtor);
            }
          }
          // Remove ourselves in global set of known threads
          globalData.threads.erase (this);
        }

        /* Call destructor functions. Called after we are removed from global data
         * to make recursive Free() calls work. */
        for (size_t i = 0; i < n; i++)
        {
          const auto& dtor = std::get<1> (threadValues[i]);
          if (dtor.func) dtor.func (std::get<0> (threadValues[i]), dtor.context);
        }

        pointer_tree::discard (values);
      }
    };
    static S1_THREAD_LOCAL PerThreadDataPtr threadData;

    static PerThreadData& GetThreadData()
    {
      if (!threadData) threadData = std::make_shared<PerThreadData> ();
      return *threadData;
    }

    Handle Alloc (DestructorFunc dtor, uintptr_t dtorContext)
    {
      std::lock_guard<std::mutex> lockGlobal (globalData.mutex);

      // Find an unused handle
      size_t unused_index = globalData.usedSlots.find_first_unset ();
      if (unused_index >= globalData.usedSlots.size())
      {
        unused_index = globalData.usedSlots.size();
        size_t max_index = std::min<size_t> (std::numeric_limits<Handle>::max(), std::numeric_limits<size_t>::max()-1);
        if (unused_index > max_index)
          throw std::domain_error ("no TLS handles available");
        globalData.usedSlots.resize (unused_index + 1);
      }

      globalData.usedSlots.set (unused_index);

      auto slots_size = globalData.usedSlots.size();
      globalData.slotInfo.resize (slots_size);
      auto& newSlotInfo = globalData.slotInfo[unused_index];
      newSlotInfo.threadDtor.func = dtor;
      newSlotInfo.threadDtor.context = dtorContext;

      globalData.requiredSlotCount.store (slots_size, std::memory_order_release);

      return static_cast<Handle> (unused_index);
    }

    void Free (Handle slot, DestructorFunc dtor, uintptr_t dtorContext)
    {
      void** slot_values;
      size_t num_slot_values = 0;
      {
        std::lock_guard<std::mutex> lockGlobal (globalData.mutex);

        if ((slot >= globalData.usedSlots.size()) || !globalData.usedSlots.test (slot))
          return; // Invalid slot index

        // Collect current values of slot from all threads
        slot_values = reinterpret_cast<void**> (alloca (globalData.threads.size() * sizeof(void*)));
        for (const auto& threadData : globalData.threads)
        {
          if (auto value_p = pointer_tree::exchange (threadData->values, slot, nullptr))
            slot_values[num_slot_values++] = *value_p;
        }

        // Flag slot as "unused"
        globalData.usedSlots.reset (slot);
        if (slot == globalData.usedSlots.size ()-1)
        {
          // Unset last slot -> try to reduce size of arrays
          size_t last_slot = globalData.usedSlots.find_last ();
          size_t new_size = last_slot != bitset::npos ? last_slot + 1 : 0;
          globalData.usedSlots.resize (new_size);
          globalData.slotInfo.resize (new_size);
          globalData.requiredSlotCount.store (new_size, std::memory_order_release);
        }
      }

      // Call dtor on all values
      if (dtor)
      {
        for (size_t i = 0; i < num_slot_values; i++)
        {
          dtor (slot_values[i], dtorContext);
        }
      }
    }

    void SetValue (Handle slot, void* value)
    {
      auto& threadData = GetThreadData();
      size_t needSlotsSize = globalData.requiredSlotCount.load (std::memory_order_acquire);
      if (threadData.valuesSize != needSlotsSize)
      {
        pointer_tree::resize (threadData.values, needSlotsSize);
        threadData.valuesSize = needSlotsSize;
      }
      pointer_tree::set (threadData.values, slot, value);
    }

    void* GetValue (Handle slot)
    {
      auto& threadData = GetThreadData();
      if (slot >= threadData.valuesSize) return nullptr;
      if (auto value_p = pointer_tree::get (threadData.values, slot))
        return *value_p;
      return nullptr;
    }

  } // namespace dynamic_tls
} // namespace s1
