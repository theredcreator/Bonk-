#include <raylib.h>
#include <iostream>

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int plr_score = 0;
int cpu_score = 0;
Music FxMusic;
Sound FxWin;
Sound FxLose;
Sound FxBounce;

class Ball {
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y = -speed_y;
            PlaySound(FxBounce);
        }
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            PlaySound(FxLose);
            ResetBall();
        }
        if (x - radius <= 0) {
            plr_score++;
            PlaySound(FxWin);
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() /2 ;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle {
    protected:
        void LimitMovement() {
            if (y <= 0) {
                y = 0;
            }

            if (y + height >= GetScreenHeight()) {
                y = GetScreenHeight() - height;
            }
        }

    public:
        float x, y;
        float width, height;
        int speed;
    
    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle: public Paddle {

    public:
        void Update(int ball_y) {
            if (y + height / 2 > ball_y) {
                y -= speed;
            }
            if (y + height / 2 <= ball_y) {
                y += speed;
            }
            LimitMovement();
        }
        
};

Ball ball;
Paddle player;
CpuPaddle cpu;



int main() {

    std::cout << "Starting..." << std::endl;
    const int WIDTH = 1280;
    const int HEIGHT = 800;
    InitWindow(WIDTH, HEIGHT, "BONK!");
    InitAudioDevice();
    FxMusic = LoadMusicStream("Resources/Music.wav");
    FxWin = LoadSound("Resources/Win.wav");
    SetSoundVolume(FxWin, 0.5f);
    FxLose = LoadSound("Resources/Fail.wav");
    SetSoundVolume(FxLose, 0.5f);
    FxBounce = LoadSound("Resources/Bounce.wav");
    SetSoundVolume(FxBounce, 0.5f);
    PlayMusicStream(FxMusic);
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height= 120;
    player.x = WIDTH - player.width - 10;
    player.y = HEIGHT / 2 - player.height / 2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height= 120;
    cpu.x = 10;
    cpu.y = HEIGHT / 2 - cpu.height / 2;
    cpu.speed = 6;

    SetMusicVolume(FxMusic, 0.02f);

    while (WindowShouldClose() == false) {
        UpdateMusicStream(FxMusic);

        if (!IsMusicStreamPlaying(FxMusic)) {
            SeekMusicStream(FxMusic, 0.0f);
            PlayMusicStream(FxMusic);
        }

        BeginDrawing();

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
            PlaySound(FxBounce);
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
            PlaySound(FxBounce);
        }
        
        ClearBackground(Dark_Green);
        DrawRectangle(WIDTH / 2, 0, WIDTH / 2, HEIGHT, Green);
        DrawCircle(WIDTH / 2, HEIGHT / 2, 150, Light_Green);
        DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, WHITE);

        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), WIDTH / 4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i", plr_score), 3 * WIDTH / 4 -20, 20, 80, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
