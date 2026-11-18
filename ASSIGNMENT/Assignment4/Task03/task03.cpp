#include <iostream>
#include <string>

class Address {
    std::string city;
    int pin;

public:
    Address(const std::string& c, int p) : city(c), pin(p) {
        std::cout << "Address constructed\n";
    }
    ~Address() { std::cout << "Address destroyed\n"; }

    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Person {
protected:
    std::string name;
    int age;
    Address home;

public:
    Person(const std::string& n, int a, const std::string& city, int pin)
        : name(n), age(a), home(city, pin) {
        std::cout << "Person constructed\n";
    }
    virtual ~Person() { std::cout << "Person destroyed\n"; }
};

class Student : virtual public Person {
protected:
    long long regNo;

public:
    Student(const std::string& n, int a, const std::string& city, int pin,
            long long reg)
        : Person(n, a, city, pin), regNo(reg) {
        std::cout << "Student constructed\n";
    }
    ~Student() override { std::cout << "Student destroyed\n"; }
};

class Staff : virtual public Person {
protected:
    std::string department;

public:
    Staff(const std::string& n, int a, const std::string& city, int pin,
          const std::string& dept)
        : Person(n, a, city, pin), department(dept) {
        std::cout << "Staff constructed\n";
    }
    ~Staff() override { std::cout << "Staff destroyed\n"; }
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const std::string& n, int a, const std::string& city, int pin,
                      long long reg, const std::string& dept)
        : Person(n, a, city, pin),
          Student(n, a, city, pin, reg),
          Staff(n, a, city, pin, dept) {
        std::cout << "TeachingAssistant constructed\n";
    }
    ~TeachingAssistant() override {
        std::cout << "TeachingAssistant destroyed\n";
    }
};

class Course {
    std::string title;

public:
    explicit Course(const std::string& t) : title(t) {}
    std::string getTitle() const { return title; }
};

class CourseStudent : public Person {
    long long regNo;
    Course course;

public:
    CourseStudent(const std::string& n, int a, const std::string& city, int pin,
                  long long reg, const std::string& courseName)
        : Person(n, a, city, pin), regNo(reg), course(courseName) {
        std::cout << "CourseStudent constructed: " << regNo
                  << ", " << course.getTitle() << '\n';
    }
};

int main() {
    std::cout << "-- Student object --\n";
    {
        Student s("Kavya", 19, "Salem", 636001, 192311001);
        std::cout << "-- end Student scope --\n";
    }

    std::cout << "\n-- Staff object --\n";
    {
        Staff st("Ravi", 40, "Chennai", 600001, "Physics");
        std::cout << "-- end Staff scope --\n";
    }

    std::cout << "\n-- TeachingAssistant object --\n";
    {
        TeachingAssistant ta("Meena", 24, "Madurai", 625001, 192311002,
                             "Computer Science");
        std::cout << "-- end TeachingAssistant scope --\n";
    }

    std::cout << "\n-- Member class demonstration --\n";
    CourseStudent cs("Kavya", 19, "Salem", 636001, 192311001, "C++");
    return 0;
}
