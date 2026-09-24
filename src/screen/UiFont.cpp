#include "UiFont.hpp"

#include <filesystem>

bool loadUiFont(sf::Font& font)
{
    return font.openFromFile(std::filesystem::path{ASSET_DIR} / "fonts" / "upheavtt.ttf");
}

sf::Font makeUiFont()
{
    sf::Font font;
    if(!loadUiFont(font))
    {
        return font;
    }
    return font;
}
