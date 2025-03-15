#pragma once
#include "QtIncludes.h"

namespace RCLib {

class IPane;

class PanesLayout : public QObject
{
	Q_OBJECT

public:
	virtual ~PanesLayout() = default;
	virtual void AddPane(IPane* pPane);
	virtual const std::vector<IPane*>& GetPanes() const;
	virtual void CreateDefaultLayout();
	virtual void ApplyLayout();
	virtual bool HandleEvent(QObject* watched, QEvent* event);

protected:
	std::vector<IPane*> m_panes;
};

} // namespace RCLib
