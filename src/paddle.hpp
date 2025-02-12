#pragma once

#include <ddengine/ddengine.hpp>
#include "ball.hpp"

#define PLAYER_SIDE_LEFT 0
#define PLAYER_SIDE_RIGHT 1

class Paddle
{
  public:
  glm::vec2 position;
  glm::vec2 size;
  float speed;
  int score;
  bool isAI;

  Paddle(){};
  Paddle(int side, bool isAI, Ball *ball);

  void update();
  void setScore(int score);
  void reset();
  void toggleState();

  private:
  glm::vec2 startPosition;
  SpriteEntity *spriteEntity = nullptr;
  Text *text = nullptr;
  Ball *ball = nullptr;
  std::string downKey;
  std::string upKey;
  glm::vec4 color;
  bool hidden = true;

  float predictBallY(glm::vec2 ballPosition, glm::vec2 ballVelocity);
  void updatePlayerMovement();
  void updateAIMovement();
};