## 🌦️ Weather CLI

A lightweight command-line tool to fetch current weather data for any city using the OpenWeatherMap API. Built with C++ and libcurl.

---

### 🚀 Features

- Get current weather by city name
- Supports Celsius and Fahrenheit
- Caches responses for faster repeat queries
- Automatically creates config file on first run

---

### 🛠️ Installation

#### 1. Build and install

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -S . -B release
cmake --build release --config Release
sudo cmake --install release
```
##### Package to create .deb and .rpm files
```bash
cd release
cpack
```

This installs the binary to `/usr/local/bin/weather`.

#### 2. First run

```bash
weather
```

On first run, it creates a config file at `~/.weather/settings.ini` with default values.

---

### ⚙️ Configuration

Edit `~/.weather/settings.ini` to customize:

```ini
[weather]
units=C
cache_enabled=true
default_city=Burnaby
api_key=YOUR_API_KEY
```

Get your API key from [OpenWeatherMap](https://openweathermap.org/api).

---

### 📦 Dependencies

- C++23
- libcurl

Install libcurl on Ubuntu:

```bash
sudo apt install libcurl4-openssl-dev
```
Install libcurl, gcc14, gcc14-c++ for C++23 on OpenSuse:

```bash
sudo zypper install gcc14 gcc14-c++ make cmake libcurl-devel

```
---

### 📄 License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0).
You are free to:
- Use, modify, and distribute the software
- Share it with others
- Contribute improvements
As long as:
- You include the same license in derivative works
- You disclose source code when distributing binaries
For full details, see the GPL-3.0 License.
go to https://www.gnu.org/licenses/gpl-3.0.en.html
