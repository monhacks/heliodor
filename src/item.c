#include "global.h"
#include "item.h"
#include "berry.h"
#include "string_util.h"
#include "text.h"
#include "event_data.h"
#include "malloc.h"
#include "secret_base.h"
#include "item_menu.h"
#include "strings.h"
#include "load_save.h"
#include "item_use.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "pokedex.h"
#include "pokeball.h"
#include "other_saves.h"
#include "convert_save.h"

static bool8 CheckPyramidBagHasItem(u16 itemId, u16 count);
static bool8 CheckPyramidBagHasSpace(u16 itemId, u16 count);
void ItemId_GetHoldEffectParam_Script();
static void AddItemWhileConverting(struct SaveBlock1 *nlSaveBlock1Ptr, u16 itemId, u16 quantity);

EWRAM_DATA struct BagPocket gBagPockets[POCKETS_COUNT] = {0};
EWRAM_DATA struct ItemSlot gBagTMHMPocket[BAG_TMHM_COUNT] = {0};
EWRAM_DATA struct ItemSlot gBagBerriesPocket[BAG_BERRIES_COUNT] = {0};
EWRAM_DATA struct ItemSlot gBagMailPocket[BAG_MAIL_COUNT] = {0};
EWRAM_DATA struct ItemSlot gBagKeyItemsPocket[BAG_KEYITEMS_COUNT] = {0};

#include "data/text/item_descriptions.h"
#include "data/items.h"

static const u16 sKeyItems[] = {
    ITEM_NONE,
    ITEM_ADVENTURE_GUIDE,
    ITEM_JOURNAL,
    ITEM_MACH_BIKE,
    ITEM_ACRO_BIKE,
    ITEM_BICYCLE,
    ITEM_OLD_ROD,
    ITEM_GOOD_ROD,
    ITEM_SUPER_ROD,
    ITEM_ITEMFINDER,
    ITEM_WAILMER_PAIL,
    ITEM_TOWN_MAP,
    ITEM_TM_CASE,
    ITEM_BERRY_POUCH,
    ITEM_MAIL_CASE,
    ITEM_POKEBLOCK_CASE,
    ITEM_POWER_PAD,
    ITEM_COIN_CASE,
    ITEM_SOOT_SACK,
    ITEM_POWDER_JAR,
    ITEM_VS_RECORDER,
    ITEM_VS_SEEKER,
    ITEM_FAME_CHECKER,
    ITEM_POKE_RADAR,
    ITEM_POINT_CARD,
    ITEM_PAL_PAD,
    ITEM_CLOTHING_TRUNK,
    ITEM_APRICORN_BOX,
    ITEM_BERRY_POTS,
    ITEM_UNOWN_REPORT,
    ITEM_GB_SOUNDS,
    ITEM_MEDAL_BOX,
    ITEM_LETTER,
    ITEM_DEVON_GOODS,
    ITEM_GO_GOGGLES,
    ITEM_DEVON_SCOPE,
    ITEM_BASEMENT_KEY,
    ITEM_SCANNER,
    ITEM_STORAGE_KEY,
    ITEM_ROOM_1_KEY,
    ITEM_ROOM_2_KEY,
    ITEM_ROOM_4_KEY,
    ITEM_ROOM_6_KEY,
    ITEM_SECRET_KEY,
    ITEM_LIFT_KEY,
    ITEM_CARD_KEY,
    ITEM_METEORITE,
    ITEM_RED_ORB,
    ITEM_BLUE_ORB,
    ITEM_JADE_ORB,
    ITEM_RUBY,
    ITEM_SAPPHIRE,
    ITEM_EMERALD,
    ITEM_MAGMA_EMBLEM,
    ITEM_OAKS_PARCEL,
    ITEM_GOLD_TEETH,
    ITEM_TEA,
    ITEM_POKE_FLUTE,
    ITEM_SILPH_SCOPE,
    ITEM_TEACHY_TV,
    ITEM_BIKE_VOUCHER,
    ITEM_CONTEST_PASS,
    ITEM_SS_TICKET,
    ITEM_TRI_PASS,
    ITEM_RAINBOW_PASS,
    ITEM_EON_TICKET,
    ITEM_MYSTIC_TICKET,
    ITEM_AURORA_TICKET,
    ITEM_OLD_SEA_MAP,
    ITEM_OVAL_CHARM,
    ITEM_SHINY_CHARM,
    ITEM_CATCHING_CHARM,
    ITEM_EXP_CHARM
};

// code
u16 GetBagItemQuantity(u16 *quantity)
{
    return *quantity;
}

static void SetBagItemQuantity(u16 *quantity, u16 newValue)
{
    *quantity =  newValue;
}

static u16 GetPCItemQuantity(u16 *quantity)
{
    return *quantity;
}

static void SetPCItemQuantity(u16 *quantity, u16 newValue)
{
    *quantity = newValue;
}

void ApplyNewEncryptionKeyToBagItems(u32 newKey)
{
    u32 pocket, item;
    for (pocket = 0; pocket < POCKETS_COUNT; pocket++)
    {
        for (item = 0; item < gBagPockets[pocket].capacity; item++)
            ApplyNewEncryptionKeyToHword(&(gBagPockets[pocket].itemSlots[item].quantity), newKey);
    }
}

void ApplyNewEncryptionKeyToBagItems_(u32 newKey) // really GF?
{
    ApplyNewEncryptionKeyToBagItems(newKey);
}

