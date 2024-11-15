#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Base class for User
class User {
protected:
    string username;
    string password;
public:
    User(string uname, string pass) : username(uname), password(pass) {}
    virtual void showMenu() = 0;  // Pure virtual function for menu display
    string getUsername() { return username; }
    string getPassword() { return password; }
    virtual string getRole() = 0;  // Pure virtual function to get role
};

// Question class for holding test questions and answers
class Question {
private:
    string questionText;
    string correctAnswer;
public:
    Question(string qText, string ans) : questionText(qText), correctAnswer(ans) {}

    // Mark this function as const because it doesn't modify the object
    string getQuestionText() const { return questionText; }

    // Mark this function as const because it doesn't modify the object
    bool checkAnswer(string answer) const {
        return answer == correctAnswer;
    }
};


// Test class for holding test data (questions, test name)
class Test {
private:
    string testName;
    vector<Question> questions;
public:
    Test(string name) : testName(name) {}

    void addQuestion(Question q) {
        questions.push_back(q);
    }

    // Mark this function as const because it doesn't modify the object
    string getTestName() const { return testName; }

    // Mark this function as const because it doesn't modify the object
    vector<Question> getQuestions() const { return questions; }
};


// Admin class for managing users
class Admin : public User {
public:
    Admin(string uname, string pass) : User(uname, pass) {}
    void showMenu() override {
        cout << "Admin Menu:\n";
        cout << "1. Add User\n";
        cout << "2. Remove User\n";
        cout << "3. View Test Schedule\n";
        cout << "4. View Test Performance\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
    }
    string getRole() override { return "admin"; }

    void addUser(User* user) {
        cout << "User " << user->getUsername() << " added.\n";
    }

    void removeUser(string username) {
        cout << "User " << username << " removed.\n";
    }

    void viewTestSchedule() {
        cout << "Viewing test schedule.\n";
    }

    void viewTestPerformance() {
        cout << "Viewing test performance.\n";
    }
};

// Teacher class for managing tests and questions
class Teacher : public User {
private:
    vector<Test> tests;  // A teacher can have multiple tests
public:
    Teacher(string uname, string pass) : User(uname, pass) {}
    
    void showMenu() override {
        cout << "Teacher Menu:\n";
        cout << "1. Create Test\n";
        cout << "2. Add Questions to Test\n";
        cout << "3. View Student Performance\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
    }
    string getRole() override { return "teacher"; }

    void createTest(string testName) {
        Test newTest(testName);
        tests.push_back(newTest);
        cout << "Test '" << testName << "' created.\n";
    }

    void addQuestionsToTest(string testName) {
        // Find the test
        for (Test& test : tests) {
            if (test.getTestName() == testName) {
                string questionText, correctAnswer;
                cout << "Enter question: ";
                cin.ignore();  // To clear the newline left in the buffer
                getline(cin, questionText);
                cout << "Enter correct answer: ";
                getline(cin, correctAnswer);

                Question q(questionText, correctAnswer);
                test.addQuestion(q);
                cout << "Question added to test '" << testName << "'.\n";
                return;
            }
        }
        cout << "Test not found.\n";
    }

    vector<Test> getTests() {
        return tests;
    }
};

// Student class for attempting tests
class Student : public User {
private:
    vector<Test> tests;  // A student can attempt multiple tests
public:
    Student(string uname, string pass) : User(uname, pass) {}

    void showMenu() override {
        cout << "Student Menu:\n";
        cout << "1. View Test Schedule\n";
        cout << "2. Attempt Test\n";
        cout << "3. View Test Performance\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
    }
    string getRole() override { return "student"; }

// Student class
void viewTestSchedule(const vector<Test>& tests) {
    cout << "Test Schedule:\n";
    for (const Test& test : tests) {  // Use const reference in loop to avoid unnecessary copies
        cout << "Test: " << test.getTestName() << endl;
    }
}

void attemptTest(string testName, const vector<Test>& tests) {
    for (const Test& test : tests) {  // Use const reference for accessing tests
        if (test.getTestName() == testName) {
            vector<Question> questions = test.getQuestions();
            int score = 0;
            for (const Question& q : questions) {  // Use const reference for accessing questions
                string answer;
                cout << q.getQuestionText() << "\nYour answer: ";
                cin >> answer;
                if (q.checkAnswer(answer)) {
                    score++;
                }
            }
            cout << "Your score for the test '" << testName << "': " << score << "/" << questions.size() << endl;
            return;
        }
    }
    cout << "Test not found.\n";
}


