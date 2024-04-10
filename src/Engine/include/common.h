#ifndef COMMON
#define COMMON_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * These are the different usage scenarios of static, use them accordingly
 * to emphasize the declaration semantics.
 *
 * EXAMPLE
 * When defining global variables, instead of doing:
 * 	int my_global_variable;
 * do:
 *  _global int my_global_variable;
 *
 * Then, when somebody wants to see all the global variables, they just search
 * for "_global".
 */
#define _global static
#define _persist static
#define _internal static
#define _inline static inline

#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))

#define ASSERT(expr) do{if (!(expr)) { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ASSERTION FAILED: %s, at %u", __FILE__, __LINE__); exit(10); } }while(0)
/**
 * Use this to surpress "unreferenced formal parameter" warnings
 */
#define UNUSED( ParamName ) \
((void)(0 ? ((ParamName) = (ParamName)) : (ParamName)))

unsigned char *read_file_and_null_terminate(const char *path);
#endif