#include "PanesLayout.h"

using namespace RCLib;

#include <PDXModdingAssistant.h>
#include <Widgets/Pane.h>

PanesLayout::PanesLayout(const char* name, QObject* pParent)
  : QObject(pParent)
{
	setObjectName(name);
}

PanesLayout::~PanesLayout() {}

void PanesLayout::AddPane(Pane* pPane)
{
	m_panes.push_back(pPane);
	pPane->installEventFilter(this);
}

void PanesLayout::CreateDefaultLayout()
{
	const QRect screenGeometry = PDXModdingAssistant::g_pMainWnd->screen()->geometry();
	const int   screenWidth    = screenGeometry.width();
	const int   screenHeight   = screenGeometry.height();

	const int rowsCount    = m_panes.size() > 4 ? 2 : 1;
	const int columnsCount = m_panes.size() / rowsCount + 1;
	const int columnWidth  = screenWidth / columnsCount;
	const int rowHeight    = screenHeight / rowsCount;

	int row = 0, column = 0;
	for (Pane* pPane : m_panes)
	{
		QRect rect(column * columnWidth, row * rowHeight, columnWidth, rowHeight);
		pPane->setGeometry(rect);

		++column;
		if (column > columnsCount)
		{
			++row;
			column = 0;
		}
	}
}

void PanesLayout::ApplyLayout()
{
	for (Pane* pPane : m_panes)
	{
		QSettings settings;
		pPane->setVisible(settings.value(objectName() + '/' + pPane->windowTitle() + "/visible").toBool());
		pPane->restoreGeometry(settings.value(objectName() + '/' + pPane->windowTitle() + "/geometry").toByteArray());
		pPane->restoreState(settings.value(objectName() + '/' + pPane->windowTitle() + "/windowState").toByteArray());
	}
}

bool PanesLayout::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Hide)
	{
		for (Pane* pPane : m_panes)
		{
			if (watched == pPane)
			{
				QSettings settings;
				if (!PDXModdingAssistant::g_pMainWnd->IsClosing())
					settings.setValue(objectName() + '/' + pPane->windowTitle() + "/visible", false);
				settings.setValue(objectName() + '/' + pPane->windowTitle() + "/geometry", pPane->saveGeometry());
				settings.setValue(objectName() + '/' + pPane->windowTitle() + "/windowState", pPane->saveState());
				break;
			}
		}
	}
	else if (event->type() == QEvent::Show)
	{
		for (Pane* pPane : m_panes)
		{
			if (watched == pPane)
			{
				QSettings settings;
				settings.setValue(objectName() + '/' + pPane->windowTitle() + "/visible", true);
				pPane->restoreGeometry(settings.value(objectName() + '/' + pPane->windowTitle() + "/geometry").toByteArray());
				pPane->restoreState(settings.value(objectName() + '/' + pPane->windowTitle() + "/windowState").toByteArray());
				break;
			}
		}
	}
	return false;
}
