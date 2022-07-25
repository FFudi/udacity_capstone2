#include "renderer.h"
#include <iostream>
#include <string>
#include <sstream>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

// void Renderer::Render(Snake const snake, SDL_Point const &food, SDL_Point const &slowFood) {
void Renderer::Render(Snake const snake, SDL_Point const &food, SDL_Point const &slowFood, SDL_Point const &obstacle) {
  SDL_Rect block;
  SDL_Rect block_slow;
  SDL_Rect block_obstacle;
  
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  block_slow.w = screen_width / grid_width;
  block_slow.h = screen_height / grid_height;

  block_obstacle.w = screen_width / grid_width;
  block_obstacle.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render slowfood
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xCC, 0x00, 0xFF);
  block_slow.x = slowFood.x * block_slow.w;
  block_slow.y = slowFood.y * block_slow.h;
  SDL_RenderFillRect(sdl_renderer, &block_slow);

  // Render obstacle
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  block_obstacle.x = obstacle.x * block_obstacle.w;
  block_obstacle.y = obstacle.y * block_obstacle.h;
  SDL_RenderFillRect(sdl_renderer, &block_obstacle);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, float snakeSpeed, int fps)
{
  std::ostringstream _speed;
  _speed.precision(3);
  _speed << std::fixed << snakeSpeed;
  auto speed = _speed.str();

  std::string title{"Snake Score: " + std::to_string(score) + " Snake Speed: " + speed + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
