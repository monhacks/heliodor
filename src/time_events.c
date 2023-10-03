#include "global.h"
#include "time_events.h"
#include "event_data.h"
#include "field_weather.h"
#include "pokemon.h"
#include "random.h"
#include "overworld.h"
#include "rtc.h"
#include "script.h"
#include "task.h"

static const u16 sRespawnableHiddenItems[] =
{
	FLAG_HIDDEN_ITEM_UNDERWATER_124_CARBOS,
	FLAG_HIDDEN_ITEM_UNDERWATER_124_GREEN_SHARD,
	FLAG_HIDDEN_ITEM_UNDERWATER_124_PEARL,
	FLAG_HIDDEN_ITEM_UNDERWATER_124_BIG_PEARL,
	FLAG_HIDDEN_ITEM_UNDERWATER_124_HEART_SCALE_1,
	FLAG_HIDDEN_ITEM_UNDERWATER_124_CALCIUM,
	FLAG_HIDDEN_ITEM_UNDERWATER_124_HEART_SCALE_2,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_HEART_SCALE,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_ULTRA_BALL,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_STARDUST,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_PEARL,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_IRON,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_YELLOW_SHARD,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_BIG_PEARL,
	FLAG_HIDDEN_ITEM_UNDERWATER_126_BLUE_SHARD,
	FLAG_HIDDEN_ITEM_UNDERWATER_127_STAR_PIECE,
	FLAG_HIDDEN_ITEM_UNDERWATER_127_HP_UP,
	FLAG_HIDDEN_ITEM_UNDERWATER_127_HEART_SCALE,
	FLAG_HIDDEN_ITEM_UNDERWATER_127_RED_SHARD,
	FLAG_HIDDEN_ITEM_UNDERWATER_128_PROTEIN,
	FLAG_HIDDEN_ITEM_UNDERWATER_128_PEARL,
	FLAG_HIDDEN_ITEM_FALLARBOR_TOWN_NUGGET,
	FLAG_HIDDEN_ITEM_LAVARIDGE_TOWN_ICE_HEAL,
	FLAG_HIDDEN_ITEM_ARTISAN_CAVE_B1F_CALCIUM,
	FLAG_HIDDEN_ITEM_ARTISAN_CAVE_B1F_ZINC,
	FLAG_HIDDEN_ITEM_ARTISAN_CAVE_B1F_PROTEIN,
	FLAG_HIDDEN_ITEM_ARTISAN_CAVE_B1F_IRON,
	FLAG_HIDDEN_ITEM_GRANITE_CAVE_B2F_EVERSTONE_1,
	FLAG_HIDDEN_ITEM_GRANITE_CAVE_B2F_EVERSTONE_2,
	FLAG_HIDDEN_ITEM_JAGGED_PASS_GREAT_BALL,
	FLAG_HIDDEN_ITEM_JAGGED_PASS_FULL_HEAL,
	FLAG_HIDDEN_ITEM_LILYCOVE_CITY_HEART_SCALE,
	FLAG_HIDDEN_ITEM_LILYCOVE_CITY_PP_UP,
	FLAG_HIDDEN_ITEM_LILYCOVE_CITY_POKE_BALL,
	FLAG_HIDDEN_ITEM_MT_PYRE_EXTERIOR_ULTRA_BALL,
	FLAG_HIDDEN_ITEM_MT_PYRE_EXTERIOR_MAX_ETHER,
	FLAG_HIDDEN_ITEM_MT_PYRE_SUMMIT_ZINC,
	FLAG_HIDDEN_ITEM_MT_PYRE_SUMMIT_RARE_CANDY,
	FLAG_HIDDEN_ITEM_PETALBURG_CITY_RARE_CANDY,
	FLAG_HIDDEN_ITEM_PETALBURG_WOODS_POTION,
	FLAG_HIDDEN_ITEM_PETALBURG_WOODS_TINY_MUSHROOM_1,
	FLAG_HIDDEN_ITEM_PETALBURG_WOODS_TINY_MUSHROOM_2,
	FLAG_HIDDEN_ITEM_PETALBURG_WOODS_POKE_BALL,
	FLAG_HIDDEN_ITEM_VICTORY_ROAD_B2F_ELIXIR,
	FLAG_HIDDEN_ITEM_VICTORY_ROAD_B2F_MAX_REPEL,
	FLAG_HIDDEN_ITEM_SAFARI_ZONE_NORTH_EAST_RARE_CANDY,
	FLAG_HIDDEN_ITEM_SAFARI_ZONE_NORTH_EAST_ZINC,
	FLAG_HIDDEN_ITEM_SAFARI_ZONE_SOUTH_EAST_PP_UP,
	FLAG_HIDDEN_ITEM_SAFARI_ZONE_SOUTH_EAST_FULL_RESTORE,
	FLAG_HIDDEN_ITEM_VICTORY_ROAD_1F_ULTRA_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_104_SUPER_POTION,
	FLAG_HIDDEN_ITEM_ROUTE_104_POKE_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_104_POTION,
	FLAG_HIDDEN_ITEM_ROUTE_104_ANTIDOTE,
	FLAG_HIDDEN_ITEM_ROUTE_104_HEART_SCALE,
	FLAG_HIDDEN_ITEM_ROUTE_105_HEART_SCALE,
	FLAG_HIDDEN_ITEM_ROUTE_105_BIG_PEARL,
	FLAG_HIDDEN_ITEM_ROUTE_106_POKE_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_106_STARDUST,
	FLAG_HIDDEN_ITEM_ROUTE_106_HEART_SCALE,
	FLAG_HIDDEN_ITEM_ROUTE_108_RARE_CANDY,
	FLAG_HIDDEN_ITEM_ROUTE_109_REVIVE,
	FLAG_HIDDEN_ITEM_ROUTE_109_HEART_SCALE_1,
	FLAG_HIDDEN_ITEM_ROUTE_109_GREAT_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_109_ETHER,
	FLAG_HIDDEN_ITEM_ROUTE_109_HEART_SCALE_2,
	FLAG_HIDDEN_ITEM_ROUTE_109_HEART_SCALE_3,
	FLAG_HIDDEN_ITEM_ROUTE_110_REVIVE,
	FLAG_HIDDEN_ITEM_ROUTE_110_GREAT_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_110_POKE_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_110_FULL_HEAL,
	FLAG_HIDDEN_ITEM_ROUTE_111_STARDUST,
	FLAG_HIDDEN_ITEM_ROUTE_111_PROTEIN,
	FLAG_HIDDEN_ITEM_ROUTE_111_RARE_CANDY,
	FLAG_HIDDEN_ITEM_ROUTE_113_ETHER,
	FLAG_HIDDEN_ITEM_ROUTE_113_TM_DOUBLE_TEAM,
	FLAG_HIDDEN_ITEM_ROUTE_113_NUGGET,
	FLAG_HIDDEN_ITEM_ROUTE_114_CARBOS,
	FLAG_HIDDEN_ITEM_ROUTE_114_REVIVE,
	FLAG_HIDDEN_ITEM_ROUTE_115_HEART_SCALE,
	FLAG_HIDDEN_ITEM_ROUTE_116_SUPER_POTION,
	FLAG_HIDDEN_ITEM_ROUTE_116_BLACK_GLASSES,
	FLAG_HIDDEN_ITEM_ROUTE_117_REPEL,
	FLAG_HIDDEN_ITEM_ROUTE_118_HEART_SCALE,
	FLAG_HIDDEN_ITEM_ROUTE_118_IRON,
	FLAG_HIDDEN_ITEM_ROUTE_119_CALCIUM,
	FLAG_HIDDEN_ITEM_ROUTE_119_ULTRA_BALL,
	FLAG_HIDDEN_ITEM_ROUTE_119_FULL_HEAL,
	FLAG_HIDDEN_ITEM_ROUTE_119_MAX_ETHER,
	FLAG_HIDDEN_ITEM_ROUTE_120_RARE_CANDY_1,
	FLAG_HIDDEN_ITEM_ROUTE_120_REVIVE,
	FLAG_HIDDEN_ITEM_ROUTE_120_RARE_CANDY_2,
	FLAG_HIDDEN_ITEM_ROUTE_120_ZINC,
	FLAG_HIDDEN_ITEM_ROUTE_121_HP_UP,
	FLAG_HIDDEN_ITEM_ROUTE_121_NUGGET,
	FLAG_HIDDEN_ITEM_ROUTE_121_FULL_HEAL,
	FLAG_HIDDEN_ITEM_ROUTE_121_MAX_REVIVE,
	FLAG_HIDDEN_ITEM_ROUTE_123_SUPER_REPEL,
	FLAG_HIDDEN_ITEM_ROUTE_123_REVIVE,
	FLAG_HIDDEN_ITEM_ROUTE_123_HYPER_POTION,
	FLAG_HIDDEN_ITEM_ROUTE_123_PP_UP,
	FLAG_HIDDEN_ITEM_ROUTE_123_RARE_CANDY,
	FLAG_HIDDEN_ITEM_ROUTE_128_HEART_SCALE_1,
	FLAG_HIDDEN_ITEM_ROUTE_128_HEART_SCALE_2,
    FLAG_HIDDEN_ITEM_ROUTE_128_HEART_SCALE_3,
};

