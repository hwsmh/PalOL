#pragma once
#include "../Common/GameMessage.h"
#include <memory>

class game_player
{
public:
	virtual ~game_player() {}
	virtual void deliver(const game_message& msg) = 0;
};

typedef std::shared_ptr<game_player> game_player_ptr;
