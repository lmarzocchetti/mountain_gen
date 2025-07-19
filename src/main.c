#include <m-list.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280

#define GRID_HEIGHT 2000
#define GRID_WIDTH 2000

#define SCALE 0.009
#define OCTAVES 8
#define BASE_AMP 0.001
#define BASE_FREQ 0.035
#define LACUNARITY 1.9
#define GAIN 0.20

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
            points[row * GRID_WIDTH + col].z = perlin((float)col * SCALE, (float)row * SCALE);
        }
    }
}

float fbm(float x, float y, int octaves, float base_freq, float base_amp, float lacunarity, float gain)
{
    float freq = base_freq;
    float amp = base_amp;
    float sum = 0.0f;
    float amp_sum = 0.0f;

    for (int i = 0; i < octaves; ++i)
    {
        float n = perlin(x * freq, y * freq);
        sum += n * amp;
        amp_sum += amp;

        freq *= lacunarity;
        amp *= gain;
    }

    if (amp_sum > 0.0f)
    {
        sum /= amp_sum;
    }

    return sum;
}

void calculate_fbm_heightmap(Point *points, int width, int height, float base_freq, int octaves)
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            float fx = (float)col;
            float fy = (float)row;
            points[row * GRID_WIDTH + col].x = fx;
            points[row * GRID_WIDTH + col].y = fy;
            float val = fbm(fx, fy, octaves, base_freq, BASE_AMP, LACUNARITY, GAIN);
            val = to_zero_one_range(val);
            points[row * GRID_WIDTH + col].z = val;
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
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Perlin Noise");
    SetTargetFPS(60);

    Point points[GRID_WIDTH * GRID_HEIGHT] = {0};
    calculate_perlin_noise(points, GRID_WIDTH, GRID_WIDTH);
    // calculate_fbm_heightmap(points, GRID_WIDTH, GRID_HEIGHT, BASE_FREQ, OCTAVES);

    Camera camera = {0};
    camera.position = (Vector3){18.0f, 25.0f, 18.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Heightmap image
    Color pixels[GRID_WIDTH * GRID_HEIGHT] = {0};
    fill_pixels(pixels, points, GRID_WIDTH, GRID_HEIGHT);

    // Image fbm_heightmap_image = {
    //     .data = pixels,
    //     .width = GRID_WIDTH,
    //     .height = GRID_HEIGHT,
    //     .mipmaps = 1,
    //     .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    //  };

    Image fbm_heightmap_image = GenImagePerlinNoise(520, 520, 0, 0, 1.0);

    Texture texture = LoadTextureFromImage(fbm_heightmap_image);

    Mesh mesh = GenMeshHeightmap(fbm_heightmap_image, (Vector3){16, 8, 16});
    Model model = LoadModelFromMesh(mesh);

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Vector3 map_position = {-8.0f, 0.0f, -8.0f};

    Shader shader = LoadShader("shaders/lighting.vert", "shaders/lighting.frag");
    model.materials[0].shader = shader;

    Light light = CreateLight(LIGHT_DIRECTIONAL, (Vector3){2, 2, 2}, Vector3Zero(),
                              (Color){.r = 255, .g = 0, .b = 0, .a = 255}, shader);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawModel(model, map_position, 1.0f, RED);
        DrawGrid(20, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

        // DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    UnloadShader(shader);
    UnloadTexture(texture);
    UnloadModel(model);
    CloseWindow();

    return 0;
}
