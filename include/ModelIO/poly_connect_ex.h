/*******************************************************************************

程序说明

该文件源于poly_connect.hxx

由于使用poly_connect进行法线的计算时，当程序执行完毕释放内存时会出现错误；
所以在poly_connect.hxx的基础上进行了更改，将Load函数进行了更改，
加入释放edge array的代码

  // destroy the edges array - can be skipped when using NCollection_IncAllocator
  for (Standard_Integer aNodeIter = anEdges.Lower(); aNodeIter <= anEdges.Upper(); ++aNodeIter)
  {
    for (polyedge* anEdgeIter = anEdges[aNodeIter]; anEdgeIter != NULL;)
    {
      polyedge* aTmp = anEdgeIter->next;
      anIncAlloc->Free (anEdgeIter);
      anEdgeIter = aTmp;
    }
  }

*******************************************************************************/


// Created on: 1995-03-06
// Created by: Laurent PAINNOT
// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _Poly_Connect_Ex_HeaderFile
#define _Poly_Connect_Ex_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <TColStd_Array1OfInteger.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Boolean.hxx>

#include <NCollection_IncAllocator.hxx>
#include <Poly_Triangle.hxx>
#include <Poly_Triangulation.hxx>

struct polyedge
{
  polyedge* next;         // the next edge in the list
  Standard_Integer nt[2]; // the two adjacent triangles
  Standard_Integer nn[2]; // the two adjacent nodes
  Standard_Integer nd;    // the second node of the edge
  DEFINE_STANDARD_ALLOC
};


class Poly_Triangulation;

//! Provides an algorithm to explore, inside a triangulation, the
//! adjacency data for a node or a triangle.
//! Adjacency data for a node consists of triangles which
//! contain the node.
//! Adjacency data for a triangle consists of:
//! -   the 3 adjacent triangles which share an edge of the triangle,
//! -   and the 3 nodes which are the other nodes of these adjacent triangles.
//! Example
//! Inside a triangulation, a triangle T
//! has nodes n1, n2 and n3.
//! It has adjacent triangles AT1, AT2 and AT3 where:
//! - AT1 shares the nodes n2 and n3,
//! - AT2 shares the nodes n3 and n1,
//! - AT3 shares the nodes n1 and n2.
//! It has adjacent nodes an1, an2 and an3 where:
//! - an1 is the third node of AT1,
//! - an2 is the third node of AT2,
//! - an3 is the third node of AT3.
//! So triangle AT1 is composed of nodes n2, n3 and an1.
//! There are two ways of using this algorithm.
//! -   From a given node you can look for one triangle that
//! passes through the node, then look for the triangles
//! adjacent to this triangle, then the adjacent nodes. You
//! can thus explore the triangulation step by step (functions
//! Triangle, Triangles and Nodes).
//! -   From a given node you can look for all the triangles
//! that pass through the node (iteration method, using the
//! functions Initialize, More, Next and Value).
//! A Connect object can be seen as a tool which analyzes a
//! triangulation and translates it into a series of triangles. By
//! doing this, it provides an interface with other tools and
//! applications working on basic triangles, and which do not
//! work directly with a Poly_Triangulation.
class Poly_Connect_Ex
{
public:

  DEFINE_STANDARD_ALLOC

    //=======================================================================
    //function : Poly_Connect
    //purpose  :
    //=======================================================================
	  Poly_Connect_Ex()
    : mytr    (0),
      myfirst (0),
      mynode  (0),
      myothernode (0),
      mysense (false),
      mymore  (false)
    {
      //
    }

    //=======================================================================
    //function : Poly_Connect
    //purpose  :
    //=======================================================================
  Poly_Connect_Ex(const Handle(Poly_Triangulation)& theTriangulation)
    : myTriangulation (theTriangulation),
      myTriangles (1, theTriangulation->NbNodes()),
      myAdjacents (1, 6 * theTriangulation->NbTriangles()),
      mytr    (0),
      myfirst (0),
      mynode  (0),
      myothernode (0),
      mysense (false),
      mymore  (false)
    {
      Load (theTriangulation);
    }


