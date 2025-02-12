#pragma once

#include <ddengine/ddengine.hpp>

class Paddle;

class Ball
{
  public:
  glm::vec2 position;
  glm::vec2 velocity;
  Paddle *paddleA = nullptr;
  Paddle *paddleB = nullptr;
  bool hasCollided;

  Ball();

  void update(std::vector<Paddle> &paddles);
  void reset();
  void toggleState();

  private:
  glm::vec2 startPosition;
  glm::vec2 size;
  float speed;
  float startSpeed;
  SpriteEntity *spriteEntity = nullptr;
  bool hidden = true;

  void paddleCollisionCheck(std::vector<Paddle> &paddles);
  void pointCheck(std::vector<Paddle> &paddles);
  void wallCheck();
};