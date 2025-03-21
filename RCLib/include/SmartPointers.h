#pragma once
#include <memory>

namespace RCLib
{
#ifdef USE_RCLIB_SMART_PTR
template<typename T>
class RCLIB_API SmartPtr
{
public:
	explicit SmartPtr(T* ptr = nullptr)
	  : m_ptr(ptr)
	{
		if (m_ptr)
		{
			m_refCount = new std::atomic<size_t>(1);
			RCLIB_TRACK_ALLOC(m_ptr, sizeof(T));
		}
	}

	SmartPtr(const SmartPtr& other)
	  : m_ptr(other.m_ptr)
	  , m_refCount(other.m_refCount)
	{
		if (m_refCount)
		{
			++(*m_refCount);
		}
	}

	SmartPtr(SmartPtr&& other) noexcept
	  : m_ptr(other.m_ptr)
	  , m_refCount(other.m_refCount)
	{
		other.m_ptr      = nullptr;
		other.m_refCount = nullptr;
	}

	SmartPtr& operator=(const SmartPtr& other)
	{
		if (this != &other)
		{
			release();
			m_ptr      = other.m_ptr;
			m_refCount = other.m_refCount;
			if (m_refCount)
			{
				++(*m_refCount);
			}
		}
		return *this;
	}

	SmartPtr& operator=(SmartPtr&& other) noexcept
	{
		if (this != &other)
		{
			release();
			m_ptr            = other.m_ptr;
			m_refCount       = other.m_refCount;
			other.m_ptr      = nullptr;
			other.m_refCount = nullptr;
		}
		return *this;
	}

	~SmartPtr() { release(); }

	T* operator->() const { return m_ptr; }
	T& operator*() const { return *m_ptr; }
	T* get() const { return m_ptr; }
	operator bool() const { return m_ptr != nullptr; }
	size_t use_count() const { return m_refCount ? m_refCount->load() : 0; }

	template<typename U>
	SmartPtr<U> static_pointer_cast() const
	{
		U*          ptr = static_cast<U*>(m_ptr);
		SmartPtr<U> result;
		result.m_ptr      = ptr;
		result.m_refCount = m_refCount;
		if (m_refCount)
		{
			++(*m_refCount);
		}
		return result;
	}

	template<typename U>
	SmartPtr<U> dynamic_pointer_cast() const
	{
		U* ptr = dynamic_cast<U*>(m_ptr);
		if (!ptr)
			return SmartPtr<U>();

		SmartPtr<U> result;
		result.m_ptr      = ptr;
		result.m_refCount = m_refCount;
		if (m_refCount)
		{
			++(*m_refCount);
		}
		return result;
	}

private:
	void release()
	{
		if (m_refCount)
		{
			if (--(*m_refCount) == 0)
			{
				RCLIB_TRACK_DEALLOC(m_ptr);
				delete m_ptr;
				delete m_refCount;
			}
			m_ptr      = nullptr;
			m_refCount = nullptr;
		}
	}

	T*                   m_ptr{nullptr};
	std::atomic<size_t>* m_refCount{nullptr};
};

template<typename T, typename... Args>
SmartPtr<T> make_smart(Args&&... args)
{
	return SmartPtr<T>(new T(std::forward<Args>(args)...));
}

#define SharedPtr  SmartPtr
#define MakeShared make_smart
#else
template<typename T>
using SmartPtr = std::shared_ptr<T>;

template<typename T, typename... Args>
SmartPtr<T> make_smart(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#define SharedPtr  std::shared_ptr
#define MakeShared std::make_shared
#endif

} // namespace RCLib

/**
 * @brief Macro to declare a smart pointer type for a class
 * @param ClassName The name of the class to create a smart pointer for
 */
#define RCLIB_DECLARE_PTR(ClassName) \
	class RCLIB_API ClassName;         \
	using ClassName##Ptr = RCLib::SmartPtr<ClassName>