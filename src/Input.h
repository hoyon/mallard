#ifndef MAGNUM_INPUT_H
#define MAGNUM_INPUT_H

#include <string>
#include <unordered_map>

#include <Magnum/Platform/Sdl2Application.h>

namespace M = Magnum;

class Input
{
public:
    using Key = M::Platform::Sdl2Application::KeyEvent::Key;

    static Input& get();

    Input(Input const&) = delete;
    Input(Input&&) = delete;
    Input& operator=(Input const&) = delete;
    Input& operator=(Input&&) = delete;

    void setKeyPressed(Key key);
    void setKeyReleased(Key key);

    bool isKeyPressed(Key key);

private:
    Input();

    std::unordered_map<Key, bool> _pressedMap;
};

#endif // MAGNUM_INPUT_H
