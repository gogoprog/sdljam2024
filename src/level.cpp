
#include "level.h"
#include "context.h"
#include <algorithm>
#include <queue>
#include <sstream>

struct Level::Pimpl {
    UnorderedMap<Vector2, UnorderedMap<Vector2, Path>> pathCache;
};

Level::Level() : pimpl(new Level::Pimpl()) {
    tilewidth = 64;
    tileheight = 64;

    reset();
}

Level::~Level() = default;

void Level::reset() {

    width = tilewidth * tileSpacing;
    height = tileheight * tileSpacing;

    buildCache();

    startCoords = {{5, 5}, {tilewidth - 5, tileheight - 5}};

    /* setLockedRoad(beginCoords); */
    /* setLockedRoad({beginCoords.x, beginCoords.y - 1}); */
    /* setLockedRoad(endCoords); */
    /* setLockedRoad({endCoords.x, endCoords.y + 1}); */

    Vector<Vector2> roads = {
        {65, 54},   {66, 50},   {65, 51},   {66, 48},   {66, 49},   {65, 49},   {65, 50},   {65, 59},   {69, 56},
        {68, 56},   {68, 57},   {67, 57},   {66, 57},   {65, 57},   {65, 58},   {66, 55},   {66, 56},   {65, 56},
        {65, 53},   {65, 64},   {66, 64},   {67, 63},   {66, 63},   {65, 63},   {32, 60},   {93, 90},   {32, 43},
        {32, 42},   {32, 41},   {32, 40},   {32, 39},   {32, 38},   {32, 37},   {32, 36},   {32, 35},   {32, 34},
        {37, 112},  {37, 111},  {32, 18},   {32, 17},   {31, 60},   {92, 90},   {31, 44},   {31, 43},   {31, 42},
        {31, 41},   {31, 40},   {31, 39},   {31, 38},   {31, 37},   {31, 36},   {31, 35},   {31, 34},   {36, 111},
        {31, 17},   {91, 90},   {30, 44},   {30, 43},   {30, 42},   {30, 41},   {30, 40},   {30, 39},   {30, 38},
        {30, 37},   {30, 36},   {30, 35},   {35, 111},  {35, 110},  {30, 17},   {29, 60},   {90, 90},   {29, 44},
        {29, 43},   {29, 42},   {29, 41},   {29, 40},   {29, 39},   {29, 38},   {29, 37},   {29, 36},   {34, 111},
        {34, 110},  {34, 109},  {34, 108},  {34, 107},  {29, 17},   {28, 60},   {89, 90},   {28, 41},   {28, 40},
        {28, 39},   {28, 38},   {28, 37},   {28, 36},   {33, 110},  {33, 109},  {33, 108},  {28, 17},   {27, 60},
        {88, 90},   {27, 41},   {27, 40},   {27, 39},   {27, 38},   {27, 37},   {27, 17},   {26, 60},   {87, 90},
        {26, 41},   {26, 40},   {26, 39},   {26, 38},   {26, 37},   {26, 17},   {25, 60},   {86, 90},   {25, 40},
        {25, 39},   {25, 38},   {25, 17},   {24, 60},   {24, 59},   {85, 90},   {24, 40},   {24, 39},   {24, 17},
        {23, 59},   {22, 59},   {83, 90},   {22, 42},   {22, 41},   {22, 40},   {22, 39},   {22, 17},   {21, 59},
        {21, 58},   {82, 90},   {21, 43},   {21, 42},   {21, 41},   {21, 40},   {21, 39},   {21, 17},   {20, 58},
        {81, 90},   {20, 44},   {20, 43},   {20, 39},   {20, 38},   {20, 17},   {19, 58},   {19, 57},   {80, 90},
        {19, 45},   {19, 44},   {19, 39},   {19, 38},   {19, 37},   {19, 17},   {18, 57},   {18, 46},   {79, 90},
        {18, 45},   {18, 37},   {18, 36},   {18, 17},   {17, 57},   {17, 56},   {17, 55},   {17, 50},   {17, 49},
        {17, 48},   {17, 47},   {17, 46},   {78, 90},   {17, 36},   {17, 35},   {17, 24},   {17, 17},   {17, 3},
        {17, 2},    {17, 1},    {16, 55},   {16, 54},   {16, 53},   {16, 52},   {16, 51},   {16, 50},   {77, 90},
        {16, 35},   {16, 34},   {16, 25},   {16, 24},   {16, 23},   {16, 17},   {16, 5},    {16, 4},    {16, 3},
        {16, 2},    {16, 1},    {67, 100},  {67, 86},   {67, 85},   {128, 129}, {128, 128}, {128, 127}, {128, 126},
        {128, 125}, {128, 124}, {128, 123}, {128, 122}, {128, 121}, {128, 120}, {128, 119}, {128, 118}, {128, 117},
        {128, 116}, {128, 115}, {128, 114}, {128, 113}, {6, 20},    {6, 19},    {6, 18},    {15, 33},   {15, 25},
        {15, 24},   {15, 23},   {66, 85},   {127, 129}, {127, 127}, {127, 126}, {127, 125}, {15, 18},   {127, 124},
        {15, 17},   {127, 123}, {15, 16},   {127, 122}, {15, 15},   {127, 121}, {15, 14},   {127, 120}, {127, 119},
        {127, 118}, {127, 117}, {127, 116}, {127, 115}, {15, 8},    {127, 114}, {15, 7},    {127, 113}, {15, 6},
        {127, 112}, {15, 5},    {127, 111}, {15, 4},    {15, 3},    {15, 2},    {5, 20},    {5, 19},    {5, 18},
        {5, 17},    {5, 14},    {5, 13},    {5, 12},    {5, 11},    {5, 10},    {5, 9},     {5, 8},     {5, 7},
        {5, 6},     {5, 5},     {5, 4},     {5, 3},     {5, 2},     {5, 0},     {5, -1},    {5, 1},     {75, 89},
        {14, 33},   {14, 32},   {14, 28},   {14, 26},   {14, 25},   {14, 24},   {14, 23},   {65, 85},   {126, 129},
        {14, 22},   {126, 128}, {14, 21},   {126, 127}, {14, 20},   {126, 126}, {14, 19},   {126, 125}, {14, 18},
        {126, 124}, {14, 17},   {126, 123}, {14, 16},   {126, 122}, {14, 15},   {126, 121}, {14, 14},   {126, 120},
        {14, 13},   {126, 119}, {14, 12},   {126, 118}, {126, 117}, {126, 116}, {126, 115}, {14, 8},    {126, 113},
        {14, 6},    {126, 112}, {14, 5},    {126, 111}, {14, 4},    {14, 3},    {4, 21},    {14, 2},    {4, 20},
        {4, 19},    {4, 18},    {4, 17},    {4, 16},    {4, 15},    {4, 14},    {4, 13},    {4, 12},    {4, 11},
        {4, 10},    {4, 9},     {4, 8},     {4, 7},     {4, 6},     {4, 5},     {4, 4},     {4, 3},     {4, 2},
        {4, 0},     {4, -1},    {4, 1},     {125, 113}, {13, 6},    {125, 112}, {13, 5},    {13, 4},    {13, 3},
        {3, 21},    {13, 2},    {3, 20},    {64, 64},   {3, 19},    {64, 63},   {3, 18},    {64, 62},   {3, 17},
        {64, 61},   {3, 16},    {64, 60},   {3, 15},    {64, 59},   {3, 14},    {64, 58},   {3, 13},    {64, 57},
        {3, 12},    {64, 56},   {3, 11},    {64, 55},   {3, 10},    {64, 54},   {3, 9},     {64, 53},   {3, 8},
        {64, 52},   {3, 7},     {64, 51},   {3, 6},     {64, 50},   {3, 5},     {3, 4},     {3, 3},     {3, 2},
        {3, 0},     {3, 1},     {73, 88},   {34, 43},   {74, 89},   {41, 52},   {34, 42},   {74, 88},   {41, 51},
        {34, 41},   {41, 50},   {34, 40},   {41, 49},   {34, 39},   {6, 14},    {41, 48},   {34, 38},   {62, 84},
        {123, 128}, {6, 13},    {33, 60},   {71, 87},   {41, 47},   {34, 37},   {123, 127}, {11, 20},   {6, 12},
        {34, 36},   {123, 126}, {11, 19},   {6, 11},    {23, 17},   {34, 35},   {123, 125}, {11, 18},   {6, 10},
        {123, 124}, {6, 9},     {123, 123}, {6, 8},     {123, 122}, {6, 7},     {132, 125}, {123, 121}, {6, 6},
        {132, 124}, {123, 120}, {11, 13},   {6, 5},     {132, 123}, {13, 32},   {123, 119}, {11, 12},   {6, 4},
        {132, 122}, {13, 31},   {123, 118}, {11, 11},   {6, 3},     {132, 121}, {33, 38},   {61, 84},   {122, 128},
        {10, 21},   {125, 126}, {13, 19},   {1, 18},    {62, 62},   {13, 30},   {39, 114},  {123, 117}, {11, 10},
        {6, 2},     {33, 37},   {122, 127}, {10, 20},   {125, 125}, {13, 18},   {1, 17},    {62, 61},   {13, 29},
        {39, 113},  {123, 116}, {11, 9},    {6, 1},     {33, 36},   {122, 126}, {10, 19},   {125, 124}, {13, 17},
        {-1, 9},    {1, 16},    {62, 60},   {13, 28},   {123, 115}, {11, 8},    {6, 0},     {33, 35},   {122, 125},
        {125, 123}, {13, 16},   {-1, 8},    {1, 15},    {62, 59},   {123, 114}, {11, 7},    {94, 90},   {1, 8},
        {62, 52},   {124, 124}, {12, 17},   {34, 19},   {41, 29},   {11, 2},    {12, 30},   {0, 5},     {46, 118},
        {11, 4},    {12, 32},   {0, 7},     {1, 10},    {62, 54},   {124, 126}, {12, 19},   {1, 7},     {41, 15},
        {124, 123}, {34, 18},   {1, 20},    {62, 64},   {41, 28},   {11, 1},    {12, 29},   {0, 4},     {46, 117},
        {11, 3},    {12, 31},   {0, 6},     {1, 9},     {62, 53},   {30, 60},   {124, 125}, {12, 18},   {1, 6},
        {41, 14},   {124, 122}, {34, 17},   {1, 19},    {62, 63},   {11, 0},    {0, 3},     {1, 5},     {124, 121},
        {12, 14},   {122, 114}, {10, 7},    {7, 5},     {41, 62},   {133, 123}, {0, 16},    {61, 60},   {1, 4},
        {124, 120}, {12, 13},   {1, 3},     {124, 119}, {12, 12},   {0, 11},    {61, 55},   {10, 4},    {7, 2},
        {41, 59},   {125, 120}, {13, 13},   {1, 12},    {62, 56},   {122, 122}, {72, 87},   {0, 1},     {0, 13},
        {61, 57},   {0, 0},     {126, 114}, {14, 7},    {42, 14},   {2, 6},     {1, 1},     {124, 117}, {12, 10},
        {33, 17},   {0, 19},    {61, 63},   {10, 0},    {125, 116}, {13, 9},    {122, 118}, {10, 11},   {7, 9},
        {41, 66},   {102, 110}, {123, 113}, {11, 6},    {0, 9},     {61, 53},   {63, 84},   {124, 128}, {0, 21},
        {10, 2},    {7, 0},     {41, 57},   {125, 118}, {13, 11},   {122, 120}, {10, 13},   {7, 11},    {23, 40},
        {102, 112}, {1, 14},    {62, 58},   {1, -1},    {124, 115}, {12, 8},    {0, 10},    {61, 54},   {10, 3},
        {7, 1},     {41, 58},   {125, 119}, {13, 12},   {1, 11},    {62, 55},   {122, 121}, {23, 41},   {0, 12},
        {61, 56},   {1, 0},     {124, 116}, {12, 9},    {0, 18},    {61, 62},   {125, 115}, {13, 8},    {122, 117},
        {10, 10},   {7, 8},     {41, 65},   {123, 112}, {11, 5},    {0, 8},     {124, 127}, {122, 113}, {10, 6},
        {7, 4},     {41, 61},   {133, 122}, {125, 122}, {13, 15},   {33, 34},   {122, 124}, {0, 15},    {61, 59},
        {0, 2},     {10, 5},    {7, 3},     {41, 60},   {133, 121}, {125, 121}, {13, 14},   {-1, 6},    {1, 13},
        {62, 57},   {122, 123}, {72, 88},   {0, 14},    {61, 58},   {1, 2},     {124, 118}, {12, 11},   {33, 18},
        {0, 20},    {61, 64},   {10, 1},    {41, 56},   {125, 117}, {13, 10},   {122, 119}, {10, 12},   {7, 10},
        {23, 39},   {41, 67},   {102, 111}, {63, 85},   {124, 129}, {38, 112},  {122, 115}, {10, 8},    {7, 6},
        {41, 63},   {125, 114}, {13, 7},    {38, 113},  {122, 116}, {10, 9},    {7, 7},     {41, 64},   {0, 17},
        {61, 61},   {125, 127}, {33, 39},   {125, 128}, {33, 40},   {64, 85},   {125, 129}, {33, 41},   {33, 42},
        {13, 24},   {33, 43},   {2, 1},     {2, -1},    {2, 0},     {2, 2},     {2, 3},     {84, 90},   {2, 4},
        {2, 5},     {72, 93},   {72, 94},   {2, 7},     {63, 51},   {2, 8},     {63, 52},   {2, 9},     {63, 53},
        {2, 10},    {63, 54},   {2, 11},    {63, 55},   {2, 12},    {63, 56},   {2, 13},    {63, 57},   {2, 14},
        {63, 58},   {2, 15},    {63, 59},   {2, 16},    {63, 60},   {2, 17},    {63, 61},   {2, 18},    {63, 62},
        {2, 19},    {63, 63},   {2, 20},    {63, 64},   {12, 1},    {12, 2},    {12, 3},    {12, 4},    {124, 112},
        {12, 5},    {124, 113}, {12, 6},    {124, 114}, {12, 7},    {7, 12},    {7, 13},    {7, 19},    {7, 20},
        {129, 120}, {129, 121}, {129, 123}, {129, 124}, {129, 125}, {129, 126}, {129, 127}, {129, 128}, {68, 86},
        {68, 98},   {68, 99},   {68, 100},  {8, 1},     {8, -1},    {8, 0},     {8, 2},     {8, 3},     {8, 4},
        {8, 5},     {8, 6},     {8, 7},     {8, 8},     {8, 9},     {8, 10},    {8, 11},    {8, 12},    {8, 13},
        {8, 19},    {8, 20},    {130, 121}, {130, 123}, {130, 124}, {130, 125}, {130, 126}, {130, 127}, {69, 86},
        {69, 96},   {69, 97},   {69, 98},   {9, 1},     {9, -1},    {9, 0},     {9, 2},     {9, 3},     {9, 4},
        {9, 5},     {9, 6},     {9, 7},     {9, 8},     {9, 9},     {9, 10},    {9, 11},    {9, 12},    {9, 13},
        {9, 19},    {9, 20},    {131, 121}, {131, 123}, {131, 124}, {131, 125}, {131, 126}, {70, 86},   {70, 87},
        {70, 95},   {70, 96},   {71, 94},   {71, 95},   {73, 91},   {73, 92},   {74, 90},   {74, 91},   {75, 90},
        {76, 90},   {95, 90},   {34, 60},   {34, 61},   {34, 62},   {35, 16},   {35, 17},   {35, 18},   {35, 19},
        {35, 20},   {40, 114},  {40, 115},  {40, 116},  {40, 117},  {40, 118},  {40, 119},  {40, 120},  {35, 35},
        {35, 36},   {35, 37},   {35, 38},   {35, 39},   {35, 40},   {35, 41},   {35, 42},   {96, 90},   {35, 57},
        {35, 58},   {35, 59},   {35, 60},   {35, 61},   {35, 62},   {35, 63},   {35, 64},   {36, 16},   {36, 20},
        {36, 21},   {41, 120},  {36, 34},   {41, 121},  {36, 35},   {36, 36},   {36, 37},   {36, 38},   {36, 39},
        {36, 40},   {36, 41},   {36, 42},   {97, 90},   {97, 91},   {97, 92},   {97, 93},   {36, 57},   {36, 58},
        {97, 102},  {36, 59},   {97, 103},  {36, 60},   {97, 104},  {36, 61},   {97, 105},  {36, 62},   {97, 106},
        {36, 63},   {97, 107},  {36, 64},   {56, 16},   {97, 108},  {37, 15},   {37, 16},   {37, 21},   {37, 22},
        {37, 31},   {37, 32},   {37, 33},   {42, 120},  {37, 34},   {42, 121},  {37, 37},   {37, 38},   {37, 39},
        {37, 40},   {37, 41},   {37, 42},   {37, 43},   {98, 90},   {98, 91},   {98, 92},   {98, 93},   {98, 94},
        {98, 95},   {98, 96},   {98, 97},   {98, 98},   {98, 99},   {37, 56},   {98, 100},  {37, 57},   {98, 101},
        {37, 58},   {98, 102},  {37, 59},   {98, 103},  {37, 60},   {98, 104},  {37, 61},   {98, 105},  {37, 62},
        {98, 106},  {37, 63},   {98, 107},  {37, 64},   {98, 108},  {38, 15},   {38, 22},   {38, 23},   {38, 24},
        {38, 29},   {38, 30},   {38, 31},   {43, 120},  {38, 43},   {38, 44},   {38, 56},   {38, 57},   {38, 58},
        {99, 102},  {38, 59},   {99, 103},  {38, 60},   {99, 104},  {38, 61},   {99, 105},  {38, 62},   {99, 106},
        {38, 63},   {99, 107},  {38, 64},   {99, 108},  {39, 15},   {39, 24},   {39, 25},   {39, 28},   {39, 29},
        {44, 119},  {44, 120},  {39, 44},   {39, 45},   {39, 56},   {39, 57},   {39, 58},   {39, 59},   {100, 103},
        {39, 60},   {100, 104}, {39, 61},   {100, 105}, {39, 62},   {100, 106}, {39, 63},   {100, 107}, {39, 64},
        {100, 108}, {40, 15},   {40, 27},   {40, 28},   {40, 29},   {45, 118},  {45, 119},  {40, 45},   {40, 46},
        {40, 52},   {40, 53},   {40, 54},   {40, 55},   {40, 56},   {40, 57},   {40, 58},   {40, 59},   {40, 60},
        {40, 61},   {40, 62},   {101, 106}, {40, 63},   {101, 107}, {40, 64},   {101, 108}, {42, 29},   {47, 116},
        {47, 117},  {42, 49},   {42, 50},   {42, 56},   {42, 57},   {42, 58},   {42, 59},   {42, 60},   {42, 61},
        {42, 62},   {42, 63},   {42, 64},   {43, 14},   {43, 29},   {48, 116},  {43, 57},   {43, 58},   {43, 59},
        {43, 60},   {43, 61},   {43, 62},   {43, 63},   {43, 64},   {44, 14},   {49, 115},  {44, 29},   {49, 116},
        {44, 58},   {44, 59},   {44, 60},   {44, 61},   {44, 62},   {44, 63},   {44, 64},   {45, 14},   {45, 15},
        {50, 115},  {45, 29},   {50, 116},  {50, 117},  {45, 58},   {45, 59},   {45, 60},   {45, 61},   {45, 62},
        {45, 63},   {46, 14},   {46, 15},   {46, 16},   {46, 20},   {46, 21},   {51, 115},  {46, 29},   {51, 116},
        {51, 117},  {46, 58},   {46, 59},   {46, 60},   {46, 61},   {46, 62},   {46, 63},   {47, 14},   {47, 15},
        {47, 16},   {47, 17},   {47, 18},   {47, 19},   {47, 20},   {47, 23},   {47, 24},   {47, 29},   {52, 117},
        {52, 118},  {47, 58},   {47, 59},   {47, 60},   {47, 61},   {47, 62},   {47, 63},   {48, 14},   {48, 15},
        {89, 108},  {48, 16},   {48, 23},   {48, 24},   {48, 29},   {53, 118},  {53, 119},  {48, 58},   {48, 59},
        {48, 60},   {48, 61},   {48, 62},   {48, 63},   {49, 14},   {90, 107},  {49, 15},   {90, 108},  {49, 16},
        {49, 23},   {49, 24},   {49, 25},   {49, 29},   {54, 119},  {49, 58},   {49, 59},   {49, 60},   {49, 61},
        {49, 62},   {49, 63},   {116, 130}, {91, 106},  {50, 14},   {91, 107},  {50, 15},   {91, 108},  {50, 16},
        {50, 24},   {50, 25},   {50, 26},   {50, 29},   {14, 27},   {55, 119},  {50, 57},   {50, 58},   {50, 59},
        {50, 60},   {50, 61},   {50, 62},   {50, 63},   {-1, 5},    {117, 130}, {92, 105},  {92, 106},  {51, 14},
        {92, 107},  {51, 15},   {92, 108},  {51, 16},   {51, 24},   {51, 25},   {51, 26},   {51, 27},   {51, 28},
        {51, 29},   {15, 26},   {56, 118},  {56, 119},  {51, 57},   {51, 58},   {51, 59},   {51, 60},   {51, 61},
        {51, 62},   {51, 63},   {93, 105},  {93, 106},  {93, 107},  {52, 15},   {93, 108},  {52, 16},   {52, 24},
        {52, 25},   {52, 26},   {52, 27},   {52, 28},   {16, 26},   {57, 118},  {52, 57},   {52, 58},   {52, 59},
        {52, 60},   {52, 61},   {52, 62},   {52, 63},   {94, 104},  {94, 105},  {94, 106},  {94, 107},  {53, 15},
        {94, 108},  {53, 16},   {53, 24},   {53, 25},   {53, 26},   {53, 27},   {58, 117},  {58, 118},  {53, 56},
        {53, 57},   {53, 58},   {53, 59},   {53, 60},   {53, 61},   {53, 62},   {53, 63},   {95, 103},  {95, 104},
        {95, 105},  {95, 106},  {54, 14},   {95, 107},  {54, 15},   {95, 108},  {54, 16},   {54, 24},   {54, 25},
        {54, 26},   {59, 116},  {59, 117},  {54, 56},   {54, 57},   {54, 58},   {54, 59},   {54, 60},   {54, 61},
        {54, 62},   {54, 63},   {54, 64},   {96, 103},  {96, 104},  {96, 105},  {96, 106},  {96, 107},  {55, 15},
        {96, 108},  {55, 16},   {55, 23},   {55, 24},   {55, 25},   {60, 115},  {60, 116},  {55, 56},   {55, 57},
        {55, 58},   {55, 59},   {55, 60},   {55, 61},   {55, 62},   {55, 63},   {55, 64},   {56, 23},   {61, 114},
        {61, 115},  {56, 55},   {56, 56},   {56, 57},   {56, 58},   {56, 59},   {56, 60},   {56, 61},   {56, 62},
        {56, 63},   {56, 64},   {62, 113},  {62, 114},  {57, 55},   {57, 56},   {57, 57},   {57, 58},   {57, 59},
        {57, 60},   {57, 61},   {57, 62},   {57, 63},   {57, 64},   {63, 112},  {63, 113},  {58, 55},   {58, 56},
        {58, 57},   {58, 58},   {58, 59},   {58, 60},   {58, 61},   {58, 62},   {58, 63},   {58, 64},   {64, 112},
        {59, 54},   {59, 55},   {59, 56},   {59, 57},   {59, 58},   {59, 59},   {59, 60},   {59, 61},   {59, 62},
        {59, 63},   {59, 64},   {65, 112},  {60, 54},   {60, 55},   {60, 56},   {60, 57},   {60, 58},   {60, 59},
        {60, 60},   {60, 61},   {60, 62},   {60, 63},   {60, 64},   {102, 116}, {102, 117}, {66, 112},  {102, 118},
        {102, 119}, {102, 120}, {67, 112},  {68, 112},  {2, -2},    {69, 112},  {69, 113},  {3, -2},    {4, -2},
        {36, 65},   {56, 17},   {97, 109},  {37, 65},   {57, 17},   {98, 109},  {38, 65},   {99, 109},  {39, 65},
        {100, 109}, {40, 65},   {101, 109}, {42, 65},   {43, 65},   {44, 65},   {37, 66},   {57, 18},   {98, 110},
        {38, 66},   {99, 110},  {39, 66},   {100, 110}, {40, 66},   {101, 110}, {42, 66},   {103, 110}, {43, 66},
        {104, 110}, {44, 66},   {38, 67},   {58, 19},   {99, 111},  {39, 67},   {100, 111}, {40, 67},   {101, 111},
        {42, 67},   {103, 111}, {43, 67},   {104, 111}, {44, 67},   {105, 111}, {42, 68},   {103, 112}, {43, 68},
        {44, 68},   {45, 68},   {43, 69},   {44, 69},   {45, 69},   {44, 70},   {45, 70},   {45, 71},   {46, 71},
        {46, 72},   {46, 73},   {47, 73},   {47, 74},   {48, 74},   {48, 75},   {49, 75},   {49, 76},   {50, 76},
        {50, 77},   {51, 77},   {51, 78},   {52, 78},   {113, 122}, {52, 79},   {113, 123}, {53, 79},   {114, 123},
        {54, 80},   {115, 124}, {55, 81},   {116, 125}, {55, 82},   {116, 126}, {56, 82},   {117, 126}, {56, 83},
        {117, 127}, {57, 83},   {118, 127}, {58, 84},   {119, 128}, {59, 84},   {120, 128}, {60, 84},   {121, 128},
        {48, 17},   {89, 109},  {49, 17},   {90, 109},  {50, 17},   {91, 109},  {51, 17},   {92, 109},  {52, 17},
        {93, 109},  {53, 17},   {94, 109},  {54, 17},   {95, 109},  {55, 17},   {96, 109},  {48, 18},   {89, 110},
        {49, 18},   {90, 110},  {50, 18},   {91, 110},  {51, 18},   {92, 110},  {52, 18},   {93, 110},  {53, 18},
        {94, 110},  {54, 18},   {95, 110},  {55, 18},   {96, 110},  {56, 18},   {97, 110},  {48, 19},   {89, 111},
        {49, 19},   {90, 111},  {50, 19},   {91, 111},  {51, 19},   {92, 111},  {52, 19},   {93, 111},  {53, 19},
        {94, 111},  {54, 19},   {95, 111},  {55, 19},   {96, 111},  {56, 19},   {97, 111},  {57, 19},   {98, 111},
        {106, 111}, {107, 111}, {108, 111}, {109, 111}, {48, 20},   {89, 112},  {49, 20},   {90, 112},  {50, 20},
        {91, 112},  {51, 20},   {92, 112},  {52, 20},   {93, 112},  {53, 20},   {94, 112},  {54, 20},   {95, 112},
        {55, 20},   {96, 112},  {56, 20},   {97, 112},  {57, 20},   {98, 112},  {99, 112},  {100, 112}, {101, 112},
        {109, 112}, {110, 112}, {111, 112}, {112, 112}, {70, 113},  {71, 113},  {47, 21},   {88, 113},  {48, 21},
        {89, 113},  {49, 21},   {90, 113},  {50, 21},   {91, 113},  {51, 21},   {92, 113},  {52, 21},   {93, 113},
        {53, 21},   {94, 113},  {54, 21},   {95, 113},  {55, 21},   {96, 113},  {56, 21},   {97, 113},  {98, 113},
        {99, 113},  {100, 113}, {103, 113}, {112, 113}, {113, 113}, {114, 113}, {71, 114},  {72, 114},  {46, 22},
        {87, 114},  {47, 22},   {88, 114},  {48, 22},   {89, 114},  {49, 22},   {90, 114},  {50, 22},   {91, 114},
        {51, 22},   {92, 114},  {52, 22},   {93, 114},  {53, 22},   {94, 114},  {54, 22},   {95, 114},  {55, 22},
        {96, 114},  {56, 22},   {97, 114},  {98, 114},  {103, 114}, {114, 114}, {115, 114}, {120, 114}, {121, 114},
        {72, 115},  {85, 115},  {86, 115},  {50, 23},   {91, 115},  {51, 23},   {92, 115},  {52, 23},   {93, 115},
        {53, 23},   {94, 115},  {54, 23},   {95, 115},  {103, 115}, {115, 115}, {116, 115}, {117, 115}, {118, 115},
        {119, 115}, {120, 115}, {121, 115}, {72, 116},  {83, 116},  {84, 116},  {85, 116},  {101, 116}, {103, 116},
        {115, 116}, {116, 116}, {117, 116}, {118, 116}, {119, 116}, {120, 116}, {121, 116}, {72, 117},  {73, 117},
        {40, 25},   {81, 117},  {82, 117},  {83, 117},  {101, 117}, {103, 117}, {114, 117}, {115, 117}, {116, 117},
        {117, 117}, {118, 117}, {119, 117}, {120, 117}, {121, 117}, {73, 118},  {74, 118},  {75, 118},  {76, 118},
        {77, 118},  {78, 118},  {79, 118},  {80, 118},  {40, 26},   {81, 118},  {101, 118}, {103, 118}, {113, 118},
        {114, 118}, {115, 118}, {116, 118}, {117, 118}, {118, 118}, {119, 118}, {120, 118}, {121, 118}, {101, 119},
        {103, 119}, {114, 119}, {115, 119}, {116, 119}, {117, 119}, {118, 119}, {119, 119}, {120, 119}, {121, 119},
        {103, 120}, {114, 120}, {115, 120}, {116, 120}, {117, 120}, {118, 120}, {119, 120}, {120, 120}, {121, 120},
        {103, 121}, {114, 121}, {115, 121}, {116, 121}, {117, 121}, {118, 121}, {119, 121}, {120, 121}, {121, 121},
        {103, 122}, {104, 122}, {114, 122}, {115, 122}, {116, 122}, {117, 122}, {118, 122}, {119, 122}, {120, 122},
        {121, 122}, {104, 123}, {105, 123}, {115, 123}, {116, 123}, {117, 123}, {118, 123}, {119, 123}, {120, 123},
        {121, 123}, {105, 124}, {106, 124}, {107, 124}, {113, 124}, {114, 124}, {116, 124}, {117, 124}, {0, -1},
        {118, 124}, {119, 124}, {120, 124}, {121, 124}, {107, 125}, {108, 125}, {112, 125}, {113, 125}, {114, 125},
        {115, 125}, {-1, 0},    {117, 125}, {118, 125}, {119, 125}, {120, 125}, {121, 125}, {108, 126}, {109, 126},
        {110, 126}, {111, 126}, {112, 126}, {113, 126}, {114, 126}, {115, 126}, {118, 126}, {119, 126}, {120, 126},
        {121, 126}, {110, 127}, {111, 127}, {112, 127}, {113, 127}, {114, 127}, {115, 127}, {116, 127}, {119, 127},
        {120, 127}, {121, 127}, {109, 128}, {110, 128}, {111, 128}, {112, 128}, {113, 128}, {114, 128}, {115, 128},
        {116, 128}, {117, 128}, {118, 128}, {111, 129}, {-1, 4},    {117, 129}, {118, 129}, {119, 129}};

    for (auto &r : roads) {
        setRoad(r, true);
    }
}

