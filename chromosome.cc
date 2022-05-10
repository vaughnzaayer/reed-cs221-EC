/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  // Defines range
  std::uniform_int_distribution<int> dist(0, order_.size() - 1);
  std::swap(order_[dist(generator_)], order_[dist(generator_)]);

  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());
  
  // Include size():
  std::uniform_int_distribution<int> dist(0, order_.size());
  
  // Pick 2 random indexes:
  // Sorry I'm bad at naming variables lol, English isn't my first language
  auto some1 = dist(generator_);
  auto some2 = dist(generator_);
  auto [index1, index2] = std::minmax(some1, some2);
  
  // Make the CHILDREN:
  auto child1 = create_crossover_child(this, other, index1, index2);
  auto child2 = create_crossover_child(other, this, index1, index2);
  
  return std::make_pair(child1, child2);
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
        assert(j < p2->order_.size());
      }
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{

  // The longer the path distance becomes, the greater the return value
  return ((1.0 / (1 + calculate_total_distance())) * 100);
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
bool
Chromosome::is_valid() const
{

  for (auto i : order_) {     // Check every city in the order vector 
                              // there should be only one of each element from 0 to order length - 1
    if(std::count(order_.begin(), order_.end(), i) != 1) {
      return false;
    }
  }

  return true;

}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.

// bool
// Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
// {
//   // use std::find to detect if a value is present in a segment of a vector
//   auto location = std::find(&order_[begin], &order_[end], value);
//   if (location == &order_[end]){  // If std::find returns a reference to the end of the vector,
//                                   // the value is not present
//     return false;
//   }
//   return true;
// }

bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  const auto last = order_.cbegin() + end;
  return std::find(order_.cbegin() + begin, last, value) != last;
}