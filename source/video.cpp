#include <string>

#include <coreinit/memory.h>
#include <SDL2/SDL_ttf.h>

#include <fa-solid-900_ttf.h>
#include "input.hpp"
#include "main.hpp"
#include "unlink.hpp"
#include "video.hpp"


void draw_background(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}


void draw_rectangle(int x, int y, int w, int h, int r, int g, int b, int a) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}


void draw_text(const char* text, int x, int y, int size, SDL_Color color) {
    void* font_data = nullptr;
    uint32_t font_size = 0;
    // We will fetch the system font from the shared data.
    OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0, &font_data, &font_size);

    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)font_data, font_size), 1, size);
    if (font == NULL) {
        return;
    }

    // UTF8 allows us to render all the special system characters.
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    if (surface == NULL) {
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // This is bad practice. We are creating and destroying the font every time we draw text.
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void draw_icon(const char* icon, int x, int y, int size, SDL_Color color) {
    // fa-solid-900.ttf is a font file that contains icons. This is a custom font file.
    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)fa_solid_900_ttf, fa_solid_900_ttf_size), 1, size);
    if (font == NULL) {
        return;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, icon, color);
    if (surface == NULL) {
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Again, bad practice. I'll fix this later on.
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}


int get_text_width(const char* text, int size) {
    void* font_data = nullptr;
    uint32_t font_size = 0;
    // We are essentially recreating the font here.
    OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0, &font_data, &font_size);

    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)font_data, font_size), 1, size);
    if (font == NULL) {
        return 0;
    }

    int width = 0;
    int height = 0;
    TTF_SizeUTF8(font, text, &width, &height); // This works with unicode characters.

    TTF_CloseFont(font);
    return width; // We could return height as well, but we don't need it.
}


void draw_screen_bars() {
    // These lines draw the top bar (with title, version, and author).
    draw_rectangle(0, 0, SCREEN_WIDTH, 90, 125, 0, 125, 255);
    draw_text("Wii U Account Swap", 64, 10, 50);
    draw_text(APP_VERSION, 64 + get_text_width("Wii U Account Swap", 50) + 16, 10, 50, {176, 176, 176, 255});
    draw_text("Nightkingale", SCREEN_WIDTH - 64 - get_text_width("Nightkingale", 50), 10, 50);

    // These lines draw the bottom bar (with current user and account file).
    draw_rectangle(0, 940, SCREEN_WIDTH, 140, 125, 0, 125, 255);
    draw_text("Current User: ", 64, 955, 40);
    draw_text(MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 40), 955, 40, {176, 176, 176, 255});

    if (INKAY_EXISTS)
        // Draw the plugin checkmark icon next to the name.
        draw_icon("\ue55c", 64 + get_text_width("Current User: ", 40) + get_text_width(MII_NICKNAME.c_str(), 40) + 16, 960, 40, {176, 176, 176, 255});
    else
        // Draw the plugin exit icon next to the name.
        draw_icon("\ue560", 64 + get_text_width("Current User: ", 40) + get_text_width(MII_NICKNAME.c_str(), 40) + 16, 960, 40, {176, 176, 176, 255});

    draw_text(ACCOUNT_FILE.c_str(), 64, 1005, 40);

    draw_text("\ue07d Navigate", SCREEN_WIDTH - 64 - get_text_width("\ue07d Navigate", 50), 975, 50);
}


void draw_confirm_button() {
    // Draw the confirm and decline buttons over the bottom bar.
    draw_rectangle(0, 940, SCREEN_WIDTH, 140, 125, 0, 125, 255);
    draw_text("\ue000 Confirm", 64, 975, 50);
    draw_text("\ue001 Decline", SCREEN_WIDTH - 64 - get_text_width("\ue001 Decline", 50), 975, 50);
    SDL_RenderPresent(renderer);
}
