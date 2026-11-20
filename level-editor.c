#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

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
		all_recs.size++;
		all_recs.capacity *= 2;
		all_recs.item[all_recs.size - 1] = new;
	} else {
		all_recs.size++;
		all_recs.item[all_recs.size - 1] = new;
	}
}

void rectangle_remove ()
{
}

Vector2 mouse_init_pos;
Rectangle new = {0};

void draw_new_rectangle (Rectangle *rec_to_add)
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		mouse_init_pos = GetMousePosition();
	}
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		if (GetMouseX() - mouse_init_pos.x < 0 && GetMouseY() - mouse_init_pos.y > 0) {
			new = (Rectangle) { GetMouseX(), mouse_init_pos.y, 
				mouse_init_pos.x - GetMouseX(),
				GetMouseY() - mouse_init_pos.y
			};
			DrawRectangleRec(new, (Color){0xaa, 0xcc, 0xee, 0x77});
		}
		else if (GetMouseX() - mouse_init_pos.x > 0 && GetMouseY() - mouse_init_pos.y < 0) {

			new = (Rectangle) { mouse_init_pos.x, GetMouseY(), 
				GetMouseX() - mouse_init_pos.x,
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

	if (IsKeyPressed(KEY_DOWN)) rec_to_add->y += 1;
	if (IsKeyPressed(KEY_UP)) rec_to_add->y -= 1;
	if (IsKeyPressed(KEY_LEFT)) rec_to_add->x -= 1;
	if (IsKeyPressed(KEY_RIGHT)) rec_to_add->x += 1;

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		if (rec_to_add->width != 0 && rec_to_add->height != 0) {
			rectangle_append(*rec_to_add);
			*rec_to_add = (Rectangle){0};
			// new = (Rectangle){0};
		}
	}

	if (IsKeyPressed(KEY_R)) {
		*rec_to_add = (Rectangle){0};
	}
	DrawRectangleRec(*rec_to_add, (Color){0x22, 0x77, 0x22, 0xcc});
	printf("x %f y %f w %f h %f\n\n", new.x, new.y, new.width, new.height);
}

