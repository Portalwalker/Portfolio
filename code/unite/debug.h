#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG

    #include "dep/onHim.h"
    #include "dep/principles.h"
    #include "dep/trix.h"

    #define DOES_FUNC_RETURN_VOID (__PRETTY_FUNCTION__[0] == 'v') // detects first character 'v' in "void function_signature(type param1, type param2)"

    #define debug_return(expr, msg)  if ( (expr) ) { write(STDERR, "[!] <---| ", 10); write(STDERR, __func__, sizeof(__func__) - 1); write(STDERR, " => ", 4); write(STDERR, msg, count_til_nul(msg)); write(STDERR, "\n", 1); if (DOES_FUNC_RETURN_VOID) { return; } else { return EVIL; } }
    #define debug_warn(expr, msg)    if ( (expr) ) { write(STDERR, "[!] @~~~> ", 10); write(STDERR, __func__, sizeof(__func__) - 1); write(STDERR, " => ", 4); write(STDERR, msg, count_til_nul((msg))); write(STDERR, "\n", 1); }

#else

    #define debug_return(expr, msg)
    #define debug_warn(expr, msg)

#endif // DEBUG



#endif // DEBUG_H
