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

#include "base/common.h"
#include "intermediate/FunctionGraph.h"

#include "intermediate/NullVisitor.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"

#include <functional>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/strong_components.hpp>

namespace s1
{
  namespace intermediate
  {
    class CallTrackingVisitor : public NullVisitor
    {
    public:
      typedef std::function<void (const uc::String&)> callback_type;
      CallTrackingVisitor (callback_type callback) : callback (callback) {}

      void OpBlock (const SequencePtr& subSequence,
        const Sequence::IdentifierToRegMap& identToRegs_imp,
        const Sequence::IdentifierToRegMap& identToRegs_exp) override;

      void OpBranch (const RegisterPtr& conditionReg,
        const SequenceOpPtr& seqOpIf,
        const SequenceOpPtr& seqOpElse) override;
      void OpWhile (const RegisterPtr& conditionReg,
        const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
        const SequenceOpPtr& seqOpBody) override;

      void OpFunctionCall (const uc::String& funcIdent,
        const std::vector<RegisterPtr>& inParams,
        const std::vector<RegisterPtr>& outParams) override;
    private:
      callback_type callback;
    };

    void CallTrackingVisitor::OpBlock (const SequencePtr& subSequence,
      const Sequence::IdentifierToRegMap& identToRegs_imp,
      const Sequence::IdentifierToRegMap& identToRegs_exp)
    {
      subSequence->Visit (*this);
    }

    void CallTrackingVisitor::OpBranch (const RegisterPtr& conditionReg,
      const SequenceOpPtr& seqOpIf,
      const SequenceOpPtr& seqOpElse)
    {
      auto seqIf = boost::dynamic_pointer_cast<SequenceOpBlock> (seqOpIf);
      S1_ASSERT(seqIf, S1_ASSERT_RET_VOID);
      seqIf->Visit (*this);

      if (seqOpElse)
      {
        auto seqElse = boost::dynamic_pointer_cast<SequenceOpBlock> (seqOpElse);
        S1_ASSERT(seqElse, S1_ASSERT_RET_VOID);
        seqElse->Visit (*this);
      }
    }

    void CallTrackingVisitor::OpWhile (const RegisterPtr& conditionReg,
      const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
      const SequenceOpPtr& seqOpBody)
    {
      auto seqBody = boost::dynamic_pointer_cast<SequenceOpBlock> (seqOpBody);
      S1_ASSERT(seqBody, S1_ASSERT_RET_VOID);
      seqBody->Visit (*this);
    }

    void CallTrackingVisitor::OpFunctionCall (const uc::String& funcIdent,
      const std::vector<RegisterPtr>& inParams,
      const std::vector<RegisterPtr>& outParams)
    {
      callback (funcIdent);
    }

    //-----------------------------------------------------------------------

    // Function graph internal data
    struct FunctionGraph::Data
    {
      // Vertex property: function identifier
      typedef boost::adjacency_list <boost::vecS,
        boost::vecS, boost::bidirectionalS, uc::String> FunctionCallGraph;
      /// The actual graph
      FunctionCallGraph graph;

      /// Whether function recursiveness was recorded
      bool haveRecursive : 1;
      /// Recursive functions
      boost::unordered_set<uc::String> recursive;

      Data () : haveRecursive (false) {}
    };

    FunctionGraph::FunctionGraph (const Program& prog) : data (new Data)
    {
      BuildGraph (prog);
    }

    FunctionGraph::~FunctionGraph ()
    {
    }

    bool FunctionGraph::IsRecursive (const uc::String& ident) const
    {
      if (!data->haveRecursive)
      {
        CollectRecursive ();
        data->haveRecursive = true;
      }
      return data->recursive.find (ident) != data->recursive.end ();
    }

    void FunctionGraph::BuildGraph (const Program& prog)
    {
      auto& graph = data->graph;

      size_t num_f = prog.GetNumFunctions ();
      // Add all functions as vertices
      boost::unordered_map<uc::String, Data::FunctionCallGraph::vertex_descriptor> identToVertex;
      for (size_t f = 0; f < num_f; f++)
      {
        const auto& ident = prog.GetFunction (f)->GetIdentifier ();
        identToVertex[ident] = boost::add_vertex (ident, graph);
      }

      // Visit each function, track calls to other functions
      for (size_t f = 0; f < num_f; f++)
      {
        auto currentFunc = prog.GetFunction (f);
        const auto& currentIdent = currentFunc->GetIdentifier ();
        CallTrackingVisitor visitor (
          [&](const uc::String& calledIdent)
          {
            // Add edge from currentFunc to calledIdent to graph
            auto currentVertexIt = identToVertex.find (currentIdent);
            S1_ASSERT(currentVertexIt != identToVertex.end (), S1_ASSERT_RET_VOID);
            auto calledVertexIt = identToVertex.find (calledIdent);
            S1_ASSERT(calledVertexIt != identToVertex.end (), S1_ASSERT_RET_VOID);
            boost::add_edge (currentVertexIt->second, calledVertexIt->second, graph);
            // "Simple" recursion: handle straight away
            if (currentIdent == calledIdent)
            {
              data->recursive.insert (currentIdent);
            }
        });
        currentFunc->GetBody ()->Visit (visitor);
      }
    }

    void FunctionGraph::CollectRecursive () const
    {
      const auto& graph = data->graph;

      /* Determine recursiveness by looking at strong components
       * in graph:
       * If a function appears in a strong component, it may
       * somehow be called from itself. */
      std::vector<int> component (boost::num_vertices (graph), 0);
      auto num_components = boost::strong_components (graph,
        boost::make_iterator_property_map (component.begin (), boost::get (boost::vertex_index, graph)));

      // Compute sizes of components
      std::vector<size_t> component_sizes (num_components, 0);
      for (int c : component)
      {
        component_sizes[c]++;
      }

      typedef Data::FunctionCallGraph::vertex_descriptor vertex_id;
      // Now look at individual functions
      for (vertex_id v = 0; v < component.size (); v++)
      {
        /* For non-recursive functions we will get components
         * with just a lone function. Ignore these.
         * (The case of a function simply calling itself was
         * already handled in BuildGraph().) */
        int comp = component[v];
        if (component_sizes[comp] == 1) continue;

        data->recursive.insert (boost::get (boost::vertex_bundle, graph, v));
      }
    }
  } // namespace intermediate
} // namespace s1
