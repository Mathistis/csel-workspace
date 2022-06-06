#include "HostCounter.h"

#include <algorithm> // for std::find

HostCounter::HostCounter()
{
}

// bool HostCounter::isNewHost(std::string hostname)
// {

//     return myHosts.find(hostname) == myHosts.end();
// }

void HostCounter::notifyHost(std::string hostname)
{
    // add the host in the list if not already in
    myHosts.insert(std::pair<std::string, int>(hostname, 1)).second;
}

int HostCounter::getNbOfHosts()
{
    return myHosts.size();
}