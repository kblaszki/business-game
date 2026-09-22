/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/Font.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

class ResourceManager
{
public:
    explicit ResourceManager(std::filesystem::path resourcesRoot);

    [[nodiscard]] const sf::Font& getFont(const std::string& relativePath);

    [[nodiscard]] const std::filesystem::path& resourcesRoot() const;

private:
    std::filesystem::path root;
    std::unordered_map<std::string, sf::Font> fonts;
};
