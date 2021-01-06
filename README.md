# Axis
An open source Game Engine written in C and C++.

## Platforms
Axis currently supports Windows and Linux(Tested in Ubuntu) only.

## Building

### Windows
- Modify the .bat script to support the specific IDE in use, then run to generate projects.

### Linux
Axis has extra development depedencies needed for Linux. The following packages are needed to compile the project:

- `libxcursor`
- `libxrandr`
- `libxinerama`
- `libxi`
- `libglew`

On Debian and Debian derivative distributions, install these packages by running:

`sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev`

Axis is then configured and compiled as follows
- Modify the .sh script to support the specific IDE in use.
- Open terminal in script folder execute the following lines of commands
```
./LinuxGenProjects.sh
```

## Features

### Logging
Axis Engine utilizes spdlog library (https://github.com/gabime/spdlog) to handle all logging activities. Axis generates log information
then prints to console and saves in a log file named Axis.log

### Math
Axis Engine utilizes glm Math library (https://github.com/g-truc/glm.git) to handle fast Math calculations.

### Rendering

### Graphical User Interface

### Audio

## License
Axis is developed by MikeMathuKE, and uses the [GNU General Public License v2.0] (https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

## Development
Axis engine is inspired by Hazel Engine designed by theCherno
