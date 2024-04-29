#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <random>

#include <iostream>
#include <fstream>
#include <sstream>

#include <omp.h>

#include "Generator.h"

class MarkovGeneratorMP : public Generator
{
public:
	std::vector<std::vector<std::string>> get_word_ngrams(std::string& data);
	MarkovGeneratorMP(int max_ngram_len, std::string& path);
	virtual ~MarkovGeneratorMP() {}
	void generate_ngram_markov();
	std::string generate_word_chain(std::string starter, int words_to_gen);

};

