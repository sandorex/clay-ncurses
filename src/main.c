// clay experimental implementation with ncurses

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CLAY_IMPLEMENTATION
#include "clay.h"

// Initialize ncurses
void Clay_Ncurses_Initialize() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_BLUE, -1);
    init_pair(4, COLOR_CYAN, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_YELLOW, -1);
}

// Measure the dimensions of the text
static Clay_Dimensions Clay_Ncurses_MeasureText(Clay_String *str, Clay_TextElementConfig *config) {
    // TODO what happens if text is wrapped
    Clay_Dimensions dimensions = {0};
    dimensions.width = str->length;
    dimensions.height = 1; // ncurses text is always one line high
    return dimensions;
}

void Clay_Ncurses_Render(Clay_RenderCommandArray commands) {
    clear();
    for (int i = 0; i < commands.length; i++) {
        Clay_RenderCommand *cmd = &commands.internalArray[i];
        switch (cmd->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = cmd->config.rectangleElementConfig;
                int x = cmd->boundingBox.x;
                int y = cmd->boundingBox.y;
                int w = cmd->boundingBox.width;
                int h = cmd->boundingBox.height;
                attron(COLOR_PAIR(1));
                for (int j = 0; j < h; j++) {
                    mvhline(y + j, x, ' ', w);
                }
                attroff(COLOR_PAIR(1));
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = cmd->config.textElementConfig;
                int x = cmd->boundingBox.x;
                int y = cmd->boundingBox.y;
                char *text = strndup(cmd->text.chars, cmd->text.length);
                mvprintw(y, x, "%s", text);
                free(text);
                break;
            }
            default:
                break;
        }
    }
    refresh();
}

// TODO
// . event loops
// . get terminal resize events
// . get terminal mouse events
// . test more complicated layout

int main(void) {
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(arena, (Clay_Dimensions){86, 26});

    Clay_Ncurses_Initialize();
    Clay_SetMeasureTextFunction(Clay_Ncurses_MeasureText);

    Clay_BeginLayout();
    CLAY(CLAY_RECTANGLE({ .color = {255, 0, 0, 255} }), CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_FIXED(40), .height = CLAY_SIZING_FIXED(10) } })) {}
    CLAY(CLAY_TEXT(CLAY_STRING("Hello, ncurses!"), CLAY_TEXT_CONFIG({ .fontSize = 16, .textColor = {0, 255, 0, 255} })));
    Clay_RenderCommandArray commands = Clay_EndLayout();

    Clay_Ncurses_Render(commands);

    getch();
    endwin();
    return 0;
}
