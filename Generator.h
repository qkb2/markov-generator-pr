#pragma once

#include <string>
#include <vector>
#include <map>

class Generator
{
public:
	virtual std::vector<std::vector<std::string>> get_word_ngrams(std::string& data) = 0;
	Generator(int max_ngram_len, std::string path) : max_ngram_len(max_ngram_len), path(path) {}
	virtual ~Generator() {}
	virtual void generate_ngram_markov() = 0;
	virtual std::string generate_word_chain(std::string starter, int words_to_gen) = 0;
protected:
	int max_ngram_len;
	std::string path;
	std::map<std::string, std::map<std::string, int>> word_counts_for_context;
};

