#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <vector>

std::vector<std::string> release_line;
std::mutex release_line_access;

std::vector<std::string> order_line;
std::mutex order_line_access;

std::string dish()
{
    std::srand(time(NULL));
    int dish = rand() % 5;
    if (dish == 0) return "pizza";
    else if (dish == 1) return "soup";
    else if (dish == 2) return "steak";
    else if (dish == 3) return "salad";
    else return "sushi";
}

void kitchen()
{
    int dishesCounter = 0;
    std::string dish;
    while (dishesCounter < 10)
    {
        if (order_line.size() > 0)
        {
            dish = order_line[0];
            order_line.erase(order_line.begin());
            std::srand(time(NULL));
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 11 + 5));
            release_line_access.lock();
            release_line.push_back(dish);
            release_line_access.unlock();
            std::cout << dish << " is ready for delivery." << std::endl;
            ++dishesCounter;
        }
    }
}

void order()
{
    int orderCounter = 0;
    std::string order;
    while (orderCounter < 10)
    {
        std::srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 6 + 5));
        order = dish();
        std::cout << "New order: " << order << std::endl;
        order_line_access.lock();
        order_line.push_back(order);
        order_line_access.unlock();
        ++orderCounter;
    }
}

void courier()
{
    int deliverCounter = 0;
    std::string cookedDish;
    while (deliverCounter < 10)
    {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        if (release_line.size() > 0)
        {
            release_line_access.lock();
            cookedDish = release_line[0];
            release_line.erase(release_line.begin());
            release_line_access.unlock();
            std::cout << cookedDish << " is delivered." << std::endl;
            ++deliverCounter;
        }
    }
}

int main() {
    std::thread onlineOrders (order);
    std::thread cooking (kitchen);
    std::thread delivery (courier);
    onlineOrders.join();
    cooking.join();
    delivery.join();
    return 0;
}
