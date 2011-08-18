/** @file
 includes macro definition.
*/

#ifndef PIPEY_MACRO_H
#define PIPEY_MACRO_H

/** SAFE_TRY(X); expends to try{(X);} catch(...) { };

 This macro is useful in a destructor that do not allow to throw exceptions.
*/

#define SAFE_TRY(X) try{(X);} catch(...) { }

#endif