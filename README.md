# This tool is now deprecated
  
## I recommend using the [Rocksniffer](https://github.com/kokolihapihvi/RockSniffer/releases) tool to which this used as it has discord rich presence built in
  
## Thank you to everyone who supported, downloaded & raised issues
  
  
  
# Rocksmith Discord Rich Presence

Small Project to try to implement Discord RIch Presence to track Rocksmith Songs live.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for your needs.

### Prerequisites

The only prerequisite for this to work is [Rocksniffer](https://github.com/kokolihapihvi/RockSniffer/releases)

```
Once Installed, remember where you installed Rocksniffer for use with this project.
```

### Installing

To download this project, navigate to the [releases](https://github.com/OhhLoz/Rocksmith-Discord-Rich-Presence/releases) of this project. Once downloaded, extract the contents of the .zip/.rar to the Rocksniffer's output folder.

```
The output folder should contain files such as, album_cover, song_details.txt, notes.txt, etc
```

### Running

For it to track the songs and update correctly, Rocksniffer must be running & have detected Rocksmith alongside this project running. There will be debug printouts whilst this project runs.

Run Rocksniffer -> Run RocksmithRPC (this project) -> Run Rocksmith2014

## Built With

* [Cygwin](https://www.cygwin.com/) - The GCC compiler used
* [CMake](https://cmake.org/) - Used to build the project
* [Discord Libraries](https://github.com/discordapp/discord-rpc) - Discord Libraries to connect to the discord rich presence developer applications
* [VSCode](https://code.visualstudio.com/) - Used to code the whole project

## Authors

* **Laurence Ure** - *Initial work* - [Website](http://www.laurenceure.me)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
