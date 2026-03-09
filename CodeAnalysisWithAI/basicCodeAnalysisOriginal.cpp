#include <iostream>
#include <vector>
#include <string>
class DataProcessor
{
public:
    // Long method with multiple responsibilities
    void processData(std::string type, std::vector<int> data, bool sortFlag, bool filterFlag)
    {
        std::cout << "Processing " << type << " data..." << std::endl;
        // Duplicate validation logic
        if (data.size() == 0)
        {
            std::cout << "Error: No data to process" << std::endl;
            return;
        }
        if (sortFlag)
        {
            // Inefficient sorting implementation
            for (int i = 0; i < data.size(); i++)
            {
                for (int j = i + 1; j < data.size(); j++)
                {
                    if (data[i] > data[j])
                    {
                        int temp = data[i];
                        data[i] = data[j];
                        data[j] = temp;
                    }
                }
            }
            std::cout << "Data sorted" << std::endl;
        }
        if (filterFlag)
        {
            std::vector<int> filtered;
            for (int i = 0; i < data.size(); i++)
            {
                if (data[i] > 10)
                { // Magic number
                    filtered.push_back(data[i]);
                }
            }
            data = filtered;
            std::cout << "Data filtered (values > 10)" << std::endl;
        }
        // Duplicate validation logic
        if (data.size() == 0)
        {
            std::cout << "Warning: No data remaining after processing" << std::endl;
            return;
        }
        // Display results
        std::cout << "Final data: ";
        for (int i = 0; i < data.size(); i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
        // Calculate basic stats
        int sum = 0;
        for (int i = 0; i < data.size(); i++)
        {
            sum += data[i];
        }
        double average = (double)sum / data.size();
        std::cout << "Average: " << average << std::endl;
        int max = data[0];
        for (int i = 1; i < data.size(); i++)
        {
            if (data[i] > max)
            {
                max = data[i];
            }
        }
        std::cout << "Maximum: " << max << std::endl;
    }
};
int main()
{
    DataProcessor processor;
    std::vector<int> testData = {15, 3, 8, 12, 1, 20, 7};
    processor.processData("numerical", testData, true, true);
    return 0;
}