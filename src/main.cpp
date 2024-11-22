#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "PokeOS"
#define SQUARE_SIZE 50

Texture texture;

void loadDatabase()
{
	auto db = SQLite::Database(ASSETS_PATH "pokemon.db");

	auto query = SQLite::Statement(db, "SELECT COUNT(*) FROM pokemon");
	query.executeStep();
	int count = query.getColumn(0);
	printf("Count: %d\n", count);
}

void loadAssets()
{
	texture = LoadTexture(ASSETS_PATH "bg.png");
}

void draw_background()
{
	static float scale = 5;

	ImGui::Begin("Background");
	ImGui::DragFloat("Scale", &scale, 1, 1.0, 10.0);
	ImGui::End();

	Vector2 position = {
		SCREEN_WIDTH / 2 - texture.width * float(scale) / 2,
		SCREEN_HEIGHT / 2 - texture.height * float(scale) / 2};
	DrawTextureEx(texture, position, 1, scale, WHITE);
}

int main()
{
	loadDatabase();
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);
	rlImGuiSetup(true);

	loadAssets();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();

		ClearBackground(Color{0x38, 0x3c, 0x7d, 0xFF});
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