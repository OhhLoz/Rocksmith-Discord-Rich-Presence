#include "RocksmithRPC.h"
#include <cstdio>
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    //Client ID: 452428491359649793
    //g++ -L lib -l discord-rpc Main.cpp RocksmithRPC.h RocksmithRPC.cpp -o RocksmithRPC
    //g++ Main.cpp RocksmithRPC.h RocksmithRPC.cpp -o RocksmithRPC -L lib -l discord-rpc
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
