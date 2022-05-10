#pragma once
#include "cities.hh"
#include "chromosome.hh"

class ClimbChromosome : public Chromosome {
public:
  ClimbChromosome(const Cities* cities_ptr) 
   : Chromosome(cities_ptr)
  {}

  virtual ~ClimbChromosome() = default;

  virtual void mutate() override;

  virtual Chromosome* clone() const override
  {
    return new ClimbChromosome(cities_ptr_);
  }
};
