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
            order_line_access.lock();
            dish = order_line[0];
            order_line.erase(order_line.begin());
            order_line_access.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 11 + 5));
            release_line_access.lock();
            release_line.push_back(dish);
            std::cout << dish << " is ready for delivery." << std::endl;
            release_line_access.unlock();
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
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 6 + 5));
        order = dish();
        order_line_access.lock();
        std::cout << "New order: " << order << std::endl;
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
            std::cout << cookedDish << " is delivered." << std::endl;
            release_line_access.unlock();
            ++deliverCounter;
        }
    }
}

int main() {
    std::srand(time(NULL));
    std::thread onlineOrders (order);
    std::thread cooking (kitchen);
    std::thread delivery (courier);
    onlineOrders.join();
    cooking.join();
    delivery.join();
    return 0;
}
