#include "MarkovGenerator.h"
#include <omp.h>

MarkovGenerator::MarkovGenerator(int max_ngram_len, std::string& path) : max_ngram_len(max_ngram_len), path(path) {}

std::vector<std::vector<std::string>> MarkovGenerator::get_word_ngrams(std::string& data)
{
    std::vector<std::vector<std::string>> ngrams;
    std::istringstream iss(data);
    std::string word;
    std::vector<std::string> words;
    while (iss >> word) {
        words.push_back(word);
        if (words.size() >= max_ngram_len) {
            std::vector<std::string> words_to_add;
            for (int i = (int) words.size()-max_ngram_len; i < words.size(); i++)
            {
                words_to_add.push_back(words[i]);
            }
            ngrams.push_back(words_to_add);
        }
    }
    return ngrams;
}

//introduce noise with cuda

void MarkovGenerator::generate_ngram_markov()
{
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Error: File not found.");
    }

    std::string line;
    std::string data;
    while (std::getline(file, line))
    {
        for (char& c : line) {
            if (c > 127 || c < 0) {
                continue; // Skip characters outside the ASCII range
            }
            if (std::isalpha(c) || std::isspace(c)) {
                data += std::tolower(c);
            }
        }
    }
    auto ngrams = this->get_word_ngrams(data);
    for (int i = 0; i < ngrams.size(); i++)
    {
        const auto& ngram = ngrams[i];
        std::string context = ngram[0];
        for (int j = 1; j < ngram.size() - 1; j++)
        {
            context = context + " " + ngram[j];
        }
        std::string word_to_pred = ngram[ngram.size() - 1];
        word_counts_for_context[context][word_to_pred]++;
    }
    file.close();
}

std::string MarkovGenerator::generate_word_chain(std::string starter, int words_to_gen)
{
    std::string text = starter;
    std::string current_word = starter;
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < words_to_gen; i++)
    {
        // Check if the current context exists in the word counts
        if (word_counts_for_context.find(current_word) != word_counts_for_context.end()) {
            // Get all possible next words and their counts
            const std::map<std::string, int>& next_word_counts = word_counts_for_context[current_word];
            if (!next_word_counts.empty()) {
                // Choose the next word based on the counts
                std::vector<std::string> candidates;
                for (const auto& pair : next_word_counts) {
                    for (int j = 0; j < pair.second; j++) {
                        candidates.push_back(pair.first);
                    }
                }
                // Randomly select the next word from candidates
                std::uniform_int_distribution<> dis(0, (int) candidates.size() - 1);
                int random_index = dis(gen);
                std::string next_word = candidates[random_index];

                // Update text and current_word in a critical section

                    text += " " + next_word;
                    current_word = current_word.substr(current_word.find(' ') + 1) + " " + next_word;
                
            }
            else {
                // If there are no next words, break the loop
                break;
            }
        }
        else {
            // If the current context doesn't exist in the word counts, break the loop
            break;
        }
    }
    return text;
}
