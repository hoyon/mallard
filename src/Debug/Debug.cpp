#include "Debug.h"

#include <Corrade/Utility/String.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Text/Alignment.h>
#include <Magnum/Text/Renderer.h>

Debug* Debug::_instance;

Debug& Debug::get()
{
    if (!_instance) {
        _instance = new Debug;
    }
    return *_instance;
}

Debug::Debug()
    : _fontCache{M::Vector2i{2048}}
{
    _font = _manager.loadAndInstantiate("FreeTypeFont");

    if (!_font || !_font->openFile("assets/square.ttf", 160.f)) {
        M::Fatal{} << "Failed to open debug font 'square.ttf'";
    }

    _font->fillGlyphCache(_fontCache,
                          "abcedfghijklmnopqrstuvwxyz"
                          "ABCEDFGHIJKLMNOPQRSTUVWXYZ"
                          "0123456789?!<>()*&^%$£\"!'"
                          " .,\\|[]{}@#~`:;-=_+");

    _textRenderer.reset(new M::Text::Renderer2D(*_font, _fontCache, 0.05f, M::Text::Alignment::TopRight));

    _textRenderer->reserve(100, M::GL::BufferUsage::DynamicDraw, M::GL::BufferUsage::StaticDraw);

    auto projection =
        M::Matrix3::scaling(M::Vector2::yScale(M::Vector2(M::GL::defaultFramebuffer.viewport().size()).aspectRatio()));

    _textShader
        .setTransformationProjectionMatrix(projection * M::Matrix3::translation(1.0f / projection.rotationScaling().diagonal()))
        .setColor(M::Color3{1.0f})
        .bindVectorTexture(_fontCache.texture());
}

void Debug::draw()
{
    auto combined = M::Utility::String::join(_messages, '\n');

    if (_lastRendered != combined) {
        _textRenderer->render(combined);
        _lastRendered = combined;
    }
    _messages.clear();

    _textRenderer->mesh().draw(_textShader);
}

void Debug::addMessage(const std::string& message)
{
    _messages.push_back(message);
}
