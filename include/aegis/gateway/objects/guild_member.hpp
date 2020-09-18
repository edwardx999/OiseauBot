//
// guild_member.hpp
// ****************
//
// Copyright (c) 2020 Sharon Fox (sharon at xandium dot io)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/snowflake.hpp"
#include "aegis/gateway/objects/user.hpp"
#include <nlohmann/json.hpp>

namespace aegis
{

namespace gateway
{

namespace objects
{

/**\todo Needs documentation
 */
struct guild_member
{
    objects::user _user; /**<\todo Needs documentation */
    snowflake guild_id; /**<\todo Needs documentation */
    std::string nick; /**<\todo Needs documentation */
    std::vector<snowflake> roles; /**<\todo Needs documentation */
    std::string joined_at; /**<\todo Needs documentation */
    bool deaf = false; /**<\todo Needs documentation */
    bool mute = false; /**<\todo Needs documentation */
};

/// \cond TEMPLATES
inline void from_json(const nlohmann::json& j, guild_member& m)
{
    m._user = j["user"];
    if (j.count("nick") && !j["nick"].is_null())
        m.nick = j["nick"].get<std::string>();
    if (j.count("guild_id") && !j["guild_id"].is_null())
        m.guild_id = j["guild_id"];
    if (j.count("roles") && !j["roles"].is_null())
        for (const auto & _role : j["roles"])
            m.roles.push_back(_role);
    if (j.count("joined_at"))
        m.joined_at = j["joined_at"].get<std::string>();
    if (j.count("deaf"))
        m.deaf = j["deaf"];
    if (j.count("mute"))
        m.mute = j["mute"];
}
/// \endcond

}

}

}
