#pragma once

#include <ddengine/ddengine.hpp>

class Button
{
  public:
  glm::vec2 position;
  glm::vec2 size;
  bool clicked = false;
  Button(){};
  Button(glm::vec2 position, glm::vec2 size, const std::string &text);

  void update();
  void toggle();

  private:
  SpriteEntity *spriteEntity = nullptr;
  Text *text = nullptr;
  std::vector<Line> lines;
  bool hidden = false;
};