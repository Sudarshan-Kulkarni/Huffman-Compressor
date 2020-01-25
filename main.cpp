#include "huffman.h"

bool fileExists(string fileName)
{
    fstream infile(fileName.c_str());
    return infile.good();
}

int main(int argc, char *argv[])
{
    if(argc!=3)
    {
        cout<<"Wrong number of arguments provided."<<endl<<"Usage:'./app.exe <-(c/d)> <filepath>";
        return -1;
    }

    string mode = argv[1];
    string fileName = argv[2];

    if(!fileExists(fileName))
    {
        cout<<"File doesn't exist"<<endl;
        return -1;
    }
    
    Huffman  h;

    if(mode=="-c")
    {
        h.compressFile(fileName);    
    }
    else if(mode=="-d") 
    {
        h.decompressFile(fileName);        
    }
    else
    {
        cout<<"Invalid mode entered"<<endl;
        return -1;
    }    
}
