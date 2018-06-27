#include <memory>

template <class Derived>
struct Base {
};

struct Derived : public Base<Derived> {

};

class Adapter {
public:
    template <typename T>
    Adapter(const T& adapted) : holder_(std::make_unique<Holder<T>>(adapted)) {}

private:
    class HolderBase;

    template <typename T>
    class Holder;

    std::unique_ptr<HolderBase> holder_;
};

class Adapter::HolderBase {
    virtual ~HolderBase() { }
    virtual std::unique_ptr<HolderBase> clone() const = 0;
};

template <typename T>
class Adapter::Holder : Adapter::HolderBase {
    Holder(const T& adapted) : adapted_(adapted) {}

    std::unique_ptr<HolderBase> clone() const override
    {
        return std::make_unique<Holder<T>>(adapted_);
    }

    T adapted_;
};