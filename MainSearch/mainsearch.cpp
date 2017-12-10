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
#include<string>
#include<vector>
#include<sstream>
#include<map>
#include<algorithm>
using namespace std;

class MainSearch
{
public:
	vector<string> articleWords;  // Vector to store new article
	vector<string> newsWords;	// Vector to store normalized words
	vector<vector<string>> companies;  // 2D vector to store company names along with Synonyms 
	map<string, int> freqCount;			// Hashmap to store primary name along with count of occurance

	//Read companies from companies.dat file
	void readCompanies()
	{
		ifstream fin;
		string line;

		fin.open("companies.dat"); // opens the file 
		if (!fin.is_open())
		{
			cout << "Error in opening companies.dat, please verify" << endl;
		}
		while (getline(fin, line))			// Read from company.dat file line by line 
		{
			vector<string> company;
			line.erase(remove_if(line.begin(), line.end(), ispunct), line.end()); // Remove Punctuations from the line
			istringstream iss1(line);
			string words;
			// Storing the Company names and Synonyms in a 2D vector
			while (getline(iss1, words, '\t'))		// Distinguish Synonyms 
				company.push_back(words);			// Push Synonym
			companies.push_back(company);			// Push each line of companies
		}
	}

	//Get Article from Standard Input
	void getArticleInput()
	{
		string line, news;

		cout << "Enter the news article :" << endl;

		// Keep reading the input till the period symbol
		while (getline(cin, line))
		{
			if (line == ".")
				break;
			news += line;
		}
		news.erase(remove_if(news.begin(), news.end(), ispunct), news.end()); // Remove Punctuations from the Article

		istringstream iss(news);

		// Storing the News Article word by word in a Vector 
		while (iss)
		{
			string words;
			iss >> words;
			articleWords.push_back(words);
		}
		// Ignoring the words "a, an, the, and, or, but" from the news article
		for (int i = 0; i < articleWords.size() - 1; i++)
		{
			if (articleWords[i] != "a" && articleWords[i] != "an" && articleWords[i] != "and" && articleWords[i] != "the" && articleWords[i] != "or" && articleWords[i] != "but")
			{
				newsWords.push_back(articleWords[i]);
			}
		}
	}

	//Calculate the relevance and display
	void getRelevance()
	{
		// Identify each company name in the article
		int count, temp, t;
		string wordCmp, s;
		wordCmp.clear();
		for (int j = 0; j < companies.size(); j++)
		{
			vector<string> company = companies[j];
			for (int k = 0; k < company.size(); k++) // Primary Name + Synonyms Vector
			{
				int count = 0;
				istringstream iss1(company[k]);
				while (iss1 >> s)
				{
					count++;					// To keep a count of words in Company name
				}
				temp = count;
				for (int i = 0; i < newsWords.size(); i++)
				{
					wordCmp = newsWords[i];

					if (count > 1)	// If there is more than 1 word in the Company name	then enter 
					{
						t = i; // Temp iteration for appending the next word from news

						for (int app = 0; app<count; app++)
						{
							if (t<newsWords.size() - 1)  //  check of end of Buffer before trying to access i+1
							{
								// Append the String to compare with company name such that number of newsWords = noumber of Company words 
								wordCmp = wordCmp + " " + newsWords[t + 1];
								count--;
								t++; // iterate to next word 
							}
						}
					}
					if (wordCmp == company[k]) // check If news article string is same as company name 
					{
						//check if the key is in hashmap
						if (freqCount.find(company[0]) != freqCount.end())
						{
							int value = freqCount[company[0]];
							freqCount[company[0]] = value + 1;
						}
						else
							freqCount[company[0]] = 1;
					}
					count = temp;
				}
			}
		}

		// Display the primary name of the company identified along with relevance in Percentage 
		cout << endl << "The Primary names of the companies identified in the Article along with Relevance as below:" << endl;
		for (map<string, int>::iterator it = freqCount.begin(); it != freqCount.end(); ++it)
			cout << it->first << " => " << (float(it->second) / newsWords.size()) * 100 << "%" << endl;

	}

};

int main()
{
	MainSearch search;
	search.readCompanies();
	search.getArticleInput();
	search.getRelevance();
	return 0;
}