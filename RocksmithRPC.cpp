#include "include/discord_rpc.h"
#include <cstdio>
#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
    //Client ID: 452428491359649793
    std::cout << "Initialising Discord Listeners" << std::endl;
    InitDiscord();
    return 0;
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

void InitDiscord()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.errored = handleDiscordError;
    handlers.disconnected = handleDiscordDisconnected;
    // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
    Discord_Initialize("452428491359649793", &handlers, 1, "221680");
}

void UpdatePresence(const char* state, const char* details, const char* smallImageText, const char* largeImageText)
{
    //Set required variables
    char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    if (sizeof(state) > 128 || strlen(state) < 1)
    {
        std::cout << "\nState parameter is too long or not set\nPress any key to exit..." << std::endl;
        Shutdown();
    }
    discordPresence.state = state;
    if (sizeof(details) > 128 || strlen(details) < 1)
    {
        std::cout << "\nDetails parameter is too long or not set\nPress any key to exit..." << std::endl;
        Shutdown();
    }

    sprintf(buffer, "%s", details);
    discordPresence.details = buffer;

    discordPresence.largeImageKey = "album_cover";
    discordPresence.smallImageKey = "rocksmithlogo";

    if (!(strlen(smallImageText) < 1))
        discordPresence.largeImageText = largeImageText;

    if (!(strlen(smallImageText) < 1))
        discordPresence.smallImageText = smallImageText;

    //Actaully update the presence
    Discord_UpdatePresence(&discordPresence);
}

void Shutdown()
{
    char temp;
    std::cout << "Shutting Down..." << std::endl;
    Discord_Shutdown();
    std::cin >> temp;
    exit(1);
}
// void UpdatePresence()
// {
//     char buffer[256];
//     DiscordRichPresence discordPresence;
//     memset(&discordPresence, 0, sizeof(discordPresence));
//     discordPresence.state = "In a Group";
//     sprintf(buffer, "Ranked | Mode: %d", GameEngine.GetMode());
//     discordPresence.details = buffer;
//     discordPresence.endTimestamp = time(0) + 5 * 60;
//     discordPresence.largeImageKey = "canary-large";
//     discordPresence.smallImageKey = "ptb-small";
//     discordPresence.partyId = GameEngine.GetPartyId();
//     discordPresence.partySize = 1;
//     discordPresence.partyMax = 6;
//     discordPresence.matchSecret = "4b2fdce12f639de8bfa7e3591b71a0d679d7c93f";
//     discordPresence.spectateSecret = "e7eb30d2ee025ed05c71ea495f770b76454ee4e0";
//     discordPresence.instance = 1;
//     Discord_UpdatePresence(&discordPresence);
// }
