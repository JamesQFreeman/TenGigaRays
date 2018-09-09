#pragma once
#include "hitable.h"
#include <vector>
class hitable_list :
	public hitable
{
public:
	hitable_list() {}
	~hitable_list() {}
	hitable_list(std::vector<hitable*> l) { hit_list = l; }
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	std::vector<hitable*> hit_list;
	void add_item(hitable *p) {
		hit_list.push_back(p);
	}
};

bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
	hit_record temp_rec;
	bool any_hit = false;
	double closset_so_far = t_max;
	for (auto hit_item : hit_list) {
		if (hit_item->hit(r, t_min, closset_so_far, temp_rec)) {
			any_hit = true;
			closset_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return any_hit;
}

