#include "paddle.hpp"

#include <ddengine/core/engine.hpp>

Paddle::Paddle(int side, bool isAI, Ball *ball)
{
  float middleWidth = Engine::window.resolutionWidth * 0.5f;
  float middleHeight = Engine::window.resolutionHeight * 0.5f;
  float offset = 10.0f;

  this->isAI = isAI;
  this->ball = ball;
  this->size = glm::vec2(15.0f, 150.0f);
  this->upKey = upKey;
  this->downKey = downKey;
  this->color = glm::vec4(1.0f); // white color
  this->speed = 350.0f;
  this->score = 0;

  if(side == PLAYER_SIDE_LEFT)
  {
    this->position = glm::vec2(offset, middleHeight - this->size.y * 0.5f);
    this->upKey = KEY_W;
    this->downKey = KEY_S;
  }

  if(side == PLAYER_SIDE_RIGHT)
  {
    this->position = glm::vec2(Engine::window.resolutionWidth - offset - this->size.x, middleHeight - this->size.y * 0.5f);
    this->upKey = KEY_UP;
    this->downKey = KEY_DOWN;
  }

  this->startPosition = this->position;

  this->spriteEntity = new SpriteEntity(
    Transform(TransformCreateInfo{
      .position = this->position,
      .size = this->size
    }),
    Sprite(SpriteCreateInfo{
      .color = this->color,
      .display = 1
    })
  );

  std::string fontName = "dogica_font";

  auto font = Engine::resourceManager.getResource<Font>(fontName);

  glm::vec2 textPosition(middleWidth + 10.0f, 10.0f);

  if(this->position.x < middleWidth)
  {
    textPosition = glm::vec2(middleWidth - font->getTextWidth("00") - 10.0f, 10.0f);
  }

  this->text = new Text(TextCreateInfo{
    .font = font,
    .value = "00",
    .position = textPosition,
    .color = glm::vec4(1.0f, 1.0f, 1.0f, 0.75f),
    .display = 1
  });
}

void Paddle::update()
{
  if(!this->isAI)
  {
    this->updatePlayerMovement();
  }
  else
  {
    this->updateAIMovement();
  }

  if(this->position.y < 0)
  {
    this->position.y = 0.0f;
  }

  if(this->position.y > Engine::window.resolutionHeight - this->size.y)
  {
    this->position.y = Engine::window.resolutionHeight - this->size.y;
  }

  this->spriteEntity->transform.position = this->position;
  this->spriteEntity->update();

  this->text->update();
}

void Paddle::setScore(int score)
{
  this->score = score;
  this->text->setValue(this->score < 10 ? "0" + std::to_string(score) : std::to_string(this->score));
}

void Paddle::reset()
{
  this->position = this->startPosition;
}

void Paddle::toggleState()
{
  this->hidden = !hidden;

  this->spriteEntity->sprite.display = this->hidden ? 1 : 0;
  this->text->setDisplay(this->hidden ? 1 : 0);

  this->reset();
}

float Paddle::predictBallY(glm::vec2 ballPosition, glm::vec2 ballVelocity)
{
  if (std::abs(ballVelocity.x) < 0.0001f) return -1;

  float velocityY = ballVelocity.y;

  float timeToPaddle = (this->position.x - ballPosition.x) / ballVelocity.x;
  if(timeToPaddle < 0) return -1; // Ball is moving away from paddle

  float y = ballPosition.y + ballVelocity.y * timeToPaddle;

  while(y < 0 || y > Engine::window.resolutionHeight)
  {
    if(y < 0)
    {
      y = -y;
    }
    else if(y > Engine::window.resolutionHeight)
    {
      y = 2 * Engine::window.resolutionHeight - y;
    }

    velocityY = -velocityY;
  }

  return y;
}

void Paddle::updatePlayerMovement()
{
  if(Engine::input.held(this->upKey))
  {
    this->position.y -= speed * Engine::tempo.deltaTime;
  }

  if(Engine::input.held(this->downKey))
  {
    this->position.y += speed * Engine::tempo.deltaTime;
  }
}

void Paddle::updateAIMovement()
{
  float predictedY = this->predictBallY(ball->position, ball->velocity);

  if(predictedY == -1) return;

  if(this->position.y + this->size.y * 0.5f < predictedY)
  {
    this->position.y += speed * Engine::tempo.deltaTime;
  }

  if(this->position.y + this->size.y * 0.5f > predictedY)
  {
    this->position.y -= speed * Engine::tempo.deltaTime;
  }
}
