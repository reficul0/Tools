#ifndef PCH_H
#define PCH_H

#define NOMINMAX
#ifdef _WIN32
	// Исключаем редко используемые компоненты из заголовков Windows
	#define WIN32_LEAN_AND_MEAN     
	#include <windows.h>  
#endif // _WIN32

#include <queue>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#endif //PCH_H
