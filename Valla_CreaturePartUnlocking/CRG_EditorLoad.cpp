#include "stdafx.h"
#include "CRG_EditorLoad.h"

CRG_EditorLoad::CRG_EditorLoad()
{
	MessageManager.AddListener(this, kEnterCreatureEditor);
	MessageManager.AddListener(this, kMsgSwitchEditor);
	MessageManager.AddListener(this, kMsgEnterEditor);
	MessageManager.AddListener(this, kMsgSwitchGameMode);
}


CRG_EditorLoad::~CRG_EditorLoad()
{
}

// For internal use, do not modify.
int CRG_EditorLoad::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int CRG_EditorLoad::Release()
{
	return DefaultRefCounted::Release();
}

void CRG_EditorLoad::ResetSuppress()
{
	mbSuppress = false;
	mbEditorActive = false;
}

// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool CRG_EditorLoad::HandleMessage(uint32_t messageID, void* message)
{
	if (messageID == kMsgSwitchGameMode) {
		if (!mbSuppress) {
			mbEditorActive = false;
		}
	}
	else {
		mbEditorActive = true;
		mbSuppress = true;
		App::ScheduleTask(this, &CRG_EditorLoad::ResetSuppress, 0.4f);
	}
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return false;
}
