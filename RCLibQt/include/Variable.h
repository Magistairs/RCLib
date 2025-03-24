#pragma once
#include "RCLibQt_Fwd.h"

#include <QString>
#include <QVariant>
#include <mutex>
#include <string_view>

namespace RCLib::Qt
{

/**
 * @brief Thread-safe variable wrapper for QVariant values
 * 
 * Provides a thread-safe way to store and access QVariant values with change notification support.
 * Values are cached to prevent blocking reads while maintaining thread safety.
 */
class RCLIB_QT_API Variable
{
public:
	/**
	 * @brief Constructs a new Variable with the given name and initial value
	 * @param name The name identifier for this variable
	 * @param value The initial value to store
	 */
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

	/**
	 * @brief Gets the name of this variable
	 */
	const QString& GetName() const { return m_name; }

	/**
	 * @brief Gets the current value, using a cached copy if a write is in progress
	 */
	QVariant GetValue();

	/**
	 * @brief Sets a new value and notifies of the change
	 */
	void SetValue(const QVariant& value);

	/**
	 * @brief Emits the value changed signal
	 */
	void EmitValueChanged();

private:
	QString  m_name;
	QVariant m_value;
	QVariant m_cachedValue;

	mutable std::mutex m_dataMutex;
};

} // namespace RCLib::Qt
