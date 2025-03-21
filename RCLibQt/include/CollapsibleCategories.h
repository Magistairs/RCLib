#pragma once
#include "RCLibQt_Fwd.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QString>
#include <QObject>
#include <QEvent>

namespace RCLib {
namespace Qt {

class CollapsibleCategories : public QTreeWidget
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