    void viewTestPerformance() {
        cout << "Viewing test performance.\n";
    }
};

// Function to handle login
User* login(vector<User*> users) {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    // Check if username and password match any user
    for (User* user : users) {
        if (user->getUsername() == username && user->getPassword() == password) {
            cout << "Login successful!\n";
            return user;
        }
    }
    cout << "Invalid credentials.\n";
    return nullptr;
}

int main() {
    vector<User*> users;
    
    // Create some users
    Admin* admin = new Admin("admin1", "adminpass");
    Teacher* teacher = new Teacher("teacher1", "teacherpass");
    Student* student = new Student("student1", "studentpass");

    // Add users to the vector
    users.push_back(admin);
    users.push_back(teacher);
    users.push_back(student);

    // Main login loop
    while (true) {
        User* loggedInUser = login(users);  // Login prompt
        if (!loggedInUser) continue;  // If login fails, prompt again

        // Show menu based on role
        if (loggedInUser->getRole() == "admin") {
            int choice;
            while (true) {
                admin->showMenu();
                cin >> choice;
                switch (choice) {
                    case 1: {
                        string username, password, role;
                        cout << "Enter username: ";
                        cin >> username;
                        cout << "Enter password: ";
                        cin >> password;
                        cout << "Enter role (teacher/student): ";
                        cin >> role;

                        if (role == "teacher") {
                            Teacher* newTeacher = new Teacher(username, password);
                            admin->addUser(newTeacher);
                            cout << "Teacher added successfully.\n";
                        } else if (role == "student") {
                            Student* newStudent = new Student(username, password);
                            admin->addUser(newStudent);
                            cout << "Student added successfully.\n";
                        } else {
                            cout << "Invalid role.\n";
                        }
                        break;
                    }
                    case 2: {
                        string username;
                        cout << "Enter username to remove: ";
                        cin >> username;
                        admin->removeUser(username);
                        break;
                    }
                    case 3:
                        admin->viewTestSchedule();
                        break;
                    case 4:
                        admin->viewTestPerformance();
                        break;
                    case 5:
                        return 0; // Exit program entirely if desired
                    default:
                        cout << "Invalid choice.\n";
                }
            }
        } else if (loggedInUser->getRole() == "teacher") {
            int choice;
            while (true) {
                teacher->showMenu();
                cin >> choice;
                switch (choice) {
                    case 1: {
                        string testName;
                        cout << "Enter test name: ";
                        cin >> testName;
                        teacher->createTest(testName);
                        break;
                    }
                    case 2: {
                        string testName;
                        cout << "Enter test name to add questions to: ";
                        cin >> testName;
                        teacher->addQuestionsToTest(testName);
                        break;
                    }
                    case 3:
                        cout << "Viewing student performance.\n";
                        break;
                    case 4:
                        break;  // Exit back to login screen
                    default:
                        cout << "Invalid choice.\n";
                }
                if (choice == 4) break;  // Exit teacher menu
            }
        } else if (loggedInUser->getRole() == "student") {
            int choice;
            while (true) {
                student->showMenu();
                cin >> choice;
                switch (choice) {
                    case 1: {
                        student->viewTestSchedule(teacher->getTests());
                        break;
                    }
                    case 2: {
                        string testName;
                        cout << "Enter test name to attempt: ";
                        cin >> testName;
                        student->attemptTest(testName, teacher->getTests());
                        break;
                    }
                    case 3:
                        student->viewTestPerformance();
                        break;
                    case 4:
                        break;  // Exit back to login screen
                    default:
                        cout << "Invalid choice.\n";
                }
                if (choice == 4) break;  // Exit student menu
            }
        }
    }

    return 0;
}
