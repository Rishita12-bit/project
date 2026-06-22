#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Date {
    int year = 0;
    int month = 0;
    int day = 0;

    Date() {}

    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
};

const string MONTH_NAMES[] = {
    "", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int daysInMonth(int year, int month) {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month];
}

bool isValidDate(const Date& date) {
    if (date.year < 1 || date.month < 1 || date.month > 12) {
        return false;
    }
    return date.day >= 1 && date.day <= daysInMonth(date.year, date.month);
}

string dateKey(const Date& date) {
    ostringstream out;
    out << setw(4) << setfill('0') << date.year << "-"
        << setw(2) << setfill('0') << date.month << "-"
        << setw(2) << setfill('0') << date.day;
    return out.str();
}

int weekdayOfFirstDay(int year, int month) {
    if (month < 3) {
        month += 12;
        --year;
    }

    int k = year % 100;
    int j = year / 100;
    int h = (1 + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    return (h + 6) % 7; // Sunday = 0
}

int readInt(const string& prompt) {
    int value = 0;

    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Date readDate() {
    Date date;

    while (true) {
        date.year = readInt("Year: ");
        date.month = readInt("Month (1-12): ");
        date.day = readInt("Day: ");

        if (isValidDate(date)) {
            return date;
        }

        cout << "That date is not valid. Please try again.\n\n";
    }
}

void printLine() {
    cout << "\n------------------------------------------------------------\n";
}

void displayCalendar(int year, int month, const map<Date, vector<string>>& events) {
    printLine();
    cout << "                 " << MONTH_NAMES[month] << " " << year << "\n";
    cout << "  Sun   Mon   Tue   Wed   Thu   Fri   Sat\n";

    int firstWeekday = weekdayOfFirstDay(year, month);
    int totalDays = daysInMonth(year, month);

    for (int i = 0; i < firstWeekday; ++i) {
        cout << "      ";
    }

    for (int day = 1; day <= totalDays; ++day) {
        Date date{year, month, day};
        bool hasEvent = events.find(date) != events.end();

        cout << setw(5) << day << (hasEvent ? "*" : " ");

        if ((firstWeekday + day) % 7 == 0) {
            cout << "\n";
        }
    }

    cout << "\n\nDates marked with * have events.\n";
}

void addEvent(map<Date, vector<string>>& events) {
    printLine();
    cout << "Add Event\n";
    Date date = readDate();

    cout << "Event title or description: ";
    string eventText;
    getline(cin, eventText);

    if (eventText.empty()) {
        cout << "Empty events are not saved.\n";
        return;
    }

    events[date].push_back(eventText);
    cout << "Saved event on " << dateKey(date) << ".\n";
}

void viewEventsForDate(const map<Date, vector<string>>& events) {
    printLine();
    cout << "View Events\n";
    Date date = readDate();

    auto found = events.find(date);
    if (found == events.end()) {
        cout << "No events found on " << dateKey(date) << ".\n";
        return;
    }

    cout << "\nEvents on " << dateKey(date) << ":\n";
    for (size_t i = 0; i < found->second.size(); ++i) {
        cout << "  " << (i + 1) << ". " << found->second[i] << "\n";
    }
}

void viewAllEvents(const map<Date, vector<string>>& events) {
    printLine();
    cout << "All Events\n";

    if (events.empty()) {
        cout << "No events have been added yet.\n";
        return;
    }

    for (const auto& entry : events) {
        cout << dateKey(entry.first) << "\n";
        for (size_t i = 0; i < entry.second.size(); ++i) {
            cout << "  - " << entry.second[i] << "\n";
        }
    }
}

void moveMonth(int& year, int& month, int offset) {
    month += offset;

    while (month < 1) {
        month += 12;
        --year;
    }

    while (month > 12) {
        month -= 12;
        ++year;
    }
}

void pauseForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    map<Date, vector<string>> events;
    int currentYear = 2026;
    int currentMonth = 6;

    cout << "Calendar Event Planner\n";
    cout << "Events are stored while the program is running.\n";

    while (true) {
        displayCalendar(currentYear, currentMonth, events);
        cout << "\nMenu\n";
        cout << "  1. Previous month\n";
        cout << "  2. Next month\n";
        cout << "  3. Jump to month\n";
        cout << "  4. Add event\n";
        cout << "  5. View events on a date\n";
        cout << "  6. View all events\n";
        cout << "  7. Exit\n";

        int choice = readInt("\nChoose an option: ");

        if (choice == 1) {
            moveMonth(currentYear, currentMonth, -1);
        } else if (choice == 2) {
            moveMonth(currentYear, currentMonth, 1);
        } else if (choice == 3) {
            int year = readInt("Year: ");
            int month = readInt("Month (1-12): ");
            if (year >= 1 && month >= 1 && month <= 12) {
                currentYear = year;
                currentMonth = month;
            } else {
                cout << "Invalid month. Keeping the current calendar view.\n";
                pauseForEnter();
            }
        } else if (choice == 4) {
            addEvent(events);
            pauseForEnter();
        } else if (choice == 5) {
            viewEventsForDate(events);
            pauseForEnter();
        } else if (choice == 6) {
            viewAllEvents(events);
            pauseForEnter();
        } else if (choice == 7) {
            cout << "\nGoodbye.\n";
            break;
        } else {
            cout << "Please choose a menu option from 1 to 7.\n";
            pauseForEnter();
        }
    }

    return 0;
}
