#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

int main( )
{
    boost::filesystem::fstream file("ttt.cpp", std::ios::out| std::ios::in | std::ios::binary);
   // file << "asdfasdf";
    file.seekp(153);
    file.write("AAABBBCCCRRRAAA", 15);
    file.close( );

    return 0;
}