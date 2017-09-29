/**
 *  Basic hash table implementation
 *   Aaron S. Crandall - 2017 <acrandal@gmail.com>
 *
 */

#ifndef __HASH_H
#define __HASH_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;
/*
	private:
		void rehash();
		int hash_function(KEYTYPE key);
		
	public:
		bool insert(KEYTYPE key, VALTYPE val);
		bool contains(KEYTYPE key);
		int remove(KEYTYPE key);
		VALTYPE* find(KEYTYPE key);
		int size();            // Elements currently in table
		bool empty();          // Is the hash empty?
		float load_factor();   // Return current load factor
		void clear();          // Empty out the table
		int bucket_count();    // Total number of buckets in table
*/

template <typename KEYTYPE, typename VALTYPE>
class Hashtable
{
	private:
		vector<vector<VALTYPE>> _hash;
		/**
		 *  Rehash the table into a larger table when the load factor is too large
		 */
		void rehash() 
		{
			int newsize = find_next_prime(_hash.size()); // find next tablesize
			vector<VALTYPE> _temp;

			for (int i = 0; i < _hash.size(); i++) // put all elements into temp vector
			{
				for (int j = 0; j < _hash[i].size(); j++)
				{
					_temp.push_back(_hash[i][j]);
				}
			}
			_hash.clear(); // clear hashtable
			_hash.resize(newsize); // resize hash table
			for (int x = 0; x < _temp.size(); x++)
			{
				insert(_temp[x].myword, _temp[x]); // re insert elements
			}
		}

		/**
		 *  Function that takes the key (a string or int) and returns the hash key
		 *   This function needs to be implemented for several types it could be used with!
		 */
		int hash_function(int key) 
		{
			return (key * 37) % _hash.size(); // return key
		}

		int hash_function(string key)
		{
			int k = 0;
			for (int i = 0; i < key.length(); i++)
			{
				k = (37 * k) + key[i];
			}
			return k % _hash.size();
		}

		
	public:
		/**
		 *  Basic constructor
		 */
		Hashtable( int startingSize = 101 )
		{
			_hash.resize(startingSize);
			
		}

		/**
		 *  Add an element to the hash table
		 */
		bool insert(KEYTYPE key, VALTYPE val)
		{
			int index = this->hash_function(key); // get key
			int duplicate_index = 0;
			bool duplicate = false; 

			for (int i = 0; i < _hash[index].size(); i++) // check for duplicate
			{
				if (val.myword == _hash[index][i].myword)
				{
					duplicate_index = i;
					duplicate = true;
				}
			}
			if (duplicate) // replace duplicate
			{
				_hash[index][duplicate_index] = val;
			}
			else
			{
				_hash[index].push_back(val); // insert
			}
			if (load_factor() > 1) // rehash 
			{
				rehash();
			}
			return true;
		}

		/**
		 *  Return whether a given key is present in the hash table
		 */
		bool contains(KEYTYPE key) 
		{
			int index = this->hash_function(key); // get key

			for (int i = 0; i < _hash[index].size(); i++) //find element
			{
				if (key == _hash[index][i].myword)
				{
					return true;
				}
			}
			return false;
		}
		/**
		 *  Completely remove key from hash table
		 *   Returns number of elements removed
		 */
		int remove(KEYTYPE key) 
		{
			int index = this->hash_function(key); // get key
			int erase_index = -1;
			for (int i = 0; i < _hash[index].size(); i++) // find index
			{
				if (key == _hash[index][i].myword)
				{
					erase_index = i;
				}
			}
			if (erase_index >= 0)
			{
				_hash[index].erase(_hash[index].begin() + erase_index); // erase 
				return 1;
			}
			return 0;
		}

		/**
		 *  Searches the hash and returns a pointer
		 *   Pointer to Word if found, or nullptr if nothing matches
		 */
		VALTYPE* find(KEYTYPE key)
		{
			VALTYPE * ret = nullptr; // return pointer
			int index = this->hash_function(key); // get key

			for (int i = 0; i < _hash[index].size(); i++) // find and set pointer to word
			{
				if (key ==_hash[index][i].myword)
				{
					ret = new VALTYPE;
					*ret = _hash[index][i];
				}
			}
			return ret; // return pointer
		}

		/**
		 *  Return current number of elements in hash table
		 */
		int size() 
		{
			int size = 0;

			for (int i = 0; i < _hash.size(); i++) // count elements and return 
			{
				size += _hash[i].size();
			}
			return size;
			
		}

		/**
		 *  Return true if hash table is empty, false otherwise
		 */
		bool empty() // check if empty
		{
			
			if (this->size() == 0)
			{
				return true;
			}
			return false;
		}

		/**
		 *  Calculates the current load factor for the hash
		 */
		float load_factor() 
		{
			float load = (float)size() / (float)_hash.size(); // # of elements / tablesize

			return load;
		}

		/**
		 *  Returns current number of buckets (elements in vector)
		 */
		int bucket_count()
		{
			return _hash.size(); // return # of elements
		}

		/**
		 *  Deletes all elements in the hash
		 */
		void clear() 
		{
			_hash.clear(); // empties hashtable
		}
		void print(int index)
		{
			int count = 0;
			cout << endl;
			if (index == -1)
			{
				index = size();
			}
			for (int i = 0; i < _hash.size() && count < index; i++) // loop and print
			{
				for (int j = 0; j < _hash[i].size() && count < index; j++)
				{
					cout << _hash[i][j].myword << " : " << _hash[i][j].definition << endl;
					count++;
				}
			}
			cout << endl;
		}
		void random()
		{
			srand((unsigned)time(NULL));
			int randint = rand() % 101 + 1;
			while (_hash[randint].size() == 0)
			{
				randint = rand() % _hash.size() + 1;
			}
			cout << _hash[randint][0].myword << " : " << _hash[randint][0].definition << endl;
		}

		int find_next_prime(int tablesize)
		{
			int newnum = tablesize * 2; // double table size
			bool cont = true;
			while (cont == true)
			{
				for (int i = 2; i < newnum; i++) // check if it has dividers 
				{
					if (newnum % i == 0)
					{
						newnum++;
						cont = true; // if so then break and check next number
						break;
					}
					cont = false;
				}
				
			}
			return newnum; // return next prime
		}
};


#endif
