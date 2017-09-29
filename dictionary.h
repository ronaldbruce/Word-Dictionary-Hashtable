#ifndef __DICT_H
#define __DICT_H
#include "hashtable.h"
#include "word.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

class Dictionary
{
	private:
		Hashtable<string, Word> _dict;  // Primary dictionary store

	int parseline( string line ) 
	{
		string content1; // holds words filenames etc
		string content2; // holds definitions
		for (int i = 0; i < line.length(); i++)
		{
			line[i] = tolower(line[i]); // convert to lower case
		}
		if (line.find("help") != -1) // help
		{
			help();
		}
		else if (line.find("add") != -1) // add word def
		{
			parse_spaces(line, content1, content2);
			add(content1, content2);
		}
		else if (line.find("remove") != -1) // remove word def
		{
			parse_spaces(line, content1);
			del(content1);
		}
		else if (line.find("define") != -1) // finde word and print def
		{
			parse_spaces(line, content1);
			define(content1);
		}
		else if (line.find("unload") != -1)// delete words in file from dict
		{
			parse_spaces(line, content1);
			clearfile(content1);
		}
		else if (line.find("load") != -1) // load file 
		{
			parse_spaces(line, content1);
			loadfile(content1);
		}
		else if (line.find("size") != -1) // print size
		{
			cout << "size: " << _dict.size() << endl;
		}
		else if (line.find("clear") != -1) // clear dict
		{
			_dict.clear();
		}
		else if (line.find("print") != -1) // print dict
		{
			parse_spaces(line, content1);
			if (content1 != "")
			{
				print(stoi(content1)); // prints # of elements passed
			}
			else
			{
				print(-1); // prints all elements
			}
			
		}
		else if (line.find("random") != -1)
		{
			_dict.random(); // prints random word(not very random but sorta random)
		}
		else if (line.find("quit") != -1 || line.find("exit") != -1)
		{
			return 0; // to break loop
		}
		return 1; 
	}
	void help() // prints options
	{
		cout << "add \"word\" \"definition\"->Add(or update!) a word and it’s definition.Must handle quotes "<< endl;
		cout << "remove \"word\" -> Remove a given word. Must handle quotes" << endl;
		cout << "define \"word\" -> Define a word by printing out it’s definition or \"unknown word\"" << endl;
		cout << "load \"filename\" -> Load in a JSON file of dictionary words" << endl;
		cout << "unload \"filename\" -> Remove words from a given JSON file of dictionary words" << endl;
		cout << "size -> Print out current number of words in the dictionary" << endl;
		cout << "clear -> Remove ALL words from the dictionary" << endl;
		cout << "print [#words] -> Print out all words, unless user gives a maximum number" << endl;
		cout << "random -> Print out a single word chosen randomly from the dictionary" << endl;
		cout << "quit -> Quit the user interface and shut down" << endl;
	}
	void loadfile(string filename) 
	{
		ifstream myfile(filename); // opens filestream
		string line;
		while (getline(myfile, line)) // gets lines
		{
			parsefileline(line,false); // parses and inserts
		}
	}
	void clearfile(string filename)
	{
		ifstream myfile(filename); // opens stream		
		string line;
		while (getline(myfile, line)) // gets lines
		{
			parsefileline(line, true); //parses and deletes
		}
	}
	void parsefileline(string line, bool delete_word)
	{
		int word_start_index = -1;
		int word_length = -1;
		int def_start_index = -1;
		int def_length = line.find_last_of('"'); // gets indexes of quotes
		int quote_count = 1;

		if (line.find("word") == -1) // return if word is not in line
		{
			return;
		}
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '\"')
			{
				quote_count++; // look for quotation marks
			}
			if (quote_count == 3)
			{
				word_start_index = i; // find start index of word
			}
			if (quote_count == 4)
			{
				word_length = i; // find end index of word
			}
			if (quote_count == 7)
			{
				def_start_index = i; // find start index of def
			}
		}
		if (delete_word == true && word_start_index != -1 && word_length != -1)
		{
			word_length = (word_length - word_start_index) - 1; // adjust to remove quotes 'n shit
			word_start_index += 2;

			string word = line.substr(word_start_index, word_length); // take desired substrings of line
			del(word);
		}
		else if (word_start_index != -1 && word_length != -1 && def_start_index != -1) // check for index not found 
		{

			word_length = (word_length - word_start_index) - 1; // adjust to remove quotes 'n shit
			def_length = (def_length - def_start_index) - 2;
			word_start_index += 2;
			def_start_index += 2;

			string word = line.substr(word_start_index, word_length); // take desired substrings of line
			string def = line.substr(def_start_index, def_length);
			add(word, def);
		}
	}
	void define(string word)
	{
		
		for (int i = 0; i < word.length(); i++)
		{
			word[i] = toupper(word[i]); // converts to uppercase
		}
		Word * x = _dict.find(word); // find word
		if (x != nullptr)
		{
			cout << endl << x->definition << endl; // print word
		}
		else
		{
			cout << "could not find word" << endl;
		}
		
	}
	void add(string word, string def)
	{
		Word *x = new Word;
		for (int i = 0; i < word.length(); i++) // convert to uppercase
		{
			word[i] = toupper(word[i]);
		}
		x->myword = word; // set new word
		x->definition = def;
		_dict.insert(word, *x); // insert new word
	}
	void del(string word)
	{
		for (int i = 0; i < word.length(); i++) // to upper
		{
			word[i] = toupper(word[i]);
		}
		int success = _dict.remove(word); // remove word

		if (success == 1)
		{
			cout << endl << "successful delete" << endl; // print if word is found and deleted
		}
	}
	void parse_spaces(string line, string &content1, string &content2)
	{
		int index_of_space1 = line.find_first_of(' '); // get indexes of spaces
		int index_of_space2 = -1;
		int space_count = 0;
		line.erase(remove(line.begin(), line.end(), '\"'), line.end()); // strips quotes

		for (int i = index_of_space1 + 1; i < line.length(); i++) // find indexes
		{
			if (line[i] == ' ')
			{
				index_of_space2 = i;
				break;
			}
		}
		content1 = line.substr(index_of_space1, index_of_space2 - index_of_space1); // split up lines
		content1.erase(remove(content1.begin(), content1.end(), ' '), content1.end());
		content2 = line.substr(index_of_space2, line.size());
	}
	void parse_spaces(string line, string &content1)
	{
		int index_of_space1 = line.find_first_of(' '); // same thing but only one set of content
		line.erase(remove(line.begin(), line.end(), '\"'), line.end());
		if (index_of_space1 != -1)
		{
			content1 = line.substr(index_of_space1, line.size());
			content1.erase(remove(content1.begin(), content1.end(), ' '), content1.end());
		}
		else
		{
			content1 = "";
		}
		
		
	}
	void print(int size)
	{
		_dict.print(size);
	}
	
	public:
		Dictionary()	// Default constructor
		{

		}

	/**
	 *  Run the main dictionary user interface
	 */
	void run_ui() {
		// print out header
		cout << "+------------------------------------------+" << endl;
		cout << "|-- Welcome to the best dictionary ever! --|" << endl;
		cout << "+------------------------------------------+" << endl;

		string instr;
		cout << " Enter command (^D or EOF quits): ";

		// read in user input until eof
		while (getline(cin, instr)) {
			cout << "Entered: " << instr << endl;
			int cont = parseline(instr);
			if (cont == 0)
			{
				break;
			}
			cout << "Enter command: ";
		}
		cout << endl;
	}

};



#endif
