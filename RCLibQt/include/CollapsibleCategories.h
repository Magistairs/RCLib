#pragma once
#include "RCLibQt_Fwd.h"

#include <QEvent>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace RCLib::Qt
{

/**
 * @brief A widget that displays collapsible categories with items.
 * 
 * This class provides a tree widget with special handling for categories
 * that can be expanded and collapsed. Each category can contain multiple
 * items, and the widget provides a clean interface for managing them.
 * 
 * Features:
 * - Categories are displayed as top-level items
 * - Items can be added to any category
 * - Categories can be expanded/collapsed with a single click
 * - Custom arrow delegate for better visual appearance
 */
class RCLIB_QT_API CollapsibleCategories : public QTreeWidget
{
public:
	/**
	 * @brief Constructs a CollapsibleCategories widget.
	 * @param parent The parent widget.
	 */
	explicit CollapsibleCategories(QWidget* parent = nullptr);
	virtual ~CollapsibleCategories() = default;

	/**
	 * @brief Adds a new category to the widget.
	 * @param category The name of the category to add.
	 * 
	 * If the category already exists, this method does nothing.
	 */
	void AddCategory(const QString& category);

	/**
	 * @brief Adds an item to a specific category.
	 * @param category The category to add the item to.
	 * @param item The text of the item to add.
	 * 
	 * If the specified category doesn't exist, the item is not added.
	 */
	void AddItem(const QString& category, const QString& item);

	/**
	 * @brief Removes all categories and items from the widget.
	 */
	void Clear();

	/**
	 * @brief Event filter for handling layout changes.
	 * 
	 * This override ensures proper layout updates when content widgets
	 * change their size.
	 */
	virtual bool eventFilter(QObject* object, QEvent* event) override;

private:
	QMap<QString, QTreeWidgetItem*> m_categories; ///< Maps category names to their tree items
};

} // namespace RCLib::Qt
