#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int MAGIC_NUMBER = 10;

class DataProcessor
{
private:
    std::string type;
    std::vector<int> data;
    int filterThreshold;

    void sort()
    {
        std::sort(data.begin(), data.end());
    }
    void filter()
    {
        data.erase(
            std::remove_if(data.begin(), data.end(),
                           [this](int v)
                           { return v <= filterThreshold; }),
            data.end());
    }

    void printResults() const {
        std::cout << "Final data: ";
        for (int v : data) std::cout << v << " ";
        std::cout << std::endl;
    }

    void printStats() const
    {
        if (data.empty())
            return;
        int sum = 0;
        for (int v : data)
            sum += v;
        std::cout << "Average: " << static_cast<double>(sum) / data.size() << std::endl;
        std::cout << "Maximum: " << *std::max_element(data.begin(), data.end()) << std::endl;
    }

public:
    DataProcessor(const std::string &type, std::vector<int> &data,
                  int threshold = MAGIC_NUMBER) : type(type), data(data), filterThreshold(threshold) {}

    // Long method with multiple responsibilities - SOLUTION - Single Responsibility Principle
    void processData(bool sortFlag,
                     bool filterFlag)
    {
        std::cout << "Processing " << type << " data..." << std::endl;
        if (data.empty())
        {
            std::cerr << "Error: No data to process" << std::endl;
            return;
        }
        if (sortFlag) sort();
        if (filterFlag) filter();
        // Display results
        printResults();
        printStats();
    }
};
int main()
{
    std::vector<int> testData = {15, 3, 8, 12, 1, 20, 7};
    DataProcessor processor("numerical", testData);
    processor.processData(true, true);
    return 0;
}