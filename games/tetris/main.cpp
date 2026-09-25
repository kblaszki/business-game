#include <filesystem>
#include <iostream>
#include <sgl/loop/App.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/resources/ResourceError.hpp>
#include <sgl/scene/SceneStack.hpp>
#include <sgl/sfml/SfmlClock.hpp>
#include <sgl/sfml/SfmlPlatform.hpp>
#include <tetris/app/Bindings.hpp>
#include <tetris/app/Scenes.hpp>
#include <tetris/app/Theme.hpp>

namespace
{

constexpr std::uint64_t kDefaultSeed{42};

[[nodiscard]] int fail(sgl::ResourceError err)
{
    std::cerr << sgl::describe(err) << '\n';
    return 1;
}

} // namespace

int main()
{
    sgl::sfml::SfmlPlatform platform{{1280u, 720u}, "Tetris"};

    const auto fontLoaded =
        platform.assets().font("ui", std::filesystem::path{ASSET_DIR} / sgl::tetris::uiFontRelativePath);
    if(!fontLoaded)
    {
        return fail(fontLoaded.error());
    }
    const sgl::FontId fontId = *fontLoaded;

    const sgl::tetris::Actions actions = sgl::tetris::makeActions();
    sgl::ActionMap map = sgl::tetris::defaultBindings(actions);
    const sgl::tetris::AppServices services{actions, fontId, kDefaultSeed};

    sgl::SceneStack stack{sgl::tetris::pause(services)};
    stack.push(sgl::tetris::mainMenu(services)());

    sgl::sfml::SfmlClock clock;
    sgl::RenderQueue queue;
    sgl::App{platform, clock, std::move(map), stack, queue}.run();
    return 0;
}
