#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <cmath>

void Graph::addPerson(int ID , const Person &person){
    people.emplace_back(ID, person);
}

const Person* Graph::getPerson(int ID) const {
    for(int i = 0 ; i < people.size() ; i++){
        if (people[i].first == ID) return &(people[i].second);
    }
    throw std::runtime_error("Person with given id not found");
}
const std::vector<std::pair<int, Person>> &Graph::getPeople() const {
    return people;
}
std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    for (const auto& pair : graph.people) {
        os << "ID: " << pair.first << ", Name: " << pair.second.getName() << ", Age: " << pair.second.getAge()
        <<  ", Gender: " << pair.second.getGender() << ", Occupation: " << pair.second.getOccupation() << ", Friends: [";
        const auto& friends = pair.second.getListOfFriends();
        for (size_t i = 0; i < friends.size(); ++i) {
            os << friends[i];
            if (i != friends.size() - 1) {
                os << ", ";
            }
        }
        os << "]" << std::endl;
    }
    return os;
}

std::vector<int> Graph::suggestFriends(int person_id, int mode) const {
    const Person* person = getPerson(person_id);
    if (mode == 1 ) return suggestFriendsByCommonFriends( person );
    else if (mode == 2) return suggestFriendsByOccupation( person );
    else if (mode == 3 ) return suggestFriendsByAge( person ) ;
    else
    {
        std::cout<<"Invalid mode"<<std::endl;
        return{};
    }
}

std::vector<int> Graph::suggestFriendsByCommonFriends(const Person *person) const {
    std::vector<int> suggestedFriends;
    std::vector<int> currentFriends = person->getListOfFriends();
    for ( int i = 0 ; i < people.size() ; i++ )
        if (people[i].first != person->getId()&& std::find(currentFriends.begin(), currentFriends.end(), people[i].first) == currentFriends.end()) {
            std::vector<int> friendOfFriend = people[i].second.getListOfFriends();
            std::vector<int> commonFriends;
            std::set_intersection(person->getListOfFriends().begin(), person->getListOfFriends().end(),
                                  friendOfFriend.begin(), friendOfFriend.end(),
                                  std::back_inserter(commonFriends));

            if (!commonFriends.empty()) {
                suggestedFriends.push_back(people[i].first);
            }
        }
    return suggestedFriends;
}

std::vector<int> Graph::suggestFriendsByOccupation(const Person *person) const
{
    std::vector<int> suggestedFriends;
    std::string occupation = person->getOccupation();
    for(const auto & p : people)
    {
        if (person->getId() != p.first && occupation == p.second.getOccupation())
        {
            std::cout<< p.second.getOccupation() << std::endl;
            suggestedFriends.push_back(p.first);
        }
    }
    return suggestedFriends;
}

std::vector<int> Graph::suggestFriendsByAge(const Person *person) const {
    std::vector<int> suggestedFriends;
    std::vector<int> currentFriends = person->getListOfFriends();
    for ( int i = 0 ; i < people.size() ; i++ )
    {
        if (people[i].first != person->getId()&&std::find(currentFriends.begin(), currentFriends.end(), people[i].first) == currentFriends.end()) {
            int currentAge = people[i].second.getAge();
            if (std::abs(currentAge - person->getAge()) <= 3) {
                suggestedFriends.push_back(people[i].first);
            }
        }
    }
    return suggestedFriends;
}

int Graph::getSize() const{
    return people.size();
}

void Graph::degreeCentrality() const {
    for (const auto& person : people) {
        int id = person.first;
        const Person& currentPerson = person.second;
        int degree = currentPerson.getListOfFriends().size();
        std::cout << "Person " << id << ": Degree centrality : " << degree << std::endl;
    }
}

double Graph::clusteringCoefficient(int id) const {
    const Person* person = getPerson(id);
    const std::vector<int>& friends = person->getListOfFriends();
    int numFriends = friends.size();

    if (numFriends < 2) {
        std::cerr << "Error: Not enough friends to calculate clustering coefficient.\n";
        return 0.0;
    }

    // Count the number of connections between the person's friends
    int numConnections = 0;
    for (int i = 0; i < numFriends; ++i) {
        for (int j = i + 1; j < numFriends; ++j) {
            if (getPerson(friends[i])->isFriendWith(friends[j])) {
                ++numConnections;
            }
        }
    }
    double clusteringCoefficient = (2.0 * numConnections) / (numFriends * (numFriends - 1));
    return clusteringCoefficient;
}

std::vector<std::pair<int, Person>> Graph::getGraph() const {
    return people;
}

std::vector<std::vector<int>> Graph::girvanNewman(int iterations) const {
    std::vector<std::vector<int>> communities;
    Graph copyGraph = *this;

    for (int i = 0; i < iterations; i++) {
        double maxWeight = 0.0;
        std::vector<int> maxWeightEdges;

        for (const auto& pair : copyGraph.people) {
            int u = pair.first;
            for (const auto& friendID : pair.second.getListOfFriends()) {
                int v = friendID;
                double weight = edgeWeight(copyGraph, u, v);
                if (weight > maxWeight) {

                    maxWeight = weight;
                    maxWeightEdges = {u, v};
                }
            }
        }

        if (!maxWeightEdges.empty()) {
            copyGraph.removeFriendship(maxWeightEdges[0], maxWeightEdges[1]);
        }
    }

    int minID = copyGraph.people.begin()->first;
    int maxID = copyGraph.people.rbegin()->first;

    std::vector<int> community(maxID - minID + 1, 0);

    for (const auto& pair : copyGraph.people) {
        if (community[pair.first - minID] == 0) {
            std::vector<int> members;
            members.push_back(pair.first);
            community[pair.first - minID] = 1;

            for (const auto& friendID : pair.second.getListOfFriends()) {
                if (community[friendID - minID] == 0) {
                    members.push_back(friendID);
                    community[friendID - minID] = 1;
                }
            }

            communities.push_back(members);
        }
    }

    return communities;
}

double Graph::edgeWeight(const Graph& graph, int u, int v) {
    const Person* personU = graph.getPerson(u);
    const Person* personV = graph.getPerson(v);

    if (personU == nullptr || personV == nullptr) {
        return 0.0;
    }

    std::vector<int> commonFriends;
    std::vector<int> friendsU = personU->getListOfFriends();
    std::vector<int> friendsV = personV->getListOfFriends();

    std::set_intersection(friendsU.begin(), friendsU.end(),
                          friendsV.begin(), friendsV.end(),
                          std::back_inserter(commonFriends));

    return static_cast<double>(commonFriends.size());
}

void Graph::removeFriendship(int id1, int id2)
{
    Person* personOne = const_cast<Person*>(getPerson(id1));
    std::vector<int> listToUpdate1 = personOne->getListOfFriends();
    std::vector<int> emptyList1;
    std::vector<int> emptyList2;

    for (int i : listToUpdate1 )
    {
        if ( i != id2) emptyList1.emplace_back(i);
    }
    personOne->setListOfFriends(emptyList1);

    Person* personTwo = const_cast<Person*>(getPerson(id2));
    std::vector<int> listToUpdate2 = personTwo->getListOfFriends();
    for (int i : listToUpdate2 )
    {
        if ( i != id1) emptyList2.emplace_back(i);
    }
    personTwo->setListOfFriends(emptyList2);
}
