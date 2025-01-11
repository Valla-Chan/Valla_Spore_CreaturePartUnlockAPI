#pragma once

#include <Spore\BasicIncludes.h>

#define CRG_EditorLoadPtr intrusive_ptr<CRG_EditorLoad>

using namespace Simulator;

class CRG_EditorLoad 
	: public App::IMessageListener
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("CRG_EditorLoad");

	CRG_EditorLoad();
	~CRG_EditorLoad();

	int AddRef() override;
	int Release() override;
	void ResetSuppress();

	bool mbEditorActive = false; // if the editor is actually to be used by the player
	
	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;
private:
	bool mbSuppress = false;
};
