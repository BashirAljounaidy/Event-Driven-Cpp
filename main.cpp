#include <iostream>
#include <vector>
#include <functional>
#include <windows.h>
#include <iomanip>

using namespace std;

class Event {
private:
    vector<function<void()>> listeners;
public:
    void addListener(function<void()> listener) {
        listeners.push_back(listener);
    }

    void trigger() {
        for (auto& listener : listeners) {
            listener();
        }
    }
};

class Button {
private:
    string name;
    int left_top_x;     // X-coordinate of the top-left corner
    int left_top_y;     // Y-coordinate of the top-left corner
    int right_down_x;   // X-coordinate of the bottom-right corner
    int right_down_y;   // Y-coordinate of the bottom-right corner
    Event ButtonEvent;

public:
    // Constructor to initialize the button
    Button(string n, int x, int y, int width, int height)
        : name(n), left_top_x(x), left_top_y(y), right_down_x(x + width), right_down_y(y + height) {}

    string GetName() const {
        return name;
    }

    // Method to check if a point is inside the button
    bool isInside(int x, int y) const {
        return x >= left_top_x && x <= right_down_x &&
            y >= left_top_y && y <= right_down_y;
    }

    void addListener(function<void()> listener) {
        ButtonEvent.addListener(listener);
    }

    void OnClick() {
        ButtonEvent.trigger();
    }
};

class Counter {
private:
    unsigned short value;
public:
    Counter() : value(0) {}

    void increaserCounter() {
        value++;
    }

    void resetCounter() {
        value = 0;
    }

    void displayDialog() const {
        system("cls");  // Clear the console screen
        cout << "+=================================+" << endl;
        cout << "| " << setw(20) << "Count = " << value << setw(12) << " |" << endl;
        cout << "+=================================+" << endl;
        cout << endl;
        cout << "+================+================+" << endl;
        cout << "|     Count      |      Reset     |" << endl;
        cout << "+================+================+" << endl;
    }
};

int main() {
    Counter myCounter;
    myCounter.displayDialog();

    // Define buttons with coordinates
    Button CountButton("Count", 0, 90, 100, 50);
    Button ResetButton("Reset", 130, 90, 100, 50);

    // Set up event listeners for Count and Reset buttons
    CountButton.addListener([&]() {
        myCounter.increaserCounter();
        myCounter.displayDialog();
        });

    ResetButton.addListener([&]() {
        myCounter.resetCounter();
        myCounter.displayDialog();
        });

    // Main loop to detect button clicks
    while (true) {
        POINT cursorPosition;
        GetCursorPos(&cursorPosition);

        bool isRightButtonDown = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
        if (isRightButtonDown) {
            if (CountButton.isInside(cursorPosition.x, cursorPosition.y)) {
                CountButton.OnClick();
                Sleep(300);  // Debounce
            }
            if (ResetButton.isInside(cursorPosition.x, cursorPosition.y)) {
                ResetButton.OnClick();
                Sleep(300);  // Debounce
            }
        }

        Sleep(10);  // Small delay to reduce CPU usage
    }

    return 0;
}
