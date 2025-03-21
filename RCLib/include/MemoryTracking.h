#pragma once

namespace RCLib
{

#ifdef _DEBUG
#define RCLIB_TRACK_ALLOC(ptr, size) IEngine::Get().GetMemoryManager()->TrackAllocation(ptr, size, __FILE__, __LINE__)

#define RCLIB_TRACK_DEALLOC(ptr) IEngine::Get().GetMemoryManager()->TrackDeallocation(ptr)

#define RCLIB_NEW(type, ...)              \
	[&]() {                                 \
		auto* ptr = new type(__VA_ARGS__);    \
		RCLIB_TRACK_ALLOC(ptr, sizeof(type)); \
		return ptr;                           \
	}()

#define RCLIB_DELETE(ptr)       \
	[&]() {                       \
		if (ptr)                    \
		{                           \
			RCLIB_TRACK_DEALLOC(ptr); \
			delete ptr;               \
			ptr = nullptr;            \
		}                           \
	}()

#define RCLIB_NEW_ARRAY(type, count)              \
	[&]() {                                         \
		auto* ptr = new type[count];                  \
		RCLIB_TRACK_ALLOC(ptr, sizeof(type) * count); \
		return ptr;                                   \
	}()

#define RCLIB_DELETE_ARRAY(ptr) \
	[&]() {                       \
		if (ptr)                    \
		{                           \
			RCLIB_TRACK_DEALLOC(ptr); \
			delete[] ptr;             \
			ptr = nullptr;            \
		}                           \
	}()
#else
#define RCLIB_TRACK_ALLOC(ptr, size) ((void)0)
#define RCLIB_TRACK_DEALLOC(ptr)     ((void)0)
#define RCLIB_NEW(type, ...)         new type(__VA_ARGS__)
#define RCLIB_DELETE(ptr)            delete ptr
#define RCLIB_NEW_ARRAY(type, count) new type[count]
#define RCLIB_DELETE_ARRAY(ptr)      delete[] ptr
#endif

//// Helper class for automatic memory tracking
// template<typename T>
// class MemoryTracker
//{
// public:
//	static void* operator new(size_t size)
//	{
//		void* ptr = ::operator new(size);
//		RCLIB_TRACK_ALLOC(ptr, size);
//		return ptr;
//	}
//
//	static void operator delete(void* ptr)
//	{
//		if (ptr)
//		{
//			RCLIB_TRACK_DEALLOC(ptr);
//			::operator delete(ptr);
//		}
//	}
//
//	static void* operator new[](size_t size)
//	{
//		void* ptr = ::operator new[](size);
//		RCLIB_TRACK_ALLOC(ptr, size);
//		return ptr;
//	}
//
//	static void operator delete[](void* ptr)
//	{
//		if (ptr)
//		{
//			RCLIB_TRACK_DEALLOC(ptr);
//			::operator delete[](ptr);
//		}
//	}
// };

} // namespace RCLib
