#include "Object/Variable.h"

QVariant Variable::GetValue()
{
	if (m_dataMutex.try_lock())
	{
		m_cachedValue = m_value;
		m_dataMutex.unlock();
	}
	return m_cachedValue;
}

void Variable::SetValue(const QVariant& value)
{
	m_dataMutex.lock();
	m_value = value;
	m_dataMutex.unlock();

	EmitValueChanged();
}

void Variable::EmitValueChanged() {}
