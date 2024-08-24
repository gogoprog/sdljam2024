#include "level.h"
#include <algorithm>
#include <queue>
#include <sstream>

Level::Level() {
    tilewidth = 128;
    tileheight = 128;

    width = tilewidth * tileSpacing;
    height = tileheight * tileSpacing;

    buildCache();

    startCoords = {{5, 5}, {120, 120}};

    /* setLockedRoad(beginCoords); */
    /* setLockedRoad({beginCoords.x, beginCoords.y - 1}); */
    /* setLockedRoad(endCoords); */
    /* setLockedRoad({endCoords.x, endCoords.y + 1}); */

    Vector<Vector2> roads = {
        {2, -2},  {3, -2},  {4, -2},  {0, -1},  {1, -1},  {2, -1},  {4, -1},  {5, -1},  {8, -1},  {9, -1},  {-1, 0},
        {0, 0},   {1, 0},   {2, 0},   {3, 0},   {4, 0},   {5, 0},   {6, 0},   {7, 0},   {8, 0},   {9, 0},   {10, 0},
        {11, 0},  {0, 1},   {1, 1},   {2, 1},   {3, 1},   {4, 1},   {5, 1},   {6, 1},   {7, 1},   {8, 1},   {9, 1},
        {10, 1},  {11, 1},  {12, 1},  {16, 1},  {17, 1},  {0, 2},   {1, 2},   {2, 2},   {3, 2},   {4, 2},   {5, 2},
        {6, 2},   {7, 2},   {8, 2},   {9, 2},   {10, 2},  {11, 2},  {12, 2},  {13, 2},  {14, 2},  {15, 2},  {16, 2},
        {17, 2},  {0, 3},   {1, 3},   {2, 3},   {3, 3},   {4, 3},   {5, 3},   {6, 3},   {7, 3},   {8, 3},   {9, 3},
        {10, 3},  {11, 3},  {12, 3},  {13, 3},  {14, 3},  {15, 3},  {16, 3},  {17, 3},  {-1, 4},  {0, 4},   {1, 4},
        {2, 4},   {3, 4},   {4, 4},   {5, 4},   {6, 4},   {7, 4},   {8, 4},   {9, 4},   {10, 4},  {11, 4},  {12, 4},
        {13, 4},  {14, 4},  {15, 4},  {16, 4},  {-1, 5},  {0, 5},   {1, 5},   {2, 5},   {3, 5},   {4, 5},   {5, 5},
        {6, 5},   {7, 5},   {8, 5},   {9, 5},   {10, 5},  {11, 5},  {12, 5},  {13, 5},  {14, 5},  {15, 5},  {16, 5},
        {-1, 6},  {0, 6},   {1, 6},   {2, 6},   {3, 6},   {4, 6},   {5, 6},   {6, 6},   {7, 6},   {8, 6},   {9, 6},
        {10, 6},  {11, 6},  {12, 6},  {13, 6},  {14, 6},  {15, 6},  {0, 7},   {1, 7},   {2, 7},   {3, 7},   {4, 7},
        {5, 7},   {6, 7},   {7, 7},   {8, 7},   {9, 7},   {10, 7},  {11, 7},  {12, 7},  {13, 7},  {14, 7},  {15, 7},
        {-1, 8},  {0, 8},   {1, 8},   {2, 8},   {3, 8},   {4, 8},   {5, 8},   {6, 8},   {7, 8},   {8, 8},   {9, 8},
        {10, 8},  {11, 8},  {12, 8},  {13, 8},  {14, 8},  {15, 8},  {-1, 9},  {0, 9},   {1, 9},   {2, 9},   {3, 9},
        {4, 9},   {5, 9},   {6, 9},   {7, 9},   {8, 9},   {9, 9},   {10, 9},  {11, 9},  {12, 9},  {13, 9},  {0, 10},
        {1, 10},  {2, 10},  {3, 10},  {4, 10},  {5, 10},  {6, 10},  {7, 10},  {8, 10},  {9, 10},  {10, 10}, {11, 10},
        {12, 10}, {13, 10}, {0, 11},  {1, 11},  {2, 11},  {3, 11},  {4, 11},  {5, 11},  {6, 11},  {7, 11},  {8, 11},
        {9, 11},  {10, 11}, {11, 11}, {12, 11}, {13, 11}, {0, 12},  {1, 12},  {2, 12},  {3, 12},  {4, 12},  {5, 12},
        {6, 12},  {7, 12},  {8, 12},  {9, 12},  {10, 12}, {11, 12}, {12, 12}, {13, 12}, {14, 12}, {0, 13},  {1, 13},
        {2, 13},  {3, 13},  {4, 13},  {5, 13},  {6, 13},  {7, 13},  {8, 13},  {9, 13},  {10, 13}, {11, 13}, {12, 13},
        {13, 13}, {14, 13}, {0, 14},  {1, 14},  {2, 14},  {3, 14},  {4, 14},  {5, 14},  {6, 14},  {12, 14}, {13, 14},
        {14, 14}, {15, 14}, {0, 15},  {1, 15},  {2, 15},  {3, 15},  {4, 15},  {13, 15}, {14, 15}, {15, 15}, {0, 16},
        {1, 16},  {2, 16},  {3, 16},  {4, 16},  {13, 16}, {14, 16}, {15, 16}, {0, 17},  {1, 17},  {2, 17},  {3, 17},
        {4, 17},  {5, 17},  {12, 17}, {13, 17}, {14, 17}, {15, 17}, {0, 18},  {1, 18},  {2, 18},  {3, 18},  {4, 18},
        {5, 18},  {6, 18},  {11, 18}, {12, 18}, {13, 18}, {14, 18}, {15, 18}, {0, 19},  {1, 19},  {2, 19},  {3, 19},
        {4, 19},  {5, 19},  {6, 19},  {7, 19},  {8, 19},  {9, 19},  {10, 19}, {11, 19}, {12, 19}, {13, 19}, {14, 19},
        {0, 20},  {1, 20},  {2, 20},  {3, 20},  {4, 20},  {5, 20},  {6, 20},  {7, 20},  {8, 20},  {9, 20},  {10, 20},
        {11, 20}, {0, 21},  {3, 21},  {4, 21},  {10, 21}};

    for (auto &r : roads) {
        setRoad(r, true);
    }
}

