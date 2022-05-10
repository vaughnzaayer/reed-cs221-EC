/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"
#include "climb_chromosome.hh"

#include <cassert>
#include <algorithm>

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
  : mut_rate_(mut_rate)
{
  uint i = 0;
  while (i < pop_size){
    Chromosome* c = new Chromosome(cities_ptr);
    pop_.push_back(c);
    i++;
  }
}

// Clean up as necessary
Deme::~Deme()
{
  for (auto c : pop_){
    delete c;
  }
}
 
// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
  std::vector<Chromosome*> newPop;
  newPop.reserve(pop_.size());

  for (unsigned i = 0; i < pop_.size()/2; i++){
    auto parent1 = select_parent();
    auto parent2 = select_parent();

    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(generator_) <= mut_rate_) {
      parent1->mutate();
    }
    if (dist(generator_) <= mut_rate_) {
      parent2->mutate();
    }

    auto offspring = parent1->recombine(parent2);
    newPop.push_back(offspring.first);
    newPop.push_back(offspring.second);

  }
  
  for(auto c : pop_) {
    delete c;
  }

  std::swap(pop_, newPop);
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  assert(pop_.size()); // If there are no elements, returns an error
  Chromosome* theBest = pop_[0]; // By default, theBest is initialized to the first elem in pop_
  assert(theBest != nullptr);
  for (auto c1 : pop_){
    // If the chromosome fitness is better than the current best, replace theBest
    assert(c1 != nullptr);
    if (c1->get_fitness() > theBest->get_fitness()){
      theBest = c1;
    }
  }
  return theBest;
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
  // Total sum of fitness in pop_
  const double totalFitness = std::accumulate(pop_.cbegin(), pop_.cend(), 0,
    [](double sum, auto c){return sum + c->get_fitness(); });

  // Picks a random fitness sum value
  static std::uniform_real_distribution<double> dist(0.0, 1);
  const double threshold = totalFitness * dist(generator_);

  // Find first chromosome whose sume exeeds the threshold
  double fitSum = 0; // Total fitness

  const auto chromy = std::find_if(pop_.cbegin(),pop_.cend(),[&](auto c){
    fitSum += c->get_fitness();
    return fitSum >= threshold;
  });

  return *chromy;
}
