#include <arkanoid/app/Assets.hpp>
#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/app/Scenes.hpp>

#include <eng/loop/App.hpp>
#include <eng/render/RenderQueue.hpp>
#include <eng/resources/ResourceError.hpp>
#include <eng/scene/SceneStack.hpp>
#include <eng/sfml/SfmlClock.hpp>
#include <eng/sfml/SfmlPlatform.hpp>

#include <filesystem>
#include <iostream>

namespace
{

[[nodiscard]] int fail(eng::ResourceError err)
{
    std::cerr << eng::describe(err) << '\n';
    return 1;
}

} // namespace

int main()
{
    eng::sfml::SfmlPlatform platform{{1280u, 720u}, "Business game"};

    eng::TextureId background{};
    eng::TextureId brick{};
    eng::TextureId paddle{};
    eng::TextureId ball{};
    eng::TextureId capsuleWide{};
    eng::TextureId capsuleMulti{};
    eng::TextureId capsuleSlow{};
    eng::TextureId capsuleExtra{};

    for(const arkanoid::TextureSpec& spec: arkanoid::textureSpecs())
    {
        const auto loaded = platform.assets().texture(spec.key, spec.build());
        if(!loaded)
        {
            return fail(loaded.error());
        }

        if(spec.key == arkanoid::AssetKeys::background)
        {
            background = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::brick)
        {
            brick = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::paddle)
        {
            paddle = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::ball)
        {
            ball = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::capsuleWide)
        {
            capsuleWide = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::capsuleMulti)
        {
            capsuleMulti = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::capsuleSlow)
        {
            capsuleSlow = *loaded;
        }
        else if(spec.key == arkanoid::AssetKeys::capsuleExtra)
        {
            capsuleExtra = *loaded;
        }
    }

    const auto fontLoaded =
        platform.assets().font("ui", std::filesystem::path{ASSET_DIR} / arkanoid::uiFontRelativePath);
    if(!fontLoaded)
    {
        return fail(fontLoaded.error());
    }
    const eng::FontId fontId = *fontLoaded;

    const arkanoid::TextureIds textures{
        .background = background,
        .brick = brick,
        .paddle = paddle,
        .ball = ball,
        .capsule = {capsuleWide, capsuleMulti, capsuleSlow, capsuleExtra},
    };

    const arkanoid::Actions actions = arkanoid::makeActions();
    eng::ActionMap map = arkanoid::defaultBindings(actions);
    const arkanoid::AppServices services{actions, textures, fontId};

    eng::SceneStack stack{arkanoid::pause(services)};
    stack.push(arkanoid::mainMenu(services)());

    eng::sfml::SfmlClock clock;
    eng::RenderQueue queue;
    eng::App{platform, clock, std::move(map), stack, queue}.run();
    return 0;
}