void Level::render(uint32_t ticks, Renderer &renderer) {
    auto &terrain = renderer.getTerrain("Snd2Watr");

    int i = 0;
    for (int y = 0; y < tileheight; ++y) {
        for (int x = 0; x < tilewidth; ++x) {
            if (cachedTypes[i] != Tile::FILL) {
                renderer.draw({x * tileSpacing, y * tileSpacing}, terrain, cachedTypes[i]);
            } else {
                static const int tiles[] = {Tile::FILL, Tile::FILL1};
                int r = (x + y + ticks / 500) % 2;

                renderer.draw({x * tileSpacing, y * tileSpacing}, terrain, tiles[r]);
            }
            ++i;
        }
    }
}

bool Level::findPath(Path &path, const Vector2 &start, const Vector2 &end) {
    struct Node {
        Path path;
        float distanceLeft;

        bool operator<(const Node &other) const {
            if (distanceLeft == other.distanceLeft) {
                return path.size() > other.path.size();
            }
            return distanceLeft > other.distanceLeft;
        }
    };

    std::priority_queue<Node> q;
    q.push({{start}, (start - end).getLength()});

    Map<Vector2, int> visited;

    while (!q.empty()) {
        const auto node = q.top();
        q.pop();
        const auto last = node.path.back();

        if (visited.find(last) == visited.end() || visited[last] > node.path.size()) {
            visited[last] = node.path.size();

            if (last == end) {
                path = node.path;
                return true;
                break;
            } else {
                for (auto &direction : directions) {
                    auto next = last + direction;

                    if (getRoad(next) && !locks[next]) {
                        if (std::find(node.path.begin(), node.path.end(), next) == node.path.end()) {
                            auto copy = node;
                            copy.path.push_back(next);
                            copy.distanceLeft = (next - end).getLength();
                            q.push(copy);
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Level::isFree(const Vector2 &coords) {
    return (!roadmap[coords] && !locks[coords]);
}

bool Level::canBuildAt(const Vector2 &coords) {

    if (!isFree(coords)) {
        return false;
    }
    if (!isFree({coords.x, coords.y - 1})) {
        return false;
    }
    if (!isFree({coords.x - 1, coords.y - 1})) {
        return false;
    }
    if (!isFree({coords.x - 1, coords.y})) {
        return false;
    }

    return true;
}

void Level::buildCache() {
    cachedTypes.resize(tilewidth * tileheight);
    updateCache({0, 0}, {tilewidth - 1, tileheight - 1});
}

void Level::updateCache(const Vector2 &from, const Vector2 &to) {
    auto minx = std::max<int>(from.x, 0);
    auto maxx = std::min<int>(to.x, tilewidth - 1);
    auto miny = std::max<int>(from.y, 0);
    auto maxy = std::min<int>(to.y, tileheight - 1);

    for (int x = minx; x <= maxx; ++x) {
        for (int y = miny; y <= maxy; ++y) {
            Vector2 pos{x, y};
            int type = Tile::FILL;

            if (getRoad(pos)) {
                type = Tile::FILL2;
            } else {

                if (getRoad({x + 1, y + 1})) {
                    type = Tile::CORNER4;
                }
                if (getRoad({x - 1, y + 1})) {
                    type = Tile::CORNER3;
                }
                if (getRoad({x - 1, y - 1})) {
                    type = Tile::CORNER1;
                }
                if (getRoad({x + 1, y - 1})) {
                    type = Tile::CORNER2;
                }

                if (getRoad({x + 1, y})) {
                    type = Tile::E;

                    if (getRoad({x, y + 1})) {
                        type = Tile::SE;
                    } else if (getRoad({x, y - 1})) {
                        type = Tile::NE;
                    }
                } else if (getRoad({x - 1, y})) {
                    type = Tile::W;

                    if (getRoad({x, y + 1})) {
                        type = Tile::SW;
                    } else if (getRoad({x, y - 1})) {
                        type = Tile::NW;
                    }
                } else if (getRoad({x, y - 1})) {
                    type = Tile::N;
                } else if (getRoad({x, y + 1})) {
                    type = Tile::S;
                }
            }

            cachedTypes[y * tilewidth + x] = type;
        }
    }
}

void Level::setLockedRoad(const Vector2 &coords) {
    setRoad(coords, true);
    locks[coords] = true;
}

void Level::dump() {

    std::stringstream ss;

    ss << "Vector<Vector2> roads = { ";

    for (auto &kv : roadmap) {
        if (kv.second) {
            auto c = kv.first;
            ss << "{" << c.x << ", " << c.y << "}, ";
        }
    }

    ss.seekp(-2, std::ios_base::end);

    ss << " };\n";

    puts(ss.str().c_str());
}
