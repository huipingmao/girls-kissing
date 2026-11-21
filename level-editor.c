#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

struct all_recs {
	Rectangle *item;
	int size;
	int capacity;
};

struct all_recs all_recs = {0};

void rectangle_append (Rectangle new)
{
	puts("appending new rectangle");
	if (!all_recs.size) {
		Rectangle *tmp = (Rectangle *)malloc(sizeof(Rectangle) * 2);
		all_recs.item = tmp;
		all_recs.size++;
		all_recs.capacity += 2;
		all_recs.item[all_recs.size - 1] = new;
		return;
	}

	all_recs.size++;

	if (all_recs.size >= all_recs.capacity) {
		Rectangle *tmp = (Rectangle *)realloc(all_recs.item, sizeof(Rectangle) * all_recs.capacity * 2);
		all_recs.item = tmp;
		// all_recs.size++;
		all_recs.capacity *= 2;
		all_recs.item[all_recs.size - 1] = new;
	} else {
		// all_recs.size++;
		all_recs.item[all_recs.size - 1] = new;
	}
}

void rectangle_remove (int index)
{
	// if (!all_recs.size) return;

	for (; index < all_recs.size - 1; index++) {
		all_recs.item[index] = all_recs.item[index + 1];
	}

	all_recs.item[all_recs.size - 1] = (Rectangle) {0};
	all_recs.size--;

	if (all_recs.capacity == 2 && all_recs.size == 0) { all_recs.capacity = 0; free(all_recs.item); all_recs.item = NULL; return; }

	if (all_recs.size < all_recs.capacity / 2 && all_recs.capacity > 2) {
		Rectangle *tmp = (Rectangle *)realloc(all_recs.item, sizeof(Rectangle) * (all_recs.capacity / 2));
		all_recs.item = tmp;
		all_recs.capacity /= 2;
	}
}

void load_rectangles ()
{
}

void free_rectangles ()
{
	free(all_recs.item);
	all_recs.item = NULL;
	all_recs.size = 0;
	all_recs.capacity = 0;
}

Vector2 mouse_init_pos;
Rectangle new = {0};

void draw_new_rectangle (Rectangle *rec_to_add)
{
	DrawRectangleRec(*rec_to_add, (Color){0x22, 0x77, 0x22, 0xcc});
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		mouse_init_pos = GetMousePosition();
	}
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		if (GetMouseX() - mouse_init_pos.x < 0 && GetMouseY() - mouse_init_pos.y > 0) {
			new = (Rectangle) {
				GetMouseX(), 
				mouse_init_pos.y, 
				mouse_init_pos.x - GetMouseX(),
				GetMouseY() - mouse_init_pos.y
			};
			DrawRectangleRec(new, (Color){0xaa, 0xcc, 0xee, 0x77});
		}
		else if (GetMouseX() - mouse_init_pos.x > 0 && GetMouseY() - mouse_init_pos.y < 0) {

			new = (Rectangle) {
				mouse_init_pos.x,
				GetMouseY(), 
				GetMouseX() - mouse_init_pos.x,
				mouse_init_pos.y - GetMouseY()
			};
			DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});
		}
		else if (GetMouseX() - mouse_init_pos.x < 0 && GetMouseY() - mouse_init_pos.y < 0) {
			new = (Rectangle) { 
				GetMouseX(),
				GetMouseY(),
				mouse_init_pos.x - GetMouseX(), 
				mouse_init_pos.y - GetMouseY() 
			};
			DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});
		}
		else {
			new = (Rectangle) { 
				mouse_init_pos.x, 
				mouse_init_pos.y, 
				GetMouseX() - mouse_init_pos.x,
				GetMouseY() - mouse_init_pos.y
			};
			DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});
		}
	}

	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		*rec_to_add = new;
	}

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) rec_to_add->y -= 10;
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) rec_to_add->y += 10;
		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) rec_to_add->x -= 10;
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) rec_to_add->x += 10;
	} else {
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) rec_to_add->y -= 1;
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) rec_to_add->y += 1;
		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) rec_to_add->x -= 1;
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) rec_to_add->x += 1;
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
		if (rec_to_add->width != 0 && rec_to_add->height != 0) {
			rectangle_append(*rec_to_add);
			*rec_to_add = (Rectangle){0};
			// new = (Rectangle){0};
		}
	}

	if (IsKeyPressed(KEY_R)) {
		*rec_to_add = (Rectangle){0};
	}
	// printf("x %f y %f w %f h %f\n\n", new.x, new.y, new.width, new.height);
}

