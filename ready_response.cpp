#include "pch.h"
#include "event_responses.h"
namespace test_discord {

	void on_ready_response(aegis::core& bot, aegis::gateway::events::ready ready)
	{
		auto& user = ready.user;
		bot.update_presence("Use !help in bot-spam for help", aegis::gateway::objects::activity::Listening);
	}
}