#pragma once
#include "RCLibQt_Fwd.h"

#include <QVariant>
#include <QString>
#include <mutex>
#include <string_view>

namespace RCLib {
namespace Qt {

class Variable
{
public:
	Variable(std::string_view name, const QVariant& value)
	  : m_name(QString::fromStdString(std::string(name)))
	  , m_value(value)
	{
	}
	virtual ~Variable() = default;

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

} // namespace Qt
} // namespace RCLib
