//
// resumed.hpp
// ***********
//
// Copyright (c) 2020 Sharon Fox (sharon at xandium dot io)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/fwd.hpp"

namespace aegis
{

namespace gateway
{

namespace events
{

/**\todo Needs documentation
 */
struct resumed
{
    shards::shard & shard; /**< Reference to shard object this message came from */
    std::vector<std::string> _trace; /**< Debug information for Discord */
};

}

}

}
