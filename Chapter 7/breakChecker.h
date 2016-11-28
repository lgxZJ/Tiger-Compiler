#ifndef BREAK_CHECKER_H
#define BREAK_CHECKER_H

///
/// This module actually do semantic analysis of break expressions:
///     Break can only appears inside loops, other places are illegal.
/// 

#include <stdbool.h>

///
/// \brief  record status when entering a loop scope.
/// \remark this function are usually called with leaveLoop().
///
void enterLoop(void);

///
/// \brief  cancel former status recorded by enterLoop() call.
/// \remark this function are usually called with enterLoop().
///
void leaveLoop(void);

///
/// \brief  check whether this break is inside a loop scope.
///
bool isBreakInsideLoop(void);

///
/// \brief  reset checker.
///
void resetChecker(void);

#endif