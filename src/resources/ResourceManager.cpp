/* Created by kblaszki */

#include "ResourceManager.hpp"

#include <stdexcept>

ResourceManager::ResourceManager(std::filesystem::path resourcesRoot)
    : root{std::move(resourcesRoot)}
{
    if(root.empty())
    {
        throw std::runtime_error("ResourceManager: resources root path is empty");
    }
}

const std::filesystem::path& ResourceManager::resourcesRoot() const
{
    return root;
}

const sf::Font& ResourceManager::getFont(const std::string& relativePath)
{
    if(auto it = fonts.find(relativePath); it != fonts.end())
    {
        return it->second;
    }

    const auto absolutePath = root / relativePath;
    sf::Font font;
    if(not font.openFromFile(absolutePath))
    {
        throw std::runtime_error("Failed to load font: " + absolutePath.string());
    }

    auto [it, _] = fonts.emplace(relativePath, std::move(font));
    return it->second;
}
