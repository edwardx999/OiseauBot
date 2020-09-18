#pragma once
#include "pch.h"
namespace test_discord {
	
	void message_create_response(aegis::core& bot, aegis::gateway::events::message_create message);

	void on_ready_response(aegis::core& bot, aegis::gateway::events::ready);
}