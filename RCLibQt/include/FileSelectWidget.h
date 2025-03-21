#pragma once
#include "DuoWidget.h"
#include "RCLibQt_Fwd.h"

#include <QLineEdit>

namespace RCLib
{
namespace Qt
{

class RCLIB_QT_API FileSelectWidget : public DuoWidget
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

} // namespace Qt
} // namespace RCLib
