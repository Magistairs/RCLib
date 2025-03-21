#pragma once
#include <QSet>
#include <QString>
#include <QTextEdit>

#include <mutex>

namespace TestsQt
{
/**
 * @brief Widget for displaying logs with color coding and filtering
 */
class LogWidget : public QTextEdit
{
	Q_OBJECT

public:
	explicit LogWidget(QWidget* parent = nullptr);
	~LogWidget() override = default;

	void AppendLog(const QString& message, const QString& level);
	bool SaveToFile(const QString& filePath);
	void SetFilter(const QString& level, bool enabled);
	void ClearFilters();

private:
	void SetupColors();
	void AppendText(const QString& text, const QColor& color);
	bool ShouldShowLevel(const QString& level) const;

	std::mutex    m_mutex;
	QColor        m_debugColor;
	QColor        m_infoColor;
	QColor        m_warningColor;
	QColor        m_errorColor;
	QSet<QString> m_enabledLevels;
};

} // namespace TestsQt