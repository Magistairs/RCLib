#pragma once
#include "DLLExport.h"
#include "Object.h"
#include "StdIncludes.h"

namespace RCLib
{
class RCLIB_API Serializable
{
public:
	Serializable()  = default;
	~Serializable() = default;

	Serializable(Serializable&&)      = delete;
	Serializable(const Serializable&) = delete;

	Serializable& operator=(Serializable&&)      = delete;
	Serializable& operator=(const Serializable&) = delete;

	void StartLoading();
	bool IsLoaded() const { return m_loaded; }

	void StartSaving();
	bool IsSaving() const { return m_isSaving; }

	void Unload() {}
	void Reload();

protected:
	// Load and Save are the functions called from the top level, they create the Serializer and call Serialize
	// If a Serializable object has children to Load/Save,
	// it should call Serialize() if they are in the same file
	// and Load/Save if they have their own file
	virtual void Load() {}
	virtual void Save() {}

private:
	std::mutex        m_loadingMutex;
	std::atomic<bool> m_loaded{false};

	std::mutex        m_savingMutex;
	std::atomic<bool> m_isSaving{false};
};
} // namespace RCLib