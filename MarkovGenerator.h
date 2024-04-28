#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <random>

#include <iostream>
#include <fstream>
#include <sstream>

class MarkovGenerator
{
private:
	int max_ngram_len;
	std::string path;
	std::map<std::string, std::map<std::string, int>> word_counts_for_context;

public:
	std::vector<std::vector<std::string>> get_word_ngrams(std::string& data);
	MarkovGenerator(int max_ngram_len, std::string& path);
	void generate_ngram_markov();
	std::string generate_word_chain(std::string starter, int words_to_gen);

};

