//
// image.hpp
// *********
//
// Copyright (c) 2020 Sharon Fox (sharon at xandium dot io)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include <nlohmann/json.hpp>

namespace aegis
{

namespace gateway
{

namespace objects
{

/**\todo Needs documentation
 */
struct image
{
    std::string url; /**<\todo Needs documentation */
    std::string proxy_url; /**<\todo Needs documentation */
    int32_t height = 0; /**<\todo Needs documentation */
    int32_t width = 0; /**<\todo Needs documentation */
};

/// \cond TEMPLATES
inline void from_json(const nlohmann::json& j, image& m)
{
    if (j.count("url") && !j["url"].is_null())
        m.url = j["url"].get<std::string>();
    if (j.count("proxy_url") && !j["proxy_url"].is_null())
        m.proxy_url = j["proxy_url"].get<std::string>();
    if (j.count("height") && !j["height"].is_null())
        m.height = j["height"];
    if (j.count("width") && !j["width"].is_null())
        m.width = j["width"];
}

inline void to_json(nlohmann::json& j, const image& m)
{
    j["url"] = m.url;
    //j["proxy_url"] = m.proxy_url;
    j["height"] = m.height;
    j["width"] = m.width;
}
/// \endcond

}

}

}
