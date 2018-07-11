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

## Built With

* [Cygwin](https://www.cygwin.com/) - The GCC compiler used
* [CMake](https://cmake.org/) - Used to build the project
* [Discord Libraries](https://github.com/discordapp/discord-rpc) - Discord Libraries to connect to the discord rich presence developer applications
* [VSCode](https://code.visualstudio.com/) - Used to code the whole project

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Laurence Ure** - *Initial work* - [Website](www.laurenceure.me)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
