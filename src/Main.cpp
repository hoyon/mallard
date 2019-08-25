#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Math/Color.h>

namespace M = Magnum;

class MallardApplication: public M::Platform::Application {
    public:
        explicit MallardApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;
};

namespace {
    auto windowConfiguration() {
        return M::Platform::Application::Configuration{}
            .setTitle("Mallard")
            .setSize({1280, 720});
    }
}

MallardApplication::MallardApplication(const Arguments& arguments)
    : M::Platform::Application{arguments, windowConfiguration()}
{
}

void MallardApplication::drawEvent() {
    M::GL::defaultFramebuffer.clear(M::GL::FramebufferClear::Color);

    M::GL::defaultFramebuffer.clearColor(M::Color4::red());

    swapBuffers();
}

void MallardApplication::keyPressEvent(KeyEvent& event) {
    if (event.key() == KeyEvent::Key::Q) {
        M::Debug{} << "Quiting...";

        event.setAccepted();
        exit();
    }
}

MAGNUM_APPLICATION_MAIN(MallardApplication)
