#ifndef __CORE_H_INCLUDED__
#define __CORE_H_INCLUDED__

#include <cassert>

# define loginfo std::clog << "INFO: "
# define logwarn std::clog << "WARN: "
# define logerror std::cerr << "ERROR: "

# ifndef NDEBUG
#  define logdbg std::clog << "DEBUG: "
# else
#  define logdbg 0 && (*((std::ostream *)0))
# endif

# ifdef TRACE
#  define logtrace std::clog << "TRACE: "
# else
#  define logtrace 0 && (*((std::ostream *)0))
# endif

#endif
