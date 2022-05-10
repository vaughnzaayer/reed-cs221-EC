#include <cassert>

#include "climb_chromosome.hh"

void ClimbChromosome::mutate(){
  std::uniform_int_distribution<int> dist(0, order_.size()-1);
  auto fitness1 = get_fitness();

  // Pick a random point 
  long unsigned int parent1 = dist(generator_);
  long unsigned int parent2 = parent1 - 1;
  if (parent1 == 0){ // If p==0, swap with the city at index N-1 instead of at index -1
    parent2 = order_.size()-1;
  }
  std::swap(order_[parent1], order_[parent2]);

  auto fitness2 = get_fitness();
  std::swap(order_[parent1], order_[parent2]);
  auto parent3 = parent1 + 1;
  if (parent1 == order_.size()-1){ // If p==N-1, swap with the city at index 0 instead of at index N.
    parent3 = 0;
  }
  std::swap(order_[parent1], order_[parent3]);

  // Find which of the three chromosomes produced the fittest ordering and choose that fittest chromosome as the resulting mutated chromosome.
  // There is probably a much better method than the one I used, but it still workis. 
  auto fitness3 = get_fitness();

  std::swap(order_[parent1], order_[parent3]);

  if (fitness1 > fitness2){
    if(fitness1 > fitness3){
      std::swap(order_[parent1], order_[parent3]);
    }
  }
  else{
    if(fitness2 > fitness3){ 
      std::swap(order_[parent1], order_[parent2]);
      std::swap(order_[parent1], order_[parent3]);
    }
  }
}

