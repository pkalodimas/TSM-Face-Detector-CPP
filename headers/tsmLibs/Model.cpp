
#ifndef MODEL_CPP_
#define MODEL_CPP_

#include <assert.h>
#include "Model.h"

namespace tsm {

template <typename T>
Model<T>::Model() {
	this->components = nullptr;
	this->defs = nullptr;
	this->filters = nullptr;
	this->interval = 0;
	this->maxsize = nullptr;
	this->sbin = 0;
	this->thresh = 0;
}

template <typename T>
Model<T>::~Model() {
	delete this->maxsize;
	delete this->filters;
	delete this->defs;
	delete this->components;
}

template <typename T>
Model<T> *Model<T>::modelComponents(int padx, int pady){

	for( int c=0; c<this->components->length; c++ ){

		for( int k=0; k<this->components->data[c]->parts->length; k++ ){

			Part<T> *p = &(this->components->data[c]->parts->data[k]);

			Filter<T> *f = this->filters->data[p->filterid-1];
			p->filterI = f->i;
			p->sizx = f->dims[0];
			p->sizy = f->dims[1];

			Def<T> *d = &(this->defs->data[p->defid-1]);
			p->defI = d->i;
			p->w.copy(d->w);

			assert( p->parent < k+1 );
			if( p->parent > 0 )  p->scale = d->anchor->data[2] + this->components->data[c]->parts->data[p->parent-1].scale;
			else  {
				assert( k == 0 );
				p->scale = 0;
			}

			p->step = (int) pow((float) 2, d->anchor->data[2]);
			p->startx = d->anchor->data[0] - (p->step-1) * padx;
			p->starty = d->anchor->data[1] - (p->step-1) * pady;
		}
	}

	return this;
}

} /* namespace tsm */

#endif

