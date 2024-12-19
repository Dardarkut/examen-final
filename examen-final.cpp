#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <list>
#include <regex>
#include <stdexcept>
#include <limits>
using namespace std;

struct Employee {
    string idNumber;
    string name;
    string dateOfBirth;
    string category;
    double salary;
    string address;
    string phone;
    string email;
    int age;
};

struct Project {
    string projectCode;
    string name;
    string startDate;
    string endDate;
};

map<string, Employee> employees;
map<string, Project> projects;
map<string, list<string>> assignments;

void clearConsole() {
    system("cls");
}

int getValidatedInt(const string& prompt, int min = numeric_limits<int>::min(), int max = numeric_limits<int>::max()) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            clearConsole();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Intente nuevamente.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string getValidatedString(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty()) {
            return value;
        }
        cout << "Entrada invalida. Intente nuevamente.\n";
    }
}

bool validateEmail(const string& email) {
    const regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$");
    return regex_match(email, pattern);
}

bool validateDate(const string& date) {
    const regex pattern(R"(^(\d{2})/(\d{2})/(\d{4})$)");
    if (!regex_match(date, pattern)) {
        return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    if (year > 2024 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }
    return true;
}

int validateAge(const string& dateOfBirth) {
    if (!validateDate(dateOfBirth)) {
        return false;
    }

    int birthDay = stoi(dateOfBirth.substr(0, 2));
    int birthMonth = stoi(dateOfBirth.substr(3, 2));
    int birthYear = stoi(dateOfBirth.substr(6, 4));

    int currentDay = 18; 
    int currentMonth = 12;
    int currentYear = 2024;

    int age = currentYear - birthYear;

    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }

    return age;
}

int getValidatedCategory() {
    return getValidatedInt("Seleccione la categoria:\n1. Administrador\n2. Operario\n3. Peon\nOpcion: ", 1, 3);
}

void addEmployee() {
    clearConsole();
    Employee employee;

    while (true) {
        employee.idNumber = getValidatedString("Ingrese numero de carnet (solo numeros): ");
        bool validId = true;
        for (char c : employee.idNumber) {
            if (!isdigit(c)) {
                cout << "El carnet debe contener solo numeros. Intente nuevamente.\n";
                validId = false;
                break;
            }
        }
        if (validId) {
            bool uniqueId = true;
            for (const auto& [id, emp] : employees) {
                if (emp.idNumber == employee.idNumber) {
                    cout << "El carnet ya esta registrado. Intente con otro.\n";
                    uniqueId = false;
                    break;
                }
            }
            if (uniqueId) break;
        }
    }

    employee.name = getValidatedString("Ingrese nombre: ");

    while (true) {
        employee.dateOfBirth = getValidatedString("Ingrese fecha de nacimiento (DD/MM/YYYY): ");
        int age = validateAge(employee.dateOfBirth);

        if(age >= 18){
             employee.age = age;
             break;
        } 
        cout << "El empleado debe ser mayor de edad y la fecha debe estar en el formato correcto.\n";
    }

    int categoryOption;
    while (true) {
        categoryOption = getValidatedCategory();
        if (categoryOption >= 1 && categoryOption <= 3) break;
        cout << "Categoria invalida. Seleccione una opcion valida.\n";
    }
    switch (categoryOption) {
        case 1:
            employee.category = "Administrador";
            break;
        case 2:
            employee.category = "Operario";
            break;
        case 3:
            employee.category = "Peon";
            break;
    }

    while (true) {
        cout << "Ingrese salario (250000 - 500000, por defecto 250000): ";
        string salaryInput;
        getline(cin, salaryInput);
        if (salaryInput.empty()) {
            employee.salary = 250000;
            break;
        }
        try {
            employee.salary = stod(salaryInput);
            if (employee.salary >= 250000 && employee.salary <= 500000) break;
        } catch (...) {}
        cout << "El salario debe estar entre 250000 y 500000.\n";
    }

    cout << "Ingrese direccion (por defecto San Jose): ";
    getline(cin, employee.address);
    if (employee.address.empty()) {
        employee.address = "San Jose";
    }

    employee.phone = getValidatedString("Ingrese telefono: ");

    while (true) {
        employee.email = getValidatedString("Ingrese correo: ");
        if (validateEmail(employee.email)) {
            bool uniqueEmail = true;
            for (const auto& [id, emp] : employees) {
                if (emp.email == employee.email) {
                    cout << "El correo ya esta registrado. Intente con otro.\n";
                    uniqueEmail = false;
                    break;
                }
            }
            if (uniqueEmail) break;
        } else {
            cout << "Correo invalido. Intente nuevamente.\n";
        }
    }

    employees[employee.idNumber] = employee;
    cout << "Empleado agregado correctamente.\n";
}

