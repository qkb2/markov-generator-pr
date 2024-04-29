#include "Generator.h"
#include "MarkovGenerator.h"
#include "MarkovGeneratorMP.h"
#include "MarkovGeneratorCUDA.cuh"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		return -1;
	}
	std::string path = argv[1];
	std::string opt = argv[2];
	std::string starter = "a backdrop";
	Generator * generator;
	if (opt.compare("CUDA") == 0) {
		generator = new MarkovGeneratorCUDA(3, path);
	}
	else if (opt.compare("MP") == 0) {
		generator = new MarkovGeneratorMP(3, path);
	}
	else {
		generator = new MarkovGenerator(3, path);
	}
	generator->generate_ngram_markov();
	auto text = generator->generate_word_chain(starter, 100);
	std::cout << text;
}