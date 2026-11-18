#include <iostream>
#include <string>

class Person {
protected:
    std::string name;
    int id;
    std::string phone;

public:
    Person(const std::string& n, int personId, const std::string& p)
        : name(n), id(personId), phone(p) {}
    virtual void display() const {
        std::cout << "Name: " << name << ", ID: " << id
                  << ", Phone: " << phone << '\n';
    }
    virtual ~Person() = default;
};

class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;

public:
    Student(const std::string& n, int personId, const std::string& p,
            const std::string& prog, double grade)
        : Person(n, personId, p), programme(prog), cgpa(grade) {}
    void displayStudent() const {
        std::cout << "Programme: " << programme << ", CGPA: " << cgpa << '\n';
    }
};

class Staff : virtual public Person {
protected:
    std::string department;
    double salary;

public:
    Staff(const std::string& n, int personId, const std::string& p,
          const std::string& dept, double amount)
        : Person(n, personId, p), department(dept), salary(amount) {}
    void displayStaff() const {
        std::cout << "Department: " << department << ", Salary: " << salary << '\n';
    }
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const std::string& n, int personId, const std::string& p,
                      const std::string& prog, double grade,
                      const std::string& dept, double amount)
        : Person(n, personId, p),
          Student(n, personId, p, prog, grade),
          Staff(n, personId, p, dept, amount) {}

    void display() const override {
        Person::display();
        displayStudent();
        displayStaff();
    }
};

class ResearchScholar : public Student {
private:
    double fellowship;

public:
    ResearchScholar(const std::string& n, int personId, const std::string& p,
                    const std::string& prog, double grade, double grant)
        : Person(n, personId, p), Student(n, personId, p, prog, grade),
          fellowship(grant) {}

    void displayScholar() const {
        Person::display();
        displayStudent();
        std::cout << "Fellowship: " << fellowship << '\n';
    }
};

int main() {
    TeachingAssistant ta("Kavya", 201, "9876543210", "MCA", 8.7,
                         "Computer Science", 25000);
    std::cout << "Teaching Assistant\n";
    ta.display();

    std::cout << "\nResearch Scholar\n";
    ResearchScholar rs("Arun", 202, "9123456780", "PhD", 9.1, 18000);
    rs.displayScholar();
    return 0;
}
