#include "FileSelectWidget.h"
#include "DefaultWidgetsFactory.h"
#include "RCLibQt.h"

#include <QBoxLayout>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QUrl>
#include <QValidator>

namespace RCLib::Qt
{

/**
 * @brief Validator that checks if a file path exists
 * 
 * Used to provide real-time validation feedback in the file path line edit.
 * Returns Acceptable if the path is empty or exists, Intermediate otherwise.
 */
class FileValidator : public QValidator
{
public:
	explicit FileValidator(QWidget* pParent)
	  : QValidator(pParent)
	{
	}

	State validate(QString& text, int& pos) const override
	{
		QFile file(text);
		return (text.isEmpty() || file.exists()) ? State::Acceptable : State::Intermediate;
	}
};

FileSelectWidget::FileSelectWidget(QBoxLayout* pLayout, EType type)
  : DuoWidget(pLayout)
  , m_type(type)
{
	// Create and configure path input
	m_pPathLabel = new QLineEdit;
	GetLayout()->addWidget(m_pPathLabel);
	m_pPathLabel->setValidator(new FileValidator(this));
	connect(m_pPathLabel, &QLineEdit::editingFinished, [this] { fileChanged(m_pPathLabel->text()); });

	RCLib::Qt::Impl::DefaultWidgetsFactory factory;

	// Create browse button
	auto* pBrowseButton = factory.CreateSmallButton();
	pBrowseButton->setText("Browse...");
	GetLayout()->addWidget(pBrowseButton);
	connect(pBrowseButton, &QPushButton::clicked, [this] {
		QString path = m_type == File ? QFileDialog::getOpenFileName() : QFileDialog::getExistingDirectory();
		SetFile(path.toStdString());
	});

	// Create go-to button
	auto* pGoToButton = factory.CreateSmallButton();
	pGoToButton->setText("Go To");
	GetLayout()->addWidget(pGoToButton);
	connect(pGoToButton, &QPushButton::clicked, [this] { QDesktopServices::openUrl(QUrl::fromLocalFile(m_pPathLabel->text())); });
	connect(this, &FileSelectWidget::fileChanged, [this, pGoToButton] { pGoToButton->setVisible(!m_pPathLabel->text().isEmpty()); });
	pGoToButton->setVisible(false);
}

void FileSelectWidget::SetFile(std::string_view path)
{
	QString qpath = QString::fromStdString(std::string(path));
	QFile file(qpath);
	if (qpath.isEmpty() || file.exists())
	{
		m_pPathLabel->setText(qpath);
		fileChanged(qpath);
	}
}
} // namespace RCLib::Qt