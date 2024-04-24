// Datastructures.hh
//
// Student name: Nguyen The Long
// Student email: long.nguyen@tuni.fi
// Student number: 151317891
// AI tool used and the version: Copilot
// The purpose of the use: Support for the syntax of the code
// The part(s) where it was utilized: 
// all of the code (since we need to code them ourselves)
// However, the idea of the code is originally from my knowledge

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <map>
#include <memory>
#include <string>
#include <queue>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_set>
#include <climits>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
struct Connection;
// Type for a distance (in arbitrary units)
using Distance = int;

using Path = std::vector<Connection>;
using PathWithDist = std::vector<std::pair<Connection,Distance>>;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

struct Connection
{
    AffiliationID aff1 = NO_AFFILIATION;
    AffiliationID aff2 = NO_AFFILIATION;
    Weight weight = NO_WEIGHT;
    bool operator==(const Connection& c1) const{
        return (aff1==c1.aff1) && (aff2==c1.aff2) && (weight==c1.weight);
    }
};
const Connection NO_CONNECTION{NO_AFFILIATION,NO_AFFILIATION,NO_WEIGHT};


// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: The size() function of unordered_map is O(1)
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The clear() function for unordered_map has a time complexity of O(n), where n is the number of elements in the map.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map, so the time complexity is O(n).
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function of unordered_map and insert function is O(logn)
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function of unordered_map is O(logn)
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function of unordered_map is O(logn)
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map (O(n)).
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map (O(n)).
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The find() function of unordered_map is O(logn)
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function of unordered_map is O(logn)
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the vector, so the time complexity is O(n).
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map, so the time complexity is O(n).
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: The find() function for unordered_map has a time complexity of O(1).
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function for unordered_map has a time complexity of O(logn).
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the vector, so the time complexity is O(n). 
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function for unordered_map has a time complexity of O(logn).
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the vector, so the time complexity is O(n).
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: The find() function for unordered_map has a time complexity of O(logn).
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function iterates over all elements in the vector, so the time complexity is O(n).
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: The find() function for unordered_map has a time complexity of O(1).
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: The function iterates over all elements in the vector (O(n)) and sorts them (O(n log n)).
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map (O(n)) and sorts them (O(n log n)).
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map (O(n)) and sorts them (O(n log n)).
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: The function iterates over all elements in the unordered_map (O(n)) and sorts them (O(n log n)).
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: The erase() function for unordered_map has a time complexity of O(1).
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: The function iterates over all elements in two vectors, so the time complexity is O(n^2).
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(1)
    // Short rationale for estimate: he erase() function for unordered_map has a time complexity of O(1).
    bool remove_publication(PublicationID publicationid);

    // PRG 2 functions:

    // Estimate of performance: O(n)
    // Short rationale for estimate: The find function for unordered_map has a time complexity of O(logn).
    std::vector<Connection> get_connected_affiliations(AffiliationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: The find function for unordered_map has a time complexity of O(logn).
    std::vector<Connection> get_all_connections();

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: The recursion going through all the points in the worst case 
    Path get_any_path(AffiliationID source, AffiliationID target);

    // PRG2 optional functions

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Going through all the while and for loop, leading to O(n^2)
    Path get_path_with_least_affiliations(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_of_least_friction(AffiliationID source, AffiliationID target);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Going through 2 loops, leading to O(n^2) time complexity
    PathWithDist get_shortest_path(AffiliationID source, AffiliationID target);


private:
    PublicationID NO_PARENT = -1;
    struct Affiliation {
        Name name;
        Coord coord;
        std::vector<PublicationID> publications;
    };
    std::unordered_map<AffiliationID, std::shared_ptr<Affiliation>> affiliations;
    struct Publication {
        Name name;
        Year year;
        std::vector<AffiliationID> affiliations;
        std::vector<PublicationID> references;
        PublicationID parent;
    };
    std::unordered_map<PublicationID, std::shared_ptr<Publication>> publications;

    std::vector<AffiliationID> all_affiliations;

    std::vector<AffiliationID> all_affiliations_sorted_alphabetically;

    std::vector<AffiliationID> all_affiliations_distance_increasing;

    std::unordered_map<Coord, AffiliationID, CoordHash> all_affiliations_coord;

    struct DistanceComparator {
        bool operator()(Coord c1, Coord c2) const {
            if (c1.x * c1.x + c1.y * c1.y < c2.x * c2.x + c2.y * c2.y) {
                return true;
            }
            else if (c1.x * c1.x + c1.y * c1.y > c2.x * c2.x + c2.y * c2.y) {
                return false;
            }
            else {
                if (c1.y < c2.y) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    };
    std::map<Coord, AffiliationID, DistanceComparator> all_affiliations_distance;

    std::map<Name, AffiliationID> all_affiliations_name;
    
    bool affiliations_alphabetically_data_changed = false;
    bool affiliations_distance_increasing_data_changed = false;

    std::unordered_map<AffiliationID, std::vector<Connection>> all_connections_by_id;
    std::vector<Connection> all_connections;

    bool find_path(const AffiliationID source, const AffiliationID target, std::unordered_map<AffiliationID, bool>& visited, std::vector<Connection>& path);


};

#endif // DATASTRUCTURES_HH