int main (void)
{
	InitWindow(1000, 800, "girls kissing level editor!!");
	// InitWindow(GetScreenWidth(), GetScreenHeight(), "girls kissing level editor!!");
	// SetWindowSize(GetScreenWidth(), GetScreenHeight());
	// SetWindowTitle("girls kissing level editor!!");
	// SetExitKey(KEY_Q);
	Texture2D background = LoadTexture("./resources/sprites/morgen1.png");

	Camera2D camera = {0};

	camera.target = (Vector2) {0, 0};
	camera.target = (Vector2) {GetRenderWidth(), GetRenderHeight()};
	camera.offset = (Vector2) {GetRenderWidth()/2.0f, GetRenderHeight()/2.0f};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	// Vector2 mouse_init_pos;

	Rectangle rec_to_add = {0};

	while (!WindowShouldClose()) {
		if (IsKeyDown(KEY_Q)) CloseWindow();
		BeginDrawing();
		// ClearBackground( (Color){0x11, 0x11, 0x11, 0xff} );
		// ClearBackground( (Color){0x4d, 0x25, 0x4a, 0xff} );
		ClearBackground(BLACK);
		// DrawTexturePro(background, (Rectangle) {0,0,1589,1049}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,50});
		// DrawTexturePro(background, (Rectangle) {0,0,1448,741}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,150});
		DrawTexturePro(background, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Rectangle) {0,0,(float)GetRenderWidth(),(float)GetScreenHeight()}, (Vector2) {0,0}, 0.0f, (Color) {255,255,255,50});

		if (GetMouseWheelMove() > 0) camera.zoom += 0.05;
		if (GetMouseWheelMove() < 0) camera.zoom -= 0.05;

		DrawRectangle(0, 0, 10, 10, WHITE); // test

		if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}

		// DrawRectangleRec(rec_to_add, (Color){0x22, 0x77, 0x22, 0xcc});

		draw_new_rectangle(&rec_to_add);
		// Rectangle new;
		// if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		// 	mouse_init_pos = GetMousePosition();
		// }
		// if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {

		// 	if (GetMouseX() - mouse_init_pos.x < 0 && GetMouseY() - mouse_init_pos.y > 0) {

		// 		new = (Rectangle) { GetMouseX(), mouse_init_pos.y, 
		// 			mouse_init_pos.x - GetMouseX(),
		// 			GetMouseY() - mouse_init_pos.y
		// 		};

		// 		DrawRectangleRec(new, (Color){0xaa, 0xcc, 0xee, 0x77});
		// 		// DrawRectangle(GetMouseX(), mouse_init_pos.y, 
		// 		// 	mouse_init_pos.x - GetMouseX(),
		// 		// 	GetMouseY() - mouse_init_pos.y, RED
		// 		// );
		// 	}
		// 	else if (GetMouseX() - mouse_init_pos.x > 0 && GetMouseY() - mouse_init_pos.y < 0) {

		// 		new = (Rectangle) { mouse_init_pos.x, GetMouseY(), 
		// 			GetMouseX() - mouse_init_pos.x,
		// 			mouse_init_pos.y - GetMouseY()
		// 		};
		// 		DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});


		// 		// DrawRectangle(mouse_init_pos.x, GetMouseY(), 
		// 		// 	GetMouseX() - mouse_init_pos.x,
		// 		// 	mouse_init_pos.y - GetMouseY(), RED
		// 		// );
		// 	}
		// 	else {

		// 		new = (Rectangle) { 
		// 			mouse_init_pos.x, 
		// 			mouse_init_pos.y, 
		// 			GetMouseX() - mouse_init_pos.x,
		// 			GetMouseY() - mouse_init_pos.y
		// 		};
		// 		DrawRectangleRec(new, (Color){0xaa, 0x00, 0x00, 0x77});

		// 		// DrawRectangle(mouse_init_pos.x, mouse_init_pos.y, 
		// 		// 	GetMouseX() - mouse_init_pos.x,
		// 		// 	GetMouseY() - mouse_init_pos.y, RED
		// 		// );
				
		// 	}
		// }

		// if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		// 	rec_to_add = new;
		// 	// rectangle_append(new);
		// 		// DrawRectangleRec(new, (Color){0x33, 0x22, 0x33, 0xff});
		// }

		// if (IsKeyPressed(KEY_DOWN)) rec_to_add.y += 1;
		// if (IsKeyPressed(KEY_UP)) rec_to_add.y -= 1;
		// if (IsKeyPressed(KEY_LEFT)) rec_to_add.x -= 1;
		// if (IsKeyPressed(KEY_RIGHT)) rec_to_add.x += 1;


		// if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		// 	if (rec_to_add.width != 0 && rec_to_add.height != 0) {
		// 		rectangle_append(rec_to_add);
		// 		rec_to_add = (Rectangle){0};
		// 		new = (Rectangle){0};
		// 	}
		// }

		// if (IsKeyPressed(KEY_E)) {
		// 	rec_to_add = (Rectangle){0};
		// }

		for (int i = 0; i < all_recs.size; i++) {
			DrawRectangleRec(all_recs.item[i], (Color){0xff, 0xcc, 0xff, 0xff});
		}

		BeginMode2D(camera);
		{
			DrawRectangle(0, 0, 30, 30, WHITE); // test
		// 	for (int i = 0; i < 10; i++) {
		// 		for (int j = 0; j < 10; j++) {
		// 			DrawRectangle(j * 50, i * 50, 49, 49, GRAY);
		// 		}
		// 	}
		// 	// DrawRectangle(0, 0, 10, 10, WHITE); // test
		// 	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		// 		mouse_init_pos = GetMousePosition();
		// 	}
		// 	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		// 		// Vector2 delta = GetMouseDelta();
		// 		mouse_init_pos = Vector2Scale(mouse_init_pos, -1/camera.zoom);
		// 		DrawRectangle(mouse_init_pos.x, mouse_init_pos.y, 49, 49, RED);
		// 	}

		}
		EndMode2D();

		EndDrawing();

		// printf("x %f y %f\n", GetMouseX() - mouse_init_pos.x, GetMouseY() - mouse_init_pos.y);
		// printf("x %f y %f w %f h %f\n\n", new.x, new.y, new.width, new.height);
	}

	CloseWindow();

	return 0;
}
