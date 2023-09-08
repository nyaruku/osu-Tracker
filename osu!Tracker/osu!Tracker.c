#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "config.h";
#include <dwmapi.h>
#include "wrapper.h"

// 0 = GDI | 1 = GDI+
#define gdi_mode 1

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

#pragma region Nuklear definition
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#if gdi_mode == 1
#define NK_GDIP_IMPLEMENTATION
#elif gdi_mode == 0
#define NK_GDI_IMPLEMENTATION
#endif

#pragma endregion

#pragma region "Nuklear.h" and gdi header includes
#include "nuklear.h"
#if gdi_mode == 1
#include "nuklear_gdip.h"
#elif gdi_mode == 0
#include "nuklear_gdi.h"
#endif
#pragma endregion

#pragma region WindowProc Function
static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
#if gdi_mode == 1
	if (nk_gdip_handle_event(wnd, msg, wparam, lparam))
#elif gdi_mode == 0
	if (nk_gdi_handle_event(wnd, msg, wparam, lparam))
#endif
		return 0;
	return DefWindowProcW(wnd, msg, wparam, lparam);
}
#pragma endregion

#pragma region QoL functions

void ext_TextColor(struct nk_context* ctx, int r, int g, int b) {
	ctx->style.text.color.r = r;
	ctx->style.text.color.g = g;
	ctx->style.text.color.b = b;
}

void ext_BG(struct nk_context* ctx, int r, int g, int b) {
	ctx->style.window.fixed_background.data.color.r = r;
	ctx->style.window.fixed_background.data.color.g = g;
	ctx->style.window.fixed_background.data.color.b = b;
}

void ext_BttColor(struct nk_context* ctx, int r, int g, int b) {
	ctx->style.button.text_normal.r = r;
	ctx->style.button.text_normal.g = g;
	ctx->style.button.text_normal.b = b;
}

void ext_BttBg(struct nk_context* ctx, int r, int g, int b) {
	ctx->style.button.normal.data.color.r = r;
	ctx->style.button.normal.data.color.g = g;
	ctx->style.button.normal.data.color.b = b;
}

const char* custom_strcat(size_t numStrings, ...) {
	// Initialize va_list and iterate through the strings to calculate the total length
	va_list args;
	va_start(args, numStrings);

	size_t totalLen = 0;
	for (size_t i = 0; i < numStrings; i++) {
		const char* currentString = va_arg(args, const char*);
		totalLen += strlen(currentString);
	}

	va_end(args);

	// Allocate memory for the result string
	char* result = (char*)malloc(totalLen + 1); // +1 for the null terminator

	if (result == NULL) {
		return NULL; // Memory allocation failed
	}

	// Copy the contents of each string to the result
	char* currentPos = result;
	va_start(args, numStrings);

	for (size_t i = 0; i < numStrings; i++) {
		const char* currentString = va_arg(args, const char*);
		size_t currentLen = strlen(currentString);
		memcpy(currentPos, currentString, currentLen);
		currentPos += currentLen;
	}

	va_end(args);

	// Null-terminate the result string
	*currentPos = '\0';

	return result;
}

#pragma endregion

// draw functions
static void drawMain();
static void drawSettings();
static void drawAbout();
static void start_main();

// TEXT INPUT
const int MAX_CHAR = 20; //maximum number of characters
static char char_buffer[1][20]; //characters will be stored here
static int number_of_chars; //total number of characters typed in
struct nk_context* ctx;

int main(void) {
	start_main();
	
	return 0;
}

static void start_main()
{
	WNDCLASSW wc;
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	DWORD style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	DWORD exstyle = 0;
	HWND wnd;
#if gdi_mode == 0
	ATOM atom;
	HDC dc;
#endif
	int running = 1;
	int needs_refresh = 1;

	/* Win32 */
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleW(0);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"NuklearWindowClass";
#if gdi_mode == 1
	RegisterClassW(&wc);
#elif gdi_mode == 0
	atom = RegisterClassW(&wc);
#endif

	AdjustWindowRectEx(&rect, style, FALSE, exstyle);

#if gdi_mode == 1
	wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"osu!Tracker - GDI+",
		style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, wc.hInstance, NULL);
	ctx = nk_gdip_init(wnd, WINDOW_WIDTH, WINDOW_HEIGHT);
	/* GUI */
	GdipFont* fontSmall;
	GdipFont* font;
	GdipFont* fontBig;
	// create font
	fontSmall = nk_gdipfont_create("Arial", 12);
	font = nk_gdipfont_create("Arial", 17);
	fontBig = nk_gdipfont_create("Arial", 22);
	nk_gdip_set_font(font);
