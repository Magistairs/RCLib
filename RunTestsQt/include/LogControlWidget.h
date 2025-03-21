#pragma once
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

namespace TestsQt
{

class LogControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LogControlWidget(QWidget* parent = nullptr);

signals:
	void SaveLogRequested(const QString& filePath);
	void ClearLogRequested();

private slots:
	void OnLevelChanged(int index);
	void OnConsoleOutputToggled(bool checked);
	void OnFileOutputToggled(bool checked);
	void OnSaveLog();
	void OnClearLog();

private:
	void SetupUI();
	void ConnectSignals();

	QLabel*      m_levelLabel{nullptr};
	QComboBox*   m_levelCombo{nullptr};
	QCheckBox*   m_consoleOutput{nullptr};
	QCheckBox*   m_fileOutput{nullptr};
	QPushButton* m_saveButton{nullptr};
	QPushButton* m_clearButton{nullptr};
};
} // namespace TestsQt