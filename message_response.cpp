#include "pch.h"
#include "event_responses.h"
#ifdef USE_BOOST_FLAT_MAP
#include <boost/container/flat_map.hpp>
#else
#include <unordered_map>
#endif
namespace test_discord {

	using namespace std::literals;

	std::string_view starts_with(std::string const& str, std::string_view prefix)
	{
		if (str.size() < prefix.size())
		{
			return {};
		}
		else
		{
			if (auto const prefix_end = str.data() + prefix.size(); std::equal(str.data(), prefix_end, prefix.data()))
			{
				return { prefix_end, str.size() - prefix.size() };
			}
			else
			{
				return {};
			}
		}
	}

#ifdef USE_BOOST_FLAT_MAP
	template<typename Key, typename Value>
	using map = boost::container::flat_map<Key, Value>;
#else
	template<typename Key, typename Value>
	using map = std::unordered_map<Key, Value>;
#endif

	struct command {
		std::function<void(aegis::gateway::events::message_create, std::size_t /*command token length*/)> action;
		std::string_view explanation;
		std::string_view usage;
	};
	using local_command_map = map<std::string_view, command>;
	using channel_command_map = map<std::string_view, local_command_map>;

	channel_command_map const& get_command_map();

	std::string_view first_token(std::string_view full)
	{
		auto const string_end = full.data() + full.size();
		auto const end = std::find_if(full.data(), string_end, [](char c)
									  {
										  return std::isspace(c);
									  });
		return { full.data(), std::size_t(end - full.data()) };
	}

	std::string_view after_token(std::string_view text, std::size_t token_length)
	{
		if (token_length >= text.size())
		{
			return { text.data() + text.size(), 0 };
		}
		return { text.data() + token_length + 1, text.size() - token_length - 1 };
	}

	void say_hi(aegis::gateway::events::message_create message, std::size_t token_length)
	{
		auto& channel = message.msg.get_channel();
		channel.create_message(fmt::format(FMT_STRING("Hello <@{}>"), message.msg.author.id));
	}

	void give_help(aegis::gateway::events::message_create message, std::size_t token_length)
	{
		auto& channel = message.msg.get_channel();
		auto const suffix = after_token(message.msg.get_content(), token_length);
		if (std::all_of(suffix.begin(), suffix.end(), [](char c)
						{
							return bool(std::isspace(c));
						}))
		{
			static auto const help_message = []()
			{
				auto ret = aegis::json::object();
				ret["title"] = "Help";
				ret["color"] = 0xABCDEF;
				auto& fields = ret["fields"] = aegis::json::array();
				auto& command_map = get_command_map();
				for (auto& [channel, commands] : command_map)
				{
					auto& block = fields.emplace_back(aegis::json::object());
					block["name"] = "Commands in channel "s.append(channel);
					std::string commands_text;
					{
						auto command_iter = commands.begin();
						if (command_iter != commands.end())
						{
							commands_text.append(command_iter->second.usage);
							++command_iter;
							for (; command_iter != commands.end(); ++command_iter)
							{
								commands_text.push_back('\n');
								commands_text.append(command_iter->second.usage);
							}
						}
					}
					block["value"] = std::move(commands_text);
				}
				return ret;
			}();
			channel.create_message_embed(aegis::create_message_t().embed(help_message));
		}
		else
		{
			auto& command_map = get_command_map();
			for (auto& [channel_name, commands] : command_map)
			{
				if (auto const command_iter = commands.find(suffix); command_iter != commands.end())
				{
					channel.create_message_embed(aegis::create_message_t().embed(
						{
							{"title", "Help"},
							{"color", 0x123456},
							{"fields",
								{
									{
										{"name", fmt::format(FMT_STRING("Help for {} in channel {}"), suffix, channel_name)},
										{"value", command_iter->second.explanation}
									}
								}
							}
						}));
					return;
				}
			}
			channel.create_message(fmt::format(FMT_STRING("The command {} was not found"), suffix));
		}
	}

	void no_role_error_response(aegis::gateway::events::message_create& message, std::string_view role_name)
	{
		message.msg.get_channel().create_message(fmt::format(FMT_STRING("<@{}>, the role {} does not exist"), message.msg.author.id, role_name));
	}

	void give_role(aegis::gateway::events::message_create message, std::size_t token_length)
	{
		auto& guild = message.msg.get_guild();
		auto const& roles = guild.get_roles_nocopy();
		auto const& text = message.msg.get_content();
		auto const found_suffix = after_token(text, token_length);
		for (auto const& role : roles)
		{
			if (role.second.name == found_suffix)
			{
				guild.add_guild_member_role(message.get_user().get_id(), role.second.id)
					.then([message, role_name = role.second.name](aegis::gateway::objects::role const& role) mutable
				{
					message.msg.get_channel().create_message(fmt::format(FMT_STRING("<@{}>, you have been given role {}"), message.msg.author.id, role_name));
				});
				return;
			}
		}
		no_role_error_response(message, found_suffix);
	}

	void take_role(aegis::gateway::events::message_create message, std::size_t token_length)
	{
		auto& guild = message.msg.get_guild();
		auto const& roles = guild.get_roles_nocopy();
		auto const& text = message.msg.get_content();
		auto const found_suffix = after_token(text, token_length);
		for (auto const& role : roles)
		{
			if (role.second.name == found_suffix)
			{
				guild.remove_guild_member_role(message.get_user().get_id(), role.first)
					.then([message, role_name = role.second.name](aegis::rest::rest_reply const& role) mutable
				{
					message.msg.get_channel().create_message(fmt::format(FMT_STRING("<@{}>, you have lost role {}"), message.msg.author.id, role_name));
				});
				return;
			}
		}
		no_role_error_response(message, found_suffix);
	}

	channel_command_map const& get_command_map()
	{
		static channel_command_map map
		{
			{"bot-spam"sv,
				{
					{"!hi"sv, {say_hi, "Says Hello"sv, "!Hi"sv}},
					{"!hello"sv, {say_hi, "Says Hello"sv, "!Hello"sv}},
					{"!help"sv, {give_help, "Gives the help menu"sv, "!help [command_name]"sv}}
				}
			},
			{"new-roles"sv,
				{
					{"!giveme"sv, {give_role, "Gives you a role"sv, "!giveme <role>"}},
					{"!takeaway"sv, {take_role, "Removes a role from you"sv, "!takeaway <role>"}}
				}
			}
		};
		return map;
	}

	void message_create_response(aegis::gateway::events::message_create message)
	{
		auto& channel = message.msg.get_channel();
		{
			auto const author_id = message.msg.get_author_id();
			auto const self_id = channel.get_bot().get_id();
			if (author_id == self_id)
			{
				return;
			}
		}
		auto const& command_map = get_command_map();
		auto const channel_name = channel.get_name();
		if (auto const channel_iter = command_map.find(channel_name); channel_iter != command_map.end())
		{
			auto const& [key, commands] = *channel_iter;
			auto const& text = message.msg.get_content();
			auto const token = first_token(text);
			if (auto const command_iter = commands.find(token); command_iter != commands.end())
			{
				command_iter->second.action(std::move(message), token.size());
			}
		}
	}
}