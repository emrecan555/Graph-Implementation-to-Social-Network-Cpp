#ifndef UNTITLED_GRAPH_H
#define UNTITLED_GRAPH_H
#include "Person.h"

class Graph {

private:
    std::vector<std::pair<int, Person>> people;

public:
    void removeFriendship(int id1, int id2);
    void addPerson(int ID , const Person &person);
    const Person* getPerson(int ID) const ;
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
    std::vector<int> suggestFriends(int person_id, int mode) const;
    std::vector<int> suggestFriendsByCommonFriends(const Person *person) const;
    std::vector<int> suggestFriendsByOccupation(const Person *person) const;
    std::vector<int> suggestFriendsByAge(const Person *person) const;
    int getSize() const;
    void degreeCentrality() const;
    double clusteringCoefficient(int id) const;
    std::vector<std::pair<int, Person>> getGraph() const;
    std::vector<std::vector<int>> girvanNewman(int iterations) const;
    static double edgeWeight(const Graph &graph, int u, int v);
    const std::vector<std::pair<int, Person>> &getPeople() const;
};


#endif //UNTITLED_GRAPH_H
