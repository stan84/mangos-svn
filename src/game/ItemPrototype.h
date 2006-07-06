/* 
 * Copyright (C) 2005,2006 MaNGOS <http://www.mangosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _ITEMPROTOTYPE_H
#define _ITEMPROTOTYPE_H

#include "Common.h"

enum ITEM_STAT_TYPE
{
    POWER      = 0,
    HEALTH     = 1,
    UNKNOWN    = 2,
    AGILITY    = 3,
    STRENGHT   = 4,
    INTELLECT  = 5,
    SPIRIT     = 6,
    STAMINA    = 7,
};

enum ITEM_DAMAGE_TYPE
{
    NORMAL_DAMAGE  = 0,
    HOLY_DAMAGE    = 1,
    FIRE_DAMAGE    = 2,
    NATURE_DAMAGE  = 3,
    FROST_DAMAGE   = 4,
    SHADOW_DAMAGE  = 5,
    ARCANE_DAMAGE  = 6,
};

enum ITEM_SPELLTRIGGER_TYPE
{
    USE           = 0,
    ON_EQUIP      = 1,
    CHANCE_ON_HIT = 2,
    SOULSTONE     = 4,
};

enum ITEM_BONDING_TYPE
{
    NO_BIND              = 0,
    BIND_WHEN_PICKED_UP  = 1,
    BIND_WHEN_EQUIPED    = 2,
    BIND_WHEN_USE        = 3,
    //TODO: Better name these
    QUEST_ITEM           = 4,
    QUEST_ITEM1          = 5,
};

enum INVENTORY_TYPES
{
    INVTYPE_NON_EQUIP      = 0,
    INVTYPE_HEAD           = 1,
    INVTYPE_NECK           = 2,
    INVTYPE_SHOULDERS      = 3,
    INVTYPE_BODY           = 4,
    INVTYPE_CHEST          = 5,
    INVTYPE_WAIST          = 6,
    INVTYPE_LEGS           = 7,
    INVTYPE_FEET           = 8,
    INVTYPE_WRISTS         = 9,
    INVTYPE_HANDS          = 10,
    INVTYPE_FINGER         = 11,
    INVTYPE_TRINKET        = 12,
    INVTYPE_WEAPON         = 13,
    INVTYPE_SHIELD         = 14,
    INVTYPE_RANGED         = 15,
    INVTYPE_CLOAK          = 16,
    INVTYPE_2HWEAPON       = 17,
    INVTYPE_BAG            = 18,
    INVTYPE_TABARD         = 19,
    INVTYPE_ROBE           = 20,
    INVTYPE_WEAPONMAINHAND = 21,
    INVTYPE_WEAPONOFFHAND  = 22,
    INVTYPE_HOLDABLE       = 23,
    INVTYPE_AMMO           = 24,
    INVTYPE_THROWN         = 25,
    INVTYPE_RANGEDRIGHT    = 26,
    INVTYPE_SLOT_ITEM      = 27,
    NUM_INVENTORY_TYPES    = 28,
};

enum INVENTORY_CLASS
{
    ITEM_CLASS_CONSUMABLE    = 0,
    ITEM_CLASS_CONTAINER     = 1,
    ITEM_CLASS_WEAPON        = 2,
    ITEM_CLASS_JEWELRY       = 3,
    ITEM_CLASS_ARMOR         = 4,
    ITEM_CLASS_REAGENT       = 5,
    ITEM_CLASS_PROJECTILE    = 6,
    ITEM_CLASS_TRADE_GOODS   = 7,
    ITEM_CLASS_GENERIC       = 8,
    ITEM_CLASS_BOOK          = 9,
    ITEM_CLASS_MONEY         = 10,
    ITEM_CLASS_QUIVER        = 11,
    ITEM_CLASS_QUEST         = 12,
    ITEM_CLASS_KEY           = 13,
    ITEM_CLASS_PERMANENT     = 14,
    ITEM_CLASS_JUNK          = 15,
};

enum ITEM_SUBCLASS_CONTAINER
{
    ITEM_SUBCLASS_CONTAINER               = 0,
    ITEM_SUBCLASS_SOUL_CONTAINER          = 1,
    ITEM_SUBCLASS_HERB_CONTAINER          = 2,
    ITEM_SUBCLASS_ENCHANTING_CONTAINER    = 3,
    ITEM_SUBCLASS_ENGINEERING_CONTAINER   = 4,
};

enum INVENTORY_SUBCLASS_WEAPON
{
    ITEM_SUBCLASS_WEAPON_AXE           = 0,
    ITEM_SUBCLASS_WEAPON_AXE2          = 1,
    ITEM_SUBCLASS_WEAPON_BOW           = 2,
    ITEM_SUBCLASS_WEAPON_GUN           = 3,
    ITEM_SUBCLASS_WEAPON_MACE          = 4,
    ITEM_SUBCLASS_WEAPON_MACE2         = 5,
    ITEM_SUBCLASS_WEAPON_POLEARM       = 6,
    ITEM_SUBCLASS_WEAPON_SWORD         = 7,
    ITEM_SUBCLASS_WEAPON_SWORD2        = 8,
    ITEM_SUBCLASS_WEAPON_obsolete      = 9,
    ITEM_SUBCLASS_WEAPON_STAFF         = 10,
    ITEM_SUBCLASS_WEAPON_EXOTIC        = 11,
    ITEM_SUBCLASS_WEAPON_EXOTIC2       = 12,
    ITEM_SUBCLASS_WEAPON_UNARMED       = 13,
    ITEM_SUBCLASS_WEAPON_GENERIC       = 14,
    ITEM_SUBCLASS_WEAPON_DAGGER        = 15,
    ITEM_SUBCLASS_WEAPON_THROWN        = 16,
    ITEM_SUBCLASS_WEAPON_SPEAR         = 17,
    ITEM_SUBCLASS_WEAPON_CROSSBOW      = 18,
    ITEM_SUBCLASS_WEAPON_WAND          = 19,
    ITEM_SUBCLASS_WEAPON_FISHING_POLE  = 20,
};

enum ITEM_SUBCLASS_ARMOR
{
    ITEM_SUBCLASS_ARMOR_GENERIC     = 0,
    ITEM_SUBCLASS_ARMOR_CLOTH       = 1,
    ITEM_SUBCLASS_ARMOR_LEATHER     = 2,
    ITEM_SUBCLASS_ARMOR_MAIL        = 3,
    ITEM_SUBCLASS_ARMOR_PLATE       = 4,
    ITEM_SUBCLASS_ARMOR_BUCKLER     = 5,
    ITEM_SUBCLASS_ARMOR_SHIELD      = 6,
};

enum ITEM_SUBCLASS_PROJECTILE
{
    ITEM_SUBCLASS_ARROW             = 2,
    ITEM_SUBCLASS_BULLET            = 3,
};

enum ITEM_SUBCLASS_BOOK
{
    ITEM_SUBCLASS_BOOK                  = 0,
    ITEM_SUBCLASS_LEATHERWORKING_PATERN = 1,
    ITEM_SUBCLASS_TAILORING_PATERN      = 2,
    ITEM_SUBCLASS_ENGINEERING_SCHEMATIC = 3,
    ITEM_SUBCLASS_COOKING_RECIPE        = 5,
    ITEM_SUBCLASS_ALCHEMY_RECIPE        = 6,
    ITEM_SUBCLASS_FIRST_AID_MANUAL      = 7,
    ITEM_SUBCLASS_ENCHANTING_FORMULA    = 8,
    ITEM_SUBCLASS_FISHING_MANUAL        = 9,
};

enum ITEM_SUBCLASS_QUIVER
{
    ITEM_SUBCLASS_QUIVER            = 2,
    ITEM_SUBCLASS_AMMO_POUCH        = 3,
};

struct _Damage
{
    float DamageMin;
    float DamageMax;
    uint32 DamageType;

};

struct _ItemStat
{
    uint32 ItemStatType;
    uint32 ItemStatValue;

};
struct _Spell
{
    uint32 SpellId;
    uint32 SpellTrigger;
    uint32 SpellCharges;
    uint32 SpellCooldown;
    uint32 SpellCategory;
    uint32 SpellCategoryCooldown;

};
struct ItemPrototype
{
    uint32 ItemId;
    uint32 Class;
    uint32 SubClass;
    char* Name1;
    char* Name2;
    char* Name3;
    char* Name4;
    uint32 DisplayInfoID;
    uint32 Quality;
    uint32 Flags;
    uint32 BuyCount;
    uint32 BuyPrice;
    uint32 SellPrice;
    uint32 InventoryType;
    uint32 AllowableClass;
    uint32 AllowableRace;
    uint32 ItemLevel;
    uint32 RequiredLevel;
    uint32 RequiredSkill;
    uint32 RequiredSkillRank;
    uint32 RequiredSpell;
    uint32 RequiredHonorRank;
    uint32 RequiredCityRank;
    uint32 RequiredReputationFaction;
    uint32 RequiredReputationRank;
    uint32 MaxCount;
    uint32 Stackable;
    uint32 ContainerSlots;
    _ItemStat ItemStat[10];
    _Damage Damage[5];
    uint32 Armor;
    uint32 HolyRes;
    uint32 FireRes;
    uint32 NatureRes;
    uint32 FrostRes;
    uint32 ShadowRes;
    uint32 ArcaneRes;
    uint32 Delay;
    uint32 Ammo_type;
    float  RangedModRange;

    _Spell Spells[5];
    uint32 Bonding;
    char* Description;
    uint32 PageText;
    uint32 LanguageID;
    uint32 PageMaterial;
    uint32 StartQuest;
    uint32 LockID;
    uint32 Material;
    uint32 Sheath;
    uint32 Extra;
    uint32 Block;
    uint32 ItemSet;
    uint32 MaxDurability;
    uint32 Area;
    uint32 Unknown1;
    char* ScriptName;
};
#endif
