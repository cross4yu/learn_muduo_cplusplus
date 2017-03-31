#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <memory>

/*
int main()
{
//    int sum = accumulate()
vector<int> vec;
auto it = back_inserter(vec);
*it = 42;

for (auto i = vec.begin(); i < vec.end(); ++i)
    cout << *i <<endl;

return 0;


}

class Foo : public Observer
{
    public:
    Foo();
    virtual void update();
    void onserve(Observables* s)
    {
        s->register_(this);
    }
};
*/

#include <iostream>
#include <thread>
#include <mutex>

volatile int counter(0);
std::mutex mtx;
void attempt_10k_increases(){
    for(int i = 0; i < 10000; ++i)
    {
        if (mtx.try_lock())
        {
            ++counter;
            mtx.unlock();
        }
    }
}

int main ()
{
    std::thread threads[10];
    for (int i = 0; i < 10; ++i)
    {
        threads[i] = std::thread(attempt_10k_increases);
    }
    for (auto & th:threads) th.join();
    std::cout << counter << "success increases of the counter.\n";

    return 0;
}


class Observable
{
    public:
        void register_(weak_ptr<Observer>x);
        void notifyObservers();
    private:
        mutable MutexLock mutex_;
        std::vector<weak_ptr<Observer> > observers_;
        typedef std::vector<weak_ptr<Observer> >::iterator Iterator;
};
void Observer::notifyObservers()
{
    MutexLockGuard lock(mutex_);
    Iterator it = observers_.begin();
    while (it != observers_.end())
    {
        shared_ptr<Observer> obj(it->lock());
        if (obj)
        {
            obj->update();
            ++it;
        }
        else{
            it = observers_.erase(it);
        }
    }
}


class StockFactory : boost::noncopyable
{
    public:
        shared_ptr<Stock> get(const string &key);
    private:
        mutable MutexLock mutex_;
        std::map<string, shared_ptr<Stock> > stocks_;
};

shared_ptr<Sotck> SoctkFactory::get(const string &key)
{
    shared_ptr<Stock> pStock;
    MutexLockGuard lock(mutex_);
    weak_ptr<Stock>& wkStock = stocks_[key];
    pSotck = wkStock.lock();
    if (!pStock){
        pStock.reset(new Stock(key));
        //pStock.reset(new Sotck(key), boost::bind(&StockFactory::deleteStock, this, _1));
        wkStock = pStock;
    }
    return pStock;
}
i
template<class Y, class D> shared_ptr::shared_ptr(Y* p, D d);
template<class Y, class D> void shared_ptr::reset(Y* p, D d);

class StockFactory : boost::noncopyable
{
    private:
        void deleteStock(Stock* stock)
        {
            if (stock){
                MutexLockGuard lock(mutex_);
                stocks_.eraser(stock->key());
            }
            delete stock;
}