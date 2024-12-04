#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace Version_1
{
    struct StatResult
    {
        std::string description;
        double value;

        StatResult(const std::string& desc, double val)
            : description(desc)
            , value(val)
        {
        }
    };

    using Data = std::vector<double>;
    using Results = std::vector<StatResult>;

    class Statistics
    {
    public:
        virtual ~Statistics() = default;
        virtual void calculate(const Data& data, Results& result) const = 0;
    };

    class Average : public Statistics
    {
    public:
        void calculate(const Data& data, Results& results) const override
        {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            double avg = sum / data.size();
            results.push_back(StatResult("Avg", avg));
        }
    };

    class MinMax : public Statistics
    {
    public:
        void calculate(const Data& data, Results& results) const override
        {
            double min = *(std::min_element(data.begin(), data.end()));
            double max = *(std::max_element(data.begin(), data.end()));
            results.push_back(StatResult("Min", min));
            results.push_back(StatResult("Max", max));
        }
    };

    class Sum : public Statistics
    {
    public:
        void calculate(const Data& data, Results& results) const override
        {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            results.push_back(StatResult("Sum", sum));
        }
    };

    class StatGroup : public Statistics
    {
        std::vector<std::shared_ptr<Statistics>> statistics_;
    public:
        StatGroup(std::initializer_list<std::shared_ptr<Statistics>> strategies)
            : statistics_{strategies}
        {
        }

        void calculate(const Data& data, Results& results) const override
        {
            for (const auto& stat : statistics_)
            {
                stat->calculate(data, results);
            }
        }
    };

    class DataAnalyzer
    {
        std::shared_ptr<Statistics> strategy_;
        Data data_;
        Results results_;

    public:
        explicit DataAnalyzer(std::shared_ptr<Statistics> strategy)
            : strategy_{std::move(strategy)}
        {
        }

        void load_data(const std::string& file_name)
        {
            data_.clear();
            results_.clear();

            std::ifstream fin(file_name.c_str());
            if (!fin)
                throw std::runtime_error("File not opened");

            double d;
            while (fin >> d)
            {
                data_.push_back(d);
            }

            std::cout << "File " << file_name << " has been loaded...\n";
        }

        void set_statistics(std::shared_ptr<Statistics> strategy)
        {
            strategy_ = strategy;
        }

        void calculate()
        {
            if (strategy_)
            {
                strategy_->calculate(data_, results_);
            }
        }

        const Results& results() const
        {
            return results_;
        }
    };

    void show_results(const Results& results)
    {
        for (const auto& rslt : results)
            std::cout << rslt.description << " = " << rslt.value << std::endl;
    }

    int main()
    {
        std::shared_ptr<Statistics> avg = std::make_shared<Average>();
        std::shared_ptr<Statistics> min_max = std::make_shared<MinMax>();
        std::shared_ptr<Statistics> sum = std::make_shared<Sum>();

        auto list_of_stats = {avg, min_max, sum};
        auto std_statistics = std::make_shared<StatGroup>(list_of_stats);

        DataAnalyzer da{avg};
        da.load_data("stats_data.dat");
        da.calculate();

        show_results(da.results());

        std::cout << "\n\n";

        da.load_data("new_stats_data.dat");
        da.calculate();

        show_results(da.results());

        return 0;
    }

}

namespace Version_2
{
    struct StatResult
    {
        std::string description;
        double value;

        StatResult(const std::string& desc, double val)
            : description(desc)
            , value(val)
        {
        }
    };

    using Data = std::vector<double>;
    using Results = std::vector<StatResult>;

    enum StatisticsType
    {
        avg,
        min_max,
        sum
    };

    using Statistics = std::function<Results(const Data&)>;

    class DataAnalyzer
    {
        Statistics strategy_;
        Data data_;
        Results results_;

    public:
        DataAnalyzer(Statistics strategy)
            : strategy_(std::move(strategy))
        {
        }

        void load_data(const std::string& file_name)
        {
            data_.clear();
            results_.clear();

            std::ifstream fin(file_name.c_str());
            if (!fin)
                throw std::runtime_error("File not opened");

            double d;
            while (fin >> d)
            {
                data_.push_back(d);
            }

            std::cout << "File " << file_name << " has been loaded...\n";
        }

        void set_strategy(Statistics strategy) { strategy_ = std::move(strategy); }

        void calculate()
        {
            Results new_results = strategy_(data_);
            results_.insert(results_.end(), new_results.begin(), new_results.end());
        }

        const Results& results() const { return results_; }
    };

    struct AvgStrategy
    {
        Results operator()(const Data& data) const
        {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            double avg = sum / data.size();

            StatResult result("Avg", avg);
            return {result};
        }
    };

    struct MinMaxStrategy
    {
        Results operator()(const Data& data) const
        {
            double min = *(std::min_element(data.begin(), data.end()));
            double max = *(std::max_element(data.begin(), data.end()));
            return {StatResult("Min", min), StatResult("Max", max)};
        }
    };

    struct SumStrategy
    {
        Results operator()(const Data& data) const
        {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            return {StatResult("Sum", sum)};
        }
    };

    struct StatGroup
    {
        std::vector<Statistics> statistics_;

        StatGroup(std::initializer_list<Statistics> strategies)
            : statistics_{strategies}
        {
        }

        Results operator()(const Data& data) const
        {
            Results results;
            for (const auto& stat : statistics_)
            {
                Results new_results = stat(data);
                results.insert(results.end(), new_results.begin(), new_results.end());
            }

            return results;
        }
    };

    void show_results(const Results& results)
    {
        for (const auto& rslt : results)
            std::cout << rslt.description << " = " << rslt.value << std::endl;
    }

    int main()
    {
        AvgStrategy avg;
        MinMaxStrategy min_max;
        SumStrategy sum;

        StatGroup std_statistics{min_max, sum};
        StatGroup advanced_statistics{avg, std_statistics};

        DataAnalyzer da{std_statistics};
        da.load_data("stats_data.dat");
        da.calculate();

        show_results(da.results());

        std::cout << "\n\n";

        da.load_data("new_stats_data.dat");
        da.calculate();

        show_results(da.results());

        return 0;
    }
}

int main()
{
    Version_1::main();

    std::cout << "\n\n";

    Version_2::main();
    return 0;
}