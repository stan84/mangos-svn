/* 
 * Copyright (C) 2005-2008 MaNGOS <http://www.mangosproject.org/>
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

#include "Object.h"
#include "Player.h"
#include "BattleGround.h"
#include "BattleGroundBE.h"
#include "Creature.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Language.h"

BattleGroundBE::BattleGroundBE()
{
    m_BgObjects.resize(BG_BE_OBJECT_MAX);
}

BattleGroundBE::~BattleGroundBE()
{

}

void BattleGroundBE::Update(time_t diff)
{
    BattleGround::Update(diff);

    // after bg start we get there
    if (GetStatus() == STATUS_WAIT_JOIN && GetPlayersSize())
    {
        ModifyStartDelayTime(diff);

        if (!(m_Events & 0x01))
        {
            m_Events |= 0x01;
            for(uint32 i = BG_BE_OBJECT_DOOR_1; i <= BG_BE_OBJECT_DOOR_4; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);

            for(uint32 i = BG_BE_OBJECT_BUFF_1; i <= BG_BE_OBJECT_BUFF_2; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);

            SetStartDelayTime(START_DELAY1);
            SendMessageToAll(LANG_ARENA_ONE_MINUTE);
        }
        // After 30 seconds, warning is signalled
        else if (GetStartDelayTime() <= START_DELAY2 && !(m_Events & 0x04))
        {
            m_Events |= 0x04;
            SendMessageToAll(LANG_ARENA_THIRTY_SECONDS);
        }
        // After 15 seconds, warning is signalled
        else if (GetStartDelayTime() <= START_DELAY3 && !(m_Events & 0x08))
        {
            m_Events |= 0x08;
            SendMessageToAll(LANG_ARENA_FIFTEEN_SECONDS);
        }
        // delay expired (1 minute)
        else if (GetStartDelayTime() <= 0 && !(m_Events & 0x10))
        {
            m_Events |= 0x10;

            for(uint32 i = BG_BE_OBJECT_DOOR_1; i <= BG_BE_OBJECT_DOOR_2; i++)
                DoorOpen(i);

            for(uint32 i = BG_BE_OBJECT_BUFF_1; i <= BG_BE_OBJECT_BUFF_2; i++)
                SpawnBGObject(i, 60);

            SendMessageToAll(LANG_ARENA_BEGUN);
            SetStatus(STATUS_IN_PROGRESS);
            SetStartDelayTime(0);

            for(std::map<uint64, BattleGroundPlayer>::const_iterator itr = GetPlayers()->begin(); itr != GetPlayers()->end(); ++itr)
            {
                Player *plr = objmgr.GetPlayer(itr->first);
                if(plr)
                    plr->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
            }
        }
    }

    /*if(GetStatus() == STATUS_IN_PROGRESS)
    {
        // update something
    }*/
}

void BattleGroundBE::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattleGroundBEScore* sc = new BattleGroundBEScore;

    m_PlayerScores[plr->GetGUID()] = sc;
}

void BattleGroundBE::RemovePlayer(Player *plr, uint64 guid)
{

}

void BattleGroundBE::HandleKillPlayer(Player *player, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    if(!killer)
    {
        sLog.outError("Killer player not found");
        return;
    }

    uint32 killer_team_index = GetTeamIndexByTeamId(killer->GetTeam());

    m_TeamKills[killer_team_index]++;                       // add kills to killer's team

    if(m_TeamKills[killer_team_index] >= GetPlayersCountByTeam(player->GetTeam()))
    {
        // all opponents killed
        EndBattleGround(killer->GetTeam());
    }
}

void BattleGroundBE::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint32 SpellId = 0;
    uint64 buff_guid = 0;
    switch(Trigger)
    {
        case 4538:                                          // buff trigger?
            buff_guid = m_BgObjects[BG_BE_OBJECT_BUFF_1];
            break;
        case 4539:                                          // buff trigger?
            buff_guid = m_BgObjects[BG_BE_OBJECT_BUFF_2];
            break;
        default:
            sLog.outError("WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }

    if(buff_guid)
    {
        GameObject *obj = HashMapHolder<GameObject>::Find(buff_guid);
        if(obj)
        {
            if(!obj->isSpawned())
                return;                                     // buff not spawned yet
            obj->SetRespawnTime(BUFF_RESPAWN_TIME);
            obj->SetLootState(GO_LOOTED);
            SpellId = obj->GetGOInfo()->data3;
            if(SpellId)
                Source->CastSpell(Source, SpellId, true);
        }
    }
}

void BattleGroundBE::ResetBGSubclass()
{
    m_TeamKills[BG_TEAM_ALLIANCE] = 0;
    m_TeamKills[BG_TEAM_HORDE]    = 0;
}

bool BattleGroundBE::SetupBattleGround()
{
    // gates
    if(    !AddObject(BG_BE_OBJECT_DOOR_1, BG_BE_OBJECT_TYPE_DOOR_1, 6287.277, 282.1877, 3.810925, -2.260201, 0, 0, 0.9044551, -0.4265689, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_BE_OBJECT_DOOR_2, BG_BE_OBJECT_TYPE_DOOR_2, 6189.546, 241.7099, 3.101481, 0.8813917, 0, 0, 0.4265689, 0.9044551, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_BE_OBJECT_DOOR_3, BG_BE_OBJECT_TYPE_DOOR_3, 6299.116, 296.5494, 3.308032, 0.8813917, 0, 0, 0.4265689, 0.9044551, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_BE_OBJECT_DOOR_4, BG_BE_OBJECT_TYPE_DOOR_4, 6177.708, 227.3481, 3.604374, -2.260201, 0, 0, 0.9044551, -0.4265689, RESPAWN_IMMEDIATELY)
    // buffs
        || !AddObject(BG_BE_OBJECT_BUFF_1, BG_BE_OBJECT_TYPE_BUFF_1, 6249.042, 275.3239, 11.22033, -1.448624, 0, 0, 0.6626201, -0.7489557, 120)
        || !AddObject(BG_BE_OBJECT_BUFF_2, BG_BE_OBJECT_TYPE_BUFF_2, 6228.26, 249.566, 11.21812, -0.06981307, 0, 0, 0.03489945, -0.9993908, 120))
    {
        sLog.outErrorDb("BatteGroundBE: Failed to spawn some object!");
        return false;
    }

    return true;
}

void BattleGroundBE::UpdatePlayerScore(Player* Source, uint32 type, uint32 value)
{

    std::map<uint64, BattleGroundScore*>::iterator itr = m_PlayerScores.find(Source->GetGUID());

    if(itr == m_PlayerScores.end())                         // player not found...
        return;

    //there is nothing special in this score
    BattleGround::UpdatePlayerScore(Source,type,value);

}

/*
21:45:46 id:231310 [S2C] SMSG_INIT_WORLD_STATES (706 = 0x02C2) len: 86
0000: 32 02 00 00 76 0e 00 00 00 00 00 00 09 00 f3 09  |  2...v...........
0010: 00 00 01 00 00 00 f1 09 00 00 01 00 00 00 f0 09  |  ................
0020: 00 00 02 00 00 00 d4 08 00 00 00 00 00 00 d8 08  |  ................
0030: 00 00 00 00 00 00 d7 08 00 00 00 00 00 00 d6 08  |  ................
0040: 00 00 00 00 00 00 d5 08 00 00 00 00 00 00 d3 08  |  ................
0050: 00 00 00 00 00 00                                |  ......

spell 32724 - Gold Team
spell 32725 - Green Team
35774 Gold Team
35775 Green Team
*/