#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace clq
{

/**
 @brief  A simple data structure for a partition, where the position in the
 vector denotes the node_id and its value determines its set id.

 Good when you need constant time assignment of a node to a set and removal
 of a node from a set.
 */

class VectorPartition
{
private:
    int num_nodes;
    std::vector<int> partition_vector;
    bool is_normalised;

public:

    //#################### CONSTRUCTORS ####################
    // construct empty partition
    explicit VectorPartition (int num_nodes) :
        num_nodes (num_nodes),
        partition_vector (std::vector<int> (num_nodes, -1) ),
        is_normalised (false)
    {
    }

    // construct partition with initial_set
    explicit VectorPartition (int num_nodes, int initial_set) :
        num_nodes (num_nodes), partition_vector (std::vector<int> (num_nodes,
                initial_set) ), is_normalised (false)
    {
    }

    // construct partition from vector
    explicit VectorPartition (std::vector<int> partition) :
        num_nodes (partition.size() ), partition_vector (partition),
        is_normalised (false)
    {
    }

    // initialise partition as singleton -- with each node in its own group
    void initialise_as_singletons()
    {
        int i = 0;

        for (auto itr = partition_vector.begin(); itr != partition_vector.end(); ++itr) {
            *itr = i++; 
        }

        is_normalised = true;
    }

    // initialise as the global "all in one" partition
    void initialise_as_global()
    {
        partition_vector = std::vector<int> (partition_vector.size(), 0);
    }

    //#################### PUBLIC METHODS ####################
    // return community ID
    int find_set (int node_id) const
    {
        return partition_vector[node_id];
    }
    
    // Use to temporarily assign node to "void" community with index -1
    void isolate_node (int node_id)
    {
        partition_vector[node_id] = -1;
        is_normalised = false;
    }

    // move node to community with id set_id
    void add_node_to_set (int node_id, int set_id)
    {
        partition_vector[node_id] = set_id;
        is_normalised = false;
    }

    // count number of nodes
    int element_count() const
    {
        return partition_vector.size();
    }
	
    // count number of distinct communities in partition vector
    int set_count()
    {
        std::unordered_set<int> seen_nodes;
        seen_nodes.reserve(partition_vector.size());

        for (auto itr = partition_vector.begin(); itr != partition_vector.end(); ++itr) {
            if (*itr != -1) {
                seen_nodes.insert (*itr);
            }
        }

        return static_cast<int>(seen_nodes.size());
    }

    // return vector with all nodes that are part of community set_id
    std::vector<int> get_nodes_from_set (int set_id)
    {
        std::vector<int> nodes_in_set;

        for (int i = 0; i < num_nodes; ++i) {
            if (partition_vector[i] == set_id) {
                nodes_in_set.push_back (i);
            }
        }

        return nodes_in_set;
    }

    // return the internal vector of IDs
    std::vector<int> return_partition_vector()
    {
        return partition_vector;
    }

    // Normalise IDs in partition vector.
    // Modifies IDs such that they are contiguous and start at 0
    // e.g. 2,1,4,2 -> 0,1,2,0
    // Returns a vector mapping new IDs to previous IDs (index = new id).
    std::vector<int> normalise_ids()
    {
        if (!is_normalised) {
            int start_num = 0;
            std::unordered_map<int, int> set_old_to_new;
            set_old_to_new.reserve(partition_vector.size());
            std::vector<int> set_new_to_old;
            set_new_to_old.reserve(partition_vector.size());

            for (auto itr = partition_vector.begin(); itr != partition_vector.end(); ++itr) {
                auto found = set_old_to_new.find(*itr);
                if (found == set_old_to_new.end()) {
                    set_old_to_new.emplace(*itr, start_num);
                    set_new_to_old.push_back(*itr);
                    *itr = start_num;
                    ++start_num;
                } else {
                    *itr = found->second;
                }
            }

            is_normalised = true;
            return set_new_to_old;
        }

        // Already normalised: identity mapping, size = number of distinct ids.
        int max_id = -1;
        for (int v : partition_vector) {
            if (v > max_id) max_id = v;
        }
        std::vector<int> set_new_to_old(static_cast<std::size_t>(max_id + 1));
        for (int i = 0; i <= max_id; ++i) set_new_to_old[i] = i;
        return set_new_to_old;
    }


    //#################### Operators ####################
    bool operator== (const VectorPartition& other) const
    {
        if (this->is_normalised && other.is_normalised) {
            return (other.partition_vector == this->partition_vector);

        } else {
            VectorPartition a (*this);
            VectorPartition b (other);

            a.normalise_ids();
            b.normalise_ids();
            return a.partition_vector == b.partition_vector;
        }
    }

    bool operator!= (const VectorPartition& other) const
    {
        return ! (partition_vector == other.partition_vector);
    }
};

}
