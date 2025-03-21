#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "LogWidget.h"
#include "LogControlWidget.h"

namespace TestsQt
{
/**
 * @brief Main window of the test application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

    LogWidget* GetLogWidget() { return m_logWidget; }

private slots:
    void OnClearLog();
    void OnTestLog();
    void OnSaveLog(const QString& filePath);

private:
    void SetupUI();
    void ConnectSignals();

    QWidget* m_centralWidget{nullptr};
    LogWidget* m_logWidget{nullptr};
    LogControlWidget* m_controlWidget{nullptr};
    QPushButton* m_testButton{nullptr};
};

} // namespace TestsQt 