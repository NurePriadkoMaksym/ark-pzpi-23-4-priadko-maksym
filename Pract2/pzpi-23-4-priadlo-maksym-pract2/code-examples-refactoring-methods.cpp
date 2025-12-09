// Паганий приклад:
void processFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error opening file\n";
        return;
    }

    std::string line;
    int totalWords = 0;
    int longest = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        int count = 0;

        while (ss >> word) {
            count++;
            if (word.length() > longest) {
                longest = word.length();
            }
        }

        totalWords += count;
    }

    std::cout << "Total words: " << totalWords << "\n";
    std::cout << "Longest word length: " << longest << "\n";
}


//Гарний приклад
class FileProcessor {
public:
    void processFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "Error opening file\n";
            return;
        }

        int totalWords = 0;
        int longest = 0;

        processLines(file, totalWords, longest);
        printStats(totalWords, longest);
    }

private:
    void processLines(std::ifstream& file, int& totalWords, int& longest) {
        std::string line;

        while (std::getline(file, line)) {
            processLine(line, totalWords, longest);
        }
    }

    void processLine(const std::string& line, int& totalWords, int& longest) {
        std::stringstream ss(line);
        std::string word;
        int count = 0;

        while (ss >> word) {
            count++;
            longest = std::max(longest, (int)word.length());
        }

        totalWords += count;
    }

    void printStats(int totalWords, int longest) {
        std::cout << "Total words: " << totalWords << "\n";
        std::cout << "Longest word length: " << longest << "\n";
    }
};


//Поганий приклад
double calculateDelivery(const std::string& type, double base) {
    if (type == "standard") {
        return base;
    } else if (type == "express") {
        return base + 40;
    } else if (type == "international") {
        return base + 120;
    }
    return base;
}

//Гарний приклад
class Delivery {
public:
    virtual ~Delivery() = default;
    virtual double cost(double base) = 0;
};

class StandardDelivery : public Delivery {
public:
    double cost(double base) override { return base; }
};

class ExpressDelivery : public Delivery {
public:
    double cost(double base) override { return base + 40; }
};

class InternationalDelivery : public Delivery {
public:
    double cost(double base) override { return base + 120; }
};

std::unique_ptr<Delivery> makeDelivery(const std::string& type) {
    if (type == "standard") return std::make_unique<StandardDelivery>();
    if (type == "express") return std::make_unique<ExpressDelivery>();
    if (type == "international") return std::make_unique<InternationalDelivery>();
    return std::make_unique<StandardDelivery>();
}


//Поганий приклад
// Order.h
class Order {
public:
    Order(double basePrice, double discount)
        : basePrice(basePrice), discount(discount) {}

    double calculateTotal() const;

private:
    double getBasePrice() const; 
    double basePrice;
    double discount;
};

// Order.cpp
double Order::getBasePrice() const {
    return basePrice;
}

double Order::calculateTotal() const {
    double price = getBasePrice();
    return price - (price * discount);
}

//Гарний приклад
// Order.h
class Order {
public:
    Order(double basePrice, double discount)
        : basePrice(basePrice), discount(discount) {}

    double calculateTotal() const;

private:
    double basePrice;
    double discount;
};

// Order.cpp
double Order::calculateTotal() const {
    double price = basePrice;      // Інлайн замість getBasePrice()
    return price - (price * discount);
}

