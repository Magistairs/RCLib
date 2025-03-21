#include "GenericTreeView.h"
#include "RCLibQt.h"

using namespace RCLib::Qt;

GenericTreeView::GenericTreeView(QWidget* pParent)
  : QTreeView(pParent)
{
}

void GenericTreeView::SetModel(GenericTreeModel* pModel)
{
	m_pModel = pModel;
	setModel(m_pModel);
}
