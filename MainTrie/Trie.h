#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<memory>
#include<algorithm>
#include<queue>
#include<map>

using namespace std;
class Trie
{

public:
	class Node {

	public:
		string cName;
		shared_ptr<Node> parent;
		vector<shared_ptr<Node>> children;
		Node(string name, shared_ptr<Node> parent)
		{
			cName = name;
			this->parent = parent;
		}

		//check the existence of the comapny Name in the trie
		//If exists, return the node
		//else return Null
		shared_ptr<Node> findChild(string s)
		{
			for each (shared_ptr<Node> tmp in children)
			{
				if (tmp->cName == s)
					return tmp;
			}

			return NULL;
		}

	};

	shared_ptr<Node> root;
	shared_ptr<Node> newTrie(string name, shared_ptr<Node> parent)
	{
		Node tmp(name, parent);
		return make_shared<Node>(tmp);
	}
	Trie() {}

	//constructor
	Trie(string name, shared_ptr<Node> parent)
	{
		root = newTrie(name, nullptr);
	}

	map<string, string> populateTrie(string line, map<string, string> primaryNameFinder);
	void addWord(vector<string> companyName);

};

//Gets companies line by line and builds trie
map<string, string> Trie::populateTrie(string line, map<string, string> primaryNameMapper)
{
	line.erase(remove_if(line.begin(), line.end(), ispunct), line.end()); // Remove Punctuations from the Company name
	istringstream iss1(line);
	string tabSplitWords;
	string primaryName;
	int count = 0;

	//split words with tab
	while (getline(iss1, tabSplitWords, '\t'))
	{
		//assign the first word as primary Name for the particular line
		if (count == 0)
			primaryName = tabSplitWords;
		//insert company name and its primary Name in HashMap
		primaryNameMapper[tabSplitWords] = primaryName;

		vector<string> companyName;
		istringstream iss2(tabSplitWords);
		//split words with space
		while (iss2)
		{
			string spaceSplitWords;
			iss2 >> spaceSplitWords;
			companyName.push_back(spaceSplitWords);
		}
		addWord(companyName);
		count++;
	}
	return primaryNameMapper;
}

//adds the company Name received as parameter to Trie
void Trie::addWord(vector<string> companyName)
{
	shared_ptr<Node> current = root;

	for (int i = 0; i < companyName.size(); i++)
	{
		shared_ptr<Node> child = current->findChild(companyName[i]);
		if (child != NULL)
			current = child;
		else
		{
			shared_ptr<Node> temp = newTrie(companyName[i], current); // Create new Trie Node
			current->children.push_back(temp);
			current = temp;
		}
	}
}