#elif gdi_mode == 0
	/* GUI */
	GdiFont* fontSmall;
	GdiFont* font;
	GdiFont* fontBig;
	// create font
	//short fix = 1; // GDI behaves different on Fonts
	fontSmall = nk_gdifont_create("Arial", 16);
	font = nk_gdifont_create("Arial", 17);
	fontBig = nk_gdifont_create("Arial", 22);
	wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"osu!Tracker - GDI",
		style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, wc.hInstance, NULL);
	dc = GetDC(wnd);
	SetBkMode(dc, TRANSPARENT);
	ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);
#endif

	int toggle = 0;

#pragma region Default Design Config
	// Default Button Config
	ctx->style.button.rounding = 0;
	ctx->style.button.active.data.color.r = 0;
	ctx->style.button.active.data.color.g = 0;
	ctx->style.button.active.data.color.b = 0;
	ext_BttColor(ctx, 255, 255, 255);

	// Some Other Default Config
	ctx->style.window.padding.x = 0;
	ctx->style.window.padding.y = 0;
	ctx->style.window.min_row_height_padding = 0;
	nk_draw_set_color_inline(ctx, 1);
#pragma endregion

	while (running)
	{
		/* Input */
		MSG msg;
		nk_input_begin(ctx);
		if (needs_refresh == 0) {
			if (GetMessageW(&msg, NULL, 0, 0) <= 0)
				running = 0;
			else {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			needs_refresh = 1;
		}
		else needs_refresh = 0;
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				running = 0;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			needs_refresh = 1;
		}
		nk_input_end(ctx);

		/* GUI */
		ext_BG(ctx, 15, 15, 15);
		if (nk_begin(ctx, "", nk_rect(2, 0, WINDOW_WIDTH - 11, WINDOW_HEIGHT), NULL))
		{
#pragma region TOP
#if gdi_mode == 1
			nk_gdip_set_font(fontBig);
#elif gdi_mode == 0
			nk_gdi_set_font(fontBig);
#endif
			ext_TextColor(ctx, 150, 150, 255);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_label(ctx, username, NK_TEXT_CENTERED | NK_TEXT_ALIGN_TOP);
			ext_BG(ctx, 40, 40, 40);
#pragma endregion
#pragma region Toggle Menu
#if gdi_mode == 1
			nk_gdip_set_font(font);
#elif gdi_mode == 0
			nk_gdi_set_font(font);
#endif
			nk_layout_row_dynamic(ctx, 0, 3);
			if (toggle == 0) {
				ext_BttBg(ctx, 38, 139, 255);
			}
			else {
				ext_BttBg(ctx, 59, 69, 79);
			}
			ctx->style.button.text_alignment = NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE;
			if (nk_button_label(ctx, "Main")) {
				toggle = 0;
			}
			if (toggle == 1) {
				ext_BttBg(ctx, 38, 139, 255);
			}
			else {
				ext_BttBg(ctx, 59, 69, 79);
			}
			if (nk_button_label(ctx, "Settings")) {
				toggle = 1;
			}
			if (toggle == 2) {
				ext_BttBg(ctx, 38, 139, 255);
			}
			else {
				ext_BttBg(ctx, 59, 69, 79);
			}
			if (nk_button_label(ctx, "About")) {
				toggle = 2;
			}
#pragma endregion
			ext_BG(ctx, 20, 20, 20);

			nk_layout_row_dynamic(ctx, 503, 1);

			if (nk_group_begin(ctx, "container", NK_WINDOW_BORDER)) {
#if gdi_mode == 1
				nk_gdip_set_font(fontSmall);
#elif gdi_mode == 0
				nk_gdi_set_font(fontSmall);
#endif
				switch (toggle)
				{
				case 0: drawMain();
					break;
				case 1: drawSettings();
					break;
				case 2: drawAbout();
					break;
				}
				nk_group_end(ctx);
			}
		}
		nk_end(ctx);

		/* Draw */
#if gdi_mode == 1
		nk_gdip_render(AA, nk_rgb(15, 15, 15));
#elif gdi_mode == 0
		nk_gdi_render(nk_rgb(15, 15, 15));
#endif
	}
