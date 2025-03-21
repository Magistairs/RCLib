#include "RCLibQt.h"
#include "GenericTreeView.h"
#include "GenericTreeModel.h"
#include "RCLib.h"

using namespace RCLib::Qt;

GenericTreeView::GenericTreeView(QWidget* pParent)
  : QTreeView(pParent)
{
}

void GenericTreeView::SetModel(SharedPtr<GenericTreeModel> pModel)
{
	m_pModel = pModel;
	setModel(m_pModel.get());
}
