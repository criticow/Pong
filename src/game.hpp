#pragma once

#include <ddengine/ddengine.hpp>
#include "paddle.hpp"
#include "ball.hpp"
#include "button.hpp"

class Game : public Engine
{
  public:
  Game(int width, int height, int resWidth, int resHeight, const char *title) : 
    Engine(width, height, resWidth, resHeight, title){};

  void start()
  {
    this->scene = 1;
  }

  // This method is called in the end of the main update
  void onUpdate() override
  {
    if(scene == 0)
    {
      if(buttons[0].clicked)
      {
        setupPlayers(false, false);
      }
      if(buttons[1].clicked)
      {
        setupPlayers(false, true);
      }
      if(buttons[2].clicked)
      {
        setupPlayers(true, false);
      }
      if(buttons[3].clicked)
      {
        setupPlayers(true, true);
      }

      buttons[0].update();
      buttons[1].update();
      buttons[2].update();
      buttons[3].update();


      for(auto &text : menuTexts)
      {
        text.update();
      }
    }

    if(scene == 1)
    {
      if(input.pressed(KEY_ESC))
      {
        startMenu();
      }

      paddles[0].update();
      paddles[1].update();
  
      ball->update(paddles);
  
      if(ball->hasCollided)
      {
        paddles[0].reset();
        paddles[1].reset();
      }

      middleLine->update();
    }

  };

  // This method is called in the end of the main setup
  void onSetup() override
  {
    window.setIcon("data/icons/window");
    auto font = resourceManager.addResource<Font>("dogica_font", Font(FontCreateInfo{
      .name = "dogica",
      .path = "data/fonts/dogica.ttf",
      .size = 48,
      .pixelated = true
    }));

    auto font2 = resourceManager.addResource<Font>("dogica_font_16", Font(FontCreateInfo{
      .name = "dogica_16",
      .path = "data/fonts/dogica.ttf",
      .size = 16,
      .pixelated = true
    }));

    float titleTextWidth = font->getTextWidth("PONG");

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font,
      .value = "PONG",
      .position = glm::vec2(window.resolutionWidth * 0.5f - titleTextWidth * 0.5f, 75.0f),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    std::string techTextValue = "Made with C++ and OpenGL";
    float techTextWidth = font2->getTextWidth(techTextValue);

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font2,
      .value = techTextValue,
      .position = glm::vec2(window.resolutionWidth * 0.5f - techTextWidth * 0.5f, 75.0f + font->maxHeight + 8.0),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    std::string creditsTextValue = "Made by Iury Ferreira. Powered by ddengine. Feb/2025";
    float creditsTextWidth = font2->getTextWidth(creditsTextValue);

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font2,
      .value = creditsTextValue,
      .position = glm::vec2(window.resolutionWidth * 0.5f - creditsTextWidth * 0.5f, window.resolutionHeight - 30.0f),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font2,
      .value = "Controls",
      .position = glm::vec2(10.0f, window.resolutionHeight * 0.5f - font2->maxHeight * 4.0f),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font2,
      .value = "Menu - ESC",
      .position = glm::vec2(10.0f, window.resolutionHeight * 0.5f - font2->maxHeight * 3.0f + 12.0f),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font2,
      .value = "Left Paddle - W / S",
      .position = glm::vec2(10.0f, window.resolutionHeight * 0.5f - font2->maxHeight * 2.0f + 14.0f),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    menuTexts.push_back(Text(TextCreateInfo{
      .font = font2,
      .value = "Right Paddle - UP / Down",
      .position = glm::vec2(10.0f, window.resolutionHeight * 0.5f - font2->maxHeight * 1.0f + 16.0f),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      .layer = 2
    }));

    glm::vec2 buttonSize(300.0f, 75.0f);

    float xPos = window.resolutionWidth * 0.5f - buttonSize.x * 0.5f;
    float yPos = 200.0f;
    float padding = 10.0f;

    buttons.push_back(Button(glm::vec2(xPos, yPos), buttonSize, "PLAYER 1 VS PLAYER 2"));
    buttons.push_back(Button(glm::vec2(xPos, yPos + buttonSize.y + padding), buttonSize, "PLAYER 1 VS AI"));
    buttons.push_back(Button(glm::vec2(xPos, yPos + buttonSize.y * 2.0f + padding * 2.0f), buttonSize, "AI VS PLAYER 2"));
    buttons.push_back(Button(glm::vec2(xPos, yPos + buttonSize.y * 3.0f + padding * 3.0f), buttonSize, "AI VS AI"));

    ball = new Ball();

    paddles.push_back(Paddle(PLAYER_SIDE_LEFT, false, ball));
    paddles.push_back(Paddle(PLAYER_SIDE_RIGHT, false, ball));

    middleLine = new Line(LineCreateInfo{
      .position = glm::vec2(window.resolutionWidth * 0.5f, 0.0f),
      .length = glm::vec2(0.0f, window.resolutionHeight),
      .color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f),
      .state = InstanceState::DISABLED
    });
  };

  void onRender() override
  {
  };

  private:
  std::vector<Paddle> paddles;
  std::vector<Button> buttons;
  std::vector<Text> menuTexts;
  Ball *ball = nullptr;
  Line *middleLine = nullptr;
  int scene = 0;
  bool AI[2] = {false, false};

  void setupPlayers(bool leftAI, bool rightAI)
  {
    buttons[0].toggle();
    buttons[1].toggle();
    buttons[2].toggle();
    buttons[3].toggle();

    for(auto &text : menuTexts)
    {
      text.setDisplay(1);
    }

    paddles[0].toggleState();
    paddles[0].setScore(0);
    paddles[0].isAI = leftAI;

    paddles[1].toggleState();
    paddles[1].setScore(0);
    paddles[1].isAI = rightAI;

    middleLine->setState(InstanceState::ENABLED);

    ball->toggleState();

    scene = 1;
  }

  void startMenu()
  {
    buttons[0].toggle();
    buttons[1].toggle();
    buttons[2].toggle();
    buttons[3].toggle();

    for(auto &text : menuTexts)
    {
      text.setDisplay(0);
    }

    middleLine->setState(InstanceState::DISABLED);

    paddles[0].toggleState();
    paddles[0].setScore(0);

    paddles[1].toggleState();
    paddles[1].setScore(0);
    
    ball->toggleState();

    scene = 0;
  }
};