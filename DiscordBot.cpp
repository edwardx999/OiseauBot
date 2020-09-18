// DiscordBot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <gsl/gsl>
#include <filesystem>
#include "event_responses.h"

std::string read_whole_file(char const* const filename)
{
	auto const file_handle = std::fopen(filename, "rb");
	using namespace std::string_literals;
	if (file_handle == nullptr)
	{
		throw std::invalid_argument("Unable to open file "s + filename);
	}
#ifdef _WIN32
	gsl::final_action
#else
	gsl::final_act
#endif
		close_file([file_handle]()
								 {
									 std::fclose(file_handle);
								 });
	std::fseek(file_handle, 0, SEEK_SET);
	std::fseek(file_handle, 0, SEEK_END);
	auto const file_size = std::ftell(file_handle);
	std::fseek(file_handle, 0, SEEK_SET);
	std::string ret;
	ret.resize(file_size);
	std::fread(ret.data(), 1, file_size, file_handle);
	return ret;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << fmt::format("Usage: {} token_file\n", std::filesystem::path(argv[0]).filename().string());
		return 1;
	}
	try
	{
		namespace events = aegis::gateway::events;

		auto const token = read_whole_file(argv[1]);
		aegis::core bot(aegis::create_bot_t().log_level(spdlog::level::trace).token(token));
		auto const bind_to_bot = [&bot](auto func)
		{
			return [&bot, func = std::move(func)](auto&&... args)
			{
				func(bot, std::forward<decltype(args)>(args)...);
			};
		};
		bot.set_on_message_create(bind_to_bot(test_discord::message_create_response));
		bot.set_on_ready(bind_to_bot(test_discord::on_ready_response));
		bot.run();
		bot.yield();
	}
	catch (std::exception const& error)
	{
		char message_space[128];
		auto const error_message = error.what(); 
		auto constexpr message_space_len = std::size(message_space) - 1;
		std::size_t message_len = 0;
		for (; message_len < message_space_len && error_message[message_len]; ++message_len)
		{
			message_space[message_len] = error_message[message_len];
		}
		message_space[message_len] = '\n';
		std::cerr.write(message_space, message_len + 1);
		return 1;
	}
}