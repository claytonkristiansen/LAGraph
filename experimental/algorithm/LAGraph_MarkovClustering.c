//------------------------------------------------------------------------------
// LAGraph_MarkovClustering: 
//------------------------------------------------------------------------------

// LAGraph, (c) 2021 by The LAGraph Contributors, All Rights Reserved.
// SPDX-License-Identifier: BSD-2-Clause
// See additional acknowledgments in the LICENSE file,
// or contact permission@sei.cmu.edu for the full terms.

// Contributed by Clayton W. Kristiansen, Texas A&M University

//------------------------------------------------------------------------------

// This is a bare-bones "algorithm" that does nearly nothing all.  It simply
// illustrates how a new algorithm can be added to the experimental/algorithm
// folder.  All it does is make a copy of the G->A matrix and return it as
// the new matrix Y.  Inside, it allocates some worspace as well (the matrix W,
// which is not used).  To illustrate the use of the error msg string, it
// returns an error if the graph not directed.

// The GRB_TRY and LG_TRY macros use the LG_FREE_ALL macro to free all
// workspace and all output variables if an error occurs.  To use these macros,
// you must define the variables before using them, or before using GRB_TRY
// or LG_TRY.  The LG_TRY macro is defined in src/utility/LG_internal.h.

// To create your own algorithm, create a copy of this file, rename it
// to LAGraph_whatever.c, and use it as a template for your own algorithm.
// Then place the prototype in include/LAGraphX.h.

// See experimental/test/test_HelloWorld.c for a test for this method, and
// experimental/benchmark/helloworld_demo.c and helloworld2_demo.c for two
// methods that benchmark the performance of this algorithm.

#define LG_FREE_WORK                        \
{                                           \
    /* free any workspace used here */      \
    GrB_free (&W) ;                         \
}

#define LG_FREE_ALL                         \
{                                           \
    /* free any workspace used here */      \
    LG_FREE_WORK ;                          \
    /* free all the output variable(s) */   \
    GrB_free (&Y) ;                         \
    /* take any other corrective action */  \
}

#include "LG_internal.h"
#include "LAGraphX.h"

int LAGraph_MarkovClustering
(
    // output
    GrB_Matrix *Yhandle,    // Y, created on output
    // input: not modified
    LAGraph_Graph G,
    char *msg,
    unsigned int k,
    unsigned int r
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    GrB_Matrix W = NULL, Y = NULL ;     // declare workspace and output(s)
    LG_CLEAR_MSG ;                      // clears the msg string, if not NULL

    // the caller must pass in a non-NULL &Y on input
    LG_ASSERT (Yhandle != NULL, GrB_NULL_POINTER) ;
    (*Yhandle) = NULL ;

    // basic checks of the input graph
    LG_TRY (LAGraph_CheckGraph (G, msg)) ;


    //All operations here are on the adjacency matrix:
    //MCL Algorithm:

    //create Graph P as copy of graph G
    //normalize P using GrB_reduce, then make diag, then take inverse using select and minv binary operator. Then mult
    //loop while not in steady state
        //Graph Q is created from the expansion of graph P (take matrix to power of k)
        //Q is inflated by factor r (take each entry to power of r, then renormalize matrix)
        //Trim small probability entries GrB_select
        //Compare Q to P to check for steady state
        //If in steady state:
            //return results
        //Else :
            //Set P = Q and start over


    // the graph must be directed (a useless test, just to illustrate
    // the use of the LG_ASSERT_MSG macro)
    LG_ASSERT_MSG (G->kind == LAGraph_ADJACENCY_DIRECTED,
        GrB_INVALID_VALUE, "LAGraph_HelloWorld requires a directed graph") ;

    //--------------------------------------------------------------------------
    // allocate workspace and create the output matrix Y
    //--------------------------------------------------------------------------

    GRB_TRY (GrB_Matrix_new (&W, GrB_FP32, 5, 5)) ;     // useless workspace
    GRB_TRY (GrB_Matrix_dup (&Y, G->A)) ;

    //--------------------------------------------------------------------------
    // free workspace and return result
    //--------------------------------------------------------------------------

    LG_FREE_WORK ;
    (*Yhandle) = Y ;
    return (GrB_SUCCESS) ;
}

