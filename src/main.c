#include <m-list.h>
#include <math.h>
#include <raylib.h>
#include <stdbool.h>

#define GRID_HEIGHT 520
#define GRID_WIDTH 520

#define SCALE 0.03
#define OCTAVES 4
#define INITIAL_AMPLITUDE 1.0
#define INITIAL_FREQUENCY 1.0
#define LACUNARITY 2.0
#define GAIN 0.5

unsigned char permutation[] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,
    99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,
    11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139,
    48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,
    244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135,
    130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147,
    118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119,
    248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110,
    79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162,
    241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121,
    50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215,
    61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,
    69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219,
    203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
    71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,
    46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
    200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183,
    170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,
    98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,
    191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204,
    176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195,
    78,  66,  215, 61,  156, 180,
};

float dot_grad(int hash, float x, float y)
{
    const int h = hash & 7;
    switch (h)
    {
    case 0:
        return x + y;
    case 1:
        return x;
    case 2:
        return x - y;
    case 3:
        return -y;
    case 4:
        return -x - y;
    case 5:
        return -x;
    case 6:
        return -x + y;
    case 7:
        return y;
    default:
        return 0;
    }
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float fade(float t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float perlin(float x, float y)
{
    const int xi0 = floorf(x);
    const int yi0 = floorf(y);

    const float xf0 = x - xi0;
    const float yf0 = y - yi0;
    const float xf1 = xf0 - 1.0;
    const float yf1 = yf0 - 1.0;

    const int xi = xi0 & 0xFF;
    const int yi = yi0 & 0xFF;

    const float u = fade(xf0);
    const float v = fade(yf0);

    const int h00 = permutation[permutation[xi + 0] + yi + 0];
    const int h01 = permutation[permutation[xi + 0] + yi + 1];
    const int h10 = permutation[permutation[xi + 1] + yi + 0];
    const int h11 = permutation[permutation[xi + 1] + yi + 1];

    const float x1 = lerp(dot_grad(h00, xf0, yf0), dot_grad(h10, xf1, yf0), u);
    const float x2 = lerp(dot_grad(h01, xf0, yf1), dot_grad(h11, xf1, yf1), u);
    return lerp(x1, x2, v);
}

typedef struct {
    float x;
    float y;
    float z;
} Point;

Point point_init()
{
    return (Point){.x = 0.0, .y = 0.0, .z = 0.0};
}

float to_zero_one_range(float num)
{
    return (num + 1) / 2;
}

Color get_perlin_color(float value)
{
    unsigned char gray = (unsigned char)(value * 255.0f);
    return (Color){gray, gray, gray, 255};
}

void calculate_perlin_noise(Point *points, int width, int height)
{
    for (int row = 0; row < GRID_HEIGHT; ++row)
    {
        for (int col = 0; col < GRID_WIDTH; ++col)
        {
            points[row * GRID_WIDTH + col].x = (float)col;
            points[row * GRID_WIDTH + col].y = (float)row;
            points[row * GRID_WIDTH + col].z = to_zero_one_range(perlin((float)col * SCALE, (float)row * SCALE));
        }
    }
}

void fill_pixels(Color *pixels, Point *points, int width, int height)
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            pixels[row * width + col] = get_perlin_color(points[row * width + col].z);
        }
    }
}

int main(void)
{
    InitWindow(520, 520, "Perlin Noise");
    SetTargetFPS(60);

    Point points[GRID_WIDTH * GRID_HEIGHT] = {0};
    calculate_perlin_noise(points, GRID_WIDTH, GRID_WIDTH);

    Color pixels[GRID_WIDTH * GRID_HEIGHT] = {0};
    fill_pixels(pixels, points, GRID_WIDTH, GRID_HEIGHT);

    Image perlin_noise_image = {
        .data = pixels,
        .width = GRID_WIDTH,
        .height = GRID_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    Texture texture = LoadTextureFromImage(perlin_noise_image);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
