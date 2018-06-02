#include "include/discord_rpc.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

const char * GetActiveWindowTitle()
{
 char wnd_title[256];
 HWND hwnd=GetForegroundWindow();
 GetWindowText(hwnd,(LPWSTR)wnd_title,sizeof(wnd_title));
 return wnd_title;
}

static void handleDiscordReady(const DiscordUser *request)
{
    std::cout << "\nDiscord: ready" << std::endl;
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    std::cout << printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    std::cout << printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

void InitDiscord(const char* clientID)
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.errored = handleDiscordError;
    handlers.disconnected = handleDiscordDisconnected;
    // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
    Discord_Initialize(clientID, &handlers, 1, "221680");
}

void UpdatePresence(const char* state, const char* details, const char* largeImageText, int64_t startTime, int64_t endTime)
{
    char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    if (sizeof(state) > 128)
    {
        std::cout << "\nState parameter is too long\nPress any key to exit..." << std::endl;
        Shutdown();
    }
    else if (sizeof(state) < 1)
    {
        discordPresence.state = "Browsing Menus";
    }
    else
        discordPresence.state = state;

    if (sizeof(details) > 128 || strlen(details) < 1)
    {
        std::cout << "\nDetails parameter is too long or not set\nPress any key to exit..." << std::endl;
        Shutdown();
    }

    if(startTime > endTime)
    {
        std::cout << "\nStartTime Greater than End Time\nPress any key to exit..." << std::endl;
        Shutdown();
    }
    else if (startTime == endTime)
    {
        discordPresence.state = "Finished a Song";
    }

    sprintf(buffer, "%s", details);
    discordPresence.details = buffer;

   if (!(strlen(largeImageText) < 1))
        discordPresence.largeImageText = largeImageText;

   if (discordPresence.state == "Browsing Menus" || discordPresence.state == "Finished a Song")
        discordPresence.largeImageKey = "rocksmithlogo2";
   else
        discordPresence.largeImageKey = "album_cover";
    discordPresence.smallImageKey = "rocksmithlogo";
    discordPresence.smallImageText = "Created by sallad/OhhLoz";

    Discord_UpdatePresence(&discordPresence);
}

void Shutdown()
{
    std::cout << "Shutting Down..." << std::endl;
    Discord_Shutdown();
    exit(1);
}


int main(int argc, char const *argv[])
{
    //Client ID: 452428491359649793
    std::cout << "Initialising Discord Listeners" << std::endl;
    InitDiscord("452428491359649793");
    // while (GetActiveWindowTitle() == "Rocksmith2014")
    // {
    //     UpdatePresence("Test", "Test", "Test", 1, 2);
    // }
    UpdatePresence("Test", "Test", "Test", 1, 2);
    return 0;
}
