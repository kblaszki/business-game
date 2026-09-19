/* Created by kblaszki */
#pragma once

#include <LevelId.hpp>
#include <utility>

struct LevelDescriptor
{
    LevelId id{LevelId::Arkanoid};
};

inline const LevelDescriptor& levelDescriptor(LevelId id)
{
    static const LevelDescriptor arkanoidDescriptor{
        .id = LevelId::Arkanoid,
    };

    switch(id)
    {
        case LevelId::Arkanoid:
            return arkanoidDescriptor;
    }

    std::unreachable();
}
