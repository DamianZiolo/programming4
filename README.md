# 🚀 Minigin Project – Programming 4

## 🎮 About

This project is a **game built using Minigin**, created as part of the **Programming 4** course.

The main goal of this assignment is to **develop a custom game engine in C++ from scratch**, and then use it to recreate a classic arcade game.


> 🕹️ **Galaga**
## Repository

Source control repository:

```text
https://github.com/DamianZiolo/programming4
```

---

## Engine Design

The engine was designed with an approach inspired by Unity. Most gameplay functionality is implemented through components attached to GameObjects.

Some notable design choices:

* A Singleton-based `GameTime` class is used to provide global access to delta time.
* Projectile management is implemented using the Object Pool pattern to avoid frequent allocations and deallocations during gameplay.
* Levels are loaded from external `.txt` files, allowing enemy formations to be modified without recompiling the project.
* High scores are stored in text files and loaded when displaying the leaderboard.
* Sound playback is handled on a separate worker thread using a threaded sound system, following the approach demonstrated during the course.

---

## Design Patterns Used

The project uses all required design patterns:

* Game Loop
* Update Method
* Command
* Observer
* Component
* State

Additional patterns and techniques used:

* Singleton
* Object Pool
* Service Locator
* Threading (Sound System)

---

## Game Modes

### Solo

One player controls the ship.

### Duo

Two players cooperate and share a single score, but have 2 ships and separate lifes. The final score is saved under the same nickname.

### Versus

One player controls the ship while the second player controls the Galaga bosses by triggering attacks.

---

## Controls

### Keyboard

#### Ship

| Action     | Key   |
| ---------- | ------|
| Move Left  | A     |
| Move Right | D     |
| Shoot      | Space |

#### Galaga (Versus Mode Only)

| Action              | Key |
| ------------------- | --- |
| Trigger Bombing Run | J   |
| Trigger Beam Attack | K   |

#### Global

| Action             | Key |
| ------------------ | --- |
| Skip Current Level | F1  |
| Toggle Sound Mute  | F2  |

---

### Controller

#### Ship

| Action | Button             |
| ------ | ------------------ |
| Move   | D-Pad Left / Right |
| Shoot  | B                  |

#### Galaga (Versus Mode Only)

| Action              | Button |
| ------------------- | ------ |
| Trigger Bombing Run | A      |
| Trigger Beam Attack | B      |

---

## Statistics and High Scores

At the end of a game the player can view:

* Final Score
* Shots Fired
* Number of Hits
* Hit/Miss Ratio (%)

The game also stores and displays the Top 5 High Scores.

---

## External References

A very useful reference during development was:

https://freegalaga.com

It allowed me to play the original game, compare mechanics, and verify gameplay behaviour during implementation.

