#pragma once
#include "QtIncludes.h"
#include "GenericTreeModel.h"

namespace RCLib {

class GenericTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit GenericTreeView(QWidget* pParent = nullptr);
    virtual ~GenericTreeView() = default;

    void SetModel(std::shared_ptr<GenericTreeModel> pModel);
    GenericTreeModel* GetModel() const { return m_pModel.get(); }

protected:
    std::shared_ptr<GenericTreeModel> m_pModel;
};

} // namespace RCLib 