#include "Input.h"

Input& Input::get()
{
    static Input manager;
    return manager;
}

Input::Input()
{
}

void Input::setKeyPressed(Key key)
{
    _pressedMap.insert_or_assign(key, true);
}

void Input::setKeyReleased(Key key)
{
    _pressedMap.insert_or_assign(key, false);
}

bool Input::isKeyPressed(Key key)
{
    auto search = _pressedMap.find(key);
    if (search != _pressedMap.end()) {
        return search->second;
    } else {
        _pressedMap.insert({key, false});
        return false;
    }
}
