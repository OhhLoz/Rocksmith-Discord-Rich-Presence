
#include "include/discord_rpc.h"

class RocksmithRPC
{
    public:
        RocksmithRPC();
        ~RocksmithRPC();
        int main(int argc, char const *argv[]);

        int getCurrentState();

        static void handleDiscordReady(const DiscordUser *request);
        static void handleDiscordDisconnected(int errcode, const char* message);
        static void handleDiscordError(int errcode, const char* message);

        void InitDiscord(const char* clientID);
        void UpdatePresence(const char* state, const char* details, const char* largeImageText, int64_t startTime, int64_t endTime);
        void FormatPresence();
        void Shutdown();

        enum State { menuState, songState, postSongState, quitState };
    private:
        State currState = menuState;
};