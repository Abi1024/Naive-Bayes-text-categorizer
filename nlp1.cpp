//NLP Spring 2016 Project 1, Abiyaz Chowdhury
#include <iostream>
#include <cstdlib>
#include <map>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;
map<string,int> category_documents;  //hashmap: category --> number of training documents in that category
map<string,double> running_sum;
int num_documents = 0;
map<string,map<string,int> > category_word_counts; //number of documents in category that contain a given word;
map<string,int> distinct_words; //number of distinct words (vocabulary size) of each category
string process_string(string& v); //tokenizer
int num_categories = 0;
double laplace = 0.01; //Laplace smoothing factor

int main(){
	//training
	//prompt user for input file which is a list of documents
	string document_file;
	ifstream input1,input2;
	cout << ("Enter the filename (exclude dot slash) of the list of training documents. Make sure you are in directory TC_provided.") << endl; 
	cin >> document_file;
	document_file = "./" + document_file;
	input1.open(document_file.c_str());
	if (!input1){
		cerr << "The list of training documents failed to open. Please try again";
		return 0;
	}
	string v1,v2;
	while(input1 >> v1 >> v2){ //for each training document
		map<string,int> word_seen;
		num_documents++;
		input2.open(v1.c_str()); //v2 is the category label
		if (!input2){
			cerr << "Training document " << v1 << " failed to open. Please try again.";
			return 0;
		}
		if (category_documents[v2] == 0){
			num_categories++;
		}
		category_documents[v2]++;
		string v3;
		while (input2 >> v3){ //for each word in a training document
			process_string(v3);
			if (word_seen[v3] == 0){
				word_seen[v3] = 1;
				category_word_counts[v2][v3]++;
				distinct_words[v2]++;
			}
		}
		input2.close();
	}
	input1.close();
	cout << "Number of training documents: " << num_documents << endl;
	cout << "Number of categories: " << num_categories << endl;
	cout << "Training done" << endl;
	//==============================================================================================================================
	//testing
	int num_test = 0;
	cout << ("Enter the filename (exclude dot slash) of the list of testing documents. Make sure you are in directory TC_provided.") << endl; 
	cin >> document_file;
	document_file = "./" + document_file;
	input1.open(document_file.c_str());
	if (!input1){
		cerr << "The list of test documents failed to open. Please try again";
		return 0;
	}
	ofstream output;
	cout << ("Enter the filename (exclude dot slash) of the output file in which to print classification results. Make sure you are in directory TC_provided.") << endl; 
	cin >> document_file;
	document_file = "./" + document_file;
	output.open(document_file.c_str());
	if (!output){
		cerr << "The output file failed to open. Please try again";
		return 0;
	}
	while(input1 >> v1){ //for each test document
		num_test++;
		input2.open(v1.c_str()); //v2 is the category label
		if (!input2){
			cerr << "Testing document " << v1 << " failed to open. Please try again.";
			return 0;
		}
		string v3;
		for (map<string,int>::const_iterator itr = category_documents.begin(); itr != category_documents.end(); itr++){
			running_sum[itr->first] = 0;
		}
		while (input2 >> v3){ //for each word in the document
			process_string(v3);
			for (map<string,int>::const_iterator itr = category_documents.begin(); itr != category_documents.end(); itr++){
				double d = log(((double)category_word_counts[itr->first][v3]+laplace)/(itr->second + laplace*distinct_words[itr->first]));
				running_sum[itr->first] += d;
			}
		}
		double max_prob = 0;
		string best_category = "";
		for (map<string,int>::const_iterator itr = category_documents.begin(); itr != category_documents.end(); itr++){
			running_sum[itr->first] += log(((double)itr->second)/num_documents);
			if (max_prob == 0){
					max_prob = running_sum[itr->first];
					best_category = (itr->first);
			}else if (running_sum[itr->first] > max_prob){
					max_prob = running_sum[itr->first];
					best_category = (itr->first);
			}
		}
		input2.close();
		output << v1 << " " << best_category << endl;
	}
	cout << "Number of test documents: " << num_test << endl;
	cout << "Testing done" << endl;
	output.close();
	input1.close();
	return 0;
}

string process_string(string& v){
	transform(v.begin(), v.end(), v.begin(), ::tolower);
	v.erase(std::remove(v.begin(), v.end(), ','), v.end());
	v.erase(std::remove(v.begin(), v.end(), '.'), v.end());
	v.erase(std::remove(v.begin(), v.end(), '?'), v.end());
	v.erase(std::remove(v.begin(), v.end(), '\''), v.end());
	v.erase(std::remove(v.begin(), v.end(), '\"'), v.end());
	v.erase(std::remove(v.begin(), v.end(), '('), v.end());
	v.erase(std::remove(v.begin(), v.end(), ')'), v.end());
	v.erase(std::remove(v.begin(), v.end(), '`'), v.end());
	return v;
}