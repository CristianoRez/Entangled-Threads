#ifndef UTILS_HPP
#define UTILS_HPP




// Atribuição por movimentação.
template<typename T>
T&& my_move(T& obj) noexcept {
    return static_cast<T&&>(obj);
}



#endif