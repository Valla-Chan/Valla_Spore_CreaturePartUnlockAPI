# Creature Part Unlock API

This Spore mod allows users to unlock creature parts from custom categories in creature stage, and provides a framework for modders to enable this in their own mods.

![image](https://github.com/user-attachments/assets/b966cec2-8304-4075-8b62-e1d23250487c)

*NOTE: This mod is primarily an API mod used as a dependency for other mods, but contains additional components to modify its behavior.*

# Modders' Resources:
## How to make your custom parts category unlockable

To make your own parts category unlockable in creature stage, simply have this mod installed and take the following steps:

1. Make sure you have this mod installed, and included in your mod's release page as a requirement or recommendation.
2. Include the property `bool paletteCategoryUnlockable true` in your custom category definition (Usually in `creature_editor_palette_categories~`)
3. Create a collections entry for your palette page in `AssetBrowserCRGCollections` and `AssetBrowserFeedItems`. Examples of these files can be found in [/SMFX Projects/ExampleProject/](https://github.com/Valla-Chan/Valla_Spore_CreaturePartUnlockAPI/tree/main/SMFX%20Project/ExampleProject)
4. Make sure that your collections entry in `AssetBrowserFeedItems` references the file in `creature_editor_palette_categories~` as its `feedListItemUniqueID`.
<br>
<br>
You should now be able to see your custom parts unlocking ingame!