void SetBagItemsPointers(void)
{
    gBagPockets[ITEMS_POCKET].itemSlots = gSaveBlock1Ptr->bagPocket_Items;
    gBagPockets[ITEMS_POCKET].capacity = BAG_ITEMS_COUNT;

    gBagPockets[MEDICINE_POCKET].itemSlots = gSaveBlock1Ptr->bagPocket_Medicine;
    gBagPockets[MEDICINE_POCKET].capacity = BAG_MEDICINE_COUNT;

    gBagPockets[BALLS_POCKET].itemSlots = gSaveBlock1Ptr->bagPocket_PokeBalls;
    gBagPockets[BALLS_POCKET].capacity = BAG_POKEBALLS_COUNT;

    gBagPockets[BATTLEITEMS_POCKET].itemSlots = gSaveBlock1Ptr->bagPocket_BattleItems;
    gBagPockets[BATTLEITEMS_POCKET].capacity = BAG_BATTLEITEMS_COUNT;

    gBagPockets[TREASURES_POCKET].itemSlots = gSaveBlock1Ptr->bagPocket_Treasures;
    gBagPockets[TREASURES_POCKET].capacity = BAG_TREASURES_COUNT;

    gBagPockets[FREESPACE_POCKET].itemSlots = gSaveBlock1Ptr->pcItems;
    gBagPockets[FREESPACE_POCKET].capacity = PC_ITEMS_COUNT;

    

    gBagPockets[TMHM_POCKET].itemSlots = &gBagTMHMPocket[0];
    gBagPockets[TMHM_POCKET].capacity = BAG_TMHM_COUNT;

    gBagPockets[BERRIES_POCKET].itemSlots = &gBagBerriesPocket[0];
    gBagPockets[BERRIES_POCKET].capacity = BAG_BERRIES_COUNT;

    gBagPockets[MAIL_POCKET].itemSlots = &gBagMailPocket[0];
    gBagPockets[MAIL_POCKET].capacity = BAG_MAIL_COUNT;

    gBagPockets[KEYITEMS_POCKET].itemSlots = &gBagKeyItemsPocket[0];
    gBagPockets[KEYITEMS_POCKET].capacity = BAG_KEYITEMS_COUNT;
}

void CopyItemName(u16 itemId, u8 *dst)
{
	u32 i;

    if (itemId == ITEM_ENIGMA_BERRY)
    {
        StringCopy(dst, GetBerryInfo(ITEM_TO_BERRY(ITEM_ENIGMA_BERRY))->name);
		for (i = 1; i < 6; i++)
		{
			if (dst[i] >= 0xBB && dst[i] <= 0xD4)
				dst[i] += 26;
		}
        StringAppend(dst, gText_Berry2);
    }
    else
    {
        StringCopy(dst, ItemId_GetName(itemId));
    }
}

void CopyItemNameHandlePlural(u16 itemId, u8 *dst, u32 quantity)
{
    u8 *txtPtr;

    if (itemId == ITEM_POKE_BALL)
    {
        if (quantity < 2)
            StringCopy(dst, ItemId_GetName(ITEM_POKE_BALL));
        else
            StringCopy(dst, gText_PokeBalls);
    }
    else if (itemId == ITEM_ENIGMA_BERRY)
    {
        txtPtr = StringCopy(dst, GetBerryInfo(ITEM_TO_BERRY(ITEM_ENIGMA_BERRY))->name);
        *txtPtr = CHAR_SPACE;
        if (quantity < 2)
            StringCopy(txtPtr + 1, gText_Berry);
        else
            StringCopy(txtPtr + 1, gText_Berries);
    }
    else
    {
        if (itemId >= FIRST_BERRY_INDEX && itemId <= LAST_BERRY_INDEX)
            GetBerryCountString(dst, gBerries[itemId - FIRST_BERRY_INDEX].name, quantity);
        else
            StringCopy(dst, ItemId_GetName(itemId));
    }
}

void GetBerryCountString(u8 *dst, const u8 *berryName, u32 quantity)
{
    const u8 *berryString;
    u8 *txtPtr;
    u32 i;

    if (quantity < 2)
        berryString = gText_Berry;
    else
        berryString = gText_Berries;

    txtPtr = StringCopy(dst, berryName);
	for (i = 1; i < 6; i++)
	{
		if (dst[i] >= 0xBB && dst[i] <= 0xD4)
			dst[i] += 26;
	}
    *txtPtr = CHAR_SPACE;
    StringCopy(txtPtr + 1, berryString);
}

bool8 IsBagPocketNonEmpty(u8 pocket)
{
    u8 i;

    for (i = 0; i < gBagPockets[pocket - 1].capacity; i++)
    {
        if (gBagPockets[pocket - 1].itemSlots[i].itemId != 0)
            return TRUE;
    }
    return FALSE;
}

bool8 CheckBagHasItem(u16 itemId, u16 count)
{
    u8 i;
    u8 pocket;

    if (ItemId_GetPocket(itemId) == 0)
        return FALSE;
    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
        return CheckPyramidBagHasItem(itemId, count);
    pocket = ItemId_GetPocket(itemId) - 1;
    // Check for item slots that contain the item
    for (i = 0; i < gBagPockets[pocket].capacity; i++)
    {
        if (gBagPockets[pocket].itemSlots[i].itemId == itemId)
        {
            u16 quantity;
            // Does this item slot contain enough of the item?
            quantity = GetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity);
            if (quantity >= count)
                return TRUE;
            count -= quantity;
            // Does this item slot and all previous slots contain enough of the item?
            if (count == 0)
                return TRUE;
        }
    }
    return FALSE;
}

bool8 HasAtLeastOneBerry(void)
{
    u16 i;

    for (i = FIRST_BERRY_INDEX; i < ITEM_BRIGHT_POWDER; i++)
    {
        if (CheckBagHasItem(i, 1) == TRUE)
        {
            gSpecialVar_Result = TRUE;
            return TRUE;
        }
    }
    gSpecialVar_Result = FALSE;
    return FALSE;
}

bool8 CheckBagHasSpace(u16 itemId, u16 count)
{
    u8 i;
    u8 pocket;
    u16 slotCapacity;
    u16 ownedCount;

    if (ItemId_GetPocket(itemId) == POCKET_NONE)
        return FALSE;

    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
    {
        return CheckPyramidBagHasSpace(itemId, count);
    }

    pocket = ItemId_GetPocket(itemId) - 1;
    if (pocket != BERRIES_POCKET)
        slotCapacity = MAX_BAG_ITEM_CAPACITY;
    else
        slotCapacity = MAX_BERRY_CAPACITY;

    // Check space in any existing item slots that already contain this item
    for (i = 0; i < gBagPockets[pocket].capacity; i++)
    {
        if (gBagPockets[pocket].itemSlots[i].itemId == itemId)
        {
            ownedCount = GetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity);
            if (ownedCount + count <= slotCapacity)
                return TRUE;
            if (pocket == TMHM_POCKET || pocket == BERRIES_POCKET)
                return FALSE;
            count -= (slotCapacity - ownedCount);
            if (count == 0)
                break; //should be return TRUE, but that doesn't match
        }
    }

    // Check space in empty item slots
    if (count > 0)
    {
        for (i = 0; i < gBagPockets[pocket].capacity; i++)
        {
            if (gBagPockets[pocket].itemSlots[i].itemId == 0)
            {
                if (count > slotCapacity)
                {
                    if (pocket == TMHM_POCKET || pocket == BERRIES_POCKET)
                        return FALSE;
                    count -= slotCapacity;
                }
                else
                {
                    count = 0; //should be return TRUE, but that doesn't match
                    break;
                }
            }
        }
        if (count > 0)
            return FALSE; // No more item slots. The bag is full
    }

    return TRUE;
}

