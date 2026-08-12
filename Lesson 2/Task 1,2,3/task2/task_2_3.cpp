#include <mutex>

class Data
{
public:
    Data(int value)
        : value_(value)
    {}

    void swap1(Data &other)
    {
        if(this == &other) {
            return;
        }

        std::lock(mutex_, other.mutex_);
        std::lock_guard lock1(mutex_, std::adopt_lock);
        std::lock_guard lock2(other.mutex_, std::adopt_lock);

        int temp = value_;
        value_ = other.value_;
        other.value_ = temp;
    }

    void swap2(Data &other)
    {
        if(this == &other) {
            return;
        }

        std::scoped_lock lock(mutex_, other.mutex_);

        int temp = value_;
        value_ = other.value_;
        other.value_ = temp;
    }

    void swap3(Data &other)
    {
        if(this == &other) {
            return;
        }

        std::unique_lock lock1(mutex_);
        std::unique_lock lock2(other.mutex_);

        int temp = value_;
        value_ = other.value_;
        other.value_ = temp;
    }

private:
    std::mutex mutex_;
    int value_;
};

void swap1(Data &data1, Data &data2)
{
    data1.swap1(data2);
}

void swap2(Data &data1, Data &data2)
{
    data1.swap2(data2);
}

void swap3(Data &data1, Data &data2)
{
    data1.swap3(data2);
}