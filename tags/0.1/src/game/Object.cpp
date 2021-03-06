/* 
 * Copyright (C) 2005 MaNGOS <http://www.magosproject.org/>
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

#include "Common.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "Object.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "WorldSession.h"
#include "UpdateData.h"
#include "Util.h"
#include "MapManager.h"
#include "ObjectAccessor.h"


using namespace std;

Object::Object( )
{
    m_objectTypeId = TYPEID_OBJECT;
    m_objectType = TYPE_OBJECT;

    m_positionX = 0.0f;
    m_positionY = 0.0f;
    m_positionZ = 0.0f;
    m_orientation = 0.0f;

    m_mapId = 0;
    m_zoneId = 0;

    m_uint32Values = 0;

    m_inWorld = false;

    m_minZ = -500;

    m_valuesCount = 0;

    m_walkSpeed = 2.5f;
    m_runSpeed = 7.0f;
    m_backWalkSpeed = 2.5;
    (*((uint32*)&m_swimSpeed)) = 0x40971c72;
    m_backSwimSpeed = 4.5;
    (*((uint32*)&m_turnRate)) = 0x40490FDF;
    mSemaphoreTeleport = false;
}


Object::~Object( )
{
    if(m_uint32Values)
        delete [] m_uint32Values;
}


void Object::_Create( uint32 guidlow, uint32 guidhigh )
{
    if(!m_uint32Values) _InitValues();

    SetUInt32Value( OBJECT_FIELD_GUID, guidlow );
    SetUInt32Value( OBJECT_FIELD_GUID+1, guidhigh );
    SetUInt32Value( OBJECT_FIELD_TYPE, m_objectType );

	

}


void Object::_Create( uint32 guidlow, uint32 guidhigh, uint32 mapid, float x, float y, float z, float ang, uint32 nameId )
{
    if(!m_uint32Values) _InitValues();

    SetUInt32Value( OBJECT_FIELD_GUID, guidlow );
    SetUInt32Value( OBJECT_FIELD_GUID+1, guidhigh );
    SetUInt32Value( OBJECT_FIELD_TYPE, m_objectType );

	
	

	


    m_mapId = mapid;
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
    m_orientation = ang;
}


void Object::BuildMovementUpdateBlock(UpdateData * data, uint32 flags ) const
{
    ByteBuffer buf(500);

    buf << uint8( UPDATETYPE_MOVEMENT );          
    buf << GetGUID();                             

    _BuildMovementUpdate(&buf, flags, 0x00000000);

    data->AddUpdateBlock(buf);
}


void Object::BuildCreateUpdateBlockForPlayer(UpdateData *data, Player *target) const
{
    
    const Creature *creat = dynamic_cast<const Creature *>(this);

    if(target->isAlive())
    {
        
        if (creat && creat->GetUInt32Value(UNIT_FIELD_DISPLAYID) == 5233)
        {
            return;
        }
        if (creat)                                
        {
            ByteBuffer buf(500);

            
            buf << uint8( UPDATETYPE_CREATE_OBJECT );
            buf << GetGUID() ;                    
            buf << GetTypeId();                   

            
            _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

            
            buf << uint32( target == this ? 1 : 0 );
            buf << uint32( 0 );                   
            buf << uint32( 0 );                   
            buf << uint64( 0 );                   

            UpdateMask updateMask;
            updateMask.SetCount( m_valuesCount );
             _SetCreateBits( &updateMask, target );
            _BuildValuesUpdate( &buf, &updateMask );

            data->AddUpdateBlock(buf);
        }
        else if(!creat)                           
        {
	    Player *plyr = ObjectAccessor::Instance().FindPlayer(GetGUID());
	
            
            if(plyr && plyr!=target && plyr->IsInGroup() && target->IsInGroup() && plyr->isDead())
            {
                if(plyr->IsGroupMember(target))   
                {
                    ByteBuffer buf(500);

                    
                    buf << uint8( UPDATETYPE_CREATE_OBJECT );
                    buf << GetGUID() ;            
                    buf << GetTypeId();           

                    
                    _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

                    
                    buf << uint32( target == this ? 1 : 0 );
                    buf << uint32( 0 );           
                    buf << uint32( 0 );           
                    buf << uint64( 0 );           

                    UpdateMask updateMask;
                    updateMask.SetCount( m_valuesCount );
                    _SetCreateBits( &updateMask, target );
                    _BuildValuesUpdate( &buf, &updateMask );

                    data->AddUpdateBlock(buf);
                }
                else                              
                {
                    return;
                }
            }
            else if(plyr && plyr->isDead())       
            {
                return;
            }
            
            else                                  
            {
                ByteBuffer buf(500);

                
                buf << uint8( UPDATETYPE_CREATE_OBJECT );
                buf << GetGUID() ;                
                buf << GetTypeId();               

                
                _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

                
                buf << uint32( target == this ? 1 : 0 );
                buf << uint32( 0 );               
                buf << uint32( 0 );               
                buf << uint64( 0 );               

                UpdateMask updateMask;
                updateMask.SetCount( m_valuesCount );
                _SetCreateBits( &updateMask, target );
                _BuildValuesUpdate( &buf, &updateMask );

                data->AddUpdateBlock(buf);
            }
        }

        else                                      
        {
            ByteBuffer buf(500);

            buf << uint8( UPDATETYPE_CREATE_OBJECT );
            buf << GetGUID() ;                       
            buf << GetTypeId();                      

            
            _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

            buf << uint32( target == this ? 1 : 0 ); 
            buf << uint32( 0 );                      
            buf << uint32( 0 );                      
            buf << uint64( 0 );                      

            UpdateMask updateMask;
            updateMask.SetCount( m_valuesCount );
            _SetCreateBits( &updateMask, target );
            _BuildValuesUpdate( &buf, &updateMask );

            data->AddUpdateBlock(buf);
        }

    }
    if(target->isDead())
    {
        if(!creat)
        {
	    Player *plyr = ObjectAccessor::Instance().FindPlayer(GetGUID());

            
            if(plyr && plyr->IsGroupMember(target))
            {
                ByteBuffer buf(500);

                
                buf << uint8( UPDATETYPE_CREATE_OBJECT );
                buf << GetGUID() ;                
                buf << GetTypeId();               

                
                _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

                
                buf << uint32( target == this ? 1 : 0 );
                buf << uint32( 0 );               
                buf << uint32( 0 );               
                buf << uint64( 0 );               

                UpdateMask updateMask;
                updateMask.SetCount( m_valuesCount );
                _SetCreateBits( &updateMask, target );
                _BuildValuesUpdate( &buf, &updateMask );

                data->AddUpdateBlock(buf);
            }
            else if(plyr && plyr->isAlive())      
            {
                return;
            }
            else                                  
            {
                ByteBuffer buf(500);

                
                buf << uint8( UPDATETYPE_CREATE_OBJECT );
                buf << GetGUID() ;                
                buf << GetTypeId();               

                
                _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

                
                buf << uint32( target == this ? 1 : 0 );
                buf << uint32( 0 );               
                buf << uint32( 0 );               
                buf << uint64( 0 );               

                UpdateMask updateMask;
                updateMask.SetCount( m_valuesCount );
                _SetCreateBits( &updateMask, target );
                _BuildValuesUpdate( &buf, &updateMask );

                data->AddUpdateBlock(buf);
            }
        }
        
        else if(creat && creat->GetUInt32Value(UNIT_FIELD_DISPLAYID) == 5233)
        {
            ByteBuffer buf(500);

            
            buf << uint8( UPDATETYPE_CREATE_OBJECT );
            buf << GetGUID() ;                    
            buf << GetTypeId();                   

            
            _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

            
            buf << uint32( target == this ? 1 : 0 );
            buf << uint32( 0 );                   
            buf << uint32( 0 );                   
            buf << uint64( 0 );                   

            UpdateMask updateMask;
            updateMask.SetCount( m_valuesCount );
            _SetCreateBits( &updateMask, target );
            _BuildValuesUpdate( &buf, &updateMask );

            data->AddUpdateBlock(buf);
        }
        else if(creat)                            
        {
            return;
        }
        else                                      
        {
            ByteBuffer buf(500);

            
            buf << uint8( UPDATETYPE_CREATE_OBJECT );
            buf << GetGUID() ;                    
            buf << GetTypeId();                   

            
            _BuildMovementUpdate( &buf, 0x00000000, 0x00000000 );

            
            buf << uint32( target == this ? 1 : 0 );
            buf << uint32( 0 );                   
            buf << uint32( 0 );                   
            buf << uint64( 0 );                   

            UpdateMask updateMask;
            updateMask.SetCount( m_valuesCount );
            _SetCreateBits( &updateMask, target );
            _BuildValuesUpdate( &buf, &updateMask );

            data->AddUpdateBlock(buf);
        }
    }
}


void Object::BuildValuesUpdateBlockForPlayer(UpdateData *data, Player *target) const
{
    ByteBuffer buf(500);

    buf << (uint8) UPDATETYPE_VALUES;             
    buf << GetGUID() ;                            

    UpdateMask updateMask;
    updateMask.SetCount( m_valuesCount );
    _SetUpdateBits( &updateMask, target );
    _BuildValuesUpdate( &buf, &updateMask );

    data->AddUpdateBlock(buf);
}


void Object::BuildOutOfRangeUpdateBlock(UpdateData * data) const
{
    data->AddOutOfRangeGUID(GetGUID());
}


void Object::DestroyForPlayer(Player *target) const
{
    ASSERT(target);

    WorldPacket data;
    data.Initialize( SMSG_DESTROY_OBJECT );
    data << GetGUID();

    target->GetSession()->SendPacket( &data );
}






void Object::_BuildMovementUpdate(ByteBuffer * data, uint32 flags, uint32 flags2 ) const
{
    int spline_count = 0;

    *data << (uint32)flags;
    *data << (uint32)0;

    *data << (float)m_positionX;
    *data << (float)m_positionY;
    *data << (float)m_positionZ;
    *data << (float)m_orientation;
    *data << (float)0;

    if (flags & 0x20000000)
    {
        *data << (uint32)0;                       
        *data << (uint32)0;
        *data << (float)0;                        
        *data << (float)0;                        
        *data << (float)0;                        
        *data << (float)0;                        
    }

    if (flags & 0x1000000)
    {
        *data << (float)0;                        
    }

    if (flags & 0x4000)
    {
        *data << (uint16)0;                       
        *data << (float)0;                        
        *data << (float)0;                        
        *data << (float)0;                        
        *data << (float)0;                        
    }

    *data << m_walkSpeed;                         
    *data << m_runSpeed;                          
    *data << m_backWalkSpeed;                     
    *data << m_swimSpeed;                         
    *data << m_backSwimSpeed;                     
    *data << m_turnRate;                          

    if ((flags & 0x00200000) != 0)
    {
        *data << (uint32)flags2;

        if (flags2 & 0x10000)
        {
            *data << (float)0;                    
        }

        if (flags2 & 0x20000)
        {
            *data << (uint32)0;                   
            *data << (uint32)0;
        }

        if (flags2 & 0x40000)
        {
            *data << (float)0;                    
        }

        *data << (uint16)0;                       
        *data << (uint32)0;                       

        *data << (uint16)spline_count;            

        if (spline_count > 0)
        {
            for (int i = 0; i < spline_count; i++)
            {
                
                *data << uint32(0);               
                *data << uint32(0);
            }
        }
    }                                             
}






void Object::_BuildValuesUpdate(ByteBuffer * data, UpdateMask *updateMask) const
{
    WPAssert(updateMask && updateMask->GetCount() == m_valuesCount);

    *data << (uint8)updateMask->GetBlockCount();
    data->append( updateMask->GetMask(), updateMask->GetLength() );

    for( uint16 index = 0; index < m_valuesCount; index ++ )
    {
        if( updateMask->GetBit( index ) )
            *data << m_uint32Values[ index ];
    }
}


void Object::BuildHeartBeatMsg(WorldPacket *data) const
{
    data->Initialize(MSG_MOVE_HEARTBEAT);

    *data << GetGUID();

    *data << uint32(0);                           
    *data << uint32(0);                           

    *data << m_positionX;
    *data << m_positionY;
    *data << m_positionZ;

    *data << m_orientation;
}


void Object::BuildTeleportAckMsg(WorldPacket *data, float x, float y, float z, float ang) const
{
    
    
    data->Initialize(MSG_MOVE_TELEPORT_ACK);

    *data << GetGUID();

    
    *data << uint32(0);                           
    *data << uint32(0);                           

    *data << x;
    *data << y;
    *data << z;
    *data << ang;
}


void Object::SendMessageToSet(WorldPacket *data, bool bToSelf)
{
    MapManager::Instance().GetMap(m_mapId)->MessageBoardcast(this, data);
}






void Object::LoadValues(const char* data)
{
    if(!m_uint32Values) _InitValues();

    vector<string> tokens = StrSplit(data, " ");

    vector<string>::iterator iter;
    int index;

    for (iter = tokens.begin(), index = 0;
        index < m_valuesCount && iter != tokens.end(); ++iter, ++index)
    {
        m_uint32Values[index] = atol((*iter).c_str());
    }
}


void Object::LoadTaxiMask(const char* data)
{
    vector<string> tokens = StrSplit(data, " ");

    int index;
    vector<string>::iterator iter;

    for (iter = tokens.begin(), index = 0;
        (index < 8) && (iter != tokens.end()); ++iter, ++index)
    {
        m_taximask[index] = atol((*iter).c_str());
    }
}


void Object::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
    *updateMask = m_updateMask;
}


void Object::_SetCreateBits(UpdateMask *updateMask, Player *target) const
{
    for( uint16 index = 0; index < m_valuesCount; index++ )
    {
        if(GetUInt32Value(index) != 0)
            updateMask->SetBit(index);
    }
}



void Object::SetUInt32Value( const uint16 &index, const uint32 &value )
{
    ASSERT( index < m_valuesCount );
    m_uint32Values[ index ] = value;

    if(m_inWorld)
    {
        m_updateMask.SetBit( index );

        if(!m_objectUpdated)
        {
	    ObjectAccessor::Instance().AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}



void Object::SetUInt64Value( const uint16 &index, const uint64 &value )
{
    ASSERT( index + 1 < m_valuesCount );
    m_uint32Values[ index ] = *((uint32*)&value);
    m_uint32Values[ index + 1 ] = *(((uint32*)&value) + 1);

    if(m_inWorld)
    {
        m_updateMask.SetBit( index );
        m_updateMask.SetBit( index + 1 );

        if(!m_objectUpdated)
        {
	    ObjectAccessor::Instance().AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}



void Object::SetFloatValue( const uint16 &index, const float &value )
{
    ASSERT( index < m_valuesCount );
    m_floatValues[ index ] = value;

    if(m_inWorld)
    {
        m_updateMask.SetBit( index );

        if(!m_objectUpdated)
        {
	    ObjectAccessor::Instance().AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}


void Object::SetFlag( const uint16 &index, uint32 newFlag )
{
    ASSERT( index < m_valuesCount );
    m_uint32Values[ index ] |= newFlag;

    if(m_inWorld)
    {
        m_updateMask.SetBit( index );

        if(!m_objectUpdated)
        {
	    ObjectAccessor::Instance().AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}


void Object::RemoveFlag( const uint16 &index, uint32 oldFlag )
{
    ASSERT( index < m_valuesCount );
    m_uint32Values[ index ] &= ~oldFlag;

    if(m_inWorld)
    {
        m_updateMask.SetBit( index );

        if(!m_objectUpdated)
        {
	    ObjectAccessor::Instance().AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}
