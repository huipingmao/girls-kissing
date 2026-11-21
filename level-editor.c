#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// #define RAYGUI_IMPLEMENTATION
// #include "raygui.h"

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

Camera2D camera = {0};

Vector2 mouse_init_pos;
Rectangle new = {0};

void draw_new_rectangle (Rectangle *rec_to_add)
{
	Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
	// printf("x mp %f y mp %f\n", mouse_pos.x, mouse_pos.y);

	DrawRectangleRec(*rec_to_add, (Color){0x22, 0x77, 0x22, 0xcc});
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		// mouse_init_pos = GetMousePosition();
		mouse_init_pos = GetScreenToWorld2D(GetMousePosition(), camera);
	}
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		if (mouse_pos.x - mouse_init_pos.x < 0 && mouse_pos.y - mouse_init_pos.y > 0) {
		// if (GetMouseX() - mouse_init_pos.x < 0 && GetMouseY() - mouse_init_pos.y > 0) {
			new = (Rectangle) {
				mouse_pos.x, 
				mouse_init_pos.y, 
				mouse_init_pos.x - mouse_pos.x,
				mouse_pos.y - mouse_init_pos.y
			};
			// new = (Rectangle) {
			// 	GetMouseX(), 
			// 	mouse_init_pos.y, 
			// 	mouse_init_pos.x - GetMouseX(),
			// 	GetMouseY() - mouse_init_pos.y
			// };
			DrawRectangleRec(new, (Color){0xaa, 0x00, 0xee, 0x77});
		}
		else if (mouse_pos.x - mouse_init_pos.x > 0 && mouse_pos.y - mouse_init_pos.y < 0) {
		// else if (GetMouseX() - mouse_init_pos.x > 0 && GetMouseY() - mouse_init_pos.y < 0) {

			new = (Rectangle) {
				mouse_init_pos.x,
				mouse_pos.y, 
				mouse_pos.x - mouse_init_pos.x,
				mouse_init_pos.y - mouse_pos.y
			};
			// new = (Rectangle) {
			// 	mouse_init_pos.x,
			// 	GetMouseY(), 
			// 	GetMouseX() - mouse_init_pos.x,
			// 	mouse_init_pos.y - GetMouseY()
			// };
			DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});
		}
		else if (mouse_pos.x - mouse_init_pos.x < 0 && mouse_pos.y - mouse_init_pos.y < 0) {
		// else if (GetMouseX() - mouse_init_pos.x < 0 && GetMouseY() - mouse_init_pos.y < 0) {
			new = (Rectangle) { 
				mouse_pos.x,
				mouse_pos.y,
				mouse_init_pos.x - mouse_pos.x, 
				mouse_init_pos.y - mouse_pos.y 
			};
			// new = (Rectangle) { 
			// 	GetMouseX(),
			// 	GetMouseY(),
			// 	mouse_init_pos.x - GetMouseX(), 
			// 	mouse_init_pos.y - GetMouseY() 
			// };
			DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});
		}
		else {
			new = (Rectangle) { 
				mouse_init_pos.x, 
				mouse_init_pos.y, 
				mouse_pos.x - mouse_init_pos.x,
				mouse_pos.y - mouse_init_pos.y
			};
			// new = (Rectangle) { 
			// 	mouse_init_pos.x, 
			// 	mouse_init_pos.y, 
			// 	GetMouseX() - mouse_init_pos.x,
			// 	GetMouseY() - mouse_init_pos.y
			// };
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

	// if (IsKeyPressed(KEY_R)) {
	// 	*rec_to_add = (Rectangle){0};
	// }
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


	// camera.target = (Vector2) {0, 0};
	camera.target = (Vector2) {GetRenderWidth() / 3, GetRenderHeight() / 3};
	camera.offset = (Vector2) {GetRenderWidth()/2.0f, GetRenderHeight()/2.0f};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	// Vector2 mouse_init_pos;

	Rectangle rec_to_add = {0};
	int mode = 0;

	while (!WindowShouldClose()) {
		if (IsKeyDown(KEY_ESCAPE)) CloseWindow();
		BeginDrawing();
		// DrawTextEx(mona, "r to reset selection", (Vector2){ 1.0f, 31.0f }, (float)mona.baseSize, 2, GREEN); 
		// DrawTextEx(mona, "rmb to add new rectangle in selected area", (Vector2){ 1.0f, 51.0f }, (float)mona.baseSize, 2, GREEN); 
		// DrawTextEx(mona, "q to quit", (Vector2){ 1.0f, 71.0f }, (float)mona.baseSize, 2, GREEN); 
		// DrawTextEx(mona, TextFormat("sizeof item: %ld\n", malloc_usable_size(all_recs.item)), (Vector2){ 1.0f, 91.0f }, (float)mona.baseSize, 2, GREEN); 
		// DrawTextEx(mona, TextFormat("size %d capacity %d\n", all_recs.size, all_recs.capacity), (Vector2){ 1.0f, 111.0f }, (float)mona.baseSize, 2, GREEN); 

		// ClearBackground( (Color){0x11, 0x11, 0x11, 0xff} );
		// ClearBackground( (Color){0x4d, 0x25, 0x4a, 0xff} );
		ClearBackground(BLACK);
		// DrawTexturePro(background, (Rectangle) {0,0,1589,1049}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,50});
		// DrawTexturePro(background, (Rectangle) {0,0,1448,741}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,150});

		// DrawTexturePro(background, (Rectangle) {0,0,(float)GetRenderWidth()/1.2,(float)GetScreenHeight()/1.2}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,50});
		DrawTexturePro(background, (Rectangle) {0,0,(float)GetRenderWidth()*2,(float)GetScreenHeight()*2}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,20});

		if (GetMouseWheelMove() > 0) camera.zoom += 0.05;
		if (GetMouseWheelMove() < 0) camera.zoom -= 0.05;
		if (camera.zoom < 0.1f) camera.zoom = 0.1f;
		if (camera.zoom > 8.0f) camera.zoom = 8.0f;

		// DrawRectangle(0, 0, 10, 10, WHITE); // test

		if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}

		// DrawRectangleRec(rec_to_add, (Color){0x22, 0x77, 0x22, 0xcc});
		if (IsKeyPressed(KEY_DELETE) && selected_rec != -1) {
			rectangle_remove(selected_rec);
			selected_rec = -1;
		}

		if (mode == 1 && selected_rec != -1) {
			if (IsKeyDown(KEY_LEFT_CONTROL)) {
				// increase rectangle size
				if (IsKeyPressed(KEY_W)) { all_recs.item[selected_rec].height += 1; all_recs.item[selected_rec].y -= 1; }
				if (IsKeyPressed(KEY_S)) { all_recs.item[selected_rec].height += 1; }
				if (IsKeyPressed(KEY_A)) { all_recs.item[selected_rec].width += 1; all_recs.item[selected_rec].x -= 1; }
				if (IsKeyPressed(KEY_D)) { all_recs.item[selected_rec].width += 1; }
			} else if (IsKeyDown(KEY_LEFT_ALT)) {
				// decrease rectangle size
				if (IsKeyPressed(KEY_W)) { all_recs.item[selected_rec].height -= 1; }
				if (IsKeyPressed(KEY_S)) { all_recs.item[selected_rec].height -= 1; all_recs.item[selected_rec].y += 1; }
				if (IsKeyPressed(KEY_A)) { all_recs.item[selected_rec].width -= 1;  }
				if (IsKeyPressed(KEY_D)) { all_recs.item[selected_rec].width -= 1; all_recs.item[selected_rec].x += 1; }
			} else {
				// move rectangle
				if (IsKeyPressed(KEY_W)) { all_recs.item[selected_rec].y -= 1; }
				if (IsKeyPressed(KEY_S)) { all_recs.item[selected_rec].y += 1; }
				if (IsKeyPressed(KEY_A)) { all_recs.item[selected_rec].x -= 1; }
				if (IsKeyPressed(KEY_D)) { all_recs.item[selected_rec].x += 1; }
			}
		}

		if (mode == 0 && IsKeyPressed(KEY_R)) {
			rec_to_add = (Rectangle){0};
		}
		if (mode == 1 && IsKeyPressed(KEY_R)) {
			selected_rec = -1;
		}

		// draw_new_rectangle(&rec_to_add);
		// for (int i = 0; i < all_recs.size; i++) {
		// 	if (CheckCollisionPointRec(GetMousePosition(), all_recs.item[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))  selected_rec = i; 
		// 	if (i == selected_rec) DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xcc, 0xff});
		// 	else if (CheckCollisionPointRec(GetMousePosition(), all_recs.item[i])) DrawRectangleRec(all_recs.item[i], (Color){0xcc, 0xff, 0xcc, 0xff});
		// 	else DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xff, 0xff});
		// 	// printf("%d: x %f y %f w %f h %f\n", i, all_recs.item[i].x, all_recs.item[i].y, all_recs.item[i].width, all_recs.item[i].height);
		// }
		// printf("\n");

		if (IsKeyPressed(KEY_ONE)) mode = 0;
		if (IsKeyPressed(KEY_TWO)) mode = 1;
		if (IsKeyPressed(KEY_THREE)) mode = 2;
		if (IsKeyPressed(KEY_T)) camera.target = (Vector2) {0};


		BeginMode2D(camera);
		{
			// draw a fucking grid
			for (int i = -100; i < 100; i++) {
				DrawLine(-2000, i * 20, 2000, i * 20, (Color){0x00,0x00,0x00,0xff});
			}
			for (int i = -100; i < 100; i++) {
				DrawLine(i * 20, -2000, i * 20, 2000, (Color){0x00,0x00,0x00,0xff});
			}

			for (int i = 0; i < all_recs.size; i++) {
				if (mode == 1 && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), all_recs.item[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))  selected_rec = i; // select rect on mouse press 
				// if (mode == 1 && i == selected_rec) DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xcc, 0xff}); // color selected rec
				if (i == selected_rec) DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xcc, 0xff}); // color selected rec
				else if (mode == 1 && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), all_recs.item[i])) DrawRectangleRec(all_recs.item[i], (Color){0xcc, 0xff, 0xcc, 0xff}); // color rec colliding with mouse
				else DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xff, 0xff});
				// printf("%d: x %f y %f w %f h %f\n", i, all_recs.item[i].x, all_recs.item[i].y, all_recs.item[i].width, all_recs.item[i].height);
				if (mode == 2) { printf("{%f, %f, %f, %f}\n", all_recs.item[i].x, all_recs.item[i].y, all_recs.item[i].width, all_recs.item[i].height);};
			}
			if (mode == 2) { mode = 1; printf("\n"); }
			// printf("\n");
			if (mode == 0) draw_new_rectangle(&rec_to_add);
			// SetMouseOffset(camera.target.x, camera.target.y);
			// draw_new_rectangle(&rec_to_add);
			// GuiWindowBox((Rectangle){100, 100, 100, 20}, "GIRLS KISSING");
			// DrawRectangle(0, 0, 30, 30, WHITE); // test
			// DrawTextEx(mona, TextFormat("\n", ), (Vector2){ 1.0f, 91.0f }, (float)mona.baseSize, 2, GREEN); 
			DrawTextEx(mona, "(0, 0)!", (Vector2){ 0.0f, 0.0f }, (float)mona.baseSize, 2, RED); 
               // DrawGrid(100, 50);
		}
		EndMode2D();

		DrawTextEx(mona, TextFormat("rectangle number: %d\n", all_recs.size), (Vector2){ 1.0f, 1.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawTextEx(mona, TextFormat("selected rec %d\n", selected_rec), (Vector2){ 1.0f, 21.0f }, (float)mona.baseSize, 2, GREEN); 

		DrawTextEx(mona, TextFormat("(%d %d)", GetMouseX(), GetMouseY()), (Vector2){ GetMouseX(), GetMouseY() - 25}, (float)mona.baseSize, 2, WHITE); 
		// DrawTextEx(mona, TextFormat("(x %.0f y %.0f w %.0f h %.0f)", rec_to_add.x, rec_to_add.y, rec_to_add.width, rec_to_add.height), (Vector2){ GetMouseX(), GetMouseY() - 45}, (float)mona.baseSize, 1, GRAY);
		// 
		DrawTextEx(mona, TextFormat("(w %.0f h %.0f", rec_to_add.width, rec_to_add.height), (Vector2){ GetMouseX(), GetMouseY() - 45}, (float)mona.baseSize, 1, GRAY); 
		DrawTextEx(mona, TextFormat(" x %.0f y %.0f)", rec_to_add.x, rec_to_add.y), (Vector2){ GetMouseX(), GetMouseY() - 65}, (float)mona.baseSize, 1, GRAY); 

		if (mode == 0) DrawTextEx(mona, "current mode: drawing", (Vector2){ 1.0f, 41.0f }, (float)mona.baseSize, 2, BLUE); 
		if (mode == 1) DrawTextEx(mona, "current mode: editing", (Vector2){ 1.0f, 41.0f }, (float)mona.baseSize, 2, YELLOW); 
		DrawTextEx(mona, "press \"3\" to dump all rectangle data to output", (Vector2){ 1.0f, 61.0f }, (float)mona.baseSize, 2, GREEN); 
		DrawFPS(GetRenderWidth() - 100, GetScreenHeight() - 20);

		EndDrawing();

		// printf("x %f y %f\n", GetMouseX() - mouse_init_pos.x, GetMouseY() - mouse_init_pos.y);
		// printf("x %f y %f w %f h %f\n\n", new.x, new.y, new.width, new.height);
	}

	CloseWindow();

	return 0;
}
