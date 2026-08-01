/* Created by kblaszki */

#include <filesystem>
#include <gtest/gtest.h>
#include <resources/ResourceManager.hpp>

using namespace ::testing;

TEST(ResourceManagerShould, throwWhenFontIsMissing)
{
    const auto missingRoot = std::filesystem::temp_directory_path() / "business-game-missing-resources";
    std::filesystem::create_directories(missingRoot);

    ResourceManager resources{missingRoot};
    EXPECT_THROW(
        { [[maybe_unused]] const sf::Font& font = resources.getFont("fonts/prototype.ttf"); }, std::runtime_error);
}

TEST(ResourceManagerShould, loadAndCacheFontFromResourcesRoot)
{
    const auto resourcesRoot = std::filesystem::path{RESOURCES_ROOT_FOR_TESTS};
    ResourceManager resources{resourcesRoot};

    const sf::Font& first = resources.getFont("fonts/prototype.ttf");
    const sf::Font& second = resources.getFont("fonts/prototype.ttf");
    EXPECT_EQ(&first, &second);
}
