// replication.hpp
#pragma once

class Replication {
public:
    Replication();
    ~Replication();

    void syncState();
    void mergeUpdates();
};