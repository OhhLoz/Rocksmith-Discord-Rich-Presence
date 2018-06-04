#include "RocksmithRPC.h"
#include "include/discord_rpc.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

int RocksmithRPC::getCurrentState()
{
    return this->currState;
}

void RocksmithRPC::Shutdown()
{
    std::cout << "Shutting Down..." << std::endl;
    Discord_Shutdown();
    exit(1);
}

void RocksmithRPC::handleDiscordReady(const DiscordUser *request)
{
    std::cout << "\nDiscord: ready" << std::endl;
}

void RocksmithRPC::handleDiscordDisconnected(int errcode, const char* message)
{
    std::cout << printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

void RocksmithRPC::handleDiscordError(int errcode, const char* message)
{
    std::cout << printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

void RocksmithRPC::InitDiscord(const char* clientID)
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.errored = handleDiscordError;
    handlers.disconnected = handleDiscordDisconnected;
    // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
    Discord_Initialize(clientID, &handlers, 1, "221680");
}

void RocksmithRPC::UpdatePresence(const char* state, const char* details, const char* largeImageText, int64_t startTime, int64_t endTime)
{
    char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    if (sizeof(state) > 128)
    {
        std::cout << "\nState parameter is too long\nPress any key to exit..." << std::endl;
        Shutdown();
    }

    if (sizeof(details) > 128)
    {
        std::cout << "\nDetails parameter is too long or not set\nPress any key to exit..." << std::endl;
        Shutdown();
    }

    if (sizeof(state) < 1)
    {
        discordPresence.state = "Browsing Menus";
    }
    else
        discordPresence.state = state;

    if (sizeof(details) < 1)
    {
        discordPresence.details = "Browsing Menus";
    }
    else
        discordPresence.details = details;

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

   if (currState == menuState)
        discordPresence.largeImageKey = "rocksmithlogo2";
   else
        discordPresence.largeImageKey = "album_cover";

    discordPresence.smallImageKey = "rocksmithlogo";
    discordPresence.smallImageText = "Created by sallad/OhhLoz";

    Discord_UpdatePresence(&discordPresence);
}

void RocksmithRPC::FormatPresence()
{
    switch(currState)
    {
        case menuState:
            UpdatePresence("Browsing Menus", "Browsing Menus", "Rocksmith", 1, 2);
            break;
        case songState:
            //Get Song Name
            //Get Song Length
            //Get Album Cover
            //Get Album Name
            //Get Accuracy
            break;
        case postSongState:
            UpdatePresence("Finished Playing a Song", "Browsing Menus", "Rocksmith", 1, 2);
            break;
        case quitState:
            Shutdown();
            break;
    }
}
