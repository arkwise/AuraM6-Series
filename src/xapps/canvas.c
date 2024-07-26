/////////////////////////////////////////////////////////////////////////////////
//
//  Simple Window Example
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "canvas.h"
#include "window.h"
#include "tbox.h"

#define BTN_OK 0x4269

PWindow w = NULL;
PCanvas c = NULL;
TRect r;
p_bitmap Bmp1;

// Function to draw shapes on the canvas
void DrawShapes(PWidget o, p_bitmap buffer, PRect w)
{
    // Clear the canvas with a background color
    rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, makecol(255, 255, 255));

    // Draw a circle
    circle(buffer, (w->a.x + w->b.x) / 2, (w->a.y + w->b.y) / 4, 20, makecol(255, 0, 0));

    // Draw a square
    rect(buffer, (w->a.x + w->b.x) / 2 - 20, (w->a.y + w->b.y) / 2 - 20,
         (w->a.x + w->b.x) / 2 + 20, (w->a.y + w->b.y) / 2 + 20, makecol(0, 255, 0));

    // Draw a cross (X)
    line(buffer, (w->a.x + w->b.x) / 4, (w->a.y + w->b.y) / 4,
         (w->a.x + w->b.x) / 4 + 40, (w->a.y + w->b.y) / 4 + 40, makecol(0, 0, 255));
    line(buffer, (w->a.x + w->b.x) / 4 + 40, (w->a.y + w->b.y) / 4,
         (w->a.x + w->b.x) / 4, (w->a.y + w->b.y) / 4 + 40, makecol(0, 0, 255));

    // Draw a triangle
    triangle(buffer, (w->a.x + w->b.x) * 3 / 4, (w->a.y + w->b.y) * 3 / 4,
             (w->a.x + w->b.x) * 3 / 4 - 20, (w->a.y + w->b.y) * 3 / 4 + 40,
             (w->a.x + w->b.x) * 3 / 4 + 20, (w->a.y + w->b.y) * 3 / 4 + 40, makecol(255, 255, 0));
}

// Function to draw the setup image
void Draw1(PWidget o, p_bitmap buffer, PRect w)
{
    draw_sprite(buffer, Bmp1, (w->a.x + w->b.x - Bmp1->w) / 2, (w->a.y + w->b.y - Bmp1->h) / 2);
}

// Application event handler
l_bool AppEventHandler(PWidget o, PEvent Ev)
{
    if (Ev->Type == EV_MESSAGE && Ev->Message == BTN_OK)
    {
        WidgetDispose(WIDGET(w));
        return true;
    }
    return false;
}

l_int Main(int argc, l_text *argv)
{
    RectAssign(&r, 0, 0, 400, 300);
    w = CreateWindow(&Me, r, "Simple Window Example", WF_FRAME | WF_CAPTION | WF_MINIMIZE | WF_CENTERED);
    InsertWidget(DeskTop, WIDGET(w));
    WIDGET(w)->AppEvHdl = &AppEventHandler;

    RectAssign(&r, 0, 0, 400, 200); // Adjust the size to fit within the window
    c = CreateCanvas(&Me, r);
    WIDGET(c)->Draw = &Draw1; // Draw the setup image initially
    WIDGET(c)->Flags |= WFForceBuffer;
    InsertWidget(WIDGET(w), WIDGET(c)); // Ensure the canvas is inserted within the window

    // Update the draw function to DrawShapes after some event if needed
    WIDGET(c)->Draw = &DrawShapes;

    WidgetDrawAll(WIDGET(w));
    return true;
}

void Close(void)
{
    // Cleanup logic if necessary
}
