/*-------------------------------------------------------------------------------

BARONY
File: stat.cpp
Desc: shared functions for the Stat struct within editor.exe and barony.exe

Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
See LICENSE for details.

-------------------------------------------------------------------------------*/


#include "main.hpp"
#include "game.hpp"
#include "stat.hpp"
#include "entity.hpp"
#include "items.hpp"
#include "prng.hpp"

// Constructor
Stat::Stat(Sint32 sprite) :
	sneaking(MISC_FLAGS[1]),
	allyItemPickup(MISC_FLAGS[2]),
	allyClass(MISC_FLAGS[3]),
	playerRace(MISC_FLAGS[4]),
	playerPolymorphStorage(MISC_FLAGS[5]),
	playerSummonLVLHP(MISC_FLAGS[6]),
	playerSummonSTRDEXCONINT(MISC_FLAGS[7]),
	playerSummonPERCHR(MISC_FLAGS[8]),
	playerSummon2LVLHP(MISC_FLAGS[9]),
	playerSummon2STRDEXCONINT(MISC_FLAGS[10]),
	playerSummon2PERCHR(MISC_FLAGS[11]),
	monsterIsCharmed(MISC_FLAGS[12]),
	playerShapeshiftStorage(MISC_FLAGS[13]),
	monsterTinkeringStatus(MISC_FLAGS[14]),
	monsterMimicLockedBy(MISC_FLAGS[14]),
	monsterDemonHasBeenExorcised(MISC_FLAGS[15]),
	bleedInflictedBy(MISC_FLAGS[17]),
	burningInflictedBy(MISC_FLAGS[18]),
	monsterNoDropItems(MISC_FLAGS[19]),
	monsterForceAllegiance(MISC_FLAGS[20])
{
	this->type = NOTHING;
	strcpy(this->name, "");
	strcpy(this->obituary, Language::get(1500));
	this->defending = false;
	this->poisonKiller = 0;
	this->burningInflictedBy = 0;
	this->bleedInflictedBy = 0;
	this->killer = KilledBy::UNKNOWN;
	this->killer_monster = NOTHING;
	this->killer_uid = 0;
	this->killer_item = WOODEN_SHIELD;
	this->killer_name = "";
	this->sex = static_cast<sex_t>(local_rng.rand() % 2);
	this->stat_appearance = 0;
	this->HP = 10;
	this->MAXHP = 10;
	this->OLDHP = this->HP;
	this->MP = 10;
	this->MAXMP = 10;
	this->STR = 0;
	this->DEX = 0;
	this->CON = 0;
	this->INT = 0;
	this->PER = 0;
	this->CHR = 0;
	this->EXP = 0;
	this->LVL = 1;
	this->GOLD = 0;
	this->HUNGER = 800;

	//random variables to add to base
	this->RANDOM_LVL = 0;
	this->RANDOM_GOLD = 0;
	this->RANDOM_STR = 0;
	this->RANDOM_DEX = 0;
	this->RANDOM_CON = 0;
	this->RANDOM_INT = 0;
	this->RANDOM_PER = 0;
	this->RANDOM_CHR = 0;
	this->RANDOM_MAXHP = 0;
	this->RANDOM_HP = 0;
	this->RANDOM_MAXMP = 0;
	this->RANDOM_MP = 0;
	int c;
	for ( c = 0; c < std::max<int>(NUMPROFICIENCIES, NUMEFFECTS); c++ )
	{
		if ( c < NUMPROFICIENCIES )
		{
			this->PROFICIENCIES[c] = 0;
		}
		if ( c < NUMEFFECTS )
		{
			this->EFFECTS[c] = false;
		}
		if ( c < NUMEFFECTS )
		{
			this->EFFECTS_TIMERS[c] = 0;
		}
	}

	for ( c = 0; c < ITEM_SLOT_NUM; c = c + ITEM_SLOT_NUMPROPERTIES )
	{
		this->EDITOR_ITEMS[c] = 0;
		this->EDITOR_ITEMS[c + 1] = 0;
		this->EDITOR_ITEMS[c + 2] = 10;
		this->EDITOR_ITEMS[c + 3] = 1;
		this->EDITOR_ITEMS[c + 4] = 1;
		this->EDITOR_ITEMS[c + 5] = 100;
		this->EDITOR_ITEMS[c + 6] = 0;
	}
	for ( c = 0; c < 32; c++ )
	{
		this->MISC_FLAGS[c] = 0;
	}

	for ( c = 0; c < NUMSTATS; c++ )
	{
		this->PLAYER_LVL_STAT_BONUS[c] = -1;
	}

	for ( c = 0; c < NUMSTATS * 2; c++ )
	{
		this->PLAYER_LVL_STAT_TIMER[c] = 0;
	}

	this->leader_uid = 0;
	this->FOLLOWERS.first = NULL;
	this->FOLLOWERS.last = NULL;
	this->inventory.first = NULL;
	this->inventory.last = NULL;
	this->helmet = NULL;
	this->breastplate = NULL;
	this->gloves = NULL;
	this->shoes = NULL;
	this->shield = NULL;
	this->weapon = NULL;
	this->cloak = NULL;
	this->amulet = NULL;
	this->ring = NULL;
	this->mask = NULL;
#if defined(USE_FMOD) || defined(USE_OPENAL)
	this->monster_sound = nullptr;
#endif
	this->monster_idlevar = 1;
	this->magic_effects.first = NULL;
	this->magic_effects.last = NULL;

	if ( multiplayer != CLIENT )
	{
		setDefaultMonsterStats(this, (int)sprite);
	}
}

