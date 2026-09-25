#include <arkanoid/app/Assets.hpp>
#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/app/Scenes.hpp>
#include <filesystem>
#include <iostream>
#include <sgl/loop/App.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/resources/ResourceError.hpp>
#include <sgl/scene/SceneStack.hpp>
#include <sgl/sfml/SfmlClock.hpp>
#include <sgl/sfml/SfmlPlatform.hpp>

namespace
{

[[nodiscard]] int fail(sgl::ResourceError err)
{
    std::cerr << sgl::describe(err) << '\n';
    return 1;
}

} // namespace

int main()
{
    sgl::sfml::SfmlPlatform platform{{1280u, 720u}, "Arkanoid"};

    sgl::TextureId background{};
    sgl::TextureId brick{};
    sgl::TextureId paddle{};
    sgl::TextureId ball{};
    sgl::TextureId capsuleWide{};
    sgl::TextureId capsuleMulti{};
    sgl::TextureId capsuleSlow{};
    sgl::TextureId capsuleExtra{};

    for(const sgl::arkanoid::TextureSpec& spec: sgl::arkanoid::textureSpecs())
    {
        const auto loaded = platform.assets().texture(spec.key, spec.build());
        if(!loaded)
        {
            return fail(loaded.error());
        }

        if(spec.key == sgl::arkanoid::AssetKeys::background)
        {
            background = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::brick)
        {
            brick = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::paddle)
        {
            paddle = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::ball)
        {
            ball = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::capsuleWide)
        {
            capsuleWide = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::capsuleMulti)
        {
            capsuleMulti = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::capsuleSlow)
        {
            capsuleSlow = *loaded;
        }
        else if(spec.key == sgl::arkanoid::AssetKeys::capsuleExtra)
        {
            capsuleExtra = *loaded;
        }
    }

    const auto fontLoaded =
        platform.assets().font("ui", std::filesystem::path{ASSET_DIR} / sgl::arkanoid::uiFontRelativePath);
    if(!fontLoaded)
    {
        return fail(fontLoaded.error());
    }
    const sgl::FontId fontId = *fontLoaded;

    const sgl::arkanoid::TextureIds textures{
        .background = background,
        .brick = brick,
        .paddle = paddle,
        .ball = ball,
        .capsule = {capsuleWide, capsuleMulti, capsuleSlow, capsuleExtra},
    };

    const sgl::arkanoid::Actions actions = sgl::arkanoid::makeActions();
    sgl::ActionMap map = sgl::arkanoid::defaultBindings(actions);
    const sgl::arkanoid::AppServices services{actions, textures, fontId};

    sgl::SceneStack stack{sgl::arkanoid::pause(services)};
    stack.push(sgl::arkanoid::mainMenu(services)());

    sgl::sfml::SfmlClock clock;
    sgl::RenderQueue queue;
    sgl::App{platform, clock, std::move(map), stack, queue}.run();
    return 0;
}
