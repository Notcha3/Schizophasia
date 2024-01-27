# Schizophasia.exe

Malware I made last week. Spent 2 days on it, mainly fixing my old code written 4 months ago.

## Small description that should be enough.
* It finds all volumes, checks their type to be either removable(flash drives, floppies or any external media) or fixed(Hard Drives/Solid state drives) and not be a cdrom or a drive without a letter assigned to it. Then the corruptor recursively scans through all the directories on them and replaces first 4096 bytes of a file with random numbers.
* It also makes some graphical effects while all this is happening(such as color palette incrementing, replacing text in all windows with random strings generated using _itow, changing the hue to random values, etc).
* And some audio payloads.

### A small notice...
Do whatever you want with it, but please don't use it against somebody(You will be violating the law lol). \
If you want to use any of the code from here, please meantion this repository in the description.
