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

struct Pokemon
{
	int id;
	std::string name;
};

struct Message
{
	std::string text;
	int logLevel;
	time_t timestamp;
};

struct Console
{
	std::vector<Message> messages;
};

static Console console;

void onLog(int logLevel, const char *message, va_list args)
{
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), message, args);

	Message msg;
	msg.text = std::string(buffer);
	msg.logLevel = logLevel;
	msg.timestamp = time(nullptr);

	console.messages.push_back(msg);
}

ImVec4 getLogColor(int logLevel)
{
	switch (logLevel)
	{
	case LOG_INFO:
		return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	case LOG_WARNING:
		return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	case LOG_FATAL:
	case LOG_ERROR:
		return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	case LOG_DEBUG:
		return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	default:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void renderConsole()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 100), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Console");

	float inputHeight = ImGui::GetFrameHeightWithSpacing();
	float paddingBottom = ImGui::GetStyle().WindowPadding.y;

	float availableHeight = ImGui::GetContentRegionAvail().y - inputHeight - paddingBottom;

	ImGui::BeginTable("Console", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0.0f, availableHeight));
	ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
	ImGui::TableSetupColumn("Timestamp", ImGuiTableColumnFlags_WidthFixed, 100);
	ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
	ImGui::TableHeadersRow();

	// TODO:
	// Use Clipper
	// see imgui_demo
	for (auto &message : console.messages)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		char str[26];
		ctime_s(str, sizeof str, &message.timestamp);

		ImGui::Text("%s", str);
		ImGui::TableSetColumnIndex(1);
		ImGui::TextColored(getLogColor(message.logLevel), "%s", message.text.c_str());
	}
	ImGui::EndTable();

	static char inputBuffer[256] = "";
	ImGui::InputText("##input", inputBuffer, IM_ARRAYSIZE(inputBuffer));
	ImGui::SameLine();
	if (ImGui::Button("Execute"))
	{
		// Handle the execute button press
		inputBuffer[0] = '\0'; // Clear the input buffer
	}

	ImGui::End();
}

void loadDatabase()
{
	auto db = SQLite::Database(ASSETS_PATH "pokemon.db");

	auto countQuery = SQLite::Statement(db, "SELECT COUNT(*) FROM pokemon");
	countQuery.executeStep();
	int count = countQuery.getColumn(0);
	TraceLog(LOG_INFO, "Found %d Pokemon\n", count);

	auto pokemonQuery = SQLite::Statement(db, "SELECT * FROM pokemon");
	while (pokemonQuery.executeStep())
	{
		Pokemon pokemon;
		pokemon.id = pokemonQuery.getColumn(0).getUInt();
		pokemon.name = pokemonQuery.getColumn(1).getText();
		TraceLog(LOG_DEBUG, "Found Pokemon %d: %s", pokemon.id, pokemon.name.c_str());
	}
}

void loadAssets()
{
	texture = LoadTexture(ASSETS_PATH "bg.png");
}

void renderBackground()
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

void renderRenderInfo()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 100), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Render Info");

	ImGui::Text("FPS: %.0f", GetFPS());
	ImGui::Text("Frame time: %.2f ms", GetFrameTime());

	ImGui::End();
}

int main()
{
	SetTraceLogLevel(LOG_ALL);
	SetTraceLogCallback(onLog);
	loadDatabase();
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

	auto monitor = GetCurrentMonitor();
	auto refreshRate = GetMonitorRefreshRate(monitor);
	TraceLog(LOG_INFO, "Monitor refresh rate: %d Hz", refreshRate);
	SetTargetFPS(refreshRate);
	rlImGuiSetup(true);

	loadAssets();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();

		ClearBackground(Color{0x38, 0x3c, 0x7d, 0xFF});
		renderBackground();
		renderConsole();
		renderRenderInfo();

		bool open = true;
		ImGui::ShowDemoWindow(&open);

		rlImGuiEnd();
		EndDrawing();
	}
	rlImGuiShutdown();
	CloseWindow();

	return 0;
}