/* Draw a screen. */
#ifndef SCREEN_H
#define SCREEN_H

#define WIN_WIDTH 80
#define WIN_HEIGHT 40
#define PADDING 1
#define INNER_WIDTH (WIN_WIDTH - BORDER_WIDTH * 2)
#define INNER_HEIGHT (WIN_HEIGHT - BORDER_WIDTH * 2)

#define BORDER_WIDTH 1
#define BORDER_HOR '-'
#define BORDER_VER '|'

/* In order to make the broder dispaly properly,
 * this routine should run at the end of drawing.
 */
void screen_draw_border(char *help_msg);

#endif
