#include <SFML/Graphics.hpp>
#include <iostream>
#include "vec2.h"

int main()
{
    vec2 v1(20, 30), v2(40, 50);

    v2 *= 2;
    v2.print();
    (v1 + v2).print();
    (v1 - v2).print();
    (v1 * 100).print();
    (v2 / 10).print();

    std::cout << (v1 == v2)<<std::endl;

    
    std::cout << (v1 != v2)<< std::endl;

    std::cout << v1.dist(v2)<<std::endl;

    std::cout << v1.distSq(v2) << std::endl;

    v1.normalize();
    std::cout << v1.length() << std::endl;
    v1.print();



    return 0;
}
