#include "UltraEngine.h"

#include "_GLOBAL.h"
#include "resource.h"

using namespace UltraEngine;

//DPI Scale
bool Callback(const Event& ev, shared_ptr<Object> extra)
{
    //Resize window if desired
    auto window = ev.source->As<Window>();
    window->SetShape(ev.position, ev.size);

    //Get the user interface
    auto ui = extra->As<Interface>();
    ui->SetScale(float(ev.data) / 100.0f);

    return true;
}

int main(int argc, const char* argv[])
{
    auto plugin = LoadPlugin("Plugins/FITextureLoader.*");
    if (plugin == NULL)
    {
        Print("Failed to load FreeImage plugin.");
        return 0;
    }

    // Get the available displays
    auto displays = GetDisplays();

    // Create a window
    auto window = CreateWindow(APPNAME, 
        0, 0, DEFAULTWINSIZES.x, DEFAULTWINSIZES.y,
        displays[0], WINDOW_TITLEBAR | WINDOW_RESIZABLE | WINDOW_CENTER);
    window->SetMinSize(DEFAULTWINSIZES.x, DEFAULTWINSIZES.y);

    // Create user interface
    auto ui = CreateInterface(window);
    iVec2 sz = ui->root->ClientSize();

#pragma region AppIcon
    // Get device context
    HWND hwnd = window->GetHandle();
    HDC hdc = GetDC(hwnd);

    // Load the icon for window titlebar and taskbar
    HICON icon = LoadIconA(GetModuleHandle(NULL), (LPCSTR)IDI_ICON_DEFAULT);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
#pragma endregion Display a custom app icon

    // Create menu
    auto mainmenu = CreateMenu("", ui->root);
    auto menu_file = CreateMenu("File", mainmenu);
    auto menu_open = CreateMenu("Open", menu_file);
    auto menu_save = CreateMenu("Save", menu_file);
    CreateMenu("", menu_file);
    auto menu_quit = CreateMenu("Quit", menu_file);

    // Main panel
    int padding = 4;
    auto mainpanel = CreatePanel(padding, mainmenu->size.y + padding, sz.x - padding * 2, sz.y - padding * 2 - mainmenu->size.y, ui->root, PANEL_BORDER);
    mainpanel->SetLayout(1, 1, 1, 1);
    mainpanel->SetColor(0.1, 0.1, 0.1);

#pragma region DPIScaleEvent
    //DPI Scale event
    ListenEvent(EVENT_WINDOWDPICHANGED, window, Callback, ui);

    //Trigger a rescale if the display scale is not 100%
    if (displays[0]->scale != 1.0f)
    {
        EmitEvent(EVENT_WINDOWDPICHANGED,
            window,
            Round(displays[0]->scale * 100.0f), 0, 0, DEFAULTWINSIZES.x * displays[0]->scale, DEFAULTWINSIZES.y * displays[0]->scale);
    }
#pragma endregion DPI Scaling event registration and event handeling

    
#pragma region MainLoop
    while (true)
    {
        /// <summary>
        /// Main program loop. Handle events throughout the app life.
        /// </summary>
        const Event ev = WaitEvent();
        switch (ev.id)
        {
        case EVENT_WIDGETACTION:
            if (ev.source == menu_quit)
            {
                return 0;
            }
            else if (ev.source == menu_open)
            {
                WString path = RequestFile("Open Image");
                if (path != "")
                {
                    auto pixmap = LoadPixmap(path);
                    if (pixmap)
                    {
                        mainpanel->SetPixmap(pixmap, PIXMAP_CONTAIN);
                        window->SetText("Image Viewer - " + StripDir(path));
                    }
                    else
                    {
                        Notify("Failed to load pixmap \"" + path + "\"", "Error", true);
                    }
                }
            }
            else if (ev.source == menu_save)
            {
                if (mainpanel->pixmap)
                {
                    WString path = RequestFile("Save Image", "", "Portable Network Graphics:png;JPEG:jpg;Bitmap:bmp", 0, true);
                    if (path != "")
                    {
                        if (!mainpanel->pixmap->Save(path))
                        {
                            Notify("Failed to save pixmap \"" + path + "\"", "Error", true);
                        }
                    }
                }
            }
            break;
        case EVENT_WINDOWCLOSE:
            if (ev.source == window) return 0;
            break;
        }
    }
#pragma endregion Main actions loop
    return 0;
}