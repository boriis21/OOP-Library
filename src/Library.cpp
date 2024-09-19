#include "Library.h"

Library& Library::getInstance() {
    static Library object;
    return object;
}


//COMMANDS
void Library::run() {
    std::cout << "### Welcome to the Library! ###\n\n";

    try {
        editions.readEdition("editions.dat");
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        users.readUsers("users.dat");
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    help();

    while (true) {
        char command[25];
        std::cout << "Enter a command: ";
        std::cin >> command;
        if (strcmp(command, "add-publication") == 0) {
            try {
                addEdition();
                editions.writeEdition("editions.dat");
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "remove-publication") == 0) {
            try {
                removeEdition();
                editions.writeEdition("editions.dat");
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "take-publication") == 0) {
            try {
                takeEdition();
                editions.writeEdition("editions.dat");
                users.writeUsers("users.dat");
            } catch (std::out_of_range& e) {
                std::cerr << e.what() << std::endl;
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "return-publication") == 0) {
            try {
                returnEdition();
                editions.writeEdition("editions.dat");
                users.writeUsers("users.dat");
            }  catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "print-publications") == 0) {
            try {
                printPublications();
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "add-user") == 0) {
            addUser();
            try {
                users.writeUsers("users.dat");
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "remove-user") == 0) {
            try {
                removeUser();
                users.writeUsers("users.dat");
            }  catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "print-who-took") == 0) {
            try {
                printUsersWhoTook();
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "print-users") == 0) {
            try {
                printAllUsers();
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(strcmp(command, "help") == 0)
            help();
        else if(strcmp(command, "exit") == 0)
            break;
        else
            std::cerr << "Invalid command!" << std::endl;
    }
}

//COMMANDS
void Library::addEdition() {
    char buffer[10];
    std::cout << "Enter the type of print edition you want to add to the library (book, magazine, comics): ";
    std::cin >> buffer;

    if (strcmp(buffer, "book") == 0) {
        Book book;
        std::cin >> book;
        try {
            editions.push(new Book(book));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else if (strcmp(buffer, "magazine") == 0) {
        Magazine magazine;
        std::cin >> magazine;
        try {
            editions.push(new Magazine(magazine));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else if (strcmp(buffer, "comics") == 0) {
        Comics comics;
        std::cin >> comics;
        try {
            editions.push(new Comics(comics));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else
        throw std::runtime_error("This type of printed edition is not supported!");
}

void Library::removeEdition() {
    unsigned libraryNumber;
    std::cout << "Enter the library number of the printed edition you want to remove: ";
    std::cin >> libraryNumber;
    size_t index = editions.findEditionIndexByNum(libraryNumber);
    if (index != -1) {
        if (editions.remove(libraryNumber)) {
            for (int i = 0; i < users.getSize(); ++i) {
                if (users[i].getTakenEditions().contains(libraryNumber))
                    users[i].removeFromTaken(editions[index]);
                else if (users[i].getReadEditions().contains(libraryNumber))
                    users[i].removeFromRead(editions[index]);
            }
            std::cout << editions[index].getTitle() << " was removed successfully!" << std::endl;
        }
    } else
        throw std::runtime_error("There is no printed edition with that library number!");
}

void Library::takeEdition() {
    char buffer[128];
    std::cout << "Enter the name of the user who wants to take a printed edition: ";
    std::cin.ignore(sizeof(buffer), '\n');
    std::cin.getline(buffer, sizeof(buffer));
    size_t userIndex = users.findUserIndexByName(buffer);

    unsigned libraryNumber;
    std::cout << "Enter the library number of the printed edition you want to take: ";
    std::cin >> libraryNumber;
    size_t editionIndex = editions.findEditionIndexByNum(libraryNumber);

    if (userIndex != -1) {
        if (users[userIndex].getTakenSize() < 5) {
            if (editionIndex != -1) {
                if (strcmp(users[userIndex][libraryNumber], "Currently taken.") != 0) {
                    if (strcmp(editions[editionIndex].getType(), "magazine") == 0 || strcmp(editions[editionIndex].getType(), "comics") == 0) {
                        if (editions[editionIndex].getCount() > 0) {
                            editions[editionIndex].setCount(editions[editionIndex].getCount() - 1);
                            users[userIndex] += editions[editionIndex];
                            std::cout << users[userIndex].getName() << " took " << editions[editionIndex].getTitle() << " successfully!" << std::endl;
                        } else {
                            throw std::out_of_range("Not enough copies of this printed editions to take.");
                        }
                    } else {
                        users[userIndex] += editions[editionIndex];
                        std::cout << users[userIndex].getName() << " took " << editions[editionIndex].getTitle() << " successfully!" << std::endl;
                    }
                } else
                    throw std::runtime_error("The user has already taken this printed edition!");
            } else
                throw std::runtime_error("There is no printed edition with that library number!");
        } else
            throw std::runtime_error("This user has already taken 5 printed editions.");
    } else
        throw std::out_of_range("There is no user with that name!");
}

void Library::returnEdition() {
    char buffer[128];
    std::cout << "Enter the name of the user who wants to return a printed edition: ";
    std::cin.ignore(sizeof(buffer), '\n');
    std::cin.getline(buffer, sizeof(buffer));
    size_t userIndex = users.findUserIndexByName(buffer);

    unsigned libraryNumber;
    std::cout << "Enter the library number of the printed edition you want to take: ";
    std::cin >> libraryNumber;
    size_t editionIndex = editions.findEditionIndexByNum(libraryNumber);

    if (userIndex != -1) {
        if (editionIndex != -1) {
            if (strcmp(users[userIndex][libraryNumber], "Currently taken.") == 0) {
                if (strcmp(editions[editionIndex].getType(), "magazine") == 0 || strcmp(editions[editionIndex].getType(), "comics") == 0) {
                    editions[editionIndex].setCount(editions[editionIndex].getCount() + 1);
                    users[userIndex] -= editions[editionIndex];
                    std::cout << users[userIndex].getName() << " returned " << editions[editionIndex].getTitle() << " successfully!" << std::endl;
                } else {
                    users[userIndex] -= editions[editionIndex];
                    std::cout << users[userIndex].getName() << " returned " << editions[editionIndex].getTitle() << " successfully!" << std::endl;
                }
            } else
                throw std::runtime_error("There is no printed edition with that library number!");
        }
        else
            throw std::runtime_error("The have not taken this printed edition.");
    }
    else
        throw std::runtime_error("There is no user with that name!");
}

void Library::printPublications() {
    if (editions.getSize() > 0) {
        EditionsArray sortedEditions = editions;

        /* for (size_t i = 0; i < sortedEditions.getSize() - 1; ++i) {
            for (size_t j = 0; j < sortedEditions.getSize() - i - 1; ++j) {
                if (sortedEditions[j] < sortedEditions[j + 1]) {
                    PrintEditions* temp = &sortedEditions[j];
                    sortedEditions[j] = sortedEditions[j + 1];
                    sortedEditions[j + 1] = *temp;
                }
            }
        } */

        for (int i = 0; i < sortedEditions.getSize(); ++i) {
            std::cout << "Title: " << sortedEditions[i].getTitle() << std::endl;
            std::cout << "Type: " << sortedEditions[i].getType() << std::endl;
            std::cout << "Description: " << sortedEditions[i].getDescription() << std::endl;
            if (strcmp(sortedEditions[i].getType(), "magazine") == 0 || strcmp(sortedEditions[i].getType(), "comics") == 0)
                std::cout << "Count: " << sortedEditions[i].getCount() << std::endl;
            std::cout << "Year: " << sortedEditions[i].getYear() << std::endl;
            std::cout << "Library number: " << sortedEditions[i].getLibraryNumber() << std::endl;
            std::cout << std::endl;
        }
    } else {
        throw std::runtime_error("There are no printed editions added yet!");
    }
}
void Library::addUser() {
    User user;
    std::cin >> user;
    users.push(user);
}

void Library::removeUser() {
    char buffer[128];
    std::cout << "Enter the name of the user you want to remove: ";
    std::cin.ignore(sizeof(buffer), '\n');
    std::cin.getline(buffer, sizeof(buffer));
    if (users[users.findUserIndexByName(buffer)].getTakenSize() == 0) {
        users.removeUser(buffer);
        std::cout << buffer << " was removed successfully!" << std::endl;
    }
    else
        throw std::runtime_error("This user has taken print. He should return them before he can be deleted!");
}

void Library::printUsersWhoTook() {
    bool flag = false;

    unsigned libraryNumber;
    std::cout << "Enter the library number of the printed edition that you want to check who currently holds it: ";
    std::cin >> libraryNumber;
    std::cout << "This printed edition is currently taken by: " << std::endl;

    for (int i = 0; i < users.getSize(); ++i) {
        for (int j = 0; j < users[i].getTakenSize(); ++j) {
            if (users[i].getTakenEditions()[j].getLibraryNumber() == libraryNumber) {
                flag = true;
                std::cout << users[i].getName() << std::endl;
            }
        }
    }

    if (!flag) {
        throw std::runtime_error("Nobody have taken this printed edition yet!");
    }
}

void Library::printAllUsers() {
    if (users.getSize() > 0) {
        UsersArray sortedUsers = users;

        for (size_t i = 0; i < sortedUsers.getSize() - 1; ++i) {
            for (size_t j = 0; j < sortedUsers.getSize() - i - 1; ++j) {
                if (sortedUsers[j] < sortedUsers[j + 1]) {
                    User temp = sortedUsers[j];
                    sortedUsers[j] = sortedUsers[j + 1];
                    sortedUsers[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < sortedUsers.getSize(); ++i) {
            std::cout << sortedUsers[i] << std::endl;
        }
    } else
        throw std::runtime_error("There are no users added yet!");
}

void Library::help() {
    std::cout << "Here is a list of the commands you can use: \n"
                 "add-publication, remove-publication, take-publication, return-publication, \n"
                 "print-publications, add-user, remove-user, print-users \n"
                 "print-who-took, help, exit.\n\n";
}


//HELPER
bool Library::compareEditions(const PrintEditions* lhs, const PrintEditions* rhs) {
    if (lhs->getYear() != rhs->getYear())
        return lhs->getYear() < rhs->getYear();

    if (strcmp(lhs->getTitle(), rhs->getTitle()) != 0)
        return strcmp(lhs->getTitle(), rhs->getTitle()) < 0;

    if (strcmp(lhs->getType(), "magazine") == 0 && strcmp(rhs->getType(), "magazine") == 0) {
        if (strcmp(lhs->getTitle(), rhs->getTitle()) == 0) {
            return lhs->getCount() < rhs->getCount();
        }
    }

    if (strcmp(lhs->getType(), "comics") == 0 && strcmp(rhs->getType(), "comics") == 0) {
        if (strcmp(lhs->getTitle(), rhs->getTitle()) == 0) {
            return lhs->getCount() < rhs->getCount();
        }
    }

    return false;
}