bool8 AddBagItem(u16 itemId, u16 count)
{
    u8 i;

    if (ItemId_GetPocket(itemId) == POCKET_NONE)
        return FALSE;

    // check Battle Pyramid Bag
    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
    {
        return AddPyramidBagItem(itemId, count);
    }
    else
    {
        struct BagPocket *itemPocket;
        struct ItemSlot *newItems;
        u16 slotCapacity;
        u16 ownedCount;
        u8 pocket = ItemId_GetPocket(itemId) - 1;

        itemPocket = &gBagPockets[pocket];
        newItems = AllocZeroed(itemPocket->capacity * sizeof(struct ItemSlot));
        memcpy(newItems, itemPocket->itemSlots, itemPocket->capacity * sizeof(struct ItemSlot));

        if (pocket != BERRIES_POCKET)
            slotCapacity = MAX_BAG_ITEM_CAPACITY;
        else
            slotCapacity = MAX_BERRY_CAPACITY;

        for (i = 0; i < itemPocket->capacity; i++)
        {
            if (newItems[i].itemId == itemId)
            {
                ownedCount = GetBagItemQuantity(&newItems[i].quantity);
                // check if won't exceed max slot capacity
                if (ownedCount + count <= slotCapacity)
                {
                    // successfully added to already existing item's count
                    SetBagItemQuantity(&newItems[i].quantity, ownedCount + count);
                    memcpy(itemPocket->itemSlots, newItems, itemPocket->capacity * sizeof(struct ItemSlot));
                    Free(newItems);
                    return TRUE;
                }
                else
                {
                    // try creating another instance of the item if possible
                    if (pocket == TMHM_POCKET || pocket == BERRIES_POCKET)
                    {
                        Free(newItems);
                        return FALSE;
                    }
                    else
                    {
                        count -= slotCapacity - ownedCount;
                        SetBagItemQuantity(&newItems[i].quantity, slotCapacity);
                        // don't create another instance of the item if it's at max slot capacity and count is equal to 0
                        if (count == 0)
                        {
                            break;
                        }
                    }
                }
            }
        }

        // we're done if quantity is equal to 0
        if (count > 0)
        {
            // either no existing item was found or we have to create another instance, because the capacity was exceeded
            for (i = 0; i < itemPocket->capacity; i++)
            {
                if (newItems[i].itemId == ITEM_NONE)
                {
                    newItems[i].itemId = itemId;
                    if (count > slotCapacity)
                    {
                        // try creating a new slot with max capacity if duplicates are possible
                        if (pocket == TMHM_POCKET || pocket == BERRIES_POCKET)
                        {
                            Free(newItems);
                            return FALSE;
                        }
                        count -= slotCapacity;
                        SetBagItemQuantity(&newItems[i].quantity, slotCapacity);
                    }
                    else
                    {
                        // created a new slot and added quantity
                        SetBagItemQuantity(&newItems[i].quantity, count);
                        count = 0;
                        break;
                    }
                }
            }

            if (count > 0)
            {
                Free(newItems);
                return FALSE;
            }
        }
        memcpy(itemPocket->itemSlots, newItems, itemPocket->capacity * sizeof(struct ItemSlot));
        Free(newItems);
        return TRUE;
    }
}

bool8 RemoveBagItem(u16 itemId, u16 count)
{
    u8 i;
    u16 totalQuantity = 0;

    if (ItemId_GetPocket(itemId) == POCKET_NONE || itemId == ITEM_NONE)
        return FALSE;

    // check Battle Pyramid Bag
    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
    {
        return RemovePyramidBagItem(itemId, count);
    }
    else
    {
        u8 pocket;
        u8 var;
        u16 ownedCount;
        struct BagPocket *itemPocket;

        pocket = ItemId_GetPocket(itemId) - 1;
        itemPocket = &gBagPockets[pocket];

        for (i = 0; i < itemPocket->capacity; i++)
        {
            if (itemPocket->itemSlots[i].itemId == itemId)
                totalQuantity += GetBagItemQuantity(&itemPocket->itemSlots[i].quantity);
        }

        if (totalQuantity < count)
            return FALSE;   // We don't have enough of the item

        if (CurMapIsSecretBase() == TRUE)
        {
            VarSet(VAR_SECRET_BASE_LOW_TV_FLAGS, VarGet(VAR_SECRET_BASE_LOW_TV_FLAGS) | SECRET_BASE_USED_BAG);
            VarSet(VAR_SECRET_BASE_LAST_ITEM_USED, itemId);
        }

        var = GetItemListPosition(pocket);
        if (itemPocket->capacity > var
         && itemPocket->itemSlots[var].itemId == itemId)
        {
            ownedCount = GetBagItemQuantity(&itemPocket->itemSlots[var].quantity);
            if (ownedCount >= count)
            {
                SetBagItemQuantity(&itemPocket->itemSlots[var].quantity, ownedCount - count);
                count = 0;
            }
            else
            {
                count -= ownedCount;
                SetBagItemQuantity(&itemPocket->itemSlots[var].quantity, 0);
            }

            if (GetBagItemQuantity(&itemPocket->itemSlots[var].quantity) == 0)
                itemPocket->itemSlots[var].itemId = ITEM_NONE;

            if (count == 0)
                return TRUE;
        }

        for (i = 0; i < itemPocket->capacity; i++)
        {
            if (itemPocket->itemSlots[i].itemId == itemId)
            {
                ownedCount = GetBagItemQuantity(&itemPocket->itemSlots[i].quantity);
                if (ownedCount >= count)
                {
                    SetBagItemQuantity(&itemPocket->itemSlots[i].quantity, ownedCount - count);
                    count = 0;
                }
                else
                {
                    count -= ownedCount;
                    SetBagItemQuantity(&itemPocket->itemSlots[i].quantity, 0);
                }

                if (GetBagItemQuantity(&itemPocket->itemSlots[i].quantity) == 0)
                    itemPocket->itemSlots[i].itemId = ITEM_NONE;

                if (count == 0)
                    return TRUE;
            }
        }
        return TRUE;
    }
}

