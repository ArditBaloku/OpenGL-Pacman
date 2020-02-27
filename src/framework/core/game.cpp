#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "util.cpp"

SpriteRenderer *Renderer;
GameObject *Player;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game()
{
  delete Renderer;
  delete Player;
}

void Game::Init()
{
  // Load shaders
  ResourceManager::LoadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", "sprite");
  // Configure shaders
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  // Load textures
  ResourceManager::LoadTexture("resources/textures/food.png", GL_TRUE, "food");
  ResourceManager::LoadTexture("resources/textures/wall.png", GL_TRUE, "wall");
  ResourceManager::LoadTexture("resources/textures/pacman.png", GL_TRUE, "pacman");

  GameLevel one;
  one.Load("../levels/level0", this->Width, this->Height);
  this->Levels.push_back(one);
  this->Level = 0;
  // Set render-specific controls
  Shader myShader = ResourceManager::GetShader("sprite");
  Renderer = new SpriteRenderer(myShader);

  // Configure player
  glm::vec2 playerPos = glm::vec2(this->Levels[this->Level].PLAYER_POSITION.x, this->Levels[this->Level].PLAYER_POSITION.y);
  Player = new GameObject(playerPos, this->Levels[this->Level].PLAYER_SIZE, ResourceManager::GetTexture("pacman"));
}

void Game::Update(GLfloat dt)
{
}

void Game::ProcessInput(GLfloat dt)
{
  if (this->State == GAME_ACTIVE)
  {
    bool collision = false;
    GLfloat velocity = this->Levels[this->Level].PLAYER_VELOCITY * dt;
    // Move playerboard
    if (this->Keys[GLFW_KEY_A])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, LEFT, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;

              if (Player->Position.x >= 0 - Player->Size.x)
                Player->Position.x -= velocity;
              else
                Player->Position.x = this->Width;
            }
          }
        }
      }

      if (!collision)
      {
        if (Player->Position.x >= 0 - Player->Size.x)
          Player->Position.x -= velocity;
        else
          Player->Position.x = this->Width;
      }
    }
    if (this->Keys[GLFW_KEY_D])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, RIGHT, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;

              if (Player->Position.x <= this->Width)
                Player->Position.x += velocity;
              else
                Player->Position.x = 0 - Player->Size.x;
            }
          }
        }
      }

      if (!collision)
      {
        if (Player->Position.x <= this->Width)
          Player->Position.x += velocity;
        else
          Player->Position.x = 0 - Player->Size.x;
      }
    }
    if (this->Keys[GLFW_KEY_W])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, UP, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;

              if (Player->Position.y >= 0)
                Player->Position.y -= velocity;
            }
          }
        }
      }

      if (!collision)
      {

        if (Player->Position.y >= 0)
          Player->Position.y -= velocity;
      }
    }
    if (this->Keys[GLFW_KEY_S])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, DOWN, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;

              if (Player->Position.y <= this->Height - Player->Size.y)
                Player->Position.y += velocity;
            }
          }
        }
      }

      if (!collision)
      {

        if (Player->Position.y <= this->Height - Player->Size.y)
          Player->Position.y += velocity;
      }
    }
  }
}

void Game::Render()
{
  if (this->State == GAME_ACTIVE)
  {
    // Draw level
    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
  }
}