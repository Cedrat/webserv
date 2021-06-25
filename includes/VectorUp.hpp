#ifndef VECTORUP_HPP
# define VECTORUP_HPP

#include <vector>

template <typename T>
class VectorUp : public std::vector<T>
{
    public :
        T* data(){
            return (&this->front());
        }

        const T* data() const{
            return (&this->front());
        }
};

#endif
