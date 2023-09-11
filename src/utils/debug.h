#pragma once

#define DEBUG_MODE 1

#if DEBUG_MODE == 1
#define LOG(x) std::cout<<x<<std::endl;
#else
#define LOG(x)
#endif

