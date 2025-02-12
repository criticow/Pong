#include "button.hpp"

#include <ddengine/core/engine.hpp>

Button::Button(glm::vec2 position, glm::vec2 size, const std::string &text)
{
  this->position = position;
  this->size = size;

  this->lines.push_back(Line(LineCreateInfo{
    .position = glm::vec2(this->position.x, this->position.y),
    .length = glm::vec2(this->size.x, 0.0f),
    .color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f),
    .state = InstanceState::ENABLED
  }));

  this->lines.push_back(Line(LineCreateInfo{
    .position = glm::vec2(this->position.x, this->position.y),
    .length = glm::vec2(0.0f, this->size.y),
    .color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f),
    .state = InstanceState::ENABLED
  }));

  this->lines.push_back(Line(LineCreateInfo{
    .position = glm::vec2(this->position.x, this->position.y + this->size.y),
    .length = glm::vec2(this->size.x, 0.0f),
    .color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f),
    .state = InstanceState::ENABLED
  }));

  this->lines.push_back(Line(LineCreateInfo{
    .position = glm::vec2(this->position.x + this->size.x, this->position.y),
    .length = glm::vec2(0.0f, this->size.y),
    .color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f),
    .state = InstanceState::ENABLED
  }));

  this->spriteEntity = new SpriteEntity(
    Transform(TransformCreateInfo{
      .position = this->position,
      .size = this->size,
    }),
    Sprite(SpriteCreateInfo{
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .display = QID_DISPLAY_DISABLED
    })
  );

  std::string fontName = "dogica_font_16";

  auto font = Engine::resourceManager.getResource<Font>(fontName);

  float textWidth = font->getTextWidth(text);

  this->text = new Text(TextCreateInfo{
    .font = font,
    .parentTransform = &this->spriteEntity->transform,
    .value = text,
    .position = glm::vec2(this->size.x * 0.5f - textWidth * 0.5f, this->size.y * 0.5f- font->maxHeight * 0.5f),
    .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    .layer = 2
  });
}

void Button::update()
{
  auto mouse = glm::vec2(Engine::window.mouse.x, Engine::window.mouse.y);

  this->spriteEntity->sprite.display = QID_DISPLAY_DISABLED;
  this->text->setColor(glm::vec4(1.0f));
  this->clicked = false;

  if(
    !this->hidden &&
    mouse.x > this->position.x && mouse.x < this->position.x + this->size.x &&
    mouse.y > this->position.y && mouse.y < this->position.y + this->size.y
  )
  {
    this->spriteEntity->sprite.display = QID_DISPLAY_ENABLED;
    this->text->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    if(Engine::input.pressed(MOUSE_BUTTON_LEFT))
    {
      this->clicked = true;
    }
  }

  this->spriteEntity->update();
  this->text->update();

  for(auto &line : this->lines)
  {
    line.update();
  }
}

void Button::toggle()
{
  this->hidden = !this->hidden;
  this->text->setDisplay(this->hidden ? 1 : 0);
  this->spriteEntity->sprite.display = this->hidden ? 1 : 0;

  for(auto &line : this->lines)
  {
    line.setState(this->hidden ? InstanceState::DISABLED : InstanceState::ENABLED);
  }
}
