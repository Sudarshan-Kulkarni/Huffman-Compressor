#include "huffman.h"

int main(int argc, char *argv[])
{
    string mode = argv[1];
    string fileName = argv[2];

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
    }    
}
