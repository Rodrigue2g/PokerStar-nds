# PokerStar-nds

PokerStar-nds is a multiplayer poker game designed for the Nintendo DS.

## Build Instructions

### Prerequisites
- DevkitARM installed (Please set DEVKITARM in your environment. `export DEVKITARM=<path to>devkitARM>`)
- Ensure the necessary libraries (dswifi9, libnds9, libmm9, etc.) are available and properly linked.

### Build Steps

1. Clone the repository: `git clone https://github.com/Rodrigue2g/PokerStar-nds.git`
2. Navigate to the project directory: `cd PokerStar-nds`
3. Set up the build environment by exporting the DevkitARM path: `export DEVKITARM=<path to>devkitARM>`
4. Build the game using: `make clean && make`
5. The build output should generate the necessary `.nds` file for gameplay.

**Attention:** It's important to build the project using `make clean && make` to ensure a clean build and avoid potential conflicts or issues during compilation.

## Gameplay Instructions

- **Objective**: Win blinds by getting the best hand and/or bluffing to push opponents to fold.
- **Controls**: Utilize the DS controls to navigate menus and make gameplay decisions. Use the right arrow to trigger the next step of the game 
- **Multiplayer**: Connect with friends via local wireless connection for multiplayer games (WEP access point only).
- **Game Flow**: 
  - Players take turns betting, raising, or folding based on their hand strength and strategy.
  - Use stylus controls to interact with the game interface.
- **Winning**: The player with the best hand at the showdown or the last remaining player after others fold wins the round.

## Troubleshooting

- If encountering issues during the build process or gameplay, refer to the project documentation or community forums for assistance.
- Ensure all dependencies and libraries are correctly configured for your development environment.


## License

PokerStar-nds is licensed under the [MIT License](LICENSE).

```
MIT License

Copyright (c) [Year] [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```


## Crédits: 

Logo (and idea) from <a href="https://www.pokerstars.com" title="poker-stars">pokerstars.com</a>

Images from <a href="https://www.google.com/imghp?hl=en" title="google-images">Goolge Images</a>

.mod files from <a href="https://modarchive.org/" title="mod-files">modarchive.org</a>
