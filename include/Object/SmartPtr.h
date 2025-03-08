#pragma once
#include "StdIncludes.h"

template<class T>
class SmartPtr
{
public:
	SmartPtr(T* p = nullptr)
	  : m_p(p)
	{
		if (m_p)
			m_p->AddRef();
	}

	~SmartPtr()
	{
		if (m_p)
			m_p->Release();
	}

	SmartPtr(const SmartPtr& sp)
	  : m_p(sp.m_p)
	{
		if (m_p)
			m_p->AddRef();
	}

	SmartPtr(SmartPtr&& sp)
	  : m_p(sp.m_p)
	{
		sp.m_p = nullptr;
	}

	SmartPtr& operator=(const SmartPtr& sp)
	{
		if (m_p != sp.m_p)
		{
			if (m_p)
				m_p->Release();
			m_p = sp.m_p;
			if (m_p)
				m_p->AddRef();
		}
		return *this;
	}

	SmartPtr& operator=(SmartPtr&& sp)
	{
		if (m_p != sp.m_p)
		{
			if (m_p)
				m_p->Release();
			m_p    = sp.m_p;
			sp.m_p = nullptr;
		}
		return *this;
	}
	T* operator->() const { return m_p; }
	T& operator*() const { return *m_p; }
	operator bool() const { return m_p != nullptr; }

private:
	T* m_p{nullptr};
};

class SmartPtrTarget
{
public:
	virtual ~SmartPtrTarget() = default;
	void AddRef() { m_nRefCount++; }
	void Release()
	{
		if (--m_nRefCount == 0)
			delete this;
	}

private:
	std::atomic_int m_nRefCount{0};
};