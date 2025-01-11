This Spore mod allows users to unlock creature parts from custom categories in creature stage, and provides a framework for modders to enable this in their own mods.

NOTE: This mod is primarily an API mod used as a dependency for other mods, but contains additional components to modify its behavior.

# Modders' Resources:
## How to make your custom parts category unlockable

To make your own parts category unlockable in creature stage, simply have this mod installed and take the following steps:

1. Make sure you have this mod installed, and included in your mod's release page as a requirement or recommendation.
2. Include the property `bool paletteCategoryUnlockable true` in your custom category definition (Usually in `creature_editor_palette_categories~`)
3. Create a collections entry for your palette page in `AssetBrowserCRGCollections` and `AssetBrowserFeedItems`. Examples of these files can be found in `/SMFX Projects/ExampleProject/`
4. Make sure that your collections entry in `AssetBrowserFeedItems` references the file in `creature_editor_palette_categories~` as its `feedListItemUniqueID`.

You should now be able to see your custom parts unlocking ingame!
