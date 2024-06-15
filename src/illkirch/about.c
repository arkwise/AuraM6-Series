
#include "kernel.h"
#include "illkirch.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include <string.h>

void AboutIllkirch ( void ) {
        TRect r;
        PWindow aw;
        PButton b;
        PLabel l;
        PImageLabel il;

        RectAssign(&r,0, 0, 300, 200);

        aw = CreateWindow( &Me, r, "About : codename illkirch", WF_FRAME|WF_CAPTION|WF_CENTERED );

        InsertWidget(DeskTop, WIDGET(aw));

        RectAssign(&r, 5, 5, 45, 50);
        il = CreateImageLabel( &Me, r, ListKey(Me.Ressource,"ICON32") );
        InsertWidget(WIDGET(aw), WIDGET(il));

        RectAssign(&r, 50, 5, 295, 50);
        l = CreateLabel(&Me, r, "Poind Mad's codename ILLKIRCH B\n\nCopyright (c) 2004 Point Mad. All rights reserved.");
        InsertWidget(WIDGET(aw), WIDGET(l));

        RectAssign(&r, 5, 55, 295, 170);
        l = CreateLabel(&Me, r, "The first visual interface creator for oZone GUI.\nUses the irreal Widget system (iWidget).\n\nDesigned by Julien Etelain (Point Mad).\n\nFor any informations mail julien@pmad.net");
        InsertWidget(WIDGET(aw), WIDGET(l));

        RectAssign(&r,
                WIDGET(aw)->ChildArea.b.x - WIDGET(aw)->ChildArea.a.x - 105,
                WIDGET(aw)->ChildArea.b.y-WIDGET(aw)->ChildArea.a.y-25,
                WIDGET(aw)->ChildArea.b.x-WIDGET(aw)->ChildArea.a.x-5,
                WIDGET(aw)->ChildArea.b.y-WIDGET(aw)->ChildArea.a.y-5);

        b = CreateButton(&Me, r, "Ok", WM_CLOSE);
        InsertWidget(WIDGET(aw), WIDGET(b));

        WidgetDrawAll(WIDGET(aw));

}
