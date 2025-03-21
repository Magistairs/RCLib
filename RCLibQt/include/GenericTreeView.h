#pragma once
#include "RCLibQt_Fwd.h"
#include <QTreeView>
#include "GenericTreeModel.h"
#include "SmartPointers.h"

namespace RCLib {
namespace Qt {

DECLARE_PTR(GenericTreeModel);

class GenericTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit GenericTreeView(QWidget* pParent = nullptr);
    virtual ~GenericTreeView() = default;

    void SetModel(SharedPtr<GenericTreeModel> pModel);
    GenericTreeModel* GetModel() const { return m_pModel.get(); }

protected:
    SharedPtr<GenericTreeModel> m_pModel;
};

} // namespace Qt
} // namespace RCLib 