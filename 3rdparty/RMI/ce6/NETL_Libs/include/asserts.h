
/*
 *    File: include/asserts.h
 *  Author: Eric DeVolder
 * Purpose: Mechanism for run-time ASSERT and WARN
 *   Notes:
 *
 */

#ifndef ASSERTS_H
#define ASSERTS_H

//////////////////////////////////////////////////////////////////////

void do_assert (int errcode);
#ifndef DOASSERT
#define DOASSERT(X) do_assert(X)
#endif

#ifndef ASSERTFAIL
#define ASSERTFAIL(EXPR,...) { ASSERTLOG(">" #EXPR " in %s() %s:%d ", __FUNCTION__, __FILE__, __LINE__); ASSERTLOG(">" __VA_ARGS__); }
#endif

#define ASSERTEQ(X,Y,ERRCODE,ACTION,...) if ((X) == (Y)) { ASSERTFAIL(ASSERTEQ, __VA_ARGS__); DOASSERT(ERRCODE); ACTION; }
#define ASSERTNE(X,Y,ERRCODE,ACTION,...) if ((X) != (Y)) { ASSERTFAIL(ASSERTNE, __VA_ARGS__); DOASSERT(ERRCODE); ACTION; }
#define ASSERTLT(X,Y,ERRCODE,ACTION,...) if ((X)  < (Y)) { ASSERTFAIL(ASSERTLT, __VA_ARGS__); DOASSERT(ERRCODE); ACTION; }
#define ASSERTLE(X,Y,ERRCODE,ACTION,...) if ((X) <= (Y)) { ASSERTFAIL(ASSERTLE, __VA_ARGS__); DOASSERT(ERRCODE); ACTION; }
#define ASSERTGT(X,Y,ERRCODE,ACTION,...) if ((X)  > (Y)) { ASSERTFAIL(ASSERTGT, __VA_ARGS__); DOASSERT(ERRCODE); ACTION; }
#define ASSERTGE(X,Y,ERRCODE,ACTION,...) if ((X) >= (Y)) { ASSERTFAIL(ASSERTGE, __VA_ARGS__); DOASSERT(ERRCODE); ACTION; }

#define WARNEQ(X,Y,...) if ((X) == (Y)) { ASSERTFAIL(WARNEQ, __VA_ARGS__); }
#define WARNNE(X,Y,...) if ((X) != (Y)) { ASSERTFAIL(WARNNE, __VA_ARGS__); }
#define WARNLT(X,Y,...) if ((X)  < (Y)) { ASSERTFAIL(WARNLT, __VA_ARGS__); }
#define WARNLE(X,Y,...) if ((X) <= (Y)) { ASSERTFAIL(WARNLE, __VA_ARGS__); }
#define WARNGT(X,Y,...) if ((X)  > (Y)) { ASSERTFAIL(WARNGT, __VA_ARGS__); }
#define WARNGE(X,Y,...) if ((X) >= (Y)) { ASSERTFAIL(WARNGE, __VA_ARGS__); }

//////////////////////////////////////////////////////////////////////

#endif // ASSERTS_H

