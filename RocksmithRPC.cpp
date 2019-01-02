
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <windows.h>
#include "discord_rpc.h"

struct Song
{
    std::string songName;
    std::string albumName;
    std::string accuracy;
    int64_t endTime;
};

enum State { menuState, songState, initState, quitState };
State currState = initState;
State prevState = initState;
Song currSong;
Song prevSong = currSong;
double versionNumber = 2.0;

static void printVariables(DiscordRichPresence s)
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

static std::vector<std::string> splitString (std::string str, char c)
{
    std::stringstream test(str);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(test, segment, c))
        seglist.push_back(segment);
    return seglist;
}

static void Shutdown()
{
    std::cout << "Shutting Down..." << std::endl;
    Discord_Shutdown();
    currState = quitState;
    exit(1);
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

static void InitDiscord(const char* clientID)
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
    Discord_Initialize(clientID, &handlers, 1, "221680");
}

static void UpdatePresence(const char* state, const char* details, const char* largeImageKey, const char* largeImageText, const char* smallImageKey, const char* smallImageText, int64_t time)
{
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
        std::cout << "State changed to MenuState" << std::endl;
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

static std::string getSongName()
{
    std::ifstream songFile;
    songFile.open("song_details.txt", std::fstream::in);
    std::string s;
    std::getline(songFile, s);
    songFile.close();
    return s.c_str();
}

static std::string getAlbumName()
{
    std::fstream albumFile;
    albumFile.open("album_details.txt", std::fstream::in);
    std::string s;
    std::getline(albumFile, s);
    albumFile.close();
    return s.c_str();
}

 static std::string getAccuracy()
 {
     std::fstream accuracyFile;
     accuracyFile.open("accuracy.txt", std::fstream::in);
     std::string s;
     std::getline(accuracyFile, s);
     accuracyFile.close();
     return s.c_str();
 }

static int64_t getEndTime()
{
    std::fstream songtimerFile;
    songtimerFile.open("song_timer.txt", std::fstream::in);
    std::string s;
    std::getline(songtimerFile, s);
    songtimerFile.close();
    if (s != "")
    {
        std::vector<std::string> splitBySlash = splitString(s, '/');
        std::vector<std::string> splitByColon = splitString(splitBySlash[1], ':');
        int seconds = std::atoi(splitByColon[1].c_str());
        seconds += (std::atoi(splitByColon[0].c_str()) * 60);
        return std::time(nullptr) + seconds;
    }
    else
        return std::time(nullptr);
}

static bool isStateChanged()
{
    if (currState != prevState)
        return true;
}

static bool isFirstTimeSongPlaying()
{
    return prevState == menuState && currState == songState;
}

static bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

static bool isSongChange(Song currSong, Song prevSong)
{
    return (currSong.songName != prevSong.songName && currSong.albumName != prevSong.albumName);
}

static void FormatPresence()
{
    static int64_t time = std::time(nullptr);
    std::ostringstream songStr;
    std::ostringstream detailsStr;
    std::ostringstream accuracyStr;

    // if (isSongChange(currSong, prevSong)) //doesnt currently do anything
    //    currState = songState;

    currSong.albumName = getAlbumName();
    currSong.songName = getSongName();
    currSong.accuracy = getAccuracy();

    //std::string currAlbumName = getAlbumName();
    // std::string currSongName = getSongName();

    //  std::cout << "Album Name: " << currSong.albumName << std::endl;
    //  std::cout << "Song Name: " << currSong.songName << std::endl;

    if (!currSong.albumName.empty() && currState != songState && !currSong.songName.empty() && isSongChange(currSong, prevSong))
    {
        currState = songState;
        std::cout << "State changed to SongState" << std::endl;
    }

    if(currState == initState || currState == menuState)
    {
        UpdatePresence("Browsing Menus", "", "rocksmithlarge", "Rocksmith 2014 Remastered", "rocksmithsmall", "Created by OhhLoz", time);
    }
    else if (isStateChanged())
    {
        // std::cout << "Previous State = " << prevState << std::endl;
        // std::cout << "Current State = " << currState << std::endl;
        switch(currState)
        {
            case menuState:
                UpdatePresence("Browsing Menus", "", "rocksmithlarge", "Rocksmith 2014 Remastered", "rocksmithsmall", "Created by OhhLoz", time);
                break;
            case songState:
                if(isFirstTimeSongPlaying() || isSongChange(currSong, prevSong))
                {
                    //std::cout<< "Start Time= " << time << std::endl;
                    time = getEndTime();
                    currSong.endTime = time;
                    //std::cout << "End Time=" << time << std::endl;
                }
                songStr << currSong.songName;
                detailsStr << currSong.albumName;
                accuracyStr << "Accuracy: " << currSong.accuracy << std::endl;
                UpdatePresence(detailsStr.str().c_str(), songStr.str().c_str(), "album_cover", accuracyStr.str().c_str(), "rocksmithsmall", "Created by OhhLoz", time);
                break;
            case quitState:
                std::cout << "Shutting Down" << std::endl;
                std::string temp;
                std::cin >> temp;
                Shutdown();
                break;
        }
    }

    prevState = currState;
    prevSong = currSong;
    //std::cout << "PrevState & Song set to CurrState & Song" << std::endl;
    //  if (isStateChanged())
    //  {
    //      std::cout << "End Previous State = " << prevState << std::endl;
    //      std::cout << "End Current State = " << currState << std::endl;
    //  }
}

int main(int argc, char const *argv[])
{
    //Client ID: 452428491359649793
    //cmake --build "D:\Documents\Apps\Rocksmith Discord Rich Presence\cmake-build-debug" --target RocksmithRPC -- -j 2
    const char* clientID = "452428491359649793";
    std::cout << "Welcome to RocksmithRPC Version: " << versionNumber << std::endl;
    std::cout << "Please report any issues on the github (www.github.com/Rocksmith-Discord-Rich-Presence)" << std::endl;
    std::cout << "Thank you for using my program!" << std::endl << std::endl;
    std::cout << "Initialising Discord Listeners" << std::endl;
    // std::cout << "Active Window: " << GetActiveWindow() << std::endl;
    InitDiscord(clientID);
    // while (GetActiveWindowTitle() == "Rocksmith2014")
    // {
    //     UpdatePresence("Test", "Test", "Test", 1, 2);
    // }
    while(currState != State::quitState)
    {
       FormatPresence();
       Sleep(1000);
    }
    std::cout << "Shutting Down" << std::endl;
    std::string temp;
    std::cin >> temp;
    Shutdown();
    return 0;
}
