// Datastructures.cc
//
// Student name: Nguyen The Long
// Student email: long.nguyen@tuni.fi
// Student number: 151317891
// AI tool used and the version: Copilot, ChatGPT
// The purpose of the use: Support for the syntax of the code, support for the pseudocode of the algorithm
// The part(s) where it was utilized: 
// Copilot: all of the code (since we need to implement all of the methods)
// ChatGPT: the pseudocode of the algorithm for the method get_shortest_path()

#include "datastructures.hh"

#include <queue>
#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations.size();
}

void Datastructures::clear_all()
{
    affiliations.clear();
    publications.clear();
    all_affiliations.clear();
    all_affiliations_sorted_alphabetically.clear();
    all_affiliations_distance_increasing.clear();
    all_affiliations_coord.clear();
    affiliations_alphabetically_data_changed = false;
    affiliations_distance_increasing_data_changed = false;
    all_affiliations_distance.clear();
    all_affiliations_name.clear();

    // Project 2 Features
    all_connections.clear();
    all_connections_by_id.clear();

}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    return all_affiliations;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    if (affiliations.find(id) != affiliations.end()) {
        return false;
    }


    affiliations[id] = std::make_shared<Affiliation>(Affiliation{name, xy, std::vector<PublicationID>()});
    all_affiliations.push_back(id);

    // Handle the method get_affiliations_distance_increasing()
    all_affiliations_distance.insert(std::make_pair(xy, id));
    affiliations_distance_increasing_data_changed = true;

    // Handle the method get_affiliations_alphabetically()
    all_affiliations_name.insert(std::make_pair(name, id));
    affiliations_alphabetically_data_changed = true;

    // Handle the method get_affiliations_closest_to()
    all_affiliations_coord.insert(std::make_pair(xy, id));

    // Project 2 Databases
    all_connections_by_id[id] = std::vector<Connection>();


    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto it = affiliations.find(id);
    if (it == affiliations.end()) {
        return NO_NAME;
    }
    return it->second->name;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = affiliations.find(id);
    if (it == affiliations.end()) {
        return NO_COORD;
    }
    return it->second->coord;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    if (affiliations_alphabetically_data_changed) {
        all_affiliations_sorted_alphabetically.clear();
        for (auto affiliation_name : all_affiliations_name) {
            all_affiliations_sorted_alphabetically.push_back(affiliation_name.second);
        }
        affiliations_alphabetically_data_changed = false;
    }
    return all_affiliations_sorted_alphabetically;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{   
    if (affiliations_distance_increasing_data_changed) {
        all_affiliations_distance_increasing.clear();

        for (auto affiliation_distance : all_affiliations_distance) {
            all_affiliations_distance_increasing.push_back(affiliation_distance.second);
        }
        affiliations_distance_increasing_data_changed = false;
    }

    return all_affiliations_distance_increasing;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto it = all_affiliations_coord.find(xy);
    if (it != all_affiliations_coord.end()) {
        return it->second;
    }

    return NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    affiliations_alphabetically_data_changed = true;
    affiliations_distance_increasing_data_changed = true;
    auto it = affiliations.find(id);
    if (it == affiliations.end()) {
        return false;
    }
    affiliations[id]->coord = newcoord;

    // Update the all_affiliations_coord
    for (auto& affiliation_coord : all_affiliations_coord) {
        if (affiliation_coord.second == id) {
            all_affiliations_coord.erase(affiliation_coord.first);
            all_affiliations_coord.insert(std::make_pair(newcoord, id));
            break;
        }
    }

    // Update the all_affiliations_distance
    for (auto& affiliation_distance : all_affiliations_distance) {
        if (affiliation_distance.second == id) {
            all_affiliations_distance.erase(affiliation_distance.first);
            all_affiliations_distance.insert(std::make_pair(newcoord, id));
            break;
        }
    }

    return true;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    if (publications.find(id) != publications.end()) {
        return false;
    }

    publications[id] = std::make_shared<Publication>(Publication{name, year, affiliations, std::vector<PublicationID>(), NO_PARENT});


    for (auto affiliation : affiliations) {
        this->affiliations[affiliation]->publications.push_back(id);
    }

    for (auto affiliation1 : publications[id]->affiliations) {
        for (auto affiliation2 : publications[id]->affiliations) {
            if (affiliation1 == affiliation2) {
                break;
            }

            bool connection_existed = false;
            if (all_connections_by_id.find(affiliation1) != all_connections_by_id.end()) {
                for (auto& connection : all_connections_by_id[affiliation1]) {
                    if (connection.aff1 == affiliation1 && connection.aff2 == affiliation2) {
                        connection.weight++;
                        // update weight in all_connections
                        for (auto& connection_all : all_connections) {
                            if (connection_all.aff1 == affiliation1 && connection_all.aff2 == affiliation2) {
                                connection_all.weight++;
                                break;
                            }
                        }
                        connection_existed = true;
                        break;
                    }
                }
                if (!connection_existed) {
                    all_connections_by_id[affiliation1].push_back(Connection{affiliation1, affiliation2, 1});
                    all_connections_by_id[affiliation2].push_back(Connection{affiliation2, affiliation1, 1});
                    if (affiliation1 < affiliation2) {
                        all_connections.push_back(Connection{affiliation1, affiliation2, 1});
                    }
                    else {
                        all_connections.push_back(Connection{affiliation2, affiliation1, 1});
                    }
                }
            }
            if (connection_existed) {
                auto it_reverse = all_connections_by_id.find(affiliation2);
                if (it_reverse != all_connections_by_id.end()) {
                    for (auto& connection : it_reverse->second) {
                        if (connection.aff1 == affiliation2 && connection.aff2 == affiliation1) {
                            connection.weight++;
                            // update weight in all_connections
                            for (auto& connection_all : all_connections) {
                                if (connection_all.aff1 == affiliation2 && connection_all.aff2 == affiliation1) {
                                    connection_all.weight++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    }


    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> all_publications;
    for (auto publication : publications) {
        all_publications.push_back(publication.first);
    }
    return all_publications;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = publications.find(id);
    if (it == publications.end()) {
        return NO_NAME;
    }
    return it->second->name;

}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto it = publications.find(id);
    if (it == publications.end()) {
        return NO_YEAR;
    }
    return it->second->year;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    if (publications.find(id) == publications.end()) {
        return {NO_AFFILIATION};
    }
    return publications[id]->affiliations;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    if (publications.find(id) == publications.end() || publications.find(parentid) == publications.end()) {
        return false;
    }

    // Add reference
    publications[id]->parent = parentid;
    return true;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    if (publications.find(id) == publications.end()) {
        return {NO_PUBLICATION};
    }
    std::vector<PublicationID> direct_references;

    for (auto publication : publications) {
        if (publication.second->parent == id) {
            direct_references.push_back(publication.first);
        }
    }

    std::sort(direct_references.begin(), direct_references.end(), [](PublicationID id1, PublicationID id2) {
        return id1 < id2;
    });

    return direct_references;

}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    if (affiliations.find(affiliationid) == affiliations.end() || publications.find(publicationid) == publications.end()) {
        return false;
    }

    // Add affiliation to publication
    publications[publicationid]->affiliations.push_back(affiliationid);
    affiliations[affiliationid]->publications.push_back(publicationid);

    // Project 2 Features    
    for (auto affiliation1 : publications[publicationid]->affiliations) {
        AffiliationID affiliation2 = affiliationid;

        if (affiliation1 == affiliation2) {
            break;
        }
        bool connection_existed = false;
        if (all_connections_by_id.find(affiliation1) != all_connections_by_id.end()) {
            for (auto& connection : all_connections_by_id[affiliation1]) {
                if (connection.aff1 == affiliation1 && connection.aff2 == affiliationid) {
                    connection.weight++;
                    // update weight in all_connections
                    for (auto& connection_all : all_connections) {
                        if (connection_all.aff1 == affiliation1 && connection_all.aff2 == affiliationid) {
                            connection_all.weight++;
                            break;
                        }
                    }
                    connection_existed = true;
                    break;
                }
            }
            if (!connection_existed) {
                all_connections_by_id[affiliation1].push_back(Connection{affiliation1, affiliationid, 1});
                all_connections_by_id[affiliationid].push_back(Connection{affiliationid, affiliation1, 1});
                if (affiliation1 < affiliationid) {
                    all_connections.push_back(Connection{affiliation1, affiliationid, 1});
                }
                else {
                    all_connections.push_back(Connection{affiliationid, affiliation1, 1});
                }
            }
        }
        if (connection_existed) {
            auto it_reverse = all_connections_by_id.find(affiliationid);
            if (it_reverse != all_connections_by_id.end()) {
                for (auto& connection : it_reverse->second) {
                    if (connection.aff1 == affiliationid && connection.aff2 == affiliation1) {
                        connection.weight++;
                        // update weight in all_connections
                        for (auto& connection_all : all_connections) {
                            if (connection_all.aff1 == affiliationid && connection_all.aff2 == affiliation1) {
                                connection_all.weight++;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    
    auto it = std::find_if(affiliations.begin(), affiliations.end(), [id](std::pair<AffiliationID, std::shared_ptr<Affiliation>> affiliation) {
        return affiliation.first == id;
    });

    if (it == affiliations.end()) {
        return {NO_PUBLICATION};
    }

    return it->second->publications;
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    if (publications.find(id) == publications.end()) {
        return NO_PUBLICATION;
    }
    return publications[id]->parent;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{

    std::vector<std::pair<Year, PublicationID>> publications_after;
    for (auto publication : affiliations[affiliationid]->publications) {
        if (publications[publication]->year >= year) {
            publications_after.push_back(std::make_pair(publications[publication]->year, publication));
        }
    }

    std::sort(publications_after.begin(), publications_after.end(), [](std::pair<Year, PublicationID> pair1, std::pair<Year, PublicationID> pair2) {
        if (pair1.first == pair2.first) {
            return pair1.second < pair2.second;
        }
        return pair1.first < pair2.first;
    });

    return publications_after;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> referenced_by_chain;
    PublicationID current_id = id;

    if (publications.find(id) == publications.end()) {
        return {NO_PUBLICATION};
    }

    while (current_id != NO_PARENT) {
        if (current_id != id) {
            referenced_by_chain.push_back(current_id);
        }
        if (publications.find(current_id) != publications.end()) {
            current_id = publications[current_id]->parent;
        }
        else {
            break;
        }
    }

    std::sort(referenced_by_chain.begin(), referenced_by_chain.end(), [](PublicationID id1, PublicationID id2) {
        return id1 < id2;
    });

    return referenced_by_chain;
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    std::vector<PublicationID> all_references;

    if (publications.find(id) == publications.end()) {
        return {NO_PUBLICATION};
    }

    std::vector<PublicationID> direct_references = get_direct_references(id);
    for (auto direct_reference : direct_references) {
        all_references.push_back(direct_reference);
        std::vector<PublicationID> indirect_references = get_all_references(direct_reference);
        all_references.insert(all_references.end(), indirect_references.begin(), indirect_references.end());
    }

    std::sort(all_references.begin(), all_references.end(), [](PublicationID id1, PublicationID id2) {
        return id1 < id2;
    });

    return all_references;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    std::vector<AffiliationID> all_affiliations;
    for (auto affiliation : affiliations) {
        all_affiliations.push_back(affiliation.first);
    }

    std::sort(all_affiliations.begin(), all_affiliations.end(), [this, xy](AffiliationID id1, AffiliationID id2) {
        return sqrt(pow(affiliations[id1]->coord.x - xy.x, 2) + pow(affiliations[id1]->coord.y - xy.y, 2)) < sqrt(pow(affiliations[id2]->coord.x - xy.x, 2) + pow(affiliations[id2]->coord.y - xy.y, 2));
    });


    // return three first coordinates only
    if (all_affiliations.size() > 3) {
        return {all_affiliations[0], all_affiliations[1], all_affiliations[2]};
    }
    return all_affiliations;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    if (affiliations.find(id) == affiliations.end()) {
        return false;
    }

    // Remove affiliation
    affiliations.erase(id);
    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    // handle the test non-existing publications
    if (publications.find(id1) == publications.end() || publications.find(id2) == publications.end()) {
        return NO_PUBLICATION;
    }
    
    if (publications[id1]->parent == publications[id2]->parent) {
        return publications[id1]->parent;
    }

    std::vector<PublicationID> id1_chain = get_referenced_by_chain(id1);
    std::vector<PublicationID> id2_chain = get_referenced_by_chain(id2);
     for (auto id1_chain_id : id1_chain) {
        for (auto id2_chain_id : id2_chain) {
            if (id1_chain_id == id2_chain_id) {
                return id1_chain_id;
            }
        }
    }
    return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    if (publications.find(publicationid) == publications.end()) {
        return false;
    }

    // Remove publication from affiliations
    for (auto affiliation : affiliations) {
        affiliation.second->publications.erase(std::remove(affiliation.second->publications.begin(), affiliation.second->publications.end(), publicationid), affiliation.second->publications.end());
    }

    // Remove publication from publications
    publications.erase(publicationid);

    // Check if the parent of the other publications is the publicationid
    for (auto publication : publications) {
        if (publication.second->parent == publicationid) {
            publication.second->parent = NO_PARENT;
        }
    }
    return true;
}

std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
    if (affiliations.find(id) == affiliations.end()) {
        return {};
    }

    if (all_connections_by_id.find(id) != all_connections_by_id.end()) {
        return all_connections_by_id[id];
    }

    return {};
}
std::vector<Connection> Datastructures::get_all_connections()
{
    return all_connections;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    if (affiliations.find(source) == affiliations.end() || affiliations.find(target) == affiliations.end()) {
        return {};
    }

    std::vector<Connection> path;
    std::unordered_map<AffiliationID, bool> visited;

    if (find_path(source, target, visited, path)) {
            return path;
    } else {

        return {};
    }
}


Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{
    if (affiliations.find(source) == affiliations.end() || affiliations.find(target) == affiliations.end()) {
        return {};
    }

    std::unordered_set<AffiliationID> affiliations;
    std::queue<Path> paths;
    paths.push({});

    while (!paths.empty()) {
        Path path = paths.front();
        paths.pop();

        AffiliationID last_affiliation = path.empty() ? source : path.back().aff2;

        if (last_affiliation == target) {
            return path;
        }

        for (auto& connection : all_connections_by_id[last_affiliation]) {
            if (affiliations.find(connection.aff2) == affiliations.end()) {
                Path new_path = path;
                new_path.push_back(connection);
                paths.push(new_path);
                affiliations.insert(connection.aff2);
            }
        }
    }

    return {};

}

Path Datastructures::get_path_of_least_friction(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_of_least_friction()");
}

PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target)
{
    
    if (affiliations.find(source) == affiliations.end() || affiliations.find(target) == affiliations.end()) {
        return {};
    }
    std::priority_queue<std::pair<int, AffiliationID>, std::vector<std::pair<int, AffiliationID>>, std::greater<std::pair<int, AffiliationID>>> pq;
    std::unordered_map<AffiliationID, int> dist;
    std::unordered_map<AffiliationID, bool> visited;
    std::unordered_map<AffiliationID, Connection> prev;

    for (auto &id : all_connections_by_id) {
        dist[id.first] = INT_MAX;
        visited[id.first] = false;
    }

    dist[source] = 0;
    pq.push(std::make_pair(0, source));

    while (!pq.empty()) {
        AffiliationID u = pq.top().second;
        pq.pop();
        int distance = pq.top().first;

        if (u == target) {
            PathWithDist path_with_dist;
            AffiliationID current_id = target;
            while (current_id != source) {
                path_with_dist.push_back(std::make_pair(prev[current_id], dist[current_id]));
                current_id = prev[current_id].aff1;
            }

            for (auto& path : path_with_dist) {
                path.second = sqrt(pow(affiliations[path.first.aff1]->coord.x - affiliations[path.first.aff2]->coord.x, 2) + pow(affiliations[path.first.aff1]->coord.y - affiliations[path.first.aff2]->coord.y, 2));
            }

            std::reverse(path_with_dist.begin(), path_with_dist.end());
            return path_with_dist;
        }

        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        for (auto &connection : all_connections_by_id[u]) {
            if (visited[connection.aff2]) {
                continue;
            }

            AffiliationID v = connection.aff2;
            
            int weight = sqrt(pow(affiliations[u]->coord.x - affiliations[v]->coord.x, 2) + pow(affiliations[u]->coord.y - affiliations[v]->coord.y, 2));

            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                prev[v] = connection;
                pq.push(std::make_pair(dist[v], v));
            }
        }
    }

    return {};
}

bool Datastructures::find_path(const AffiliationID source, const AffiliationID target, std::unordered_map<AffiliationID, bool> &visited, std::vector<Connection> &path)
{
    visited[source] = true;

    for (auto& connection : all_connections_by_id[source]) {
        if (!visited[connection.aff2]) {
            path.push_back(connection);
            find_path(connection.aff2, target, visited, path);
            if (path.back().aff2 == target) {
                return true;
            }
            path.pop_back();
        }
    }

    return false;
}


