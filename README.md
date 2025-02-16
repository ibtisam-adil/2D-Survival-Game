# 2D Survival Game

## **Game Overview**
This project is a 2D game written in C++ using the Raylib library. The game features a player who battles against bombs, minions, and a boss to achieve victory..

### **Player (5 HP)**
- Moves in 8 directions using `W`, `A`, `S`, `D`.
- Shoots projectiles in 4 directions using arrow keys.
- Takes damage when colliding with bombs or minions.

### **Projectiles (1 HP)**
- Shot by the player and move towards a target.
- Despawn upon reaching the target or hitting an entity.
- Deal 1 damage on impact.

### **Bombs (5 HP)**
- Spawn at the beginning of the level.
- Deal 5 damage to the player upon contact.
- Take damage from projectiles only.
- Ignore collisions with other bombs, minions, and the boss.

### **Boss (40 HP)**
- Spawns at the beginning of the level.
- Wanders randomly and spawns minions every 0.5 seconds.

### **Minions (2 HP)**
- Spawn near the boss and move faster than the boss.
- Deal 1 damage to the player upon contact and despawn.

---

## **Implementation Details**

### **Data Structures**

1. **Entity Storage**:
    - `std::vector` is used to store dynamic collections of game entities like projectiles, minions, and bombs. This allows efficient addition and iteration.
    - Each entity class (e.g., `Player`, `Projectile`, `Bomb`, `Boss`, and `Minion`) encapsulates its behavior and state.
    
2. **Game State Management**:
    - A `std::stack<GameState>` manages transitions between states like the main menu, gameplay, and game-over screens. This ensures easy and logical state switching.
    
3. **Resource Management**:
    - **Textures**: Stored using an `std::unordered_map<std::string, Texture2D>` in the `localResourceManager`. This minimizes redundant loading of assets.
    - **Sounds**: Managed using a similar `std::unordered_map<std::string, Sound>` in the `soundManager`.

### **Entity Lifecycle Management**

- **Projectiles**:
    - Created dynamically when the player shoots.
    - Removed when they exceed their lifetime or hit a target.
    
- **Minions**:
    - Spawn dynamically near the boss .
    - Removed when their health reaches zero or after collisions.
    
### **Noteworthy Features**

1. **Resource Management**:
    - Using the Flyweight pattern, redundant loading of textures and sounds is avoided. Resources are loaded once and shared across the game.
    
2. **Game State Stack**:
    - The `std::stack` enables organized transitions between game states.

### **Game States**
- Main Menu: Includes `START` and `EXIT` buttons.
- Game Screen: Displays the game with entities.
- Win/Lose Screen: Shows the result and has a `MAIN MENU` button.

### **Controls**
- **Movement**: `W`, `A`, `S`, `D`.
- **Shooting**: Arrow keys.

### **Collision Detection**
- Circles are used to check collisions between entities like projectiles, bombs, and minions.

### **Code Organization**
- Game logic is separated from rendering.
- Multiple `.cpp` and `.h` files ensure clear organization.
- Variables, functions, and classes are meaningfully named.

---

## **Architectural Issues**
- Limited visual effects for interactions (e.g., explosions or damage effects).
- Input handling (IsKeyDown, IsKeyReleased) is directly managed in various functions. An input manager could provide a unified interface, making the game more adaptable to different input devices or configurations.

---

## **Future Improvements**
- Use object pooling for minions and projectiles.
- Add animations for better visual feedback.
- Implement power-ups or upgrades for the player.
- Use Observer Pattern.

---

## **References**
- Raylib Documentation: [https://www.raylib.com/](https://www.raylib.com/)
- Game Programming Patterns: [https://gameprogrammingpatterns.com](https://gameprogrammingpatterns.com)

---

## **How to Run**
1. Clone the repository:
   ```bash
   git clone git@gitea.speldesign.uu.se:5SD813-2024/ibtisam_adil_assignment.git