u8 GetPocketByItemId(u16 itemId)
{
    return ItemId_GetPocket(itemId);
}

void ClearItemSlots(struct ItemSlot *itemSlots, u8 itemCount)
{
    u16 i;

    for (i = 0; i < itemCount; i++)
    {
        itemSlots[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&itemSlots[i].quantity, 0);
    }
}

static s32 FindFreePCItemSlot(void)
{
    s8 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == ITEM_NONE)
            return i;
    }
    return -1;
}

u8 CountUsedPCItemSlots(void)
{
    u8 usedSlots = 0;
    u8 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId != ITEM_NONE)
            usedSlots++;
    }
    return usedSlots;
}

bool8 CheckPCHasItem(u16 itemId, u16 count)
{
    u8 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == itemId && GetPCItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity) >= count)
            return TRUE;
    }
    return FALSE;
}

bool8 AddPCItem(u16 itemId, u16 count)
{
    u8 i;
    s8 freeSlot;
    u16 ownedCount;
    struct ItemSlot *newItems;

    // Copy PC items
    newItems = AllocZeroed(sizeof(gSaveBlock1Ptr->pcItems));
    memcpy(newItems, gSaveBlock1Ptr->pcItems, sizeof(gSaveBlock1Ptr->pcItems));

    // Use any item slots that already contain this item
    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (newItems[i].itemId == itemId)
        {
            ownedCount = GetPCItemQuantity(&newItems[i].quantity);
            if (ownedCount + count <= MAX_PC_ITEM_CAPACITY)
            {
                SetPCItemQuantity(&newItems[i].quantity, ownedCount + count);
                memcpy(gSaveBlock1Ptr->pcItems, newItems, sizeof(gSaveBlock1Ptr->pcItems));
                Free(newItems);
                return TRUE;
            }
            count += ownedCount - MAX_PC_ITEM_CAPACITY;
            SetPCItemQuantity(&newItems[i].quantity, MAX_PC_ITEM_CAPACITY);
            if (count == 0)
            {
                memcpy(gSaveBlock1Ptr->pcItems, newItems, sizeof(gSaveBlock1Ptr->pcItems));
                Free(newItems);
                return TRUE;
            }
        }
    }

    // Put any remaining items into a new item slot.
    if (count > 0)
    {
        freeSlot = FindFreePCItemSlot();
        if (freeSlot == -1)
        {
            Free(newItems);
            return FALSE;
        }
        else
        {
            newItems[freeSlot].itemId = itemId;
            SetPCItemQuantity(&newItems[freeSlot].quantity, count);
        }
    }

    // Copy items back to the PC
    memcpy(gSaveBlock1Ptr->pcItems, newItems, sizeof(gSaveBlock1Ptr->pcItems));
    Free(newItems);
    return TRUE;
}

void RemovePCItem(u8 index, u16 count)
{
    gSaveBlock1Ptr->pcItems[index].quantity -= count;
    if (gSaveBlock1Ptr->pcItems[index].quantity == 0)
    {
        gSaveBlock1Ptr->pcItems[index].itemId = ITEM_NONE;
        CompactPCItems();
    }
}

void CompactPCItems(void)
{
    u16 i;
    u16 j;

    for (i = 0; i < PC_ITEMS_COUNT - 1; i++)
    {
        for (j = i + 1; j < PC_ITEMS_COUNT; j++)
        {
            if (gSaveBlock1Ptr->pcItems[i].itemId == 0)
            {
                struct ItemSlot temp = gSaveBlock1Ptr->pcItems[i];
                gSaveBlock1Ptr->pcItems[i] = gSaveBlock1Ptr->pcItems[j];
                gSaveBlock1Ptr->pcItems[j] = temp;
            }
        }
    }
}

void SwapRegisteredBike(void)
{
    switch (gSaveBlock1Ptr->registeredItemSelect)
    {
    case ITEM_MACH_BIKE:
        gSaveBlock1Ptr->registeredItemSelect = ITEM_ACRO_BIKE;
        break;
    case ITEM_ACRO_BIKE:
        gSaveBlock1Ptr->registeredItemSelect = ITEM_MACH_BIKE;
        break;
    }
    
    switch (gSaveBlock1Ptr->registeredItemL)
    {
    case ITEM_MACH_BIKE:
        gSaveBlock1Ptr->registeredItemL = ITEM_ACRO_BIKE;
        break;
    case ITEM_ACRO_BIKE:
        gSaveBlock1Ptr->registeredItemL = ITEM_MACH_BIKE;
        break;
    }
    
    switch (gSaveBlock1Ptr->registeredItemR)
    {
    case ITEM_MACH_BIKE:
        gSaveBlock1Ptr->registeredItemR = ITEM_ACRO_BIKE;
        break;
    case ITEM_ACRO_BIKE:
        gSaveBlock1Ptr->registeredItemR = ITEM_MACH_BIKE;
        break;
    }
}

u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 pocketPos)
{
	u8 i = 1;
	u16 newKeyItems[5];
	s32 extraItem = pocketPos - (gBagMenu->trueNumItemStacks[pocketId - 1] - 2);
	
	newKeyItems[0] = ITEM_POWER_PAD;
	
	if (HasAllHoennMons())
	{
		newKeyItems[i] = ITEM_OVAL_CHARM;
		i++;
	}
	if (HasAllMons())
	{
		newKeyItems[i] = ITEM_SHINY_CHARM;
		i++;
	}
	if (FlagGet(FLAG_SYS_GAME_CLEAR))
	{
		newKeyItems[i] = ITEM_CATCHING_CHARM;
		i++;
		
		newKeyItems[i] = ITEM_EXP_CHARM;
		i++;
	}
	
	if (extraItem > 0)
		return newKeyItems[extraItem - 1];
	
    return gBagPockets[pocketId - 1].itemSlots[pocketPos].itemId;
}

u16 BagGetQuantityByPocketPosition(u8 pocketId, u16 pocketPos)
{
    return GetBagItemQuantity(&gBagPockets[pocketId - 1].itemSlots[pocketPos].quantity);
}

static void SwapItemSlots(struct ItemSlot *a, struct ItemSlot *b)
{
    struct ItemSlot temp;
    SWAP(*a, *b, temp);
}

