#include "GameObject.h"
#include "GameObjectManager.h"
#include <random>

using namespace RCLib;

GameObject::GameObject()
{
	// Génération d'un ID unique aléatoire
	static std::random_device rd;
	static std::mt19937_64 gen(rd());
	static std::uniform_int_distribution<uint64_t> dis;
	m_guid = dis(gen);
}

void GameObject::Release()
{
}

void GameObject::Update()
{
}

void GameObject::Load()
{
}
