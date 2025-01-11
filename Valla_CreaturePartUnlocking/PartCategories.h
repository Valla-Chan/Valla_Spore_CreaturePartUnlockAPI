#pragma once

#include "stdafx.h"
#include <Spore\CommonIDs.h>
#include <Spore\App\Property.h>
#include <Spore\UTFWin\UILayout.h>
#include <Spore\UTFWin\ImageDrawable.h>

eastl::vector<uint32_t> CollectionsEntryIDs = {};

void ReadModdedCategories()
{
	eastl::vector<ResourceKey> keys;

	// pull new Collections entry props from AssetBrowserCRGCollections folder
	ResourceManager.GetRecordKeyList(keys,
		&Resource::StandardFileFilter(
			ResourceKey::kWildcardID,
			id("AssetBrowserCRGCollections"),
			TypeIDs::prop,
			ResourceKey::kWildcardID
		)
	);

	for (const ResourceKey& key : keys) {
		CollectionsEntryIDs.push_back(key.instanceID);
	}
}

const uint32_t feedListItems = 0x744717C1;

//
void AddCategories() {
	// open the Creature_parts master file
	PropertyListPtr propList;
	if (PropManager.GetPropertyList(id("creature_parts"), id("AssetBrowserFeedCategories"), propList)) {

		// get the feedListItems property of the manifest file
		size_t listItemCount;
		ResourceKey* listItemKeys;
		if (App::Property::GetArrayKey(propList.get(), feedListItems, listItemCount, listItemKeys))
		{
			ReadModdedCategories();

			// begin storing a list of the new entries
			vector<uint32_t> newItems = {};

			// loop through the new collection entry props
			for (size_t i = 0; i < CollectionsEntryIDs.size(); i++)
			{
				PropertyListPtr propListCollectionEntry;
				if (PropManager.GetPropertyList(CollectionsEntryIDs[i], id("AssetBrowserCRGCollections"), propListCollectionEntry)) {

					// get the items from the collection entry's feedListItems property
					size_t keycount;
					ResourceKey* keys;

					if (App::Property::GetArrayKey(propListCollectionEntry.get(), feedListItems, keycount, keys)) {
						// loop thru each item in the key array and add them to the master newItems array
						for (size_t j = 0; j < keycount; j++) {
							newItems.push_back(keys[j].instanceID);
						}
					}

				}
			}

			// Now that all new items have been found, begin compiling a full list that has the original items and the new ones.
			ResourceKey* finalItemKeys = new ResourceKey[listItemCount + newItems.size()]{};
			// add in original items
			for (size_t i = 0; i < listItemCount; i++) {
				finalItemKeys[i] = listItemKeys[i];
			}
			// add in new items
			for (size_t i = 0; i < newItems.size(); i++) {
				finalItemKeys[listItemCount + i] = ResourceKey(newItems[i], 0, 0);
			}

			// Transfer new key data back to the property

			App::Property* prop;
			propList->GetProperty(feedListItems, prop);

			prop->SetArrayKey(finalItemKeys, listItemCount + newItems.size());

			// idk what this does vanillacold wrote it lol
			bool ownsMemory = (prop->mnFlags & 0x20) != 0x20;
			if (!ownsMemory)
			{
				prop->mnFlags = static_cast<short>((prop->mnFlags & ~0x20) | 0x4 | 0x10);
			}
		}
	}
}