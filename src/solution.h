
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>

struct Task {
    std::string text;
    std::string date;
    std::map<std::string, int> workloads;
    using Workload = std::map<std::string, int>::value_type;

    Task(const std::string& text, const std::string& date, const std::map<std::string, int>& workloads);
    int getTotalLoad() const;
    bool operator<(const Task& newVector) const;
};

// task 1
Task::Task(const std::string& text, const std::string& date, const std::map<std::string, int>& workloads)
{
    this->text = text;
    this->date = date;
    this->workloads = workloads;
}


struct Worker {
    std::string name;
    std::string login;
    int maxLoad;
    int difference;

    Worker(const std::string& name, const std::string& login, int maxLoad);

    // might be needed for task 5
    bool isOverworked(const std::vector<Task>& tasks) const;
    bool operator<(const Worker& newVector) const;
    bool operator==(const Worker& newVector) const{
        return this->login==newVector.login;
    }
};

// task 1
Worker::Worker(const std::string& name, const std::string& login, int maxLoad)
{
    this->name = name;
    this->login = login;
    this->maxLoad = maxLoad;
    this->difference = difference;
}


// task 2
int Task::getTotalLoad() const
{
    int sum = 0;
    for(auto workload: this->workloads) {
        sum += workload.second;
    }
    return sum;
}

bool Task::operator<(const Task& newVector) const{
    if (this->getTotalLoad() != newVector.getTotalLoad()) 
        return this->getTotalLoad() < newVector.getTotalLoad();
    else {
        if (this->date != newVector.date) 
            return this->date < newVector.date; 
        else 
            return this->text < newVector.text;  
    }
}

bool Worker::operator<(const Worker& newVector) const{
    return (this->login < newVector.login);
}

// task 3
void sortTasks(std::vector<Task>& tasks)
{
    std::sort(tasks.begin(), tasks.end());
}


// task 4
int getWorkerLoad(const std::vector<Task>& allTasks, const Worker& worker)
{
    int totalLoad = 0;
    std::vector<Task> allDanilas;
    std::copy_if(allTasks.begin(), allTasks.end(), std::back_inserter(allDanilas), [&worker, &totalLoad](const Task& task) {
        for (const auto& x : task.workloads) {
            if (x.first == worker.login){
                totalLoad += x.second;
                return true; 
            }
        }
        return false;
    });


    // replace with your own return
    return totalLoad;
}

// task 5
void addTask(std::vector<Task>& tasks, const Task& newTask, const std::vector<Worker>& workers)
{
    for(auto workload: newTask.workloads) {
        int newLoad = workload.second;
        std::string login = workload.first;
        for(Worker worker: workers) {
            if (worker.login == login) {
                int load = getWorkerLoad(tasks, worker) + newLoad;
                // std::cout << ";;;;;  " << load << '\t' << newLoad <<  '\t'<<  worker.maxLoad <<'\n';

                if (worker.maxLoad < load) {
                    throw std::runtime_error("Overworked");
                }
                else {
                    tasks.push_back(newTask);
                }
                break;
            }
        }
    }
}

// task 6
void addTaskVerbose(std::vector<Task>& tasks, const Task& newTask, const std::vector<Worker>& workers)
{
    std::vector<Worker> overWorkedPeople;

    for(auto workload: newTask.workloads) {
        int newLoad = workload.second;
        std::string login = workload.first;
        for(Worker worker: workers) {

            int load = (worker.login == login) ? getWorkerLoad(tasks, worker) + newLoad :  getWorkerLoad(tasks, worker);
            // std::cout << worker.login << "\t ffffff" << load << '\t' << newLoad <<  '\t'<<  worker.maxLoad <<'\n';
            auto it = find(overWorkedPeople.begin(), overWorkedPeople.end(), worker);
            if (worker.maxLoad < load && it==overWorkedPeople.end()) {
                worker.difference = load - worker.maxLoad;

                overWorkedPeople.push_back(worker);
                // throw std::runtime_error("Overworked");
            }
            else if (worker.login == login) {
                tasks.push_back(newTask);
            }

        }
    }

    std::sort(overWorkedPeople.begin(), overWorkedPeople.end());
    std::string message = "Overworked:";
    for(auto worker: overWorkedPeople) {
        message += " " + worker.login + " by " + std::to_string(worker.difference) + ",";
    }
    if(!overWorkedPeople.empty()) {
        message = message.substr(0, message.size() - 1);
        throw std::runtime_error(message);

    }
}
