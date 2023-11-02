/**
 * Config.h
 * Author: Johannes Unger
 *
 */

#ifndef SAMBAG_CONFIG_H
#define SAMBAG_CONFIG_H

#if defined _LP64 || defined _WIN64
 #define SAMBAG_64
#else
 #define SAMBAG_32
#endif

#define SAMBAG_VERSION_BUILD "504660d"


#endif // SAMBAG_CONFIG_H

