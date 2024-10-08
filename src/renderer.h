#pragma once

#include "types.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>

struct Tile {
    SDL_Rect rect;

    inline static const int SIZE = 20;
    enum Type {
        NW,
        N,
        NE,
        W,
        FILL,
        E,
        SW,
        S,
        SE,
        FILL1,
        CORNER1,
        CORNER2,
        FILL2,
        CORNER3,
        CORNER4,
    };
};

struct Terrain {
    std::vector<Tile> tiles;
    SDL_Surface *surface;
    SDL_Texture *texture;
};

struct Frame {
    SDL_Rect rect;
    Vector2 pivot;
};

struct Atlas {
    Vector<Frame> frames;
    SDL_Surface *surface;
    SDL_Texture *texture;
};

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void init();
    void clear();
    void update(const uint32_t ticks);

    void loadAtlas(const std::string &name, const bool skip_empty = true, const int delimiter = 254,
                   const bool check_content = true, const bool skip1 = false, const int step = 1);
    void loadAtlas(const std::string &name, const int frame_width, const int frame_height, const int max_frames,
                   const bool skip1 = true);
    void loadTerrain(const std::string &name);
    void loadTexture(const std::string &name, const bool center_pivot = true);
    void loadFont(const std::string &name);

    const Terrain &getTerrain(const std::string &name);
    const Atlas &getAtlas(const std::string &name);

    void draw(const Vector2 &pos, const Atlas &atlas, const int frameindex, const bool use_pivot = true,
              const float scale = 1.0f, const bool use_camera = true, const int r = 255, const int g = 255,
              const int b = 255);
    void draw(const Vector2 &pos, const std::string &name, const int frameindex, const bool use_pivot = true,
              const int r = 255, const int g = 255, const int b = 255);
    void draw(const Vector2 &pos, const Terrain &terrain, const int tileindex);
    void draw(const Vector2 &pos, const std::string &name, const bool use_camera = false);

    void computeTextRect(SDL_Rect &rect, const Vector2 &pos, const String &text, const float scale);

    void drawText(const Vector2 &pos, const std::string &text, const float scale = 1.0f, const bool background = true,
                  const bool use_camera = false, const float alpha = 1.0f);

    void drawCenteredText(const int y, const std::string &text, const float scale = 1.0f, const bool background = true,
                          const float alpha = 1.0f);

    void drawFilledQuad(const Vector2 &pos, const Vector2 &size, const int r, const int g, const int b,
                        const float alpha = 1.0f, const bool use_camera = false);
    void drawQuad(const Vector2 &pos, const Vector2 &size, const int r, const int g, const int b,
                  const bool use_camera = false);
    void drawLine(const Vector2 &pos, const Vector2 &pos2, const int r, const int g, const int b,
                  const bool use_camera = false);

    void drawProgressBar(const Vector2 &pos, const float width, const float value, const bool gradient = false);

    void setPivot(const std::string &name, const int frameindex, const Vector2 &pivot);
    int getFramesCount(const std::string &name) const;
    void exportAtlas(const std::string &name);

    Vector2 &getCameraPosition();
    const Vector2 &getCameraPosition() const;

    void onWindowEvent(const SDL_WindowEvent &wevent);

    void toggleFullscreen();

    int width;
    int height;

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
