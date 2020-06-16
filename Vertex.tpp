template<typename T>
const T Vertex<T>::operator[](unsigned int index) const{
    switch (index) {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        default:
            perror("Array index out of bounds!");
            exit(-1);
    }
}

template<typename T>
T& Vertex<T>::operator[](unsigned int index) {
    switch (index) {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        default:
            perror("Array index out of bounds!");
            exit(-1);
    }
}
