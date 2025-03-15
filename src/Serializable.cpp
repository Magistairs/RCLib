#include "Serializable.h"
#include "TaskManager.h"
#include "Engine.h"

using namespace RCLib;

void Serializable::StartLoading()
{
	Engine::Get().GetTaskManager().AddTask([this] {
		const std::lock_guard<std::mutex> lock(m_loadingMutex);
		m_loaded = false;
		Load();
		m_loaded = true;
		});
}

void Serializable::Reload()
{
	const std::lock_guard<std::mutex> lock(m_loadingMutex);
	m_loaded = false;
	Unload();
	StartLoading();
}

void Serializable::StartSaving()
{
	Engine::Get().GetTaskManager().AddTask([this] {
		const std::lock_guard<std::mutex> lock(m_savingMutex);
		m_isSaving = true;
		Save();
		m_isSaving = false;
		});
}