#if gdi_mode == 1
	nk_gdipfont_del(font);
	nk_gdipfont_del(fontBig);
	nk_gdipfont_del(fontSmall);
	nk_gdip_shutdown();
#elif gdi_mode == 0
	nk_gdifont_del(font);
	nk_gdifont_del(fontBig);
	nk_gdifont_del(fontSmall);
	ReleaseDC(wnd, dc);
#endif

	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}
// MAIN TAB UI
void drawMain() {
}
// SETTINGS TAB UI
void drawSettings() {
	ext_BttBg(ctx, 90, 90, 200);
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, "[color=#ffffff]osu! API Credentials: [/color]", 17);
	nk_layout_row_dynamic(ctx, 20, 1);
	ctx->style.button.text_alignment = NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE;
	if (nk_button_label(ctx, "Validate Credentials")) {
	}
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, "[color=#ffffff]Client ID:[/color] Not Set", 17 | NK_TEXT_ALIGN_LEFT);
	nk_layout_row_dynamic(ctx, 20, 1);

	nk_label(ctx, "[color=#ffffff]Client Secret:[/color] Not Set", 17 | NK_TEXT_ALIGN_LEFT);
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_edit_string(ctx, NK_EDIT_FIELD, char_buffer[0], &number_of_chars, MAX_CHAR, nk_filter_default);
}
// INFO TAB UI
void drawAbout() {
	ext_TextColor(ctx, 255, 255, 255);
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, "osu!Tracker by [color=#64beff]Railgun[/color] >> made in C/C++ ", 17);
	nk_layout_row_dynamic(ctx, 20, 1);
#if gdi_mode == 1
	nk_label(ctx, "GUI Library: [color=#64beff]Immediate-Mode-UI/Nuklear[/color] (GDI+ Backend)", 17);
#elif gdi_mode == 0
	nk_label(ctx, "GUI Library: [color=#64beff]Immediate-Mode-UI/Nuklear[/color] (GDI Backend)", 17);
#endif
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, "API: [color=#64beff]cpr[/color] | [color=#64beff]libcurl[/color]", 17);
	nk_layout_row_dynamic(ctx, 20, 1);

	// You have to decleare an extra variable for gradientText()
	// If u nest it into custom_strcat() u will still get memory leaked by gradientText()
	const char* grText = gradientText("Nyaruku", "#7C7CFF", "#FF016F");
	const char* labelText = custom_strcat(2, "My Discord: @", grText);
	nk_label(ctx, labelText, 17);
	// Prevent Memory Leak
	free((void*)grText);
	free((void*)labelText);
	

	nk_layout_row_dynamic(ctx, 20, 1);

	nk_label(ctx, "osu! Username: "
		"[color=#ffffff]_[/color]"
		"[color=#FF1A7D]Railgun[/color]"
		"[color=#ffffff]_[/color]", 17);
	nk_layout_row_dynamic(ctx, 40, 1);
	nk_label(ctx, "Links", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_BOTTOM);

	ext_BttBg(ctx, 90, 90, 200);
	ctx->style.button.text_alignment = NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE;
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_button_label(ctx, "osu! Tracker - GitHub Repository")) {
		OpenLink("https://github.com/nyaruku/osuTracker");
	}
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_button_label(ctx, "Nuklear - GitHub Repository")) {
		OpenLink("https://github.com/Immediate-Mode-UI/Nuklear");
	}
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_button_label(ctx, "My osu! Profile")) {
		OpenLink("https://osu.ppy.sh/users/13817114");
	}
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_button_label(ctx, "My Discord Server")) {
		//system("cmd /c start https://github.com/nyaruku/osuTracker");
	}
}