#include "ball.hpp"

#include <ddengine/core/engine.hpp>
#include "paddle.hpp"

Ball::Ball()
{
  this->size = glm::vec2(10.0f);

  float middleWidth = Engine::window.resolutionWidth * 0.5f;
  float middleHeight = Engine::window.resolutionHeight * 0.5f;

  this->startPosition = glm::vec2(middleWidth - this->size.x * 0.5f, middleHeight - this->size.y * 0.5f);
  this->position = this->startPosition;

  this->spriteEntity = new SpriteEntity(
    Transform(TransformCreateInfo{
      .position = this->position,
      .size = this->size,
      .layer = 1
    }),
    Sprite(SpriteCreateInfo{
      .color = glm::vec4(1.0f),
      .display = 1
    })
  );

  this->velocity = glm::vec2(0.0f);
  this->speed = 450.0f;
  this->startSpeed = this->speed;
  this->hasCollided = false;

  this->reset();
}

void Ball::update(std::vector<Paddle> &paddles)
{
  this->hasCollided = false;

  if(this->velocity == glm::vec2(0.0f))
  {
    return;
  }

  paddleCollisionCheck(paddles);
  pointCheck(paddles);
  wallCheck();

  this->velocity = glm::normalize(this->velocity);
  this->position += this->velocity * this->speed * Engine::tempo.deltaTime;

  this->spriteEntity->transform.position = this->position;
  this->spriteEntity->update();
}

void Ball::reset()
{
  this->position = this->startPosition;

  this->speed = this->startSpeed;

  float angle = glm::radians(Random::choose(-60.0f, 60.0f));

  this->velocity = glm::vec2(std::cos(angle), std::sin(angle));

  int side = Random::choose(0, 1);

  if(side == 0)
  {
    this->velocity *= -1;
  }
}

void Ball::toggleState()
{
  this->hidden = !this->hidden;
  this->spriteEntity->sprite.display = this->hidden ? 1 : 0;
  this->reset();
}

void Ball::paddleCollisionCheck(std::vector<Paddle> &paddles)
{
  int index = this->velocity.x < 0 ? 0 : 1;

  auto &paddle = paddles[index];

  bool isInsidePaddle = this->position.y + this->size.y > paddle.position.y && this->position.y < paddle.position.y + paddle.size.y;

  if(index == 0)
  {
    if(this->velocity.x < 0 && this->position.x < paddle.position.x + paddle.size.x && isInsidePaddle)
    {
      this->velocity.x *= -1.0f;
      this->position.x = paddle.position.x + paddle.size.x;
      this->speed *= 1.05f;;
    }

    return;
  }

  if(this->velocity.x > 0 && this->position.x + this->size.x > paddle.position.x && isInsidePaddle)
  {
    this->velocity.x *= -1.0f;
    this->position.x = paddle.position.x - this->size.x;
    this->speed *= 1.05f;;
  }
}

void Ball::pointCheck(std::vector<Paddle> &paddles)
{
  int index = this->velocity.x < 0 ? 0 : 1;

  auto &paddle = paddles[index];

  if(index == 0 && this->position.x < paddle.position.x + paddle.size.x)
  {
    this->hasCollided = true;
    paddles[1].setScore(paddles[1].score +1);
    this->reset();

    return;
  }

  if(index == 1 && this->position.x > paddle.position.x - this->size.x)
  {
    this->hasCollided = true;

    paddles[0].setScore(paddles[0].score +1);
    this->reset();
  }
}

void Ball::wallCheck()
{
  if(this->position.y <= 0.0f)
  {
    this->position.y = 0.0f;
    this->velocity.y *= -1.0f;
  }

  if(this->position.y >= Engine::window.resolutionHeight - this->size.y)
  {
    this->position.y = Engine::window.resolutionHeight - this->size.y;
    this->velocity.y *= -1.0f;
  }
}