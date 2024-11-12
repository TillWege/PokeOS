#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "PokeOS"
#define SQUARE_SIZE 50

Texture texture;

void loadAssets()
{
	texture = LoadTexture(ASSETS_PATH"bg.png");
}

void draw_background()
{
	static int scale = 5;

	ImGui::Begin("Background");
	ImGui::SliderInt("Scale", &scale, 1, 10);
	ImGui::End();


	Vector2 position = {
		SCREEN_WIDTH / 2 - texture.width * float(scale) / 2,
		SCREEN_HEIGHT / 2 - texture.height * float(scale) / 2
	};
	DrawTextureEx(texture, position, 1, scale, WHITE);

}

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);
	rlImGuiSetup(true);

	loadAssets();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();

		ClearBackground(Color{ 0x38, 0x3c, 0x7d, 0xFF });
		draw_background();



		bool open = true;
		ImGui::ShowDemoWindow(&open);

		rlImGuiEnd();
		EndDrawing();
	}
	rlImGuiShutdown();
	CloseWindow();

	return 0;
}