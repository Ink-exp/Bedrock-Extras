# Bedrock Extras 🚀

**Bedrock Extras** is an essential, high-performance native C++ plugin built for Minecraft Bedrock Edition on Android. It enhances gameplay performance and adds client-side utilities using a lightweight modular architecture.

---

## ✨ Features

- **⚡ Sodium Culling Module:** Intelligently culls entity rendering outside player distance thresholds to drastically boost FPS in crowded areas and servers.
- **🔌 LeviLauncher Integration:** Full support for LeviLauncher / Amethyst native mod menus under the **External Tab**.
- **🎯 Zero CPU Overhead:** Uses pre-calculated distance formulas and fast pointer drops to eliminate micro-stutters and frame spikes.

---

## 🛠️ Installation

1. Go to the **[Releases](../../releases)** tab of this repository.
2. Download the latest `BedrockExtras-v1.0.zip`.
3. Open **LeviLauncher** on your Android device.
4. Import `BedrockExtras-v1.0.zip` into your **Mods / Native Plugins** list.
5. Launch Minecraft Bedrock and enjoy the boosted performance!

---

## 💻 Building from Source

This project uses **CMake** and the **Android NDK** (`arm64-v8a`).

```bash
# Clone the repository
git clone [https://github.com/Ink-exp/Bedrock-Extras.git](https://github.com/Ink-exp/Bedrock-Extras.git)
cd Bedrock-Extras

# Configure CMake with Android NDK toolchain
cmake -B build \
      -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_PLATFORM=android-28

# Compile the shared library
cmake --build build --parallel