void setDefaultMonsterStats(Stat* stats, int sprite)
{
	switch ( sprite )
	{
		case 70:
		case (1000 + GNOME):
			stats->type = GNOME;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = 0;
			stats->HP = 50;
			stats->MAXHP = 50;
			stats->MP = 50;
			stats->MAXMP = 50;
			stats->OLDHP = stats->HP;
			stats->STR = 2;
			stats->DEX = 0;
			stats->CON = 4;
			stats->INT = 0;
			stats->PER = 2;
			stats->CHR = -1;
			stats->EXP = 0;
			stats->LVL = 5;
			stats->RANDOM_GOLD = 20;
			stats->GOLD = 40;
			stats->HUNGER = 900;

			stats->setProficiency(PRO_SWORD, 35);
			stats->setProficiency(PRO_MACE, 50);
			stats->setProficiency(PRO_AXE, 45);
			stats->setProficiency(PRO_POLEARM, 25);
			stats->setProficiency(PRO_RANGED, 35);
			stats->setProficiency(PRO_SHIELD, 35);


			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_ARMOR] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_BOOTS] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_GLOVES] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_MASK] = 1;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 33; //Fish
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 10; //Random Gems
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 2; //Winny's report

			break;
		case 71:
		case (1000 + DEVIL):
			stats->type = DEVIL;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			strcpy(stats->name, "Baphomet");
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->HP = 1250;
			stats->MAXHP = stats->HP;
			stats->MP = 2000;
			stats->MAXMP = 2000;
			stats->OLDHP = stats->HP;
			stats->STR = -50;
			stats->DEX = -20;
			stats->CON = 10;
			stats->INT = 50;
			stats->PER = 500;
			stats->CHR = 50;
			stats->EXP = 0;
			stats->LVL = 30;
			stats->HUNGER = 900;

			stats->EFFECTS[EFF_LEVITATING] = true;
			stats->EFFECTS_TIMERS[EFF_LEVITATING] = 0;

			stats->setProficiency(PRO_MAGIC, 100);
			stats->setProficiency(PRO_SPELLCASTING, 100);

			break;
		case 62:
		case (1000 + LICH):
			stats->type = LICH;
			stats->sex = MALE;
			stats->stat_appearance = local_rng.rand();
			strcpy(stats->name, "Baron Herx");
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 1000;
			stats->MAXHP = stats->HP;
			stats->MP = 1000;
			stats->MAXMP = 1000;
			stats->OLDHP = stats->HP;
			stats->STR = 20;
			stats->DEX = 8;
			stats->CON = 8;
			stats->INT = 20;
			stats->PER = 80;
			stats->CHR = 50;
			stats->EXP = 0;
			stats->LVL = 25;
			stats->GOLD = 100;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			break;
		case 48:
		case (1000 + SPIDER):
			stats->type = SPIDER;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 50;
			stats->MAXHP = 50;
			stats->MP = 10;
			stats->MAXMP = 10;
			stats->OLDHP = stats->HP;
			stats->STR = 3;
			stats->DEX = 8;
			stats->CON = 4;
			stats->INT = -3;
			stats->PER = -3;
			stats->CHR = -1;
			stats->EXP = 0;
			stats->LVL = 5;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			break;
		case 36:
		case (1000 + GOBLIN):
			stats->type = GOBLIN;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 60;
			stats->MAXHP = 60;
			stats->MP = 20;
			stats->MAXMP = 20;
			stats->OLDHP = stats->HP;
			stats->STR = 5;
			stats->DEX = 0;
			stats->CON = 3;
			stats->INT = -1;
			stats->PER = 2;
			stats->CHR = -1;
			stats->EXP = 0;
			stats->LVL = 6;
			if ( local_rng.rand() % 3 == 0 )
			{
				stats->GOLD = 10;
				stats->RANDOM_GOLD = 20;
			}
			else
			{
				stats->GOLD = 0;
				stats->RANDOM_GOLD = 0;
			}
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_ARMOR] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_MASK] = 1;

			stats->setProficiency(PRO_SWORD, 35);
			stats->setProficiency(PRO_MACE, 50);
			stats->setProficiency(PRO_AXE, 45);
			stats->setProficiency(PRO_POLEARM, 25);
			stats->setProficiency(PRO_RANGED, 100);
			stats->setProficiency(PRO_SHIELD, 35);

			break;
		case 35:
		case (1000 + SHOPKEEPER):
			stats->type = SHOPKEEPER;
			stats->sex = MALE;
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 300;
			stats->MAXHP = 300;
			stats->MP = 200;
			stats->MAXMP = 200;
			stats->OLDHP = stats->HP;
			stats->STR = 10;
			stats->DEX = 4;
			stats->CON = 10;
			stats->INT = 7;
			stats->PER = 7;
			stats->CHR = 3;
			stats->RANDOM_CHR = 3;
			stats->EXP = 0;
			stats->LVL = 10;
			stats->GOLD = 300;
			stats->RANDOM_GOLD = 200;
			stats->HUNGER = 900;

			stats->FOLLOWERS.first = NULL;
			stats->FOLLOWERS.last = NULL;
			stats->setProficiency(PRO_MAGIC, 50);
			stats->setProficiency(PRO_SPELLCASTING, 50);
			stats->setProficiency(PRO_TRADING, 75);
			stats->setProficiency(PRO_APPRAISAL, 75);

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;

			break;
		case 30:
		case (1000 + TROLL):
			stats->type = TROLL;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 100;
			stats->RANDOM_HP = 20;
			stats->MAXHP = stats->HP;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 30;
			stats->MAXMP = 30;
			stats->OLDHP = stats->HP;
			stats->STR = 15;
			stats->DEX = -2;
			stats->CON = 5;
			stats->INT = -4;
			stats->PER = -2;
			stats->CHR = -1;
			stats->EXP = 0;
			stats->LVL = 12;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 33; //Random Items


			break;
		case 27:
		case (1000 + HUMAN):
			stats->type = HUMAN;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand() % 18; //NUMAPPEARANCES = 18
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 30;
			stats->RANDOM_HP = 20;
			stats->MAXHP = stats->HP;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 20;
			stats->RANDOM_MP = 20;
			stats->RANDOM_MAXMP = stats->RANDOM_MP;
			stats->MAXMP = stats->MP;
			stats->OLDHP = stats->HP;
			stats->STR = -1;
			stats->RANDOM_STR = 3;
			stats->DEX = 4;
			stats->RANDOM_DEX = 3;
			stats->CON = -2;
			stats->RANDOM_CON = 3;
			stats->INT = -1;
			stats->RANDOM_INT = 3;
			stats->PER = 0;
			stats->RANDOM_PER = 4;
			stats->CHR = -3;
			stats->RANDOM_CHR = 3;
			stats->EXP = 0;
			stats->LVL = 3;
			if ( local_rng.rand() % 2 == 0 )
			{
				stats->GOLD = 20;
				stats->RANDOM_GOLD = 20;
			}
			else
			{
				stats->GOLD = 0;
			}
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_ARMOR] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_BOOTS] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_GLOVES] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_MASK] = 1;

			stats->setProficiency(PRO_SWORD, 45);
			stats->setProficiency(PRO_MACE, 35);
			stats->setProficiency(PRO_AXE, 35);
			stats->setProficiency(PRO_POLEARM, 45);
			//stats->setProficiency(PRO_RANGED, 40);
			stats->setProficiency(PRO_SHIELD, 35);

			break;
		case 84:
		case (1000 + KOBOLD):
			stats->type = KOBOLD;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = 0;

			stats->HP = 100;
			stats->MAXHP = stats->HP;
			stats->RANDOM_HP = 20;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 60;
			stats->MAXMP = 60;
			stats->OLDHP = stats->HP;
			stats->STR = 20;
			stats->RANDOM_STR = 5;
			stats->DEX = 5;
			stats->RANDOM_DEX = 5;
			stats->CON = 3;
			stats->RANDOM_CON = 2;
			stats->INT = -2;
			stats->RANDOM_INT = 4;
			stats->PER = 14;
			stats->RANDOM_PER = 2;
			stats->CHR = 3;
			stats->RANDOM_CHR = 2;

			stats->EXP = 0;
			stats->LVL = 15;
			stats->GOLD = 80;
			stats->RANDOM_GOLD = 40;
			stats->HUNGER = 900;

			stats->setProficiency(PRO_SWORD, 75);
			stats->setProficiency(PRO_AXE, 50);
			stats->setProficiency(PRO_POLEARM, 50);
			stats->setProficiency(PRO_RANGED, 75);
			stats->setProficiency(PRO_SHIELD, 35);


			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 80; //Ranged spellbook
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 20; //Misc tools
			break;
		case 85:
		case (1000 + SCARAB):
			stats->type = SCARAB;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 60;
			stats->MAXHP = 60;
			stats->MP = 20;
			stats->MAXMP = 20;
			stats->OLDHP = stats->HP;
			stats->STR = 10;
			stats->DEX = 12;
			stats->CON = 2;
			stats->INT = -1;
			stats->PER = 8;
			stats->CHR = 0;
			stats->EXP = 0;
			stats->LVL = 15;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 50;

			stats->setProficiency(PRO_MAGIC, 50);
			stats->setProficiency(PRO_SPELLCASTING, 50);

			break;
		case 86:
		case (1000 + CRYSTALGOLEM):
			stats->type = CRYSTALGOLEM;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = 0;

			stats->HP = 200;
			stats->MAXHP = stats->HP;
			stats->RANDOM_HP = 50;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 50;
			stats->MAXMP = 50;
			stats->OLDHP = stats->HP;
			stats->STR = 50;
			stats->RANDOM_STR = 5;
			stats->DEX = 2;
			stats->RANDOM_DEX = 2;
			stats->CON = 25;
			stats->RANDOM_CON = 0;
			stats->INT = -2;
			stats->RANDOM_INT = 0;
			stats->PER = 5;
			stats->RANDOM_PER = 5;
			stats->CHR = -3;
			stats->RANDOM_CHR = 0;

			stats->EXP = 0;
			stats->LVL = 30;
			stats->GOLD = 0;
			stats->RANDOM_GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 50; //Random crystal item
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 5; //Random second crystal item
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 10; //Gem
			break;
		case 87:
		case (1000 + INCUBUS):
			stats->type = INCUBUS;
			stats->sex = sex_t::MALE;
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->MAXHP = 280;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 50;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->RANDOM_MAXHP = 25;
			stats->RANDOM_HP = stats->RANDOM_MAXHP;
			stats->RANDOM_MAXMP = 0;
			stats->RANDOM_MP = stats->RANDOM_MAXMP;
			stats->STR = 20;
			stats->RANDOM_STR = 5;
			stats->DEX = 8;
			stats->CON = 3;
			stats->RANDOM_CON = 2;
			stats->INT = -2;
			stats->RANDOM_INT = 2;
			stats->PER = 10;
			stats->RANDOM_PER = 5;
			stats->CHR = -3;
			stats->EXP = 0;
			stats->LVL = 25;

			stats->GOLD = 50;
			stats->RANDOM_GOLD = 50;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			//stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			//stats->EDITOR_ITEMS[ITEM_SLOT_BOOTS] = 1;
			//stats->EDITOR_ITEMS[ITEM_SLOT_GLOVES] = 1;

			stats->setProficiency(PRO_MACE, 75);
			stats->setProficiency(PRO_POLEARM, 60);
			stats->setProficiency(PRO_RANGED, 75);
			stats->setProficiency(PRO_MAGIC, 100);
			stats->setProficiency(PRO_LEADERSHIP, 60);

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 33; // booze potion
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 20; // confusion potion
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 50; // magicstaff

			break;
		case 88:
		case (1000 + VAMPIRE):
			stats->type = VAMPIRE;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->HP = 400;
			stats->RANDOM_HP = 0;
			stats->MAXHP = stats->HP;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 50;
			stats->RANDOM_MP = 50;
			stats->RANDOM_MAXMP = stats->RANDOM_MP;
			stats->MAXMP = stats->MP;
			stats->OLDHP = stats->HP;
			stats->STR = 40;
			stats->RANDOM_STR = 5;
			stats->DEX = 8;
			stats->RANDOM_DEX = 0;
			stats->CON = 0;
			stats->RANDOM_CON = 0;
			stats->INT = 15;
			stats->RANDOM_INT = 5;
			stats->PER = 5;
			stats->RANDOM_PER = 5;
			stats->CHR = -3;
			stats->RANDOM_CHR = -3;
			stats->EXP = 0;
			stats->LVL = 30;
			stats->GOLD = 130;
			stats->RANDOM_GOLD = 70;
			stats->HUNGER = 900;

			/*stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_ARMOR] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_BOOTS] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_GLOVES] = 1;*/

			//stats->setProficiency(PRO_SWORD, 45);
			//stats->setProficiency(PRO_MACE, 35);
			stats->setProficiency(PRO_AXE, 25);
			//stats->setProficiency(PRO_POLEARM, 45);
			stats->setProficiency(PRO_RANGED, 25);
			stats->setProficiency(PRO_SHIELD, 25);
			stats->setProficiency(PRO_MAGIC, 80);
			stats->setProficiency(PRO_SPELLCASTING, 80);

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 10; // doublet
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 25; // magicstaff
			break;
		case 89:
		case (1000 + SHADOW):
			stats->type = SHADOW;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->RANDOM_MAXMP = stats->RANDOM_MP;
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->MAXHP = 170;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 500;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->STR = 20;
			stats->RANDOM_STR = 5;
			stats->DEX = 10;
			stats->RANDOM_DEX = 5;
			stats->CON = 2;
			stats->RANDOM_CON = 2;
			stats->INT = 5;
			stats->RANDOM_INT = 4;
			stats->PER = 20;
			stats->RANDOM_PER = 5;
			stats->CHR = -1;
			stats->RANDOM_CHR = 2;
			stats->EXP = 0;
			stats->LVL = 25;
			stats->HUNGER = 900;
			stats->GOLD = 0;
			stats->RANDOM_GOLD = 0;
			stats->setProficiency(PRO_SWORD, 90);
			stats->setProficiency(PRO_MACE, 90);
			stats->setProficiency(PRO_AXE, 90);
			stats->setProficiency(PRO_POLEARM, 90);
			stats->setProficiency(PRO_RANGED, 60);
			stats->setProficiency(PRO_SHIELD, 25);
			stats->setProficiency(PRO_MAGIC, 80);
			stats->setProficiency(PRO_SPELLCASTING, 80);

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 5; //Spooky mask
			break;
		case 90:
		case (1000 + COCKATRICE):
			stats->type = COCKATRICE;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = 0;

			stats->HP = 500;
			stats->MAXHP = stats->HP;
			stats->RANDOM_HP = 100;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 100;
			stats->MAXMP = 100;
			stats->OLDHP = stats->HP;
			stats->STR = 65;
			stats->RANDOM_STR = 10;
			stats->DEX = 8;
			stats->RANDOM_DEX = 0;
			stats->CON = 20;
			stats->RANDOM_CON = 0;
			stats->INT = -2;
			stats->RANDOM_INT = 0;
			stats->PER = 25;
			stats->RANDOM_PER = 0;
			stats->CHR = -3;
			stats->RANDOM_CHR = 0;

			stats->EXP = 0;
			stats->LVL = 35;
			stats->GOLD = 50;
			stats->RANDOM_GOLD = 100;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 100; // random potion, qty 1-3
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 30; // magicstaff
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 20; // gemstones, qty 1-2
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_4] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_4 + ITEM_CHANCE] = 5; // spellbook
			break;
		case 91:
		case (1000 + INSECTOID):
			stats->type = INSECTOID;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->MAXHP = 130;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 50;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->RANDOM_MAXHP = 25;
			stats->RANDOM_HP = stats->RANDOM_MAXHP;
			stats->RANDOM_MAXMP = 0;
			stats->RANDOM_MP = stats->RANDOM_MAXMP;
			stats->STR = 18;
			stats->RANDOM_STR = 4;
			stats->DEX = 9;
			stats->CON = 15;
			stats->RANDOM_CON = 0;
			stats->INT = -2;
			stats->RANDOM_INT = 2;
			stats->PER = 15;
			stats->RANDOM_PER = 0;
			stats->CHR = -3;
			stats->EXP = 0;
			stats->LVL = 25;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_ARMOR] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_BOOTS] = 1;
			//stats->EDITOR_ITEMS[ITEM_SLOT_GLOVES] = 1;

			stats->setProficiency(PRO_SWORD, 50);
			//stats->setProficiency(PRO_MACE, 35);
			stats->setProficiency(PRO_AXE, 35);
			stats->setProficiency(PRO_POLEARM, 60);
			stats->setProficiency(PRO_RANGED, 50);
			stats->setProficiency(PRO_SHIELD, 35);

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 100; // iron daggers, qty 2-8
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 50; // shortbow
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 5; // spellbook
			//stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			//stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 20; // gemstones, qty 1-2
			//stats->EDITOR_ITEMS[ITEM_SLOT_INV_4] = 1;
			//stats->EDITOR_ITEMS[ITEM_SLOT_INV_4 + ITEM_CHANCE] = 5; // spellbook
			break;
		case 92:
		case (1000 + GOATMAN):
			stats->type = GOATMAN;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->MAXHP = 220;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 20;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->RANDOM_MAXHP = 20;
			stats->RANDOM_HP = stats->RANDOM_MAXHP;
			//stats->RANDOM_MAXMP = 20;
			//stats->RANDOM_MP = stats->RANDOM_MAXMP;
			stats->STR = 21;
			stats->DEX = 8;
			stats->CON = 9;
			stats->INT = -1;
			stats->PER = 0;
			stats->RANDOM_PER = 5;
			stats->CHR = -1;
			stats->EXP = 0;
			stats->LVL = 25;
			if ( local_rng.rand() % 3 > 0 )
			{
				stats->GOLD = 100;
				stats->RANDOM_GOLD = 50;
			}
			else
			{
				stats->GOLD = 0;
				stats->RANDOM_GOLD = 0;
			}
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_ARMOR] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_CLOAK] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_BOOTS] = 1;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 5; // spellbook

			//stats->setProficiency(PRO_SWORD, 35);
			stats->setProficiency(PRO_MACE, 80);
			stats->setProficiency(PRO_AXE, 60);
			//stats->setProficiency(PRO_POLEARM, 25);
			stats->setProficiency(PRO_RANGED, 60); //Chuck booze at you.
			//stats->setProficiency(PRO_SHIELD, 35);
			stats->setProficiency(PRO_SPELLCASTING, 60);
			stats->setProficiency(PRO_MAGIC, 60);
			break;
		case 93:
		case (1000 + AUTOMATON):
			stats->type = AUTOMATON;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->MAXHP = 115;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 20;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->RANDOM_MAXHP = 20;
			stats->RANDOM_HP = stats->RANDOM_MAXHP;
			//stats->RANDOM_MAXMP = 20;
			//stats->RANDOM_MP = stats->RANDOM_MAXMP;
			stats->STR = 20;
			stats->DEX = 5;
			stats->CON = 8;
			stats->INT = -1;
			stats->PER = 10;
			stats->CHR = -3;
			stats->EXP = 0;
			stats->LVL = 20;
			stats->HUNGER = 900;

			stats->setProficiency(PRO_SWORD, 60);
			stats->setProficiency(PRO_MACE, 60);
			stats->setProficiency(PRO_AXE, 60);
			stats->setProficiency(PRO_RANGED, 60);
			stats->setProficiency(PRO_POLEARM, 60);
			stats->setProficiency(PRO_SHIELD, 60);
			break;
		case 94:
		case (1000 + LICH_ICE):
			stats->type = LICH_ICE;
			stats->sex = FEMALE;
			stats->stat_appearance = local_rng.rand();
			strcpy(stats->name, "Erudyce");
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->HP = 2000;
			stats->MAXHP = stats->HP;
			stats->MP = 1000;
			stats->MAXMP = 1000;
			stats->OLDHP = stats->HP;
			stats->STR = 50;
			stats->DEX = 13;
			stats->CON = 20;
			stats->INT = 20;
			stats->PER = 80;
			stats->CHR = 50;
			stats->EXP = 0;
			stats->LVL = 50;
			stats->GOLD = 100;
			stats->HUNGER = 900;
			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->setProficiency(PRO_RANGED, 100);
			stats->setProficiency(PRO_MAGIC, 100);
			stats->setProficiency(PRO_SPELLCASTING, 100);
			break;
		case 95:
		case (1000 + LICH_FIRE):
			stats->type = LICH_FIRE;
			stats->sex = MALE;
			stats->stat_appearance = local_rng.rand();
			strcpy(stats->name, "Orpheus");
			stats->inventory.first = nullptr;
			stats->inventory.last = nullptr;
			stats->HP = 2000;
			stats->MAXHP = stats->HP;
			stats->MP = 1000;
			stats->MAXMP = 1000;
			stats->OLDHP = stats->HP;
			stats->STR = 50;
			stats->DEX = 13;
			stats->CON = 30;
			stats->INT = 20;
			stats->PER = 80;
			stats->CHR = 50;
			stats->EXP = 0;
			stats->LVL = 50;
			stats->GOLD = 100;
			stats->HUNGER = 900;
			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->setProficiency(PRO_SWORD, 80);
			stats->setProficiency(PRO_MAGIC, 100);
			stats->setProficiency(PRO_SPELLCASTING, 100);
			break;
		case 83:
		case (1000 + SKELETON):
			stats->type = SKELETON;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->HP = 40;
			stats->MAXHP = 40;
			stats->MP = 30;
			stats->MAXMP = 30;
			stats->OLDHP = stats->HP;
			stats->STR = 0;
			stats->DEX = -1;
			stats->CON = 1;
			stats->INT = -1;
			stats->PER = 2;
			stats->CHR = -3;
			stats->EXP = 0;
			stats->LVL = 2;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->setProficiency(PRO_SWORD, 35);
			stats->setProficiency(PRO_MACE, 50);
			stats->setProficiency(PRO_AXE, 45);
			stats->setProficiency(PRO_POLEARM, 25);
			stats->setProficiency(PRO_RANGED, 35);
			stats->setProficiency(PRO_SHIELD, 35);

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_HELM] = 1;

			break;

		case 75:
		case (1000 + DEMON):
			stats->type = DEMON;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 120;
			stats->MAXHP = stats->HP;
			stats->MP = 200;
			stats->MAXMP = 200;
			stats->OLDHP = stats->HP;
			stats->STR = 30;
			stats->DEX = 10;
			stats->CON = 10;
			stats->INT = 5;
			stats->PER = 50;
			stats->CHR = -4;
			stats->EXP = 0;
			stats->LVL = 20;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 50; //Random Items
			break;
		case 76:
		case (1000 + CREATURE_IMP):
			stats->type = CREATURE_IMP;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 80;
			stats->MAXHP = stats->HP;
			stats->MP = 80;
			stats->MAXMP = 80;
			stats->OLDHP = stats->HP;
			stats->STR = 20;
			stats->DEX = 7;
			stats->CON = 9;
			stats->INT = -2;
			stats->PER = 50;
			stats->CHR = -3;
			stats->EXP = 0;
			stats->LVL = 14;
			if ( local_rng.rand() % 10 )
			{
				stats->GOLD = 0;
				stats->RANDOM_GOLD = 0;
			}
			else
			{
				stats->GOLD = 20;
				stats->RANDOM_GOLD = 20;
			}
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 100; //Random Items
			break;
		case 77:
		//case 37:
		case (1000 + MINOTAUR):
			stats->type = MINOTAUR;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 400;
			stats->MAXHP = 400;
			stats->MP = 100;
			stats->MAXMP = 100;
			stats->OLDHP = stats->HP;
			stats->STR = 35;
			stats->DEX = 15;
			stats->CON = 15;
			stats->INT = 5;
			stats->PER = 5;
			stats->CHR = -5;
			stats->EXP = 0;
			stats->LVL = 20;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 100; //Random Items
			break;
		case 78:
		case (1000 + SCORPION):
			stats->type = SCORPION;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 70;
			stats->MAXHP = 70;
			stats->MP = 10;
			stats->MAXMP = 10;
			stats->OLDHP = stats->HP;
			stats->STR = 13;
			stats->DEX = 3;
			stats->CON = 4;
			stats->INT = -3;
			stats->PER = -3;
			stats->CHR = -4;
			stats->EXP = 0;
			stats->LVL = 7;
			stats->GOLD = 0;
			stats->HUNGER = 900;
			break;
		case 79:
		case 193:
		case 194:
		case 195:
		case 196:
		case 197:
		case (1000 + SLIME):
			stats->type = SLIME;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			//if ( stats->LVL >= 7 )   // blue slime
			//{
			//	stats->HP = 70;
			//	stats->MAXHP = 70;
			//	stats->MP = 70;
			//	stats->MAXMP = 70;
			//	stats->STR = 10;
			//}
			//else     // green slime
			//{
			//}
			stats->STR = 3;
			stats->HP = 60;
			stats->MAXHP = 60;
			stats->MP = 60;
			stats->MAXMP = 60;
			stats->OLDHP = stats->HP;
			stats->DEX = -4;
			stats->CON = 3;
			stats->INT = -4;
			stats->PER = -2;
			stats->CHR = -4;
			stats->EXP = 0;
			stats->GOLD = 0;
			stats->HUNGER = 900;
			break;
		case 80:
		case (1000 + SUCCUBUS):
			stats->type = SUCCUBUS;
			stats->sex = FEMALE;
			stats->stat_appearance = local_rng.rand();
			stats->HP = 60;
			stats->MAXHP = 60;
			stats->MP = 40;
			stats->MAXMP = 40;
			stats->OLDHP = stats->HP;
			stats->STR = 7;
			stats->DEX = 3;
			stats->CON = 3;
			stats->INT = 2;
			stats->PER = 2;
			stats->CHR = 5;
			stats->EXP = 0;
			stats->LVL = 10;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->setProficiency(PRO_MAGIC, 60);
			stats->setProficiency(PRO_LEADERSHIP, 40);

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 10; //Magicstaff of charm monster.
			break;
		case 81:
		case (1000 + RAT):
			stats->type = RAT;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 30;
			stats->MAXHP = 30;
			stats->MP = 10;
			stats->MAXMP = 10;
			stats->OLDHP = stats->HP;
			stats->STR = 0;
			stats->DEX = 2;
			stats->CON = 1;
			stats->INT = -2;
			stats->PER = 0;
			stats->CHR = -1;
			stats->EXP = 0;
			stats->LVL = 1;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 25; //Random Items
			break;
		case 82:
		case (1000 + GHOUL):
			stats->type = GHOUL;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 90;
			stats->MAXHP = 90;
			stats->MP = 10;
			stats->MAXMP = 10;
			stats->OLDHP = stats->HP;
			stats->STR = 8;
			stats->DEX = -3;
			stats->CON = -1;
			stats->INT = -2;
			stats->PER = -1;
			stats->CHR = -5;
			stats->EXP = 0;
			stats->LVL = 7;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 5; //Random Items
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_2 + ITEM_CHANCE] = 10; //Random Items
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_3 + ITEM_CHANCE] = 25; //Random Items
			break;
		case (1000 + SENTRYBOT):
		case (1000 + SPELLBOT):
		case 163:
		case 164:
			if ( (sprite == 1000 + SENTRYBOT) || sprite == 163 )
			{
				stats->type = SENTRYBOT;
			}
			else if ( (sprite == 1000 + SPELLBOT ) || sprite == 164 )
			{
				stats->type = SPELLBOT;
			}
			stats->MAXHP = 50;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 50;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->setProficiency(PRO_RANGED, 80);
			stats->STR = 0;
			stats->DEX = 0;
			stats->CON = 0;
			stats->PER = 4;
			stats->CHR = 0;
			stats->EXP = 0;
			stats->LVL = 3;
			stats->monsterTinkeringStatus = DECREPIT; // store the type of item that was used to summon me.
			break;
		case 165:
		case (1000 + DUMMYBOT):
			stats->type = DUMMYBOT;
			stats->MAXHP = 50;
			stats->HP = stats->MAXHP;
			stats->MAXMP = 10;
			stats->MP = stats->MAXMP;
			stats->OLDHP = stats->HP;
			stats->STR = 0;
			stats->DEX = 0;
			stats->CON = 5;
			stats->PER = 0;
			stats->CHR = 0;
			stats->EXP = 0;
			stats->LVL = 3;
			stats->monsterTinkeringStatus = DECREPIT; // store the type of item that was used to summon me.
			break;
		case 166:
		case (1000 + GYROBOT):
			stats->HP = 10;
			stats->HP = stats->MAXHP;
			stats->OLDHP = stats->HP;
			stats->STR = 0;
			stats->DEX = 5;
			stats->CON = 0;
			stats->PER = 0;
			stats->CHR = 0;
			stats->EXP = 0;
			stats->LVL = 1;
			stats->monsterTinkeringStatus = DECREPIT; // store the type of item that was used to summon me.
			break;
		case (1000 + MIMIC):
			stats->type = MIMIC;
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->MAXHP = 90;
			stats->HP = stats->MAXHP;
			stats->OLDHP = stats->HP;
			stats->RANDOM_MAXHP = 20;
			stats->RANDOM_HP = stats->RANDOM_MAXHP;
			stats->STR = 0;
			stats->DEX = 0;
			stats->CON = 0;
			stats->PER = 5;
			stats->CHR = 0;
			stats->EXP = 0;
			stats->LVL = 10;
			stats->GOLD = 0;
			stats->RANDOM_GOLD = 0;
			break;
		case 188:
		case (1000 + BAT_SMALL):
			stats->type = BAT_SMALL;
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->MAXHP = 10;
			stats->HP = stats->MAXHP;
			stats->OLDHP = stats->HP;
			stats->STR = 0;
			stats->DEX = 0;
			stats->CON = 0;
			stats->INT = -5;
			stats->PER = 10;
			stats->CHR = 0;
			stats->EXP = 0;
			stats->LVL = 1;
			stats->GOLD = 0;
			stats->RANDOM_GOLD = 0;
			break;
		case 189:
		case (1000 + BUGBEAR):
			stats->type = BUGBEAR;
			stats->sex = static_cast<sex_t>(local_rng.rand() % 2);
			stats->stat_appearance = local_rng.rand();
			stats->inventory.first = NULL;
			stats->inventory.last = NULL;
			stats->HP = 130;
			stats->RANDOM_HP = 20;
			stats->MAXHP = stats->HP;
			stats->RANDOM_MAXHP = stats->RANDOM_HP;
			stats->MP = 30;
			stats->MAXMP = 30;
			stats->OLDHP = stats->HP;
			stats->STR = 12;
			stats->DEX = 3;
			stats->CON = 8;
			stats->INT = -4;
			stats->PER = 6;
			stats->CHR = -4;
			stats->EXP = 0;
			stats->LVL = 14;
			stats->GOLD = 0;
			stats->HUNGER = 900;

			stats->setProficiency(PRO_SWORD, 60);
			stats->setProficiency(PRO_AXE, 60);
			stats->setProficiency(PRO_RANGED, 0);
			stats->setProficiency(PRO_SHIELD, 80);

			stats->EDITOR_ITEMS[ITEM_SLOT_WEAPON] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_SHIELD] = 1;

			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1] = 1;
			stats->EDITOR_ITEMS[ITEM_SLOT_INV_1 + ITEM_CHANCE] = 33; //Random Items
			break;
		case 10:
		default:
			break;
	}
}

