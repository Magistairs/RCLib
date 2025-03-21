#include "RCLibQt.h"
#include "FileSelectWidget.h"
#include "RCLib.h"
#include "DefaultWidgetsFactory.h"

#include <QLineEdit>
#include <QValidator>
#include <QFile>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

using namespace RCLib::Widgets;

class FileValidator : public QValidator
{
public:
	FileValidator(QWidget* pParent)
	  : QValidator(pParent)
	{
	}
	virtual State validate(QString& text, int& pos) const
	{
		QFile file(text);
		return (text.isEmpty() || file.exists()) ? State::Acceptable : State::Intermediate;
	}
};

FileSelectWidget::FileSelectWidget(QBoxLayout* pLayout, EType type)
  : DuoWidget(pLayout)
  , m_type(type)
{
	m_pPathLabel = new QLineEdit;
	GetLayout()->addWidget(m_pPathLabel);
	m_pPathLabel->setValidator(new FileValidator(this));
	connect(m_pPathLabel, &QLineEdit::editingFinished, [this] { fileChanged(m_pPathLabel->text()); });

	Impl::WidgetsFactoryImpl factory;

	auto* pBrowseButton = factory.CreateSmallButton();
	pBrowseButton->setText("Browse...");
	GetLayout()->addWidget(pBrowseButton);
	connect(pBrowseButton, &QPushButton::clicked, [this] {
		QString path = m_type == File ? QFileDialog::getOpenFileName() : QFileDialog::getExistingDirectory();
		SetFile(path);
	});

	auto* pGoToButton = factory.CreateSmallButton();
	pGoToButton->setText("Go To");
	GetLayout()->addWidget(pGoToButton);
	connect(pGoToButton, &QPushButton::clicked, [this] { QDesktopServices::openUrl(QUrl::fromLocalFile(m_pPathLabel->text())); });
	connect(this, &FileSelectWidget::fileChanged, [this, pGoToButton] { pGoToButton->setVisible(!m_pPathLabel->text().isEmpty()); });
	pGoToButton->setVisible(false);
}

void FileSelectWidget::SetFile(const QString& path)
{
	QFile file(path);
	if (path.isEmpty() || file.exists())
	{
		m_pPathLabel->setText(path);
		fileChanged(path);
	}
}