void CompactItemsInBagPocket(struct BagPocket *bagPocket)
{
    u16 i, j;

    for (i = 0; i < bagPocket->capacity - 1; i++)
    {
        for (j = i + 1; j < bagPocket->capacity; j++)
        {
            if (GetBagItemQuantity(&bagPocket->itemSlots[i].quantity) == 0)
                SwapItemSlots(&bagPocket->itemSlots[i], &bagPocket->itemSlots[j]);
        }
    }
}

void SortBerriesOrTMHMs(struct BagPocket *bagPocket)
{
    u16 i, j;

    for (i = 0; i < bagPocket->capacity - 1; i++)
    {
        for (j = i + 1; j < bagPocket->capacity; j++)
        {
            if (GetBagItemQuantity(&bagPocket->itemSlots[i].quantity) != 0)
            {
                if (GetBagItemQuantity(&bagPocket->itemSlots[j].quantity) == 0)
                    continue;
                if (bagPocket->itemSlots[i].itemId <= bagPocket->itemSlots[j].itemId)
                    continue;
            }
            SwapItemSlots(&bagPocket->itemSlots[i], &bagPocket->itemSlots[j]);
        }
    }
}

void MoveItemSlotInList(struct ItemSlot* itemSlots_, u32 from, u32 to_)
{
    // dumb assignments needed to match
    struct ItemSlot *itemSlots = itemSlots_;
    u32 to = to_;

    if (from != to)
    {
        s16 i, count;
        struct ItemSlot firstSlot = itemSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                itemSlots[i] = itemSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                itemSlots[i] = itemSlots[i - 1];
        }
        itemSlots[to] = firstSlot;
    }
}

void ClearBag(void)
{
    u16 i;

    for (i = 0; i < POCKETS_COUNT; i++)
    {
        ClearItemSlots(gBagPockets[i].itemSlots, gBagPockets[i].capacity);
    }
}

u16 CountTotalItemQuantityInBag(u16 itemId)
{
    u16 i;
    u16 ownedCount = 0;
    struct BagPocket *bagPocket = &gBagPockets[ItemId_GetPocket(itemId) - 1];

    for (i = 0; i < bagPocket->capacity; i++)
    {
        if (bagPocket->itemSlots[i].itemId == itemId)
            ownedCount += GetBagItemQuantity(&bagPocket->itemSlots[i].quantity);
    }

    return ownedCount;
}

static bool8 CheckPyramidBagHasItem(u16 itemId, u16 count)
{
    u8 i;
    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];

    for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
    {
        if (items[i] == itemId)
        {
            if (quantities[i] >= count)
                return TRUE;

            count -= quantities[i];
            if (count == 0)
                return TRUE;
        }
    }

    return FALSE;
}

static bool8 CheckPyramidBagHasSpace(u16 itemId, u16 count)
{
    u8 i;
    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];

    for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
    {
        if (items[i] == itemId || items[i] == ITEM_NONE)
        {
            if (quantities[i] + count <= MAX_PYRAMID_BAG_ITEM_CAPACITY)
                return TRUE;

            count = (quantities[i] + count) - MAX_PYRAMID_BAG_ITEM_CAPACITY;
            if (count == 0)
                return TRUE;
        }
    }

    return FALSE;
}

bool8 AddPyramidBagItem(u16 itemId, u16 count)
{
    u16 i;

    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];

    u16 *newItems = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
    u8 *newQuantities = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));

    memcpy(newItems, items, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
    memcpy(newQuantities, quantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));

    for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
    {
        if (newItems[i] == itemId && newQuantities[i] < MAX_PYRAMID_BAG_ITEM_CAPACITY)
        {
            newQuantities[i] += count;
            if (newQuantities[i] > MAX_PYRAMID_BAG_ITEM_CAPACITY)
            {
                count = newQuantities[i] - MAX_PYRAMID_BAG_ITEM_CAPACITY;
                newQuantities[i] = MAX_PYRAMID_BAG_ITEM_CAPACITY;
            }
            else
            {
                count = 0;
            }

            if (count == 0)
                break;
        }
    }

    if (count > 0)
    {
        for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
        {
            if (newItems[i] == ITEM_NONE)
            {
                newItems[i] = itemId;
                newQuantities[i] = count;
                if (newQuantities[i] > MAX_PYRAMID_BAG_ITEM_CAPACITY)
                {
                    count = newQuantities[i] - MAX_PYRAMID_BAG_ITEM_CAPACITY;
                    newQuantities[i] = MAX_PYRAMID_BAG_ITEM_CAPACITY;
                }
                else
                {
                    count = 0;
                }

                if (count == 0)
                    break;
            }
        }
    }

    if (count == 0)
    {
        memcpy(items, newItems, PYRAMID_BAG_ITEMS_COUNT * sizeof(*items));
        memcpy(quantities, newQuantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*quantities));
        Free(newItems);
        Free(newQuantities);
        return TRUE;
    }
    else
    {
        Free(newItems);
        Free(newQuantities);
        return FALSE;
    }
}

bool8 RemovePyramidBagItem(u16 itemId, u16 count)
{
    u16 i;

    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];

    i = gPyramidBagMenuState.cursorPosition + gPyramidBagMenuState.scrollPosition;
    if (items[i] == itemId && quantities[i] >= count)
    {
        quantities[i] -= count;
        if (quantities[i] == 0)
            items[i] = ITEM_NONE;
        return TRUE;
    }
    else
    {
        u16 *newItems = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
        u8 *newQuantities = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));

        memcpy(newItems, items, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
        memcpy(newQuantities, quantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));

        for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
        {
            if (newItems[i] == itemId)
            {
                if (newQuantities[i] >= count)
                {
                    newQuantities[i] -= count;
                    count = 0;
                    if (newQuantities[i] == 0)
                        newItems[i] = ITEM_NONE;
                }
                else
                {
                    count -= newQuantities[i];
                    newQuantities[i] = 0;
                    newItems[i] = ITEM_NONE;
                }

                if (count == 0)
                    break;
            }
        }

        if (count == 0)
        {
            memcpy(items, newItems, PYRAMID_BAG_ITEMS_COUNT * sizeof(*items));
            memcpy(quantities, newQuantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*quantities));
            Free(newItems);
            Free(newQuantities);
            return TRUE;
        }
        else
        {
            Free(newItems);
            Free(newQuantities);
            return FALSE;
        }
    }
}

