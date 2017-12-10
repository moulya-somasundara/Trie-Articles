/*
With reference to the discussion on Canvas: https://sit.instructure.com/courses/4976/discussion_topics/29969
This program will count every occurence of the company name from compnaies.dat in the article

Thus, if companies.dat has below:
Microsoft Corporation Inc(tab)Microsoft

If article is:
Microsoft software is part of Microsoft Corporation Inc

Then expected output would be:
Relevance would be :  3 / 8
(Occurence count is 3 ==> 2 for 'Microsoft' & 1 for 'Microsoft Corporation Inc')

*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<memory>
#include<algorithm>
#include<queue>
#include "Trie.h"
#include<map>

using namespace std;

class MainTrie
{
public:
	queue<shared_ptr<Trie::Node>> childNodeQ;
	map<string, string> primaryNameMapper;
	map<string, int> freqCount;
	Trie trie;

	MainTrie()
	{
		trie = Trie("Root", nullptr);
	}

	//search the company Name in Trie
	queue<shared_ptr<Trie::Node>> MainTrie::searchWord(Trie trie, queue<shared_ptr<Trie::Node>>childNodeQ, string company)
	{
		//create newQ
		queue<shared_ptr<Trie::Node>> newQ;

		while (!childNodeQ.empty())
		{
			//remove from current queue
			shared_ptr<Trie::Node> currNode = childNodeQ.front();
			shared_ptr<Trie::Node> child1 = currNode->findChild(company);
			childNodeQ.pop();

			if (child1 != NULL)
			{
				//insert into new Queue
				newQ.push(child1);

				//check if that's the end of the word
				if (currNode->findChild("") != NULL)
				{
					updateRelevanceCount(currNode);
				}
			}
			else
			{
				//check if that's the end of the word
				if (currNode->findChild("") != NULL)
				{
					updateRelevanceCount(currNode);
				}
			}
		}

		//check again in root children
		shared_ptr<Trie::Node> current = trie.root;
		shared_ptr<Trie::Node> child = current->findChild(company);
		if (child != NULL)
			newQ.push(child);

		//return new Queue
		return newQ;
	}

	//finds the primary name of the current Node and updates its frequency count in Hashmap.
	void updateRelevanceCount(shared_ptr<Trie::Node> currNode)
	{
		//find parent of currNode till Root
		//prepend the values to get the comp Name as in companies.dat
		shared_ptr<Trie::Node> temp = currNode;
		string compName;
		string primaryCompName;
		while (temp->parent != NULL)
		{
			if (compName == "")
				compName = temp->cName;
			else
				compName = temp->cName + " " + compName;
			temp = temp->parent;
		}



		//Find the primary name for the comp Name obtained in the hashmap
		map<string, string>::const_iterator it = primaryNameMapper.find(compName);
		if (it != primaryNameMapper.end())
		{
			primaryCompName = it->second;

		}

		//Update relevance count for this primary name in another hashmap where it has primary Name and its freq
		//check if the key is in hashmap
		if (freqCount.find(primaryCompName) != freqCount.end())
		{
			int value = freqCount[primaryCompName];
			freqCount[primaryCompName] = value + 1;
		}
		else
			freqCount[primaryCompName] = 1;

	}

	//get the news artcile from user, normalize it and return the vector with individual words in them
	vector<string> getArticleInput()
	{
		vector<string> articleWords;
		vector<string> newsWords;
		string line, news;

		cout << "Enter the news article :" << endl;

		while (getline(cin, line))
		{
			if (line == ".")
				break;
			news += line;
		}

		//normalize
		news.erase(remove_if(news.begin(), news.end(), ispunct), news.end());


		//split the news with spaces
		istringstream iss(news);
		while (iss)
		{
			string words;
			iss >> words;
			articleWords.push_back(words);
		}

		//remove stop words
		for (int i = 0; i < articleWords.size() - 1; i++)
		{
			if (articleWords[i] != "a" && articleWords[i] != "an" && articleWords[i] != "and" && articleWords[i] != "the" && articleWords[i] != "or" && articleWords[i] != "but")
			{
				newsWords.push_back(articleWords[i]);
			}
		}

		return newsWords;
	}

	//creates Tries for the companies present in companies.dat
	void createCompanyTrie()
	{
		ifstream fin;
		string line;
		fin.open("companies.dat"); // opens the file
		if (!fin.is_open())
		{
			cout << "Error in opening companies.dat, please verify" << endl;
		}
		while (getline(fin, line))
		{
			primaryNameMapper = trie.populateTrie(line, primaryNameMapper);
		}
	}

	//get the relevance by searching for the word in trie
	void getRelevance(vector<string> newsWords)
	{
		//search for the word in trie
		for (int i = 0; i < newsWords.size(); i++)
		{
			childNodeQ = searchWord(trie, childNodeQ, newsWords[i]);
		}

		if (!childNodeQ.empty())
		{
			childNodeQ = searchWord(trie, childNodeQ, " ");
		}


		//print the relevance for each company found
		cout << endl << "The Primary names of the companies identified in the Article along with Relevance as below:" << endl;
		for (map<string, int>::iterator it = freqCount.begin(); it != freqCount.end(); ++it)
			cout << it->first << " => " << (float(it->second) / newsWords.size()) * 100 << "%" << endl;
	}

};

int main()
{
	vector<string> articleProcessed;
	MainTrie maintrie;
	maintrie.createCompanyTrie();
	articleProcessed = maintrie.getArticleInput();
	maintrie.getRelevance(articleProcessed);
	return 0;
}