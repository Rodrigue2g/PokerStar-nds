# PokerStar-nds

PokerStar-nds is a multiplayer poker game designed for the Nintendo DS.

## Build Instructions

### Prerequisites
- DevkitARM installed (please set DEVKITARM in your environment)
- Ensure the necessary libraries (dswifi9, libnds9, libmm9, etc.) are available and properly linked.

### Build Steps

1. Clone the repository: `git clone https://github.com/Rodrigue2g/PokerStar-nds.git`
2. Navigate to the project directory: `cd PokerStar-nds`
3. Set up the build environment by exporting the DevkitARM path: `export DEVKITARM=<path to devkitARM>`
4. Build the game using: `make clean && make`
5. The build output should generate the necessary `.nds` file for gameplay.

**Attention:** It's important to build the project using `make clean && make` to ensure a clean build and avoid potential conflicts or issues during compilation.

## Gameplay Instructions

- **Objective**: Win chips by forming the best hand or forcing opponents to fold.
- **Controls**: Utilize the DS controls to navigate menus and make gameplay decisions. Use the right arrow to trigger the next step of the game 
- **Multiplayer**: Connect with friends via local wireless connection for multiplayer games.
- **Game Flow**: 
  - Players take turns betting, raising, or folding based on their hand strength and strategy.
  - Use stylus controls to interact with the game interface.
- **Winning**: The player with the best hand at the showdown or the last remaining player after others fold wins the round.

## Troubleshooting

- If encountering issues during the build process or gameplay, refer to the project documentation or community forums for assistance.
- Ensure all dependencies and libraries are correctly configured for your development environment.


## Crédits: 
 <a href="https://www.flaticon.com/fr/icones-gratuites/jeton" title="jeton icônes">Jeton icônes créées par Freepik - Flaticon</a>

