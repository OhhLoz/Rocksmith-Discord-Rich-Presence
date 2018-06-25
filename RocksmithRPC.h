#include "discord_rpc.h"
#include "discord_register.h"
#include <string>
#include <vector>

class RocksmithRPC
{
    public:
        int getCurrentState();
        std::vector<std::string> splitString (std::string str, char c);

        static void handleDiscordReady(const DiscordUser *request);
        static void handleDiscordDisconnected(int errcode, const char* message);
        static void handleDiscordError(int errcode, const char* message);

        void InitDiscord(const char* clientID);
        void UpdatePresence(const char* state, const char* details, const char* largeImageKey, const char* largeImageText, const char* smallImageKey, const char* smallImageText, int64_t time);
        void FormatPresence();
        const char* getSongName();
        const char* getAlbumName();
        const char* getAccuracy();
        const char* getNotes();
        int64_t getEndTime();
        void printVariables(DiscordRichPresence s);
        bool IsFirstTimeSongPlaying();
        void Shutdown();

        enum State { menuState, songState, quitState };
    private:
        State currState = menuState;
        State prevState = menuState;
};