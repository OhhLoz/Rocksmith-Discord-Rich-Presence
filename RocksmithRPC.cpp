#include "RocksmithRPC.h"
#include "include/discord_rpc.h"
#include <ctime>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int RocksmithRPC::getCurrentState()
{
    return this->currState;
}

void RocksmithRPC::Shutdown()
{
    std::cout << "Shutting Down..." << std::endl;
    Discord_Shutdown();
    currState = quitState;
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
    currState = menuState;
    DiscordEventHandlers handlers;
    std::memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.errored = handleDiscordError;
    handlers.disconnected = handleDiscordDisconnected;
    if (strlen(clientID) < 1)
    {
        std::cout << "\nClientID Not Set" << std::endl;
        Shutdown();
    }
    // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
    Discord_Initialize(clientID, &handlers, 1, "221680");
}

void RocksmithRPC::UpdatePresence(const char* state, const char* details, const char* largeImageKey, const char* largeImageText, const char* smallImageKey, const char* smallImageText, int64_t time)
{
    prevState = currState;
    char buffer[256];
    DiscordRichPresence discordPresence;
    std::memset(&discordPresence, 0, sizeof(discordPresence));

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

    if(std::time(nullptr) >= time && currState == songState)
    {
        currState = menuState;
    }

    if (currState == songState)
        discordPresence.endTimestamp = time;
    else if (currState == menuState)
        discordPresence.startTimestamp = time;

    sprintf(buffer, "%s", details);
    discordPresence.details = buffer;

    discordPresence.state = state;
    //discordPresence.details = details;
    discordPresence.smallImageKey = smallImageKey;
    discordPresence.smallImageText = smallImageText;
    discordPresence.largeImageKey = largeImageKey;
    discordPresence.largeImageText = largeImageText;
    printVariables(discordPresence);
    Discord_UpdatePresence(&discordPresence);
}

const char* RocksmithRPC::getSongName()
{
    std::fstream songFile;
    songFile.open("song_details", std::fstream::in);
    std::string s;
    std::getline(songFile, s);
    songFile.close();
    return s.c_str();
}

const char* RocksmithRPC::getAlbumName()
{
    std::fstream albumFile;
    albumFile.open("album_details", std::fstream::in);
    std::string s;
    std::getline(albumFile, s);
    albumFile.close();
    return s.c_str();
}

const char* RocksmithRPC::getAccuracy()
{
    std::fstream accuracyFile;
    accuracyFile.open("accuracy", std::fstream::in);
    std::string s;
    std::getline(accuracyFile, s);
    accuracyFile.close();
    return s.c_str();
}

const char* RocksmithRPC::getNotes()
{
    std::fstream notesFile;
    notesFile.open("notes", std::fstream::in);
    std::string s;
    std::getline(notesFile, s);
    notesFile.close();
    return s.c_str();
}

int64_t RocksmithRPC::getEndTime()
{
    std::fstream songtimerFile;
    songtimerFile.open("song_timer", std::fstream::in);
    std::string s;
    std::getline(songtimerFile, s);
    songtimerFile.close();
    std::vector<std::string> splitBySlash = splitString(s, '/');
    std::vector<std::string> splitByColon = splitString(splitBySlash[1], ':');
    int seconds = std::atoi(splitByColon[1].c_str());
    seconds += (atoi(splitByColon[0].c_str()) * 60);
    return std::time(nullptr) + seconds;
}

std::vector<std::string> RocksmithRPC::splitString (std::string str, char c)
{
    std::stringstream test(str);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(test, segment, c))
        seglist.push_back(segment);
    return seglist;
}

void RocksmithRPC::printVariables(DiscordRichPresence s)
{
    std::cout << "\nCURRENT SETTINGS" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "STATE IS: " << s.state << std::endl;
    std::cout << "DETAILS ARE: " << s.details << std::endl;
    std::cout << "LARGE IMAGE IS: " << s.largeImageKey << std::endl;
    std::cout << "SMALL IMAGE IS: " << s.smallImageKey << std::endl;
    std::cout << "SMALL IMAGE TEXT IS: " << s.smallImageText << std::endl;
    std::cout << "LARGE IMAGE TEXT IS: " << s.largeImageText << std::endl;
}

bool RocksmithRPC::IsFirstTimeSongPlaying()
{
    if (prevState == menuState && currState == songState)
        return true;
    return false;
}

void RocksmithRPC::FormatPresence()
{
    static const char* songName;
    static const char* albumName;
    static const char* accuracy;
    static const char* notes;
    static int64_t time = std::time(nullptr);
    std::ostringstream songStr;
    std::ostringstream detailsStr;

    switch(currState)
    {
        case menuState:
            UpdatePresence("Browsing Menus", "Browsing Menus", "rocksmithlogo2", "Rocksmith", "rocksmithlogo", "Created by sallad/OhhLoz", time);
            break;
        case songState:
            if (IsFirstTimeSongPlaying())
            {
                songName = getSongName();
                albumName = getAlbumName();
                time = getEndTime();
            }
            accuracy = getAccuracy();
            notes = getNotes();
            songStr << "Playing: " << songName;
            detailsStr << "Accuracy: " << accuracy << "Notes: " << notes;
            UpdatePresence(songStr.str().c_str(),  detailsStr.str().c_str(), "album_cover", albumName, "rocksmithlogo", "Created by sallad/OhhLoz", time);
            //Get Song Name
            //Get Song Length
            //Get Album Cover
            //Get Album Name
            //Get Accuracy
            break;
        case quitState:
            Shutdown();
            break;
    }
}
