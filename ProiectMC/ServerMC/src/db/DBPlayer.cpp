#include "../../include/db/DBPlayer.h"
#include <iostream>


DBPlayer::DBPlayer(std::nullptr_t) :player_id(0)
{
}

DBPlayer::DBPlayer() :score(0), points(0)
{
}

bool DBPlayer::operator==(std::nullptr_t)
{
    return player_id == 0 && nickname.empty() && password.empty();
}
const std::string& DBPlayer::GetName() const {
    return nickname;
}
