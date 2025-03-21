#include "LogWidget.h"

#include <QDateTime>
#include <QFile>
#include <QTextStream>

namespace TestsQt
{

LogWidget::LogWidget(QWidget* parent)
  : QTextEdit(parent)
{
	setReadOnly(true);
	SetupColors();

	// Enable all levels by default
	m_enabledLevels = {"DEBUG", "INFO", "WARNING", "ERROR"};
}

void LogWidget::SetupColors()
{
	m_debugColor   = QColor(128, 128, 128); // Gray
	m_infoColor    = QColor(0, 0, 0);       // Black
	m_warningColor = QColor(255, 165, 0);   // Orange
	m_errorColor   = QColor(255, 0, 0);     // Red
}

void LogWidget::AppendLog(const QString& message, const QString& level)
{
	if (!ShouldShowLevel(level))
	{
		return;
	}

	std::lock_guard<std::mutex> lock(m_mutex);

	QString timestamp   = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	QString fullMessage = QString("[%1][%2] %3").arg(timestamp, level, message);

	QColor color;
	if (level == "DEBUG")
		color = m_debugColor;
	else if (level == "INFO")
		color = m_infoColor;
	else if (level == "WARNING")
		color = m_warningColor;
	else if (level == "ERROR")
		color = m_errorColor;

	AppendText(fullMessage + "\n", color);
}

bool LogWidget::SaveToFile(const QString& filePath)
{
	QFile file(filePath);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << toPlainText();
		file.close();
		return true;
	}
	return false;
}

void LogWidget::SetFilter(const QString& level, bool enabled)
{
	if (enabled)
	{
		m_enabledLevels.insert(level.toUpper());
	}
	else
	{
		m_enabledLevels.remove(level.toUpper());
	}
}

void LogWidget::ClearFilters()
{
	m_enabledLevels.clear();
	m_enabledLevels = {"DEBUG", "INFO", "WARNING", "ERROR"};
}

bool LogWidget::ShouldShowLevel(const QString& level) const
{
	return m_enabledLevels.contains(level.toUpper());
}

void LogWidget::AppendText(const QString& text, const QColor& color)
{
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::End);

	QTextCharFormat format;
	format.setForeground(color);
	cursor.insertText(text, format);

	setTextCursor(cursor);
	ensureCursorVisible();
}

} // namespace TestsQt