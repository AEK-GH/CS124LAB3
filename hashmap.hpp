#ifndef HASHMAP_HPP
#define HASHMAP_HPP
#include <utility>
#include <string>
#include <vector>


class hashmap
{
public:

	~hashmap();
    hashmap(); //constructor to set consts
	hashmap(std::vector<std::pair<char, std::string> > data);
	// constructor that takes in a vector of entries


    void insert(std::pair<char, std::string> data);
    // adds a new element to the hash map

    char find(std::string cipher) const;
    // finds the char that is represented by a given code

    int hasher(const std::string input) const;
	void resize_if_necessary();
    int bucket_count() const;
    int element_count() const;
    void print() const;
private:

	// Node for sheparate chaining
	struct hash_node
	{
		std::pair<char, std::string> data;
		//pair of char, and cipher string key-value pairs
		hash_node* next = nullptr;
		//hash node is a linked list element
	};


	const double load_factor;
	const int minimum_array_size;
	int current_array_size;
	int current_num_elements;
	const char nullchar;
	const int multiplier;
	hash_node** table;
};

#endif //HASHMAP_HPP
