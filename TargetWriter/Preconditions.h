#ifndef __visualspreadsheet_preconditions__h_
#define __visualspreadsheet_preconditions__h_

#include <stdexcept>

/*****************************************************************************
 Macros that should be used for testing function preconditions.

 @author Shane Kirk
 *****************************************************************************/
#ifdef _DEBUG
#define precondition_throw(X,Y) { if (!(X)) { _ASSERT(X); throw std::runtime_error(Y); }}
#define precondition_ret(X,Y) { if (!(X)) { _ASSERT(X); return Y; }}
#else
#define precondition_throw(X,Y) { if (!(X)) { throw std::runtime_error(Y); }}
#define precondition_ret(X,Y) { if (!(X)) { return Y; }}
#endif

#endif