int selected_rec = -1;
int main (void)
{
	InitWindow(1000, 800, "girls kissing level editor!!");
	// GuiEnable();
	// InitWindow(GetScreenWidth(), GetScreenHeight(), "girls kissing level editor!!");
	// SetWindowSize(GetScreenWidth(), GetScreenHeight());
	// SetWindowTitle("girls kissing level editor!!");
	SetExitKey(KEY_ESCAPE);

	Font mona = LoadFontEx("resources/MonaspaceRadonFrozen-Regular.ttf", 20, 0, 250);
	// Texture2D background = LoadTexture("./resources/sprites/morgen1.png");
	Texture2D background = LoadTexture("./resources/sprites/hoshi.png");

	Camera2D camera = {0};

	camera.target = (Vector2) {0, 0};
	camera.target = (Vector2) {GetRenderWidth(), GetRenderHeight()};
	camera.offset = (Vector2) {GetRenderWidth()/2.0f, GetRenderHeight()/2.0f};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	// Vector2 mouse_init_pos;

	Rectangle rec_to_add = {0};

	while (!WindowShouldClose()) {
		if (IsKeyDown(KEY_ESCAPE)) CloseWindow();
		BeginDrawing();
		DrawTextEx(mona, TextFormat("rectangle number: %d\n", all_recs.size), (Vector2){ 1.0f, 11.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, "r to reset selection", (Vector2){ 1.0f, 31.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, "rmb to add new rectangle in selected area", (Vector2){ 1.0f, 51.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, "q to quit", (Vector2){ 1.0f, 71.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, TextFormat("sizeof item: %ld\n", malloc_usable_size(all_recs.item)), (Vector2){ 1.0f, 91.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, TextFormat("size %d capacity %d\n", all_recs.size, all_recs.capacity), (Vector2){ 1.0f, 111.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, TextFormat("selected rec %d\n", selected_rec), (Vector2){ 1.0f, 131.0f }, (float)mona.baseSize, 2, GREEN); 

		// ClearBackground( (Color){0x11, 0x11, 0x11, 0xff} );
		// ClearBackground( (Color){0x4d, 0x25, 0x4a, 0xff} );
		ClearBackground(BLACK);
		// DrawTexturePro(background, (Rectangle) {0,0,1589,1049}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,50});
		// DrawTexturePro(background, (Rectangle) {0,0,1448,741}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,150});

		// DrawTexturePro(background, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,50});
		DrawTexturePro(background, (Rectangle) {0,0,(float)GetRenderWidth()*2,(float)GetScreenHeight()*2}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,20});

		if (GetMouseWheelMove() > 0) camera.zoom += 0.05;
		if (GetMouseWheelMove() < 0) camera.zoom -= 0.05;

		DrawRectangle(0, 0, 10, 10, WHITE); // test

		if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}

		DrawRectangleRec(rec_to_add, (Color){0x22, 0x77, 0x22, 0xcc});
		if (IsKeyPressed(KEY_DELETE) && selected_rec != -1) {
			rectangle_remove(selected_rec);
			selected_rec = -1;
		}

		draw_new_rectangle(&rec_to_add);
		for (int i = 0; i < all_recs.size; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), all_recs.item[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))  selected_rec = i; 
			if (i == selected_rec) DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xcc, 0xff});
			else if (CheckCollisionPointRec(GetMousePosition(), all_recs.item[i])) DrawRectangleRec(all_recs.item[i], (Color){0xcc, 0xff, 0xcc, 0xff});
			else DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xff, 0xff});
			printf("%d: x %f y %f w %f h %f\n", i, all_recs.item[i].x, all_recs.item[i].y, all_recs.item[i].width, all_recs.item[i].height);
		}
		printf("\n");

		DrawGrid(100, 100);

		BeginMode2D(camera);
		{
			// SetMouseOffset(camera.target.x, camera.target.y);
			// draw_new_rectangle(&rec_to_add);
			// GuiWindowBox((Rectangle){100, 100, 100, 20}, "GIRLS KISSING");
			// DrawRectangle(0, 0, 30, 30, WHITE); // test
		}
		EndMode2D();

		EndDrawing();

		// printf("x %f y %f\n", GetMouseX() - mouse_init_pos.x, GetMouseY() - mouse_init_pos.y);
		// printf("x %f y %f w %f h %f\n\n", new.x, new.y, new.width, new.height);
	}

	CloseWindow();

	return 0;
}
