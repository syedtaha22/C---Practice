#include <iostream>
#include <sstream>
#include <iomanip>

int main()
{
    float n = 0.5f;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << n;
    std::string m = oss.str();

    std::cout << "String representation: " << m << std::endl;

    return 0;
}
