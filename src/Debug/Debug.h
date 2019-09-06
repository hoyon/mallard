#ifndef MALLARD_DEBUG_H
#define MALLARD_DEBUG_H

#include <string>

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Shaders/Vector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>

namespace M = Magnum;

class Debug
{
public:
    static Debug& get();

    Debug(Debug const&) = delete;
    Debug(Debug&&) = delete;
    Debug& operator=(Debug const&) = delete;
    Debug& operator=(Debug&&) = delete;

    void draw();

    void putMessage(const std::string& message);

private:
    Debug();

    static Debug* _instance;

    M::PluginManager::Manager<M::Text::AbstractFont> _manager;
    M::Containers::Pointer<M::Text::AbstractFont> _font;
    M::Text::GlyphCache _fontCache;

    M::Containers::Pointer<M::Text::Renderer2D> _textRenderer;
    M::Shaders::Vector2D _textShader;
};

#endif // MALLARD_DEBUG_H
