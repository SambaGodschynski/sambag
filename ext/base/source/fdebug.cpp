//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
//
// Category    : Helpers
// Filename    : base/source/fdebug.cpp
// Created by  : Steinberg, 1995
// Description : There are 2 levels of debugging messages:
//	             DEVELOPMENT               During development
//	             RELEASE                   Program is shipping.
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2010, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety  
// without prior written agreement by Steinberg Media Technologies GmbH. 
// This SDK must not be used to re-engineer or manipulate any technology used  
// in any Steinberg or Third-party application or software module, 
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this 
// software without specific prior written permission.
// 
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "base/source/fdebug.h"

#if DEVELOPMENT

#include <cstdio>
#include <cstdarg>

#if WINDOWS
#define _WIN32_WINNT 0x0400 
#include <windows.h>
#define vsnprintf _vsnprintf

#elif MAC
#include <mach/mach_time.h>
#include <mach/mach_init.h>
#include <errno.h>
#include <new>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <signal.h>

static bool AmIBeingDebugged (void);

#define THREAD_ALLOC_WATCH 0		// check allocations on specific threads

	#if THREAD_ALLOC_WATCH
		mach_port_t watchThreadID = 0;
	#endif

#endif

//--------------------------------------------------------------------------
static const int kPrintfBufferSize = 10000;	
static bool neverDebugger = false;	// so I can switch it off in the debugger...

//--------------------------------------------------------------------------
static void printDebugString (const char* string) 
{
	if (!string)
		return;
			
	#if MAC
	fprintf (stderr, "%s", string);
	
	#elif WINDOWS
	OutputDebugStringA (string);
	#endif
}


//--------------------------------------------------------------------------
//	printf style debugging output
//--------------------------------------------------------------------------
void FDebugPrint (const char *format, ...)
{
	char string[kPrintfBufferSize];
	va_list marker;
	va_start (marker, format);
	vsnprintf (string, kPrintfBufferSize, format, marker);
	
	printDebugString (string);
}

//--------------------------------------------------------------------------
//	printf style debugging output
//--------------------------------------------------------------------------
void FDebugBreak (const char *format, ...)
{
	char string[kPrintfBufferSize];
	va_list marker;
	va_start (marker, format);
	vsnprintf (string, kPrintfBufferSize, format, marker);

	printDebugString (string);

	if (neverDebugger)
		return;

#if WINDOWS	
	if (IsDebuggerPresent ()) // do not crash if no debugger present
		DebugBreak ();
#endif

#if MAC
	if (AmIBeingDebugged ())
	{
		#if __ppc64__ || __ppc__
		kill (getpid (), SIGINT);
		#elif __i386__ || __x86_64__
		{ __asm__ volatile ("int3"); }
		#endif
	}
#endif
}

//--------------------------------------------------------------------------
void FPrintLastError (const char* file, int line)
{
	#if WINDOWS
	LPVOID lpMessageBuffer; 
	FormatMessageA (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMessageBuffer, 0, NULL);
	FDebugPrint ("%s(%d) : %s\n", file, line, lpMessageBuffer);
	LocalFree (lpMessageBuffer);
	#endif

	#if MAC
		#if !__MACH__
		extern int errno;
		#endif
	FDebugPrint ("%s(%d) : Errno %d\n", file, line, errno);	
	#endif
}




#if MAC

//------------------------------------------------------------------------
void* operator new (size_t size, int, const char *file, int line)
{
	#if THREAD_ALLOC_WATCH
	mach_port_t threadID = mach_thread_self ();
	if (watchThreadID == threadID)
	{
		FDebugPrint ("Watched Thread Allocation : %s (Line:%d)\n", file ? file : "Unknown", line);
	}
	#endif
	try {
	return ::operator new (size);
	} catch (std::bad_alloc exception) { FDebugPrint ("bad_alloc exception : %s (Line:%d)", file ? file : "Unknown", line); }
	return (void*)-1;
}

//------------------------------------------------------------------------
void* operator new [](size_t size, int, const char *file, int line)
{
	#if THREAD_ALLOC_WATCH
	mach_port_t threadID = mach_thread_self ();
	if (watchThreadID == threadID)
	{
		FDebugPrint ("Watched Thread Allocation : %s (Line:%d)\n", file ? file : "Unknown", line);
	}
	#endif
	try {
	return ::operator new [](size);
	} catch (std::bad_alloc exception) { FDebugPrint ("bad_alloc exception : %s (Line:%d)", file ? file : "Unknown", line);}
	return (void*)-1;
}

//------------------------------------------------------------------------
void operator delete(void* p, int, const char *file, int line)
{
    ::operator delete (p);
}

//------------------------------------------------------------------------
void operator delete[](void* p, int, const char *file, int line)
{
    ::operator delete[] (p);
}

//------------------------------------------------------------------------
// from Technical Q&A QA1361 (http://developer.apple.com/qa/qa2004/qa1361.html)
//------------------------------------------------------------------------
bool AmIBeingDebugged(void)
    // Returns true if the current process is being debugged (either
    // running under the debugger or has a debugger attached post facto).
{
    int                 junk;
    int                 mib[4];
    struct kinfo_proc   info;
    size_t              size;

    // Initialize the flags so that, if sysctl fails for some bizarre
    // reason, we get a predictable result.

    info.kp_proc.p_flag = 0;

    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    // Call sysctl.

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);

    // We're being debugged if the P_TRACED flag is set.

    return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
}

#endif // MAC

#endif // DEVELOPMENT