  //! Initialize the algorithm to explore the adjacency data of
  //! nodes or triangles for the triangulation theTriangulation.
  Standard_EXPORT void Load (const Handle(Poly_Triangulation)& theTriangulation);

  //! Returns the triangulation analyzed by this tool.
  const Handle(Poly_Triangulation)& Triangulation() const { return myTriangulation; }

  //! Returns the index of a triangle containing the node at
  //! index N in the nodes table specific to the triangulation analyzed by this tool
  Standard_Integer Triangle (const Standard_Integer N) const { return myTriangles (N); }

  //! Returns in t1, t2 and t3, the indices of the 3 triangles
  //! adjacent to the triangle at index T in the triangles table
  //! specific to the triangulation analyzed by this tool.
  //! Warning
  //! Null indices are returned when there are fewer than 3
  //! adjacent triangles.
  void Triangles (const Standard_Integer T, Standard_Integer& t1, Standard_Integer& t2, Standard_Integer& t3) const
  {
    Standard_Integer index = 6*(T-1);
    t1 = myAdjacents(index+1);
    t2 = myAdjacents(index+2);
    t3 = myAdjacents(index+3);
  }

  //! Returns, in n1, n2 and n3, the indices of the 3 nodes
  //! adjacent to the triangle referenced at index T in the
  //! triangles table specific to the triangulation analyzed by this tool.
  //! Warning
  //! Null indices are returned when there are fewer than 3 adjacent nodes.
  void Nodes (const Standard_Integer T, Standard_Integer& n1, Standard_Integer& n2, Standard_Integer& n3) const
  {
    Standard_Integer index = 6*(T-1);
    n1 = myAdjacents(index+4);
    n2 = myAdjacents(index+5);
    n3 = myAdjacents(index+6);
  }

public:

  //! Initializes an iterator to search for all the triangles
  //! containing the node referenced at index N in the nodes
  //! table, for the triangulation analyzed by this tool.
  //! The iterator is managed by the following functions:
  //! -   More, which checks if there are still elements in the iterator
  //! -   Next, which positions the iterator on the next element
  //! -   Value, which returns the current element.
  //! The use of such an iterator provides direct access to the
  //! triangles around a particular node, i.e. it avoids iterating on
  //! all the component triangles of a triangulation.
  //! Example
  //! Poly_Connect C(Tr);
  //! for
  //! (C.Initialize(n1);C.More();C.Next())
  //! {
  //! t = C.Value();
  //! }
  Standard_EXPORT void Initialize (const Standard_Integer N);
  
  //! Returns true if there is another element in the iterator
  //! defined with the function Initialize (i.e. if there is another
  //! triangle containing the given node).
  Standard_Boolean More() const { return mymore; }

  //! Advances the iterator defined with the function Initialize to
  //! access the next triangle.
  //! Note: There is no action if the iterator is empty (i.e. if the
  //! function More returns false).-
  Standard_EXPORT void Next();
  
  //! Returns the index of the current triangle to which the
  //! iterator, defined with the function Initialize, points. This is
  //! an index in the triangles table specific to the triangulation
  //! analyzed by this tool
  Standard_Integer Value() const { return mytr; }

private:

  Handle(Poly_Triangulation) myTriangulation;
  TColStd_Array1OfInteger myTriangles;
  TColStd_Array1OfInteger myAdjacents;
  Standard_Integer mytr;
  Standard_Integer myfirst;
  Standard_Integer mynode;
  Standard_Integer myothernode;
  Standard_Boolean mysense;
  Standard_Boolean mymore;

};

