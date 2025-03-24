#pragma once
#include "DuoWidget.h"
#include "RCLibQt_Fwd.h"

#include <QLineEdit>
#include <string_view>

namespace RCLib::Qt
{

/**
 * @brief Widget for selecting files or directories with browse and go-to functionality
 * 
 * Provides a line edit with browse and go-to buttons for selecting files or directories.
 * The widget validates that the selected path exists and provides quick access to open
 * the selected path in the system's file explorer.
 */
class RCLIB_QT_API FileSelectWidget : public DuoWidget
{
	Q_OBJECT

public:
	enum EType
	{
		File,
		Directory
	};

	/**
	 * @brief Constructs a new FileSelectWidget
	 * @param pLayout The layout to use for arranging the widget elements
	 * @param type Whether this widget selects files or directories
	 */
	FileSelectWidget(QBoxLayout* pLayout, EType type);
	virtual ~FileSelectWidget() = default;

	/**
	 * @brief Sets the selected file/directory path
	 * @param path The path to set
	 */
	void SetFile(std::string_view path);

signals:
	void fileChanged(QString path);

protected:
	QLineEdit* m_pPathLabel;
	EType      m_type;
};

} // namespace RCLib::Qt
