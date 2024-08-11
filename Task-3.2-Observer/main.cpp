//
//  main.cpp
//  Task-3.2-Observer
//
//  Created by Маргарет  on 10.08.2024.
//

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>


class Observer {
public:
virtual ~Observer() = default;
virtual void onWarning(const std::string message) {}
virtual void onError(const std::string message) {}
virtual void onFatalError(const std::string message){}
};

class WarnObserver : public Observer {
void onWarning(const std::string message) {
  std::cout << "Warning: " << message << "\n";
}
};

class ErrorObserver : public Observer {
void onError(const std::string message) {
    std::ofstream f;
    f.open("errorfile.txt", std::fstream::app);
    if (f.is_open()) {
        f << "Error: " << message << "\n";
    }
    f.close();
}
};

class FatalErrorObserver : public Observer {
    
void onFatalError(const std::string message) {
    std::cout << "Fatal error: " << message << "\n";
    std::ofstream f;
    f.open("fatalerrorfile.txt", std::fstream::app);
    if (f.is_open()) {
        f << "Fatal error: " << message << "\n";
    }
    f.close();
}
};


class Observed {
public:

void addobserver(std::weak_ptr<Observer> observer) {
 observers_.push_back(observer);
}
void warning(const std::string message) const {
  for (auto& observer : observers_) {
    if(auto strong_ptr = observer.lock()) {
      strong_ptr->onWarning(message);
    }
  }
}


void error(const std::string message) const {
  for (auto& observer : observers_) {
    if(auto strong_ptr = observer.lock()) {
      strong_ptr->onError(message);
    }
  }
}


void fatalerror(const std::string message) const {
  for (auto& observer : observers_) {
    if(auto strong_ptr = observer.lock()) {
      strong_ptr->onFatalError(message);
    }
  }
}

private:
std::vector<std::weak_ptr<Observer>>observers_;
};

int main(int argc, const char * argv[]) {
    Observed observed;
    auto ptr_1 = std::make_shared<WarnObserver>();
    std::weak_ptr<WarnObserver> ptr_2 = ptr_1;
    observed.addobserver(ptr_2);
    observed.warning("mistake");
    
    auto ptr_3 = std::make_shared<ErrorObserver>();
    std::weak_ptr<ErrorObserver> ptr_4 = ptr_3;
    observed.addobserver(ptr_4);
    observed.error("mistake");
    
    auto ptr_5 = std::make_shared<FatalErrorObserver>();
    std::weak_ptr<FatalErrorObserver> ptr_6 = ptr_5;
    observed.addobserver(ptr_6);
    observed.fatalerror("mistake");
    
    return 0;
}
