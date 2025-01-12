// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "CRG_EditorLoad.h"
#include "PartCategories.h"

CRG_EditorLoad* editorload;

///  Set this to True to unlock parts from ALL modded categories
///  This will be set by detecting the presence of a certain mod file in an optional package.
bool bUnlockAll = false;

void Initialize()
{
	// Should we unlock all categories
	PropertyListPtr propList;
	if (PropManager.GetPropertyList(id("creaturepartunlocks_all"), 0x0, propList)) {
		bUnlockAll = true;
	}

	AddCategories();
	editorload = new(CRG_EditorLoad);
}

void Dispose()
{
	editorload = nullptr;
}

static const eastl::hash_set<uint32_t> editorCategories = {
		id("ce_category_mouths"),
		id("ce_category_eyes"),
		id("ce_category_eyes_&_senses"),
		id("ce_category_arms_&_legs"),
		id("ce_category_graspers"),
		id("ce_category_feet"),
		id("ce_category_weapons"),
		id("ce_category_details"),
};

///  Store the category IDs that we should or should not unlock
static eastl::hash_map<uint32_t, bool> moddedCategoriesUnlock;

member_detour(PalettePageLoad_detour, Palettes::PalettePage, bool(const ResourceKey&, uint32_t, uint32_t, uint32_t, uint32_t))
{
	bool ShouldAddCategoryUnlock() {
		// if vanilla category, return false
		if (editorCategories.find(mParentCategory.instanceID) != editorCategories.end()) { return false; }

		if (bUnlockAll) { return true; }

		auto find = moddedCategoriesUnlock.find(mParentCategory.instanceID);
		// did not find in unlock list, read the category file data
		if (find == moddedCategoriesUnlock.end()) {
			PropertyListPtr mpPropList;
			if (PropManager.GetPropertyList(mParentCategory.instanceID, mParentCategory.groupID, mpPropList))
			{
				bool unlockable = false;
				bool hasProp = App::Property::GetBool(mpPropList.get(), 0xB67D623A, unlockable); // id("paletteCategoryUnlockable")
				// store the value
				moddedCategoriesUnlock[mParentCategory.instanceID] = (hasProp && unlockable);
				return (hasProp && unlockable);
			}
		}
		// found in list, return saved value
		else {
			return find.get_node()->mValue.second;
		}
		return false;
	}


	bool detoured(const ResourceKey& name, uint32_t thumbnailGroupID, uint32_t arg_8 = 0, uint32_t layoutID = 0, uint32_t arg_10 = 0) {
		bool value = original_function(this, name, thumbnailGroupID, arg_8, layoutID, arg_10);

		if (editorload) {
			
			// in order to not break Collections, only run when editor is inactive
			if (!editorload->mbEditorActive ) { //&& !IsCreatureGame() // this failsafe is too aggressive and breaks the game
				// run this mode check because the way to get into creature stage is from these modes.
				auto mode = GameModeManager.GetActiveModeID();
				if (mode == kGameCell || mode == kGGEMode || mode == kLoadGameMode) {

					// Only run on valid modded categories
					if (ShouldAddCategoryUnlock()) {
						this->mParentCategory = ResourceKey(id("ce_category_details"), TypeIDs::Names::prop, 0x406B6E0C); // re-sort into details category
					}
				}
				
			}
		}
		return value;
	}
};




void AttachDetours()
{
	//GetPropertySwapCategory_detour::attach(GetAddress(App::PropertyList, GetProperty));
	//EditorReadProp_detour::attach(GetAddress(Palettes::PaletteMain, ReadProp));
	PalettePageLoad_detour::attach(GetAddress(Palettes::PalettePage, Load));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

