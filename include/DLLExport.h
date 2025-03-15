#ifdef _WIN32
#ifdef RCLIB_EXPORT
#define RCLIB_API __declspec(dllexport)
#else
#define RCLIB_API __declspec(dllimport)
#endif
#else
#define RCLIB_API
#endif

#pragma warning(disable : 4251) // 4251 can't be avoided with STL types
