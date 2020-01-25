#include "huffman.h"

/*
Functions to compress the file 
and the respective helper functions are given below
*/


// function to flush the contents of the previous huffman tree using post order traversal
void Huffman::traverseFlush(Huffman::Node* root)
{
    if(root==NULL)
        return;

    traverseFlush(Huffman::root->leftChild);
    traverseFlush(Huffman::root->rightChild);

    free(root);    
}

// function to flush the contents of any residual memory from previous operations
void Huffman::memFlush()
{
    if(file_in)
        file_in.close();
    if(file_out)
        file_out.close();
    
    fileName.clear();
    fileContents.clear();
    frequencyTable.clear();
    pq = priority_queue<Node*, vector<Node*>, compare>();

    traverseFlush(root);
    codeMap.clear();
}


//function to read text from a file
string Huffman::readFileContents(string fileName)
{
    string contents;
    file_in.open(fileName, ios::in); 
    
    //reading the contents of the file into a string all at once, by specifying the range(beginning iterator to the end)
    contents.assign(istreambuf_iterator<char>(file_in),istreambuf_iterator<char>());

    file_in.close();
    return contents;
}

// function to construct the frequency table.
map<char, long long int> Huffman::constructFrequencyTable(string fileContents)
{
    map<char, long long int>freqTable;
    for(char c:fileContents)
    {        
        if(freqTable.find(c)==freqTable.end())
        {
            freqTable[c] = 1;
        }
        else
        {
            freqTable[c]++;
        }        
    }
    return freqTable;

}


void Huffman::constructPriorityQueue()
{
    for(auto it=frequencyTable.begin();it!=frequencyTable.end();it++)
    {
        Node* tmp = new Node(it->first, it->second);     
        pq.push(tmp);
    }
}

Huffman::Node* Huffman::constructHuffmanTree()
{
    
    priority_queue<Node*, vector<Node*>, compare>tmp(pq);

    while(tmp.size()>1)
    {
        Node* r = new Node();
        r->freq = 0;
        r->leftChild=tmp.top();
        r->freq+=tmp.top()->freq;
        tmp.pop();

        r->freq+=tmp.top()->freq;
        r->rightChild = tmp.top();
        tmp.pop();

        tmp.push(r);
    }
    return tmp.top();
}


// Function to traverse the tree
void Huffman::traverseTree(Huffman::Node* cur, string code)
{
    if(cur->leftChild==NULL && cur->rightChild==NULL)
    {
        codeMap[cur->id] = code;
        return;
    }
    traverseTree(cur->leftChild, code+"0");    
    traverseTree(cur->rightChild, code+"1");    
}

// writes the length of the header for later use(seek) and then writes the file header
void Huffman::writefileHeader()
{
    long long int headerSize=0;
    vector<string>toWriteVec;
    for(auto it=frequencyTable.begin();it!=frequencyTable.end();it++)
    {
        string toWrite="";
        toWrite+=it->first;
        toWrite+="-";
        toWrite+=to_string(it->second);
        toWriteVec.push_back(toWrite);
        headerSize+=toWrite.size()+1;
    }
    headerSize--;
    headerSize+=4;



    long long int tmp = to_string(headerSize).size();
    headerSize = to_string(headerSize+tmp).size()==to_string(headerSize).size()?headerSize+tmp:headerSize+tmp+1;

    string str_headerSize = to_string(headerSize);

    file_out.write(str_headerSize.c_str(),str_headerSize.size());
    file_out.write("||",2);
    
    for(long long int i=0;i<toWriteVec.size();i++)
    {
        file_out.write(toWriteVec[i].c_str(), toWriteVec[i].size());
        if(i==toWriteVec.size()-1)
            break;
        file_out.write(",", 1);
    }
    file_out.write("||",2);
}

// outputs the huffman code to the file in binary format
void Huffman::writeCompressedFileContents()
{
    string fileCode="";
    for(char c:fileContents)
    {
        fileCode+=codeMap[c];
    }

    char bitbuffer=0;
    int bitcount=0;
    for(char c:fileCode)
    {
        bool x = c=='1'?1:0;
        bitbuffer=(bitbuffer<<1)|x;
        bitcount++;
        if(bitcount==8)
        {
            file_out.write(&bitbuffer, sizeof(bitbuffer));
            bitcount=0;
            bitbuffer=0;
        }
    }  

    while(bitcount>0 && bitcount<8)
    {
        bitbuffer=(bitbuffer<<1)|0;
        bitcount++;
        if(bitcount==8)
        {
            file_out.write(&bitbuffer, sizeof(bitbuffer));
            bitcount=0;
            bitbuffer=0;
        }
    }  

}

