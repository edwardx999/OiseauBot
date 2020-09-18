//
// channel_update.hpp
// ******************
//
// Copyright (c) 2020 Sharon Fox (sharon at xandium dot io)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/fwd.hpp"
#include "aegis/gateway/objects/channel.hpp"


namespace aegis
{

namespace gateway
{

namespace events
{

/// Channel object sent over the gateway on update
struct channel_update
{
    shards::shard & shard; /**< Reference to shard object this message came from */
    objects::channel channel; /**< gateway channel object */
};

}

}

}
