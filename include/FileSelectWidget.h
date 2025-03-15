#pragma once
#include "DuoWidget.h"

class FileSelectWidget : public DuoWidget
{
	Q_OBJECT

public:
	enum EType
	{
		File,
		Directory
	};

	FileSelectWidget(QBoxLayout* pLayout, EType type);
	virtual ~FileSelectWidget() {}

	void SetFile(const QString& path);

signals:
	void fileChanged(QString path);

protected:
	QLineEdit* m_pPathLabel;
	EType      m_type;
};
