#pragma once
#include "QtIncludes.h"
#include "SmartPtr.h"

namespace RCLib {

class Variable : public SmartPtrTarget
{
public:
	Variable(const QString& name, const QVariant& value)
	  : m_name(name)
	  , m_value(value)
	{
	}
	Variable(const Variable& other)            = delete;
	Variable(Variable&& other)                 = delete;
	Variable& operator=(const Variable& other) = delete;
	Variable& operator=(Variable&& other)      = delete;

	const QString& GetName() const { return m_name; }
	QVariant       GetValue();
	void           SetValue(const QVariant& value);
	void           EmitValueChanged();

private:
	QString  m_name;
	QVariant m_value;
	QVariant m_cachedValue;

	mutable std::mutex m_dataMutex;
};

using VariablePtr = SmartPtr<Variable>;

} // namespace RCLib