//=======================================================================
//function : Load
//purpose  :
//=======================================================================
inline void Poly_Connect_Ex::Load (const Handle(Poly_Triangulation)& theTriangulation)
{
  myTriangulation = theTriangulation;
  mytr = 0;
  myfirst = 0;
  mynode  = 0;
  myothernode = 0;
  mysense = false;
  mymore  = false;

  const Standard_Integer aNbNodes = myTriangulation->NbNodes();
  const Standard_Integer aNbTris  = myTriangulation->NbTriangles();
  {
    const Standard_Integer aNbAdjs = 6 * aNbTris;
    if (myTriangles.Size() != aNbNodes)
    {
      myTriangles.Resize (1, aNbNodes, Standard_False);
    }
    if (myAdjacents.Size() != aNbAdjs)
    {
      myAdjacents.Resize (1, aNbAdjs, Standard_False);
    }
  }

  myTriangles.Init(0);
  myAdjacents.Init(0);

  // We first build an array of the list of edges connected to the nodes
  // create an array to store the edges starting from the vertices
  NCollection_Array1<polyedge*> anEdges (1, aNbNodes);
  anEdges.Init (NULL);
  // use incremental allocator for small allocations
  Handle(NCollection_IncAllocator) anIncAlloc = new NCollection_IncAllocator();

  // loop on the triangles
  NCollection_Vec3<Standard_Integer> aTriNodes;
  NCollection_Vec2<Standard_Integer> anEdgeNodes;
  for (Standard_Integer aTriIter = 1; aTriIter <= aNbTris; ++aTriIter)
  {
    // get the nodes
    myTriangulation->Triangle (aTriIter).Get (aTriNodes[0], aTriNodes[1], aTriNodes[2]);

    // Update the myTriangles array
    myTriangles.SetValue (aTriNodes[0], aTriIter);
    myTriangles.SetValue (aTriNodes[1], aTriIter);
    myTriangles.SetValue (aTriNodes[2], aTriIter);

    // update the edge lists
    for (Standard_Integer aNodeInTri = 0; aNodeInTri < 3; ++aNodeInTri)
    {
      const Standard_Integer aNodeNext = (aNodeInTri + 1) % 3;  // the following node of the edge
      if (aTriNodes[aNodeInTri] < aTriNodes[aNodeNext])
      {
        anEdgeNodes[0] = aTriNodes[aNodeInTri];
        anEdgeNodes[1] = aTriNodes[aNodeNext];
      }
      else
      {
        anEdgeNodes[0] = aTriNodes[aNodeNext];
        anEdgeNodes[1] = aTriNodes[aNodeInTri];
      }

      // edge from node 0 to node 1 with node 0 < node 1
      // insert in the list of node 0
      polyedge* ced = anEdges[anEdgeNodes[0]];
      for (; ced != NULL; ced = ced->next)
      {
        // the edge already exists
        if (ced->nd == anEdgeNodes[1])
        {
          // just mark the adjacency if found
          ced->nt[1] = aTriIter;
          ced->nn[1] = aTriNodes[3 - aNodeInTri - aNodeNext];  // the third node
          break;
        }
      }

      if (ced == NULL)
      {
        // create the edge if not found
        ced = (polyedge* )anIncAlloc->Allocate (sizeof(polyedge));
        ced->next = anEdges[anEdgeNodes[0]];
        anEdges[anEdgeNodes[0]] = ced;
        ced->nd = anEdgeNodes[1];
        ced->nt[0] = aTriIter;
        ced->nn[0] = aTriNodes[3 - aNodeInTri - aNodeNext];  // the third node
        ced->nt[1] = 0;
        ced->nn[1] = 0;
      }
    }
  }

  // now complete the myAdjacents array
  Standard_Integer anAdjIndex = 1;
  for (Standard_Integer aTriIter = 1; aTriIter <= aNbTris; ++aTriIter)
  {
    // get the nodes
    myTriangulation->Triangle (aTriIter).Get (aTriNodes[0], aTriNodes[1], aTriNodes[2]);

    // for each edge in triangle
    for (Standard_Integer aNodeInTri = 0; aNodeInTri < 3; ++aNodeInTri)
    {
      const Standard_Integer aNodeNext = (aNodeInTri + 1) % 3;  // the following node of the edge
      if (aTriNodes[aNodeInTri] < aTriNodes[aNodeNext])
      {
        anEdgeNodes[0] = aTriNodes[aNodeInTri];
        anEdgeNodes[1] = aTriNodes[aNodeNext];
      }
      else
      {
        anEdgeNodes[0] = aTriNodes[aNodeNext];
        anEdgeNodes[1] = aTriNodes[aNodeInTri];
      }

      // edge from node 0 to node 1 with node 0 < node 1
      // find in the list of node 0
      const polyedge* ced = anEdges[anEdgeNodes[0]];
      while (ced->nd != anEdgeNodes[1])
      {
        ced = ced->next;
      }

      // Find the adjacent triangle
      const Standard_Integer l = ced->nt[0] == aTriIter ? 1 : 0;

      myAdjacents.SetValue (anAdjIndex,     ced->nt[l]);
      myAdjacents.SetValue (anAdjIndex + 3, ced->nn[l]);
      ++anAdjIndex;
    }
    anAdjIndex += 3;
  }

  // destroy the edges array - can be skipped when using NCollection_IncAllocator
  for (Standard_Integer aNodeIter = anEdges.Lower(); aNodeIter <= anEdges.Upper(); ++aNodeIter)
  {
    for (polyedge* anEdgeIter = anEdges[aNodeIter]; anEdgeIter != NULL;)
    {
      polyedge* aTmp = anEdgeIter->next;
      anIncAlloc->Free (anEdgeIter);
      anEdgeIter = aTmp;
    }
  }
}

