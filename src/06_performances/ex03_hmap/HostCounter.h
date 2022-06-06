#include <string>

#include <unordered_map>

class HostCounter
{
    public:
        HostCounter();

        // Announce a host to the HostCounter.
        // if the host is new, it will be added to the list, otherwise we ignore it.
        void notifyHost(std::string hostname);

        // return the number of unique hosts found so far
        int getNbOfHosts();

    private:
        // check if host is already in the list
        bool isNewHost(std::string hostname);

        std::unordered_map< std::string , int > myHosts; 
};