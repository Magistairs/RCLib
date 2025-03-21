#pragma once
#include "GenericTreeModel.h"
#include "RCLibQt_Fwd.h"

#include <QTreeView>

namespace RCLib
{
namespace Qt
{

class GenericTreeView : public QTreeView
{
	Q_OBJECT

public:
	explicit GenericTreeView(QWidget* pParent = nullptr);
	virtual ~GenericTreeView() = default;

	void SetModel(GenericTreeModel* pModel);

protected:
	GenericTreeModel* m_pModel;
};

} // namespace Qt
} // namespace RCLib