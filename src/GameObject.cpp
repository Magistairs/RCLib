#include "GameObject.h"
#include "GameObjectManager.h"

#include <assert.h>
#include <combaseapi.h>

using namespace RCLib;

RCLib::GameObject::GameObject()
{
	HRESULT res = CoCreateGuid(&m_GUID);
	assert(res == S_OK);
}

void RCLib::GameObject::Release()
{

}

void GameObject::Update()
{

}

void GameObject::Serialize(bool load, ObjectPtr pJsonObject)
{

}

void GameObject::Load() {}