static u16 SanitizeItemId(u16 itemId)
{
    if (itemId >= ITEMS_COUNT)
        return ITEM_NONE;
    else
        return itemId;
}

const u8 *ItemId_GetName(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].name;
}

// Unused
u16 ItemId_GetId(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].itemId;
}

u16 ItemId_GetPrice(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].price;
}

u8 ItemId_GetHoldEffect(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].holdEffect;
}

u8 ItemId_GetHoldEffectParam(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].holdEffectParam;
}

const u8 *ItemId_GetDescription(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].description;
}

bool8 ItemId_GetHoldability(u16 itemId)
{
    if (gItems[SanitizeItemId(itemId)].pocket == POCKET_KEY_ITEMS)
        return FALSE;
    else if (gItems[SanitizeItemId(itemId)].blockHolding)
        return FALSE;
    else
        return TRUE;
}

bool8 ItemId_GetUsability(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].fieldUseFunc != ItemUseOutOfBattle_CannotUse;
}

u8 ItemId_GetPocket(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].pocket;
}

u8 ItemId_GetType(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].type;
}

ItemUseFunc ItemId_GetFieldFunc(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].fieldUseFunc;
}

u8 ItemId_GetBattleUsage(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].battleUsage;
}

ItemUseFunc ItemId_GetBattleFunc(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].battleUseFunc;
}

u8 ItemId_GetSecondaryId(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].secondaryId;
}

void ItemId_GetHoldEffectParam_Script()
{
    VarSet(VAR_RESULT, ItemId_GetHoldEffectParam(VarGet(VAR_0x8004)));
}

void SaveFakePockets(void)
{
    u32 i, j, k = 0;

    for (i = 0; i < 50; i++)
        gSaveBlock1Ptr->bagPocket_TM[i] = 0;

    gSaveBlock1Ptr->bagPocket_HM01 = 0;
    gSaveBlock1Ptr->bagPocket_HM02 = 0;
    gSaveBlock1Ptr->bagPocket_HM03 = 0;
    gSaveBlock1Ptr->bagPocket_HM04 = 0;
    gSaveBlock1Ptr->bagPocket_HM05 = 0;
    gSaveBlock1Ptr->bagPocket_HM06 = 0;
    gSaveBlock1Ptr->bagPocket_HM07 = 0;
    gSaveBlock1Ptr->bagPocket_HM08 = 0;

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        if (gBagTMHMPocket[i].itemId >= ITEM_TM01 && gBagTMHMPocket[i].itemId <= ITEM_TM50)
            gSaveBlock1Ptr->bagPocket_TM[gBagTMHMPocket[i].itemId - ITEM_TM01] = GetBagItemQuantity(&gBagTMHMPocket[i].quantity);
        else if (gBagTMHMPocket[i].itemId == ITEM_HM01)
            gSaveBlock1Ptr->bagPocket_HM01 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM02)
            gSaveBlock1Ptr->bagPocket_HM02 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM03)
            gSaveBlock1Ptr->bagPocket_HM03 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM04)
            gSaveBlock1Ptr->bagPocket_HM04 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM05)
            gSaveBlock1Ptr->bagPocket_HM05 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM06)
            gSaveBlock1Ptr->bagPocket_HM06 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM07)
            gSaveBlock1Ptr->bagPocket_HM07 = 1;
        else if (gBagTMHMPocket[i].itemId == ITEM_HM08)
            gSaveBlock1Ptr->bagPocket_HM08 = 1;
    }

    for (i = 0; i < BAG_BERRIES_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Berries[i] = 0;

    for (i = 0; i < BAG_BERRIES_COUNT; i++)
    {
        if (gBagBerriesPocket[i].itemId >= FIRST_BERRY_INDEX && gBagBerriesPocket[i].itemId <= LAST_BERRY_INDEX)
            gSaveBlock1Ptr->bagPocket_Berries[gBagBerriesPocket[i].itemId - FIRST_BERRY_INDEX] = GetBagItemQuantity(&gBagBerriesPocket[i].quantity);
    }

    for (i = 0; i < BAG_MAIL_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Mail[i] = 0;

    for (i = 0; i < BAG_MAIL_COUNT; i++)
    {
        if (gBagMailPocket[i].itemId >= FIRST_MAIL_INDEX && gBagMailPocket[i].itemId <= LAST_MAIL_INDEX)
            gSaveBlock1Ptr->bagPocket_Mail[gBagMailPocket[i].itemId - FIRST_MAIL_INDEX] = GetBagItemQuantity(&gBagMailPocket[i].quantity);
    }

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_KeyItems[i] = 0;

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (gBagKeyItemsPocket[i].itemId != ITEM_NONE && ItemId_GetPocket(gBagKeyItemsPocket[i].itemId) == POCKET_KEY_ITEMS && GetBagItemQuantity(&gBagKeyItemsPocket[i].quantity) > 0)
        {
            for (j = 0; j < sizeof(sKeyItems); j++)
            {
                if (sKeyItems[j] == gBagKeyItemsPocket[i].itemId)
                {
                    gSaveBlock1Ptr->bagPocket_KeyItems[k] = j;
                    k++;
                    break;
                }
            }
        }
    }
}

