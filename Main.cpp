#include "RocksmithRPC.h"
#include "include/discord_rpc.h"
#include <cstdio>
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    //Client ID: 452428491359649793
    RocksmithRPC client;
    const char* clientID = "452428491359649793";
    std::cout << "Initialising Discord Listeners" << std::endl;
    client.InitDiscord(clientID);
    // while (GetActiveWindowTitle() == "Rocksmith2014")
    // {
    //     UpdatePresence("Test", "Test", "Test", 1, 2);
    // }
    while(client.getCurrentState() != client.State::quitState)
        client.FormatPresence();
    client.Shutdown();
    return 0;
}
