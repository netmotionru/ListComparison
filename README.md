# ListComparison

## Overview
CLI utility for comparing altlinux binary package lists. This is a C++17 application developed using Qt 6.4.2 and built with qmake. The project is designed to be compliant with the Filesystem Hierarchy Standard (FHS) and is licensed under the GNU General Public License v3.0 (GPL-3.0).

## Requirements
- C++17 compatible compiler (e.g., GCC 7.1+)
- Qt 6.4.2
- qmake
- ALT Linux or other FHS-compliant Linux distribution

## Installation

### Building from Source
1. Clone the repository:
   ```bash git clone https://github.com/netmotionru/ListComparison.git
   cd ListComparison
   ```

2. You can use the **app.install** script to build and install automatically. If you want to do everything manually, skip this step and go to step 3.:
   ```bash app.install
   ```


3. **Next steps if you skipped step 2!** Loading qt6-base-devel qt6-websockets-devel packages:
   ```sudo apt-get update
   sudo apt-get install qt6-base-devel qt6-websockets-devel
   ```

4. Creating a build folder build:
   ```mkdir build
   cd build
   ```

5. **export PATH=$PATH:/usr/share/qt6/bin** is needed in case the system does not find qmake after installing the packages.:
   ```export PATH=$PATH:/usr/share/qt6/bin
   qmake ../ListComparison.pro
   make
   ```

6. Install the application:
   ```make install
   ```

6. Updating the shared library cache:
   ```sudo ldconfig
   ```

## Usage
Go to the build folder and run the application with the command:
   ```cd build/app
   ./app
   ```

## Examples of using the application
- Get all available branches from https://packages.altlinux.org/
   ```./app -s
   ```

- Get all data on the selected branch and output the number of binary packages separated by architecture
   ```./app -g p10
   ```

- Get all the data for the selected two branches and output the difference, all the packets that are present in the first branch but missing in the second branch. The data is separated by architecture.
   ```./app -d p10 -d sisyphus
   ```

- The same as in the previous paragraph, but output in JSON format and save the data to the file outputJson.txt
   ```./app -j -d p10 -d sisyphus
   ```


## License
This project is licensed under the GPL-3.0 License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements
- [Qt](https://www.qt.io) for the excellent framework
- [ALT Linux](https://www.altlinux.org) for providing a robust platform

## Contact
For any questions or suggestions, please contact:
- **NETMOTION**: netmotionru@gmail.com
- Telegram: https://t.me/netmotionru

### Additional Resources
- [Qt Documentation](https://doc.qt.io)
- [C++17 Standard](https://en.cppreference.com/w/cpp/17)
- [ALT Linux Documentation](https://www.altlinux.org/Documentation)
