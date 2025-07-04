#include "Button.hpp"
#include <iostream>
#include <string>

Button::Button(const char *imagePath, Vector2 imagePosition, float scale)
{
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHight = static_cast<int>(originalHight * scale);

    ImageResize(&image, newWidth, newHight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = imagePosition;
}

Button::~Button()
{
    UnloadTexture(texture);
}

void Button::Draw()
{
    DrawTextureV(texture, position, WHITE);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle rect = {position.x, position.y, (float)texture.width, (float)texture.height};

    if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
        return true;
    else
        return false;
}
