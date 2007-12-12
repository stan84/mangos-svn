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

#ifndef MANGOS_BAG_H
#define MANGOS_BAG_H

#include "Object.h"
#include "ItemPrototype.h"
#include "Unit.h"
#include "Creature.h"
#include "Item.h"

class Bag : public Item
{
    public:

        Bag();
        ~Bag();

        bool Create(uint32 guidlow, uint32 itemid, Player* owner);

        void Clear();
        void StoreItem( uint8 slot, Item *pItem, bool update );
        void RemoveItem( uint8 slot, bool update );

        Item* GetItemByPos( uint8 slot ) const;
        uint32 GetItemCount( uint32 item ) const;

        uint8 FindFreeBagSlot() const;
        uint8 GetSlotByItemGUID(uint64 guid) const;
        bool IsEmpty() const;

        // DB operations
        void SaveToDB();
                                                            // overwrite Item::LoadFromDB
        bool LoadFromDB(uint32 guid, uint64 owner_guid, uint32 auctioncheck);
        void DeleteFromDB();

        void BuildCreateUpdateBlockForPlayer(UpdateData *data, Player *target) const;

    protected:

        // Bag Storage space
        Item* m_bagslot[28];
};

inline Item* NewItemOrBag(ItemPrototype const * proto)
{
    return (proto->InventoryType == INVTYPE_BAG) ? new Bag : new Item;
}
#endif