void listEmployees() {
    clearConsole();
    if (employees.empty()) {
        cout << "No hay empleados registrados.\n";
        return;
    }
    for (const auto& [id, employee] : employees) {
        cout << "ID: " << employee.idNumber << ", Nombre: " << employee.name << ", Edad: " << employee.age
             << ", Categoria: " << employee.category
             << ", Salario: " << employee.salary
             << ", Direccion: " << employee.address
             << ", Correo: " << employee.email << "\n";
    }
}

void removeEmployee(const string& idNumber) {
    clearConsole();
    if (!employees.count(idNumber)) {
        cout << "Empleado no encontrado: " << idNumber << "\n";
        return;
    }
    employees.erase(idNumber);
    cout << "Empleado eliminado correctamente.\n";
}

void modifyEmployee() {
    clearConsole();
    string id = getValidatedString("Ingrese el numero de carnet del empleado a modificar: ");

    if (employees.find(id) == employees.end()) {
        cout << "Empleado no encontrado.\n";
        return;
    }

    Employee& employee = employees[id];
    int option;

    do {
        clearConsole();
        cout << "Modificando empleado: " << employee.name << " (" << employee.idNumber << ")\n";
        cout << "1. Modificar nombre\n";
        cout << "2. Modificar fecha de nacimiento\n";
        cout << "3. Modificar categoria\n";
        cout << "4. Modificar salario\n";
        cout << "5. Modificar direccion\n";
        cout << "6. Modificar telefono\n";
        cout << "7. Modificar correo\n";
        cout << "0. Volver al menu principal\n";
        option = getValidatedInt("Seleccione una opcion: ", 0, 7);

        switch (option) {
            case 1:
                employee.name = getValidatedString("Ingrese el nuevo nombre: ");
                break;
            case 2: {
                string newDate;
                while (true) {
                    newDate = getValidatedString("Ingrese la nueva fecha de nacimiento (DD/MM/YYYY): ");
                    if (validateAge(newDate)) {
                        employee.dateOfBirth = newDate;
                        break;
                    }
                    cout << "Fecha invalida. El empleado debe ser mayor de edad.\n";
                }
                break;
            }
            case 3: {
                int categoryOption = getValidatedCategory();
                switch (categoryOption) {
                    case 1:
                        employee.category = "Administrador";
                        break;
                    case 2:
                        employee.category = "Operario";
                        break;
                    case 3:
                        employee.category = "Peon";
                        break;
                }
                break;
            }
            case 4:
                while (true) {
                    cout << "Ingrese el nuevo salario (250000 - 500000): ";
                    string salaryInput;
                    getline(cin, salaryInput);
                    try {
                        double newSalary = stod(salaryInput);
                        if (newSalary >= 250000 && newSalary <= 500000) {
                            employee.salary = newSalary;
                            break;
                        }
                    } catch (...) {}
                    cout << "Salario invalido.\n";
                }
                break;
            case 5:
                employee.address = getValidatedString("Ingrese la nueva direccion: ");
                break;
            case 6:
                employee.phone = getValidatedString("Ingrese el nuevo telefono: ");
                break;
            case 7: {
                string newEmail;
                while (true) {
                    newEmail = getValidatedString("Ingrese el nuevo correo: ");
                    if (validateEmail(newEmail)) {
                        employee.email = newEmail;
                        break;
                    }
                    cout << "Correo invalido.\n";
                }
                break;
            }
            case 0:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
        if (option != 0) {
            cout << "\nModificacion realizada. Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
    } while (option != 0);
}

void addProject() {
    clearConsole();
    Project project;

    while (true) {
        project.projectCode = getValidatedString("Ingrese codigo del proyecto: ");
        if (projects.count(project.projectCode)) {
            cout << "Ya existe un proyecto con ese codigo. Intente nuevamente.\n";
            continue;
        }
        break;
    }

    while (true) {
        project.name = getValidatedString("Ingrese nombre del proyecto: ");
        bool nameExists = false;
        for (const auto& [code, proj] : projects) {
            if (proj.name == project.name) {
                cout << "Ya existe un proyecto con ese nombre. Intente nuevamente.\n";
                nameExists = true;
                break;
            }
        }
        if (!nameExists) break;
    }

    while (true) {
        project.startDate = getValidatedString("Ingrese fecha de inicio (DD/MM/YYYY): ");
        if (!validateDate(project.startDate)) {
            cout << "Formato de fecha invalido. Intente nuevamente.\n";
            continue;
        }
        break;
    }

    while (true) {
        project.endDate = getValidatedString("Ingrese fecha de fin (DD/MM/YYYY): ");
        if (!validateDate(project.endDate)) {
            cout << "Formato de fecha invalido. Intente nuevamente.\n";
            continue;
        }
        break;
    }

    projects[project.projectCode] = project;
    cout << "Proyecto agregado correctamente.\n";
}

void listProjects() {
    clearConsole();
    if (projects.empty()) {
        cout << "No hay proyectos registrados.\n";
        return;
    }
    for (const auto& [code, project] : projects) {
        cout << "Codigo: " << code << ", Nombre: " << project.name << "\n";
    }
}

void consultProject(const string& projectCode) {
    clearConsole();
    if (!projects.count(projectCode)) {
        cout << "Proyecto no encontrado.\n";
        return;
    }

    const auto& project = projects[projectCode];
    cout << "Codigo: " << project.projectCode << ", Nombre: " << project.name
         << ", Fecha de Inicio: " << project.startDate << ", Fecha de Fin: " << project.endDate << "\n";

    if (assignments[projectCode].empty()) {
        cout << "No hay empleados asignados a este proyecto.\n";
        return;
    }

    cout << "Empleados asignados a este proyecto:\n";
    for (const string& employeeID : assignments[projectCode]) {
        const auto& employee = employees[employeeID];
        cout << "ID: " << employee.idNumber << ", Nombre: " << employee.name << " Categoria: " << employee.category<< "\n";
    }
}

void assignEmployeeToProject() {
    clearConsole();
    string employeeID = getValidatedString("Ingrese ID del empleado: ");
    if (!employees.count(employeeID)) {
        cout << "Empleado no encontrado.\n";
        return;
    }

    string projectCode = getValidatedString("Ingrese codigo del proyecto: ");
    if (!projects.count(projectCode)) {
        cout << "Proyecto no encontrado.\n";
        return;
    }

    for (const string& assignedID : assignments[projectCode]) {
        if (assignedID == employeeID) {
            cout << "El empleado ya esta asignado a este proyecto.\n";
            return;
        }
    }

    assignments[projectCode].push_back(employeeID);
    cout << "Empleado asignado correctamente al proyecto.\n";
}

void listEmployeesInProject() {
    clearConsole();
    string projectCode = getValidatedString("Ingrese codigo del proyecto: ");
    if (!projects.count(projectCode)) {
        cout << "Proyecto no encontrado.\n";
        return;
    }

    if (assignments[projectCode].empty()) {
        cout << "No hay empleados asignados a este proyecto.\n";
        return;
    }

    cout << "Empleados asignados al proyecto " << projectCode << ":\n";
    for (const auto& employeeID : assignments[projectCode]) {
        const auto& employee = employees[employeeID];
        cout << "ID: " << employee.idNumber << ", Nombre: " << employee.name << " Categoria: " << employee.category<< "\n";
    }
}

void listProjectsOfEmployee() {
    clearConsole();
    string employeeID = getValidatedString("Ingrese ID del empleado: ");
    if (!employees.count(employeeID)) {
        cout << "Empleado no encontrado.\n";
        return;
    }

    cout << "Proyectos asignados al empleado " << employeeID << ":\n";
    bool hasProjects = false;
    for (const auto& [projectCode, employeeList] : assignments) {
        for (const string& assignedID : employeeList) {
            if (assignedID == employeeID) {
                const auto& project = projects[projectCode];
                cout << "Codigo: " << project.projectCode << ", Nombre: " << project.name << "\n";
                hasProjects = true;
            }
        }
    }

    if (!hasProjects) {
        cout << "No hay proyectos asignados a este empleado.\n";
    }
}

void menu() {
    int option;
    do {
        option = getValidatedInt(     
            "\n      Menu \n"
            "1. Agregar Empleado\n"
            "2. Eliminar Empleado\n"
            "3. Modificar Empleado\n"
            "4. Listar Empleados\n"
            "5. Agregar Proyecto\n"
            "6. Listar Proyectos\n"
            "7. Consultar Proyecto\n"
            "8. Asignar Empleado a Proyecto\n"
            "9. Listar Empleados en Proyecto\n"
            "10. Listar Proyectos de Empleado\n"
            "0. Salir\n"
            "Opcion: ", 0, 10);
        clearConsole();
        switch (option) {
        case 1:
            addEmployee();
            break;
        case 2: {
            string id = getValidatedString("Ingrese ID del Empleado a Eliminar: ");
            removeEmployee(id);
            break;
        }
        case 3: {
            modifyEmployee();
            break;
        }
        case 4:
            listEmployees();
            break;
        case 5:
            addProject();
            break;
        case 6:
            listProjects();
            break;
        case 7: {
            string projectCode = getValidatedString("Ingrese codigo del proyecto: ");
            consultProject(projectCode);
            break;
        }
        case 8:
            assignEmployeeToProject();
            break;
        case 9:
            listEmployeesInProject();
            break;
        case 10:
            listProjectsOfEmployee();
            break;
        case 0:
            cout << "Saliendo...\n";
            break;
        }
        if (option != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(); cin.get();
        }
    } while (option != 0);
}

int main() {
    menu();
    return 0;
}