void LoadFakePockets(void)
{
    u32 i;

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        gBagTMHMPocket[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&(gBagTMHMPocket[i].quantity), 0);
    }
    for (i = 0; i < 50; i++)
    {
        if (gSaveBlock1Ptr->bagPocket_TM[i] > 0)
            AddBagItem(ITEM_TM01 + i, gSaveBlock1Ptr->bagPocket_TM[i]);
    }

    if (gSaveBlock1Ptr->bagPocket_HM01)
        AddBagItem(ITEM_HM01, 1);
    if (gSaveBlock1Ptr->bagPocket_HM02)
        AddBagItem(ITEM_HM02, 1);
    if (gSaveBlock1Ptr->bagPocket_HM03)
        AddBagItem(ITEM_HM03, 1);
    if (gSaveBlock1Ptr->bagPocket_HM04)
        AddBagItem(ITEM_HM04, 1);
    if (gSaveBlock1Ptr->bagPocket_HM05)
        AddBagItem(ITEM_HM05, 1);
    if (gSaveBlock1Ptr->bagPocket_HM06)
        AddBagItem(ITEM_HM06, 1);
    if (gSaveBlock1Ptr->bagPocket_HM07)
        AddBagItem(ITEM_HM07, 1);
    if (gSaveBlock1Ptr->bagPocket_HM08)
        AddBagItem(ITEM_HM08, 1);

    for (i = 0; i < BAG_BERRIES_COUNT; i++)
    {
        gBagBerriesPocket[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&(gBagBerriesPocket[i].quantity), 0);
    }
    for (i = 0; i < BAG_BERRIES_COUNT; i++)
    {
        if (gSaveBlock1Ptr->bagPocket_Berries[i] > 0)
            AddBagItem(FIRST_BERRY_INDEX + i, gSaveBlock1Ptr->bagPocket_Berries[i]);
    }

    for (i = 0; i < BAG_MAIL_COUNT; i++)
    {
        gBagMailPocket[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&(gBagMailPocket[i].quantity), 0);
    }
    for (i = 0; i < BAG_MAIL_COUNT; i++)
    {
        if (gSaveBlock1Ptr->bagPocket_Mail[i] > 0)
            AddBagItem(FIRST_MAIL_INDEX + i, gSaveBlock1Ptr->bagPocket_Mail[i]);
    }

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        gBagKeyItemsPocket[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&(gBagKeyItemsPocket[i].quantity), 0);
    }
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (sKeyItems[gSaveBlock1Ptr->bagPocket_KeyItems[i]] != ITEM_NONE)
            AddBagItem(sKeyItems[gSaveBlock1Ptr->bagPocket_KeyItems[i]], 1);
    }
}

static void AddItemWhileConverting(struct SaveBlock1 *nlSaveBlock1Ptr, u16 itemId, u16 quantity)
{
    u32 i, j;
    bool32 placedKeyItem = FALSE;

    switch (ItemId_GetPocket(itemId))
    {
        case POCKET_NONE:
        default:
            for (i = 0; i < PC_ITEMS_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->pcItems[i].itemId == itemId)
                {
                    nlSaveBlock1Ptr->pcItems[i].quantity += quantity;
                    break;
                }
                else if (nlSaveBlock1Ptr->pcItems[i].itemId == ITEM_NONE)
                {
                    nlSaveBlock1Ptr->pcItems[i].itemId = itemId;
                    nlSaveBlock1Ptr->pcItems[i].quantity = quantity;
                    break;
                }
            }
            break;
        case POCKET_ITEMS:
            for (i = 0; i < BAG_ITEMS_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->bagPocket_Items[i].itemId == itemId)
                {
                    nlSaveBlock1Ptr->bagPocket_Items[i].quantity += quantity;
                    break;
                }
                else if (nlSaveBlock1Ptr->bagPocket_Items[i].itemId == ITEM_NONE)
                {
                    nlSaveBlock1Ptr->bagPocket_Items[i].itemId = itemId;
                    nlSaveBlock1Ptr->bagPocket_Items[i].quantity = quantity;
                    break;
                }
            }
            break;
        case POCKET_MEDICINE:
            for (i = 0; i < BAG_MEDICINE_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->bagPocket_Medicine[i].itemId == itemId)
                {
                    nlSaveBlock1Ptr->bagPocket_Medicine[i].quantity += quantity;
                    break;
                }
                else if (nlSaveBlock1Ptr->bagPocket_Medicine[i].itemId == ITEM_NONE)
                {
                    nlSaveBlock1Ptr->bagPocket_Medicine[i].itemId = itemId;
                    nlSaveBlock1Ptr->bagPocket_Medicine[i].quantity = quantity;
                    break;
                }
            }
            break;
        case POCKET_POKE_BALLS:
            for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId == itemId)
                {
                    nlSaveBlock1Ptr->bagPocket_PokeBalls[i].quantity += quantity;
                    break;
                }
                else if (nlSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId == ITEM_NONE)
                {
                    nlSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId = itemId;
                    nlSaveBlock1Ptr->bagPocket_PokeBalls[i].quantity = quantity;
                    break;
                }
            }
            break;
        case POCKET_BATTLE_ITEMS:
            for (i = 0; i < BAG_BATTLEITEMS_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->bagPocket_BattleItems[i].itemId == itemId)
                {
                    nlSaveBlock1Ptr->bagPocket_BattleItems[i].quantity += quantity;
                    break;
                }
                else if (nlSaveBlock1Ptr->bagPocket_BattleItems[i].itemId == ITEM_NONE)
                {
                    nlSaveBlock1Ptr->bagPocket_BattleItems[i].itemId = itemId;
                    nlSaveBlock1Ptr->bagPocket_BattleItems[i].quantity = quantity;
                    break;
                }
            }
            break;
        case POCKET_TREASURES:
            for (i = 0; i < BAG_TREASURES_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->bagPocket_Treasures[i].itemId == itemId)
                {
                    nlSaveBlock1Ptr->bagPocket_Treasures[i].quantity += quantity;
                    break;
                }
                else if (nlSaveBlock1Ptr->bagPocket_Treasures[i].itemId == ITEM_NONE)
                {
                    nlSaveBlock1Ptr->bagPocket_Treasures[i].itemId = itemId;
                    nlSaveBlock1Ptr->bagPocket_Treasures[i].quantity = quantity;
                    break;
                }
            }
            break;
        case POCKET_KEY_ITEMS:
            for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
            {
                if (nlSaveBlock1Ptr->bagPocket_KeyItems[i] == ITEM_NONE)
                {
                    for (j = 1; j < sizeof(sKeyItems); j++)
                    {
                        if (sKeyItems[j] == itemId)
                        {
                            nlSaveBlock1Ptr->bagPocket_KeyItems[i] = j;
                            placedKeyItem = TRUE;
                            break;
                        }
                    }
                    if (placedKeyItem)
                        break;
                }
            }
            break;
        case POCKET_TM_HM:
            if (itemId >= ITEM_TM01 && itemId <= ITEM_TM50)
                nlSaveBlock1Ptr->bagPocket_TM[itemId - ITEM_TM01] = quantity;
            else if (itemId == ITEM_HM01)
                nlSaveBlock1Ptr->bagPocket_HM01 = 1;
            else if (itemId == ITEM_HM02)
                nlSaveBlock1Ptr->bagPocket_HM02 = 1;
            else if (itemId == ITEM_HM03)
                nlSaveBlock1Ptr->bagPocket_HM03 = 1;
            else if (itemId == ITEM_HM04)
                nlSaveBlock1Ptr->bagPocket_HM04 = 1;
            else if (itemId == ITEM_HM05)
                nlSaveBlock1Ptr->bagPocket_HM05 = 1;
            else if (itemId == ITEM_HM06)
                nlSaveBlock1Ptr->bagPocket_HM06 = 1;
            else if (itemId == ITEM_HM07)
                nlSaveBlock1Ptr->bagPocket_HM07 = 1;
            else if (itemId == ITEM_HM08)
                nlSaveBlock1Ptr->bagPocket_HM08 = 1;
            break;
        case POCKET_BERRIES:
            if (itemId >= FIRST_BERRY_INDEX && itemId <= LAST_BERRY_INDEX)
                nlSaveBlock1Ptr->bagPocket_Berries[itemId - FIRST_BERRY_INDEX] = quantity;
            break;
        case POCKET_MAIL:
            if (itemId >= FIRST_MAIL_INDEX && itemId <= LAST_MAIL_INDEX)
                nlSaveBlock1Ptr->bagPocket_Mail[itemId - FIRST_MAIL_INDEX] = quantity;
            break;
    }
}

