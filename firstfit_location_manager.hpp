#pragma once

#include "firstfit_manager.hpp"

// first fit memory manager that sorts the free list by memory location
// + fast coalescence
// + fast allocation
// + no internal fragmentation
// - high external fragmentation
class firstfit_location_manager : public firstfit_manager
{
protected:
	static bool compare(const block& a, const block& b)
	{
		return a.start < b.start;
	}

public:
	firstfit_location_manager(unsigned int memory_size)
		: firstfit_manager(memory_size, compare) { };

	virtual void coalesce()
	{
		// loop through all free blocks...
		for (auto it_a = free_blocks.begin(); it_a != free_blocks.end(); ++it_a)
		{
			// ...and again, starting from the previous iterator
			for (auto it_b = it_a; it_b != free_blocks.end(); ++it_b)
			{
				// a block can't merge with itself
				if (it_a == it_b)
					continue;

				// if the first block's end matches the second block's start...
				if (it_a->start + it_a->size == it_b->start)
				{
					// merge them!
					free_blocks.emplace_hint(it_a, it_a->start, it_a->size + it_b->size);
					free_blocks.erase(it_a);
					free_blocks.erase(it_b);
					--it_a;
				}
			}
		}
	}
};