static u32 GetMirageRnd(void)
{
    u32 hi = VarGet(VAR_MIRAGE_RND_H);
    u32 lo = VarGet(VAR_MIRAGE_RND_L);
    return (hi << 16) | lo;
}

static void SetMirageRnd(u32 rnd)
{
    VarSet(VAR_MIRAGE_RND_H, rnd >> 16);
    VarSet(VAR_MIRAGE_RND_L, rnd);
}

// unused
void InitMirageRnd(void)
{
    SetMirageRnd((Random() << 16) | Random());
}

void UpdateMirageRnd(u16 days)
{
    s32 rnd = GetMirageRnd();
    while (days)
    {
        rnd = ISO_RANDOMIZE2(rnd);
        days--;
    }
    SetMirageRnd(rnd);
}

bool8 IsMirageIslandPresent(void)
{
    u16 rnd = GetMirageRnd() >> 16;
    int i;

    for (i = 0; i < PARTY_SIZE; i++)
        if ((GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) && (GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY) & 0xFFFF) == rnd) || (FlagGet(FLAG_SYS_GAME_CLEAR) && (!FlagGet(FLAG_DEFEATED_MEWTWO) || IsMewtwoInParty())))
            return TRUE;

    return FALSE;
}

void UpdateShoalTideFlag(void)
{
    static const u8 tide[] =
    {
        1, // 00
        1, // 01
        1, // 02
        0, // 03
        0, // 04
        0, // 05
        0, // 06
        0, // 07
        0, // 08
        1, // 09
        1, // 10
        1, // 11
        1, // 12
        1, // 13
        1, // 14
        0, // 15
        0, // 16
        0, // 17
        0, // 18
        0, // 19
        0, // 20
        1, // 21
        1, // 22
        1, // 23
    };

    if (IsMapTypeOutdoors(GetLastUsedWarpMapType()))
    {
        RtcCalcLocalTime();
        if (tide[gLocalTime.hours])
            FlagSet(FLAG_SYS_SHOAL_TIDE);
        else
            FlagClear(FLAG_SYS_SHOAL_TIDE);
    }
}

