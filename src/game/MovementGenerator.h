/* 
 * Copyright (C) 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
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

#ifndef MANGOS_MOVEMENTGENERATOR_H
#define MANGOS_MOVEMENTGENERATOR_H

#include "Platform/Define.h"
#include "Policies/Singleton.h"
#include "Dynamic/ObjectRegistry.h"
#include "Dynamic/FactoryHolder.h"
#include "Common.h"
#include "MotionMaster.h"

class Unit;

class MANGOS_DLL_SPEC MovementGenerator
{
    public:
        virtual ~MovementGenerator();

        virtual void Initialize(Unit &) = 0;

        virtual void Reset(Unit &) = 0;

        virtual bool Update(Unit &, const uint32 &time_diff) = 0;

        virtual MovementGeneratorType GetMovementGeneratorType() = 0;

        virtual void unitSpeedChanged() { }
};

template<class T, class D>
class MANGOS_DLL_SPEC MovementGeneratorMedium : public MovementGenerator
{
    public:
        void Initialize(Unit &u)
        {
            //u->AssertIsType<T>();
            (static_cast<D*>(this))->Initialize(*((T*)&u));
        }
        void Reset(Unit &u)
        {
            //u->AssertIsType<T>();
            (static_cast<D*>(this))->Reset(*((T*)&u));
        }
        bool Update(Unit &u, const uint32 &time_diff)
        {
            //u->AssertIsType<T>();
            return (static_cast<D*>(this))->Update(*((T*)&u), time_diff);
        }
    public:
        // will not link if not overridden in the generators
        void Initialize(T &u);
        void Reset(T &u);
        bool Update(T &u, const uint32 &time_diff);
};

struct SelectableMovement : public FactoryHolder<MovementGenerator,MovementGeneratorType>
{
    SelectableMovement(MovementGeneratorType mgt) : FactoryHolder<MovementGenerator,MovementGeneratorType>(mgt) {}
};

template<class REAL_MOVEMENT>
struct MovementGeneratorFactory : public SelectableMovement
{
    MovementGeneratorFactory(MovementGeneratorType mgt) : SelectableMovement(mgt) {}

    MovementGenerator* Create(void *) const;
};

typedef FactoryHolder<MovementGenerator,MovementGeneratorType> MovementGeneratorCreator;
typedef FactoryHolder<MovementGenerator,MovementGeneratorType>::FactoryHolderRegistry MovementGeneratorRegistry;
typedef FactoryHolder<MovementGenerator,MovementGeneratorType>::FactoryHolderRepository MovementGeneratorRepository;
#endif