#include <iostream>
#include <conio.h>   // For kbhit() and getch()
#include <windows.h> // For Sleep() and system("cls")
#include <vector>
#include <ctime>     // For time()
#include <algorithm> // For remove_if
#include <cstdlib>   // For srand() and rand()

using namespace std;

const int lanes = 3;
const int screenHeight = 10;

struct Obstacle {
    int lane;
    int position; // 0 = top, screenHeight -1 = bottom
};

// Function to check if an obstacle should be removed
bool shouldRemoveObstacle(const Obstacle& o) {
    return o.position > screenHeight - 1;
}

void drawScreen(int carLane, const vector<Obstacle>& obstacles, int score) {
    system("cls");
    cout << "=== ASCII CAR RACE ===\n";
    cout << "Score: " << score << "\n";
    cout << "CONTROLS:\n";
    cout << "  A/a = Move LEFT  <--\n";
    cout << "  D/d = Move RIGHT -->\n";
    cout << "  W/w = ACCELERATE (boost score)\n";
    cout << "  P/p = PAUSE game\n";
    cout << "  Q/q = QUIT game\n";
    cout << "GOAL: Avoid obstacles [X] with your car [^]\n\n";
    
    for (int pos = 0; pos < screenHeight; pos++) {
        for (int lane = 0; lane < lanes; lane++) {
            bool isCar = (pos == screenHeight - 1 && lane == carLane);
            bool isObstacle = false;
            
            // Check if there's an obstacle at this position
            for (size_t i = 0; i < obstacles.size(); i++) {
                if (obstacles[i].lane == lane && obstacles[i].position == pos) {
                    isObstacle = true;
                    break;
                }
            }
            
            if (isCar) {
                cout << " [^] ";
            } else if (isObstacle) {
                cout << " [X] ";
            } else {
                cout << "  |  ";
            }
        }
        cout << "\n";
    }
    cout << "-------------------------\n";
}

int main() {
    srand((unsigned)time(0)); // Seed random once at program start
    
    int carLane = 1; // Start center lane
    vector<Obstacle> obstacles;
    int score = 0;
    int obstacleSpawnTimer = 0;
    const int obstacleSpawnInterval = 3;
    bool gameOver = false;
    
    cout << "*** WELCOME TO ASCII CAR RACE ***\n\n";
    cout << "GAME OBJECTIVE:\n";
    cout << "- Control your car [^] at the bottom\n";
    cout << "- Dodge falling obstacles [X]\n";
    cout << "- Survive as long as possible!\n";
    cout << "- Higher score = better driver!\n\n";
    
    cout << "KEYBOARD CONTROLS:\n";
    cout << "  [A] or [a] = Move car LEFT  <--\n";
    cout << "  [D] or [d] = Move car RIGHT -->\n";
    cout << "  [W] or [w] = ACCELERATE (increases score faster)\n";
    cout << "  [P] or [p] = PAUSE the game\n";
    cout << "  [Q] or [q] = QUIT the game\n\n";
    
    cout << "TIPS:\n";
    cout << "- Watch the obstacles [X] falling down\n";
    cout << "- Stay in clear lanes\n";
    cout << "- Use acceleration (W) for bonus points\n";
    cout << "- Don't let obstacles hit your car!\n\n";
    
    cout << "Ready to race? Press ANY KEY to START...";
    _getch();
    
    while (!gameOver) {
        // Spawn new obstacles
        if (obstacleSpawnTimer == 0) {
            Obstacle newObs;
            newObs.lane = rand() % lanes;
            newObs.position = 0;
            obstacles.push_back(newObs);
            obstacleSpawnTimer = obstacleSpawnInterval;
        } else {
            obstacleSpawnTimer--;
        }
        
        // Move obstacles down by 1 position
        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i].position++;
        }
        
        // Remove obstacles that went past the bottom
        obstacles.erase(
            remove_if(obstacles.begin(), obstacles.end(), shouldRemoveObstacle),
            obstacles.end()
        );
        
        drawScreen(carLane, obstacles, score);
        Sleep(200);
        
        // Handle input
        if (_kbhit()) {
            char key = _getch();
            if ((key == 'a' || key == 'A') && carLane > 0) {
                carLane--;
                cout << "\a"; // Beep sound for movement
            }
            else if ((key == 'd' || key == 'D') && carLane < lanes - 1) {
                carLane++;
                cout << "\a"; // Beep sound for movement
            }
            else if (key == 'w' || key == 'W') {
                score += 2; // Accelerate gives bonus points
                cout << "\a"; // Beep sound for acceleration
            }
            else if (key == 'p' || key == 'P') {
                cout << "\n*** GAME PAUSED ***\n";
                cout << "Press ANY KEY to resume racing...";
                _getch();
            }
            else if (key == 'q' || key == 'Q') {
                cout << "\n*** Thanks for playing! ***\n";
                cout << "Final Score: " << score << "\n";
                return 0;
            }
        }
        
        // Check collision: obstacle at car lane and bottom row?
        for (size_t i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].lane == carLane && obstacles[i].position == screenHeight - 1) {
                gameOver = true;
                break;
            }
        }
        
        // Increase score over time
        score++;
    }
    
    cout << "\n*** CRASH! GAME OVER! ***\n";
    cout << "You collided with an obstacle!\n";
    cout << "Final Score: " << score << " points\n\n";
    
    if (score < 50) {
        cout << "Beginner driver - Keep practicing!\n";
    } else if (score < 100) {
        cout << "Good driver - You're getting better!\n";
    } else if (score < 200) {
        cout << "Expert driver - Nice skills!\n";
    } else {
        cout << "MASTER DRIVER - Amazing performance!\n";
    }
    
    cout << "\nPress ANY KEY to exit...";
    _getch();
    
    return 0;
}