void TransferItemsToNewPockets(struct SaveBlock1 *nlSaveBlock1Ptr, u8 saveType)
{
    u32 i;
    u16 itemId, quantity;
    u8 keyItemId;
    struct RubySapphireSaveBlock1 *rsSaveBlock1Ptr = (struct RubySapphireSaveBlock1 *)gSaveBlock1Ptr;
    struct PreNLSaveBlock1 *preNLSaveBlock1Ptr = (struct PreNLSaveBlock1 *)gSaveBlock1Ptr;
    struct PreNLSaveBlock2 *preNLSaveBlock2Ptr = (struct PreNLSaveBlock2 *)gSaveBlock2Ptr;

    if (saveType == SAVE_TYPE_E)
    {
        for (i = 0; i < BAG_ITEMS_COUNT_PRENL; i++)
        {
            if (preNLSaveBlock1Ptr->bagPocket_Items[i].itemId != ITEM_NONE && (preNLSaveBlock1Ptr->bagPocket_Items[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey) > 0)
            {
                itemId = preNLSaveBlock1Ptr->bagPocket_Items[i].itemId;
                quantity = preNLSaveBlock1Ptr->bagPocket_Items[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_KEYITEMS_COUNT_PRENL; i++)
        {
            if (preNLSaveBlock1Ptr->bagPocket_KeyItems[i].itemId != ITEM_NONE && (preNLSaveBlock1Ptr->bagPocket_KeyItems[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey) > 0)
            {
                itemId = preNLSaveBlock1Ptr->bagPocket_KeyItems[i].itemId;
                quantity = preNLSaveBlock1Ptr->bagPocket_KeyItems[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_POKEBALLS_COUNT_PRENL; i++)
        {
            if (preNLSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId != ITEM_NONE && (preNLSaveBlock1Ptr->bagPocket_PokeBalls[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey) > 0)
            {
                itemId = preNLSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId;
                quantity = preNLSaveBlock1Ptr->bagPocket_PokeBalls[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_TMHM_COUNT_PRENL; i++)
        {
            if (preNLSaveBlock1Ptr->bagPocket_TMHM[i].itemId != ITEM_NONE && (preNLSaveBlock1Ptr->bagPocket_TMHM[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey) > 0)
            {
                itemId = preNLSaveBlock1Ptr->bagPocket_TMHM[i].itemId;
                quantity = preNLSaveBlock1Ptr->bagPocket_TMHM[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_BERRIES_COUNT_PRENL; i++)
        {
            if (preNLSaveBlock1Ptr->bagPocket_Berries[i].itemId != ITEM_NONE && (preNLSaveBlock1Ptr->bagPocket_Berries[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey) > 0)
            {
                itemId = preNLSaveBlock1Ptr->bagPocket_Berries[i].itemId;
                quantity = preNLSaveBlock1Ptr->bagPocket_Berries[i].quantity ^ preNLSaveBlock2Ptr->encryptionKey;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }
    }
    else
    {
        for (i = 0; i < BAG_ITEMS_COUNT_RS; i++)
        {
            if (rsSaveBlock1Ptr->bagPocket_Items[i].itemId != ITEM_NONE && rsSaveBlock1Ptr->bagPocket_Items[i].quantity > 0)
            {
                itemId = rsSaveBlock1Ptr->bagPocket_Items[i].itemId;
                quantity = rsSaveBlock1Ptr->bagPocket_Items[i].quantity;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_KEYITEMS_COUNT_RS; i++)
        {
            if (rsSaveBlock1Ptr->bagPocket_KeyItems[i].itemId != ITEM_NONE && rsSaveBlock1Ptr->bagPocket_KeyItems[i].quantity > 0)
            {
                itemId = rsSaveBlock1Ptr->bagPocket_KeyItems[i].itemId;
                quantity = rsSaveBlock1Ptr->bagPocket_KeyItems[i].quantity;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_POKEBALLS_COUNT_RS; i++)
        {
            if (rsSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId != ITEM_NONE && rsSaveBlock1Ptr->bagPocket_PokeBalls[i].quantity > 0)
            {
                itemId = rsSaveBlock1Ptr->bagPocket_PokeBalls[i].itemId;
                quantity = rsSaveBlock1Ptr->bagPocket_PokeBalls[i].quantity;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_TMHM_COUNT_RS; i++)
        {
            if (rsSaveBlock1Ptr->bagPocket_TMHM[i].itemId != ITEM_NONE && rsSaveBlock1Ptr->bagPocket_TMHM[i].quantity > 0)
            {
                itemId = rsSaveBlock1Ptr->bagPocket_TMHM[i].itemId;
                quantity = rsSaveBlock1Ptr->bagPocket_TMHM[i].quantity;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }

        for (i = 0; i < BAG_BERRIES_COUNT_RS; i++)
        {
            if (rsSaveBlock1Ptr->bagPocket_Berries[i].itemId != ITEM_NONE && rsSaveBlock1Ptr->bagPocket_Berries[i].quantity > 0)
            {
                itemId = rsSaveBlock1Ptr->bagPocket_Berries[i].itemId;
                quantity = rsSaveBlock1Ptr->bagPocket_Berries[i].quantity;
                AddItemWhileConverting(nlSaveBlock1Ptr, itemId, quantity);
            }
        }
    }
}
