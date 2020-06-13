template<typename T>
T* Vertex<T>::getData() {
    T* arr = new T[3];
    arr[0]=this->x;
    arr[1]=this->y;
    arr[2]=this->z;

    return arr;
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