// Write the file header (information necessary to reconstruct the tree), and write the code for the file
void Huffman::writeToFile()
{
    string hufFileName = fileName;
    hufFileName.replace(hufFileName.size()-3, hufFileName.size(), "huf");

    file_out.open(hufFileName, ios::out|ios::binary);
    writefileHeader();
    file_out.close();

    file_out.open(hufFileName, ios::app|ios::out|ios::binary);
    writeCompressedFileContents();
    file_out.close();

}


// function to compress the file.
void Huffman::compressFile(string fileName)
{
    // clearing any residual memory from previous operations
    memFlush();

    this->fileName = fileName;
    fileContents = readFileContents(fileName);

    // Constructing the frequency table
    frequencyTable = constructFrequencyTable(fileContents);

 
    // constructing the priority queue
    constructPriorityQueue();

    root = constructHuffmanTree(); 

    // Now we must save the codes in a map inorder to substitute the original text with the huffman codes
    traverseTree(root, "");

    // Write the file
    writeToFile();
}

/*
Functions to decompress the file 
and the respective helper functions are given below
*/

map<char, long long int> Huffman::reconstructFrequencyTable(string fileContents)
{
    map<char, long long int>frequencyTable;
    int state=0;
    char key;
    long long int fileHeaderBegin = fileContents.find("||")+2;
    long long int fileHeaderEnd = fileContents.find("||", fileHeaderBegin+1);
    for(long long int i=fileHeaderBegin;i<fileHeaderEnd;i++)
    {   
        string strFreq;
        switch (state)
        {
        case 0:
            key=fileContents[i];
            state++;
            i++;
            break;
        
        case 1:
            strFreq="";
            while(isdigit(fileContents[i]))
            {
                strFreq+=fileContents[i++];
            }
            frequencyTable[key] = stoll(strFreq);
            state--;
            break;        
        default:
            break;
        }
    }
    return frequencyTable;
}

// traverses the tree and decompresses the huffman code into a string
string Huffman::translateHuffman(vector<bool>cc)
{
    Node* r = root;
    long long int pos = 0;
    string decompressedContents = "";
    while(pos!=cc.size())
    {
        if(r->leftChild==NULL && r->rightChild==NULL)
        {
            decompressedContents+=r->id;
            r = root;
        }
        if(!cc[pos])
            r = r->leftChild;
        else
            r = r->rightChild;
        
        pos++;
    }
    return decompressedContents;
}


string Huffman::decompressUtil()
{
    long long int headerSize;
    vector<bool>compressedContents;
    char byt = 0;
    string str_headerSize="";
    while (byt!='|')
    {
        file_in.read(&byt, 1);
        str_headerSize+=byt;
    }
    headerSize = stoi(str_headerSize);
    file_in.seekg(headerSize, ios::beg);

    while(!file_in.eof())
    {
        file_in.read(&byt, 1);
        vector<bool>tmp;
        for(int i=0;i<8;i++)
        {
            tmp.push_back(byt&1);
            byt>>=1;
        }
        reverse(tmp.begin(),tmp.end());
        compressedContents.insert(compressedContents.end(), tmp.begin(), tmp.end());
    }
   
    return translateHuffman(compressedContents);       
}

// main control function for decompression
void Huffman::decompressFile(string fileName)
{
    // clearing any residual memory from previous operations
    memFlush();

    this->fileName = fileName;
    fileContents = readFileContents(fileName);


    frequencyTable = reconstructFrequencyTable(fileContents);


    // constructing the priority queue
    constructPriorityQueue();

    root = constructHuffmanTree();

    // Now we must save the codes in a map inorder to substitute the original text with the huffman codes
    traverseTree(root, "");


    file_in.open(fileName, ios::in|ios::binary);
    string decompressedContents = decompressUtil();
    file_in.close();

    
    string dirName = "Extracted "+ fileName.substr(0,fileName.size()-4);
    string fileNameToWrite = fileName.replace(fileName.size()-3, fileName.size(),"txt");
    if(chdir(dirName.c_str()))
    {
        mkdir(dirName.c_str());
        chdir(dirName.c_str());
    }

    file_out.open(fileNameToWrite, ios::out);
    file_out.write(decompressedContents.c_str(),decompressedContents.size());
    file_out.close();   
}

