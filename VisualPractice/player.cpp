#include "player.h"

namespace te
{
    //Player::Player(std::shared_ptr<Rectangle> paddle, unsigned int configN)
    //    : mCommandMap(createPaddleCommandMap(paddle))
    //    , mKeyMap(createPaddleKeyMap(configN))
    //{}

    //Command Player::issueCommand(SDL_Keycode key, Uint32 type) const
    //{
    //    auto keyIter = mKeyMap.find(std::make_pair(key, type));
    //    if (keyIter != mKeyMap.end())
    //    {
    //        auto commandIter = mCommandMap.find(keyIter->second);
    //        if (commandIter != mCommandMap.end())
    //        {
    //            auto command = [=]()
    //            {
    //                commandIter->second();
    //            };
    //            return command;
    //        }
    //    }
    //    return []() {};
    //}
}