//=======================================================================
//function : Initialize
//purpose  : 
//=======================================================================

inline void Poly_Connect_Ex::Initialize(const Standard_Integer N)
{
  mynode = N;
  myfirst = Triangle(N);
  mytr = myfirst;
  mysense = Standard_True;
  mymore = (myfirst != 0);
  if (mymore)
  {
    Standard_Integer i, no[3];
    //const Poly_Array1OfTriangle& triangles = myTriangulation->Triangles();
	const Poly_Array1OfTriangle& triangles = POLY_TRIGULATION_TRIANGLES(myTriangulation);
    triangles(myfirst).Get(no[0], no[1], no[2]);
    for (i = 0; i < 3; i++)
      if (no[i] == mynode) break;
    myothernode = no[(i+2)%3];
  }
}

//=======================================================================
//function : Next
//purpose  : 
//=======================================================================

inline void Poly_Connect_Ex::Next()
{
  Standard_Integer i, j;
  Standard_Integer n[3];
  Standard_Integer t[3];
  //const Poly_Array1OfTriangle& triangles = myTriangulation->Triangles();
  const Poly_Array1OfTriangle& triangles = POLY_TRIGULATION_TRIANGLES(myTriangulation);
  Triangles(mytr, t[0], t[1], t[2]);
  if (mysense) {
    for (i = 0; i < 3; i++) {
      if (t[i] != 0) {
	triangles(t[i]).Get(n[0], n[1], n[2]);
	for (j = 0; j < 3; j++) {
	  if ((n[j] == mynode) && (n[(j+1)%3] == myothernode)) {
	    mytr = t[i];
	    myothernode = n[(j+2)%3];
	    mymore = (mytr != myfirst);
	    return;
	  }
	}
      }
    }
    // sinon, depart vers la gauche.
    triangles(myfirst).Get(n[0], n[1], n[2]);
    for (i = 0; i < 3; i++)
      if (n[i] == mynode) break;
    myothernode = n[(i+1)%3];
    mysense = Standard_False;
    mytr = myfirst;
    Triangles(mytr, t[0], t[1], t[2]);
  }
  if (!mysense) {
    for (i = 0; i < 3; i++) {
      if (t[i] != 0) {
	triangles(t[i]).Get(n[0], n[1], n[2]);
	for (j = 0; j < 3; j++) {
	  if ((n[j] == mynode) && (n[(j+2)%3] == myothernode)) {
	    mytr = t[i];
	    myothernode = n[(j+1)%3];
	    mymore = Standard_True;
	    return;
	  }
	}
      }
    }
  }
  mymore = Standard_False;
}



#endif // _Poly_Connect_HeaderFile
