#include "MarkovGenerator.h"

int main(int argc, char* argv[]) {
	std::string path = argv[1];
	std::string starter = "a backdrop";
	auto generator = new MarkovGenerator(3, path);
	generator->generate_ngram_markov();
	auto text = generator->generate_word_chain(starter, 100);
	std::cout << text;
}