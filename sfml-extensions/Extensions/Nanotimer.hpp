// Copyright (c) 2016, Matthew Bentley (mattreecebentley@gmail.com) www.plflib.org

// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.


#pragma once

#include "Extensions.hpp"

// ~Nanosecond-precision cross-platform (linux/bsd/mac/windows, C++03/C++11) simple timer class:

// Mac OSX implementation:
#if defined(__MACH__)
	#include <mach/clock.h>
	#include <mach/mach.h>

	namespace sf
	{

	class NanoTimer
	{
	private:
		clock_serv_t system_clock;
		mach_timespec_t time1, time2;
	public:
		NanoTimer()
		{
			host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &system_clock);
		}

		~NanoTimer()
		{
			mach_port_deallocate(mach_task_self(), system_clock);
		}

		inline void start()
		{
			clock_get_time(system_clock, &time1);
		}

		inline double getElapsedMs()
		{
			return static_cast<double>(getElapsedNs()) / 1000000.0;
		}

		inline double getElapsedUs()
		{
			return static_cast<double>(getElapsedNs()) / 1000.0;
		}

		double getElapsedNs()
		{
			clock_get_time(system_clock, &time2);
			return ((1000000000.0 * static_cast<double>(time2.tv_sec - time1.tv_sec)) + static_cast<double>(time2.tv_nsec - time1.tv_nsec));
		}
	};




// Linux/BSD implementation:
#elif (defined(linux) || defined(__linux__) || defined(__linux)) || (defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
	#include <time.h>
	#include <sys/time.h>

	namespace sf
	{

	class NanoTimer
	{
	private:
		struct timespec time1, time2;
	public:
		NanoTimer() {}

		inline void start()
		{
			clock_gettime(CLOCK_MONOTONIC, &time1);
		}

		inline double getElapsedMs()
		{
			return getElapsedNs() / 1000000.0;
		}

		inline double getElapsedUs()
		{
			return getElapsedNs() / 1000.0;
		}

		double getElapsedNs()
		{
			clock_gettime(CLOCK_MONOTONIC, &time2);
			return ((1000000000.0 * static_cast<double>(time2.tv_sec - time1.tv_sec)) + static_cast<double>(time2.tv_nsec - time1.tv_nsec));
		}
	};




// Windows implementation:
#elif defined(_WIN32)
	#if defined(_MSC_VER) && !defined(NOMINMAX)
		#define NOMINMAX // Otherwise MS compilers act like idiots when using std::numeric_limits<>::max() and including windows.h
	#endif

	namespace sf
	{

	class NanoTimer
	{
	private:
		LARGE_INTEGER ticks1, ticks2;
		double frequency;
	public:
		NanoTimer()
		{
            timeBeginPeriod ( );
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			frequency = static_cast<double>(freq.QuadPart);
		}
        ~NanoTimer ( ) {
            timeEndPeriod ( );
        }

		inline void start()
		{
			QueryPerformanceCounter(&ticks1);
		}

		double getElapsedMs()
		{
			QueryPerformanceCounter(&ticks2);
			return (static_cast<double>(ticks2.QuadPart - ticks1.QuadPart) * 1000.0) / frequency;
		}

		inline double getElapsedUs()
		{
			return getElapsedMs() * 1000.0;
		}

		inline double getElapsedNs()
		{
			return getElapsedMs() * 1000000.0;
		}
	};
#endif
// Else: failure warning - your OS is not supported



#if defined(__MACH__) || (defined(linux) || defined(__linux__) || defined(__linux)) || (defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(_WIN32)
inline void delayNanoseconds(double delay_ns)
{
	static NanoTimer timer;
	timer.start();

	while(timer.getElapsedNs() < delay_ns)
	{};
}



inline void delayMicroseconds (double delay_us)
{
    delayNanoseconds (delay_us * 1'000.0);
}


inline void delayMilliseconds (double delay_ms)
{
    delayNanoseconds (delay_ms * 1'000'000.0);
}


} // namespace
#endif
