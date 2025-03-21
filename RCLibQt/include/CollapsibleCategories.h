#pragma once
#include "RCLibQt_Fwd.h"

#include <QEvent>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace RCLib
{
namespace Qt
{

class RCLIB_QT_API CollapsibleCategories : public QTreeWidget
{
public:
	CollapsibleCategories(QWidget* parent = nullptr);
	virtual ~CollapsibleCategories() {}

	void AddCategory(const QString& category);
	void AddItem(const QString& category, const QString& item);
	void Clear();

	virtual bool eventFilter(QObject* object, QEvent* event) override;

private:
	QMap<QString, QTreeWidgetItem*> m_categories;
};

} // namespace Qt
} // namespace RCLib
