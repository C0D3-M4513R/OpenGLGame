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
T& Vertex<T>::operator[](unsigned int index){
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
[[maybe_unused]] const char* Vertex<T>::toString() const {
    auto* out = new std::string();
    sprintf(out->data(),"x:%s ,y:%s ,z:%s",std::to_string(x).c_str(),std::to_string(y).c_str(),std::to_string(z).c_str());

    return out->c_str();
}