bool isMonsterStatsDefault(Stat& myStats)
{
	Stat baseStats(myStats.type + 1000);
	if ( baseStats.HP == myStats.HP
		&& baseStats.MP == myStats.MP
		&& baseStats.RANDOM_HP == myStats.RANDOM_HP
		&& baseStats.RANDOM_MP == myStats.RANDOM_MP
		&& baseStats.MAXHP == myStats.MAXHP
		&& baseStats.MAXMP == myStats.MAXMP
		&& baseStats.RANDOM_MAXHP == myStats.RANDOM_MAXHP
		&& baseStats.RANDOM_MAXMP == myStats.RANDOM_MAXMP
		&& baseStats.STR == myStats.STR
		&& baseStats.DEX == myStats.DEX
		&& baseStats.CON == myStats.CON
		&& baseStats.INT == myStats.INT
		&& (baseStats.PER == myStats.PER || (baseStats.PER == -3 && myStats.type == HUMAN)) // increased human PER from -3 to 0.
		&& baseStats.CHR == myStats.CHR
		&& baseStats.LVL == myStats.LVL
		&& baseStats.RANDOM_LVL == myStats.RANDOM_LVL
		&& baseStats.RANDOM_STR == myStats.RANDOM_STR
		&& baseStats.RANDOM_DEX == myStats.RANDOM_DEX
		&& baseStats.RANDOM_CON == myStats.RANDOM_CON
		&& baseStats.RANDOM_INT == myStats.RANDOM_INT
		&& baseStats.RANDOM_PER == myStats.RANDOM_PER
		&& baseStats.RANDOM_CHR == myStats.RANDOM_CHR )
	{
		for ( int i = 0; i < 112; ++i )
		{
			if ( baseStats.EDITOR_ITEMS[i] != myStats.EDITOR_ITEMS[i] )
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

void Stat::setAttribute(std::string key, std::string value)
{
	attributes[key] = value;
}