bool Level::isNextToRoad(const Vector2 &coords) {
    for (auto &direction : directions) {
        auto next = coords + direction;

        if (isRoad(next)) {
            return true;
        }
    }

    return false;
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

    if (Context::get().inputs.isKeyPressed(SDL_SCANCODE_F1))
        for (int y = 0; y < tileheight; ++y) {
            for (int x = 0; x < tilewidth; ++x) {
                Vector2 pos = {x * tileSpacing, y * tileSpacing};

                if (isFree({x, y})) {
                    renderer.drawFilledQuad(pos, {tileSpacing, tileSpacing}, 255, 0, 0, 0.5f, true);
                } else {

                    renderer.drawFilledQuad(pos, {tileSpacing, tileSpacing}, 255, 255, 255, 0.5f, true);
                }
            }
        }
}

bool Level::findPath(Path &path, const Vector2 &start, const Vector2 &end) {

    {
        auto &cache = pimpl->pathCache;
        auto it = cache.find(start);

        if (it != cache.end()) {
            auto it2 = it->second.find(end);

            if (it2 != it->second.end()) {

                path = it2->second;
                return true;
            }
        }
    }

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

    UnorderedMap<Vector2, int> visited;

    while (!q.empty()) {
        const auto node = q.top();
        q.pop();
        const auto last = node.path.back();

        if (visited.find(last) == visited.end() || visited[last] > node.path.size()) {
            visited[last] = node.path.size();

            if (last == end) {
                path = node.path;
                pimpl->pathCache[start][end] = path;
                return true;
                break;
            } else {
                for (auto &direction : directions) {
                    auto next = last + direction;

                    if (isRoad(next) && !locks[next]) {
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

    if (isFree(coords)) {
        return false;
    }
    if (isFree({coords.x, coords.y - 1})) {
        return false;
    }
    if (isFree({coords.x - 1, coords.y - 1})) {
        return false;
    }
    if (isFree({coords.x - 1, coords.y})) {
        return false;
    }

    return true;
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

            if (isRoad(pos)) {
                type = Tile::FILL2;
            } else {

                if (isRoad({x + 1, y + 1})) {
                    type = Tile::CORNER4;
                }
                if (isRoad({x - 1, y + 1})) {
                    type = Tile::CORNER3;
                }
                if (isRoad({x - 1, y - 1})) {
                    type = Tile::CORNER1;
                }
                if (isRoad({x + 1, y - 1})) {
                    type = Tile::CORNER2;
                }

                if (isRoad({x + 1, y})) {
                    type = Tile::E;

                    if (isRoad({x, y + 1})) {
                        type = Tile::SE;
                    } else if (isRoad({x, y - 1})) {
                        type = Tile::NE;
                    }
                } else if (isRoad({x - 1, y})) {
                    type = Tile::W;

                    if (isRoad({x, y + 1})) {
                        type = Tile::SW;
                    } else if (isRoad({x, y - 1})) {
                        type = Tile::NW;
                    }
                } else if (isRoad({x, y - 1})) {
                    type = Tile::N;
                } else if (isRoad({x, y + 1})) {
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

void Level::invalidatePathCache() {
    pimpl->pathCache.clear();
}
