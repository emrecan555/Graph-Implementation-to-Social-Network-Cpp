#include <iostream>
#include "Graph.h"
#include "utils.h"
#include <vector>
#include <iostream>


void displayNetwork(const Graph& graph) {
    std::cout << graph << std::endl;
}

void suggestFriends(const Graph& graph) {
    int personID, mode;
    std::cout << "Enter person ID: ";
    std::cin >> personID;
    std::cout << "Enter mode: \n1.Common Friends \n2.Similar Occupation\n3.Similar Age ";
    std::cin >> mode;

    std::vector<int> suggestedFriends = graph.suggestFriends(personID, mode);

    if (suggestedFriends.empty()) {
        std::cout << "No suggested friends." << std::endl;
    } else {
        std::cout << "Suggested friends: ";
        for (int friendID : suggestedFriends) {
            std::cout << friendID << " ";
        }
        std::cout << std::endl;
    }
}

void calculateDegreeCentrality(const Graph& graph) {
    graph.degreeCentrality();
}

void calculateClusteringCoefficient(const Graph& graph) {
    for (const auto& person : graph.getPeople()) {
        int id = person.first;
        double coefficient = graph.clusteringCoefficient(id);
        std::cout << "Person " << id << ": Clustering coefficient: " << coefficient << std::endl;
    }
}

void performGirvanNewman(const Graph& graph) {
    int iterations;
    std::cout << "Enter number of iterations: ";
    std::cin >> iterations;

    std::vector<std::vector<int>> communities = graph.girvanNewman(iterations);

    std::cout << "Communities: " << std::endl;
    for (const auto& community : communities) {
        std::cout << "Community: ";
        for (int member : community) {
            std::cout << member << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    Graph socialNetwork = readData("C:\\Users\\90537\\Desktop\\untitled\\social_network.csv");
    int choice;
    do {
        std::cout << "Social Network Options" << std::endl;
        std::cout << "1. Display network" << std::endl;
        std::cout << "2. Suggest friends for users based on common friends, similar occupation, or similar age" << std::endl;
        std::cout << "3. Calculate degree centrality for each user" << std::endl;
        std::cout << "4. Calculate clustering coefficient for each user" << std::endl;
        std::cout << "5. Detect communities using the Girvan-Newman algorithm" << std::endl;
        std::cout << "6. Exit the program" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                displayNetwork(socialNetwork);
            break;
            case 2:
                suggestFriends(socialNetwork);
            break;
            case 3:
                calculateDegreeCentrality(socialNetwork);
            break;
            case 4:
                calculateClusteringCoefficient(socialNetwork);
            break;
            case 5:
                performGirvanNewman(socialNetwork);
            break;
            case 6:
                std::cout << "Exiting..." << std::endl;
            break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    } while (choice != 6);



    return 0;


}