static void Task_WaitWeather(u8 taskId)
{
    if (IsWeatherChangeComplete())
    {
        ScriptContext_Enable();
        DestroyTask(taskId);
    }
}

void WaitWeather(void)
{
    CreateTask(Task_WaitWeather, 80);
}

void InitBirchState(void)
{
    *GetVarPointer(VAR_BIRCH_STATE) = 0;
}

void UpdateBirchState(u16 days)
{
    u16 *state = GetVarPointer(VAR_BIRCH_STATE);
    *state += days;
    *state %= 7;
}

void CycleAlteringCave(u16 days)
{
	VarSet(VAR_ALTERING_CAVE_WILD_SET, days % 7 + 1);
}

void RespawnHiddenItems(u16 days)
{
	u16 i, j, k;
	u8 numberOfFoundItems;
	u8 numberOfHiddenItems = ARRAY_COUNT(sRespawnableHiddenItems);
	u16 foundItems[numberOfHiddenItems];
	
	for (i = 0; i < days; i++)
	{
		numberOfFoundItems = 0;
		for (j = 0; j < numberOfHiddenItems; j++)
		{
			if (FlagGet(sRespawnableHiddenItems[j]))
			{
				foundItems[numberOfFoundItems] =  sRespawnableHiddenItems[j];
				numberOfFoundItems++;
			}
		}
	
		if (numberOfFoundItems == 0)
			return;
		
		k = Random() % numberOfFoundItems;
		FlagClear(foundItems[k]);
	}
}
