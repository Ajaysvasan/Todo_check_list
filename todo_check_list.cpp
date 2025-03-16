#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Task {
    string date;
    string day;
    string description;
    bool completed;
};

class TodoList {
private:
    vector<Task> tasks;
    string filename;

    // Save tasks to file
    void saveToFile() {
        ofstream file(filename);
        if (!file) {
            cerr << "Error: Unable to open file for writing!" << endl;
            return;
        }

        for (const Task& task : tasks) {
            file << task.date << "|" 
                 << task.day << "|" 
                 << task.description << "|" 
                 << (task.completed ? "1" : "0") << endl;
        }
        file.close();
    }

    // Load tasks from file
    void loadFromFile() {
        tasks.clear();
        ifstream file(filename);
        if (!file) {
            cout << "No existing task file found. Starting with empty list." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            Task task;
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);

            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                task.date = line.substr(0, pos1);
                task.day = line.substr(pos1 + 1, pos2 - pos1 - 1);
                task.description = line.substr(pos2 + 1, pos3 - pos2 - 1);
                task.completed = (line.substr(pos3 + 1) == "1");
                tasks.push_back(task);
            }
        }
        file.close();
    }

public:
    TodoList(const string& file) : filename(file) {
        loadFromFile();
    }

    // Add a new task
    void addTask() {
        Task newTask;
        newTask.completed = false;
        
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> newTask.date;
        
        cout << "Enter day (e.g., Monday): ";
        cin.ignore(); // Clear newline from buffer
        getline(cin, newTask.day);
        
        cout << "Enter task description: ";
        getline(cin, newTask.description);
        
        tasks.push_back(newTask);
        saveToFile();
        
        cout << "Task added successfully!" << endl;
    }

    // Display all tasks
    void displayTasks() {
        if (tasks.empty()) {
            cout << "No tasks found!" << endl;
            return;
        }

        cout << "\n===== TODO LIST =====" << endl;
        cout << left << setw(12) << "DATE" << setw(10) << "DAY" 
             << setw(50) << "DESCRIPTION" << "STATUS" << endl;
        cout << string(80, '-') << endl;

        for (size_t i = 0; i < tasks.size(); i++) {
            cout << left << setw(12) << tasks[i].date 
                 << setw(10) << tasks[i].day 
                 << setw(50) << tasks[i].description 
                 << (tasks[i].completed ? "[X]" : "[ ]") << endl;
        }
    }

    // Mark a task as completed
    void markCompleted() {
        displayTasks();
        if (tasks.empty()) return;
        
        int index;
        cout << "Enter task number to mark as completed (1-" << tasks.size() << "): ";
        cin >> index;
        
        if (index < 1 || index > static_cast<int>(tasks.size())) {
            cout << "Invalid task number!" << endl;
            return;
        }
        
        tasks[index-1].completed = true;
        saveToFile();
        cout << "Task marked as completed!" << endl;
    }

    // Delete a task
    void deleteTask() {
        displayTasks();
        if (tasks.empty()) return;
        
        int index;
        cout << "Enter task number to delete (1-" << tasks.size() << "): ";
        cin >> index;
        
        if (index < 1 || index > static_cast<int>(tasks.size())) {
            cout << "Invalid task number!" << endl;
            return;
        }
        
        tasks.erase(tasks.begin() + index - 1);
        saveToFile();
        cout << "Task deleted successfully!" << endl;
    }

    // Search tasks by date
    void searchByDate() {
        if (tasks.empty()) {
            cout << "No tasks found!" << endl;
            return;
        }
        
        string searchDate;
        cout << "Enter date to search (YYYY-MM-DD): ";
        cin >> searchDate;
        
        cout << "\n===== TASKS FOR " << searchDate << " =====" << endl;
        cout << left << setw(12) << "DATE" << setw(10) << "DAY" 
             << setw(50) << "DESCRIPTION" << "STATUS" << endl;
        cout << string(80, '-') << endl;
        
        bool found = false;
        for (const Task& task : tasks) {
            if (task.date == searchDate) {
                cout << left << setw(12) << task.date 
                     << setw(10) << task.day 
                     << setw(50) << task.description 
                     << (task.completed ? "[X]" : "[ ]") << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No tasks found for the specified date!" << endl;
        }
    }
};

int main() {
    TodoList todoList("tasks.txt");
    int choice;
    
    while (true) {
        cout << "\n===== TODO LIST APPLICATION =====" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Display Tasks" << endl;
        cout << "3. Mark Task as Completed" << endl;
        cout << "4. Delete Task" << endl;
        cout << "5. Search Tasks by Date" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 0:
                cout << "Exiting application. Goodbye!" << endl;
                return 0;
            case 1:
                todoList.addTask();
                break;
            case 2:
                todoList.displayTasks();
                break;
            case 3:
                todoList.markCompleted();
                break;
            case 4:
                todoList.deleteTask();
                break;
            case 5:
                todoList.searchByDate();
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    
    return 0;
}