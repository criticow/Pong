#include <ddengine/ddengine.hpp>
#include "game.hpp"

#ifdef _WIN32
  // For Windows, use WinMain for release build to suppress the console window
  #if defined(NDEBUG)
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
    {
      Game game(1280, 720, 1280, 720, "Pong");
      game.run();
      return 0;
    }
  #else
    int main()
    {
      // For other build configurations (Debug, etc.), use the original main
      Game game(1280, 720, 1280, 720, "Pong");
      game.run();
      return 0;
    }
  #endif
#endif