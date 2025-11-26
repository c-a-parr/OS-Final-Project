

#include <string>
#include <fstream>
#include <iostream>

std::string readLineFromFile(const char *filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open input file.\n";
        std::exit(1);
    }

    std::string line;
    if (!std::getline(in, line)) {
        std::cerr << "Input file is empty.\n";
        std::exit(1);
    }

    return line;
}

void parseInputLine(const std::string &line,
                    char &algo,
                    int &frameCount,
                    int refs[],
                    int &refCount)
{
    algo = 0;
    frameCount = 0;
    refCount = 0;

    int i = 0;
    int n = line.size();

    while (i < n && (line[i] == ' ' || line[i] == '\t'))
        i++;

    if (i < n)
        algo = line[i++];
    else
        return;

    while (i < n && line[i] != ',')
        i++;

    if (i < n && line[i] == ',')
        i++;

    while (i < n && line[i] >= '0' && line[i] <= '9') {
        frameCount = frameCount * 10 + (line[i] - '0');
        i++;
    }

    if (i < n && line[i] == ',')
        i++;

    while (i < n) {

        while (i < n && (line[i] == ',' || line[i] == ' ' || line[i] == '\t'))
            i++;

        if (i >= n) break;

        int val = 0;
        bool hasDigit = false;

        while (i < n && line[i] >= '0' && line[i] <= '9') {
            val = val * 10 + (line[i] - '0');
            hasDigit = true;
            i++;
        }

        if (hasDigit)
            refs[refCount++] = val;
    }
}

void wipeTable(int table[][1000],
               int &frameCount,
               int &refCount)
{
    for (int i = 0; i < frameCount; i++) {
        for (int j = 0; j < refCount; j++) {
            table[i][j] = -1;
        }
    }
}

int findPage(int frames[],
             int frameCount,
             int page)
{
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page)
            return i;
    }
    return -1;
}

void testers()
{
    std::string testLine = readLineFromFile("data/TESTER_CORRECT.txt");
    
    std::cout << "Input Line: " << testLine << "\n";

    int frameCount, refCount;
    char algo;
    int refs[100];

    int displayTable[50][1000];

    parseInputLine(testLine, algo, frameCount, refs, refCount);

    std::cout << "Algorithm: " << algo << "\n";
    std::cout << "Frame Count: " << frameCount << "\n";
    std::cout << "Reference String: ";

    for (int i = 0; i < refCount; i++) {
        std::cout << refs[i] << " ";
    }

    std::cout << "\n";

    wipeTable(displayTable, frameCount, refCount);

    for (int i = 0; i < frameCount; i++) {
        for (int j = 0; j < refCount; j++) {
            std::cout << displayTable[i][j] << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";

}

int FIFO(int frameCount,
         int refs[],
         int refCount,
         int table[][1000])
{
    std::cout << "Running FIFO" << std::endl;

    // Frames[i] = page in frame i, or -1 if empty
    int frames[50];

    // Initialize all frames as empty
    for (int i = 0; i < frameCount; i++)
    {
        frames[i] = -1;
    }

    int pageFaults = 0;

    // FIFO queue pointer/ next frame to replace
    int fifoIndex = 0;

    // t for time
    for (int t = 0; t < refCount; t++)
    {
        int page = refs[t];

        int position = findPage(frames, frameCount, page);

        bool fault = false;

        if (position == -1)
        {
            fault = true;
            pageFaults++;

            // Search for free frame first
            int freeIndex = -1;

            for (int i = 0; i < frameCount; i++)
            {
                if (frames[i] == -1)
                {
                    freeIndex = i;
                    break;
                }
            }

            if (freeIndex != -1)
            {
                // Still space, use first free frame
                frames[freeIndex] = page;
            } else {
                // No free frame, replace using FIFO
                frames[fifoIndex] = page;
                fifoIndex = (fifoIndex + 1) % frameCount;
            }
        }

        // Only write frames into table when a fault occurs
        // Else the column stays blank when printed
        // f for frame
        if (fault)
        {
            for (int f = 0; f < frameCount; f++)
            {
                table[f][t] = frames[f];
            }
        }

    }

    return pageFaults;
}

int OPT(int frameCount,
         int refs[],
         int refCount,
         int table[][1000])
{
    std::cout << "Running OPT" << std::endl;
    return 0;
}

int LRU(int frameCount,
         int refs[],
         int refCount,
         int table[][1000])
{
    std::cout << "Running LRU" << std::endl;
    return 0;
}

void printTable(int refs[],
                int refCount,
                int frameCount,
                int table[][1000],
                int pageFaults)
{

    for (int t = 0; t < refCount; t++) {
        std::cout << refs[t] << " ";
    }
    std::cout << "\n";


    for (int t = 0; t < refCount * 2; t++) {
        std::cout << "-";
    }
    std::cout << "\n";

    
    for (int f = 0; f < frameCount; f++) {
        for (int t = 0; t < refCount; t++) {
            if (table[f][t] == -1)
                std::cout << "  "; 
            else
                std::cout << table[f][t] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Total page faults = " << pageFaults << "\n";
}
         
int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string line = readLineFromFile(argv[1]);
    
    int frameCount, refCount;
    char algo;
    int refs[100];

    int displayTable[50][1000];

    parseInputLine(line, algo, frameCount, refs, refCount);

    wipeTable(displayTable, frameCount, refCount);

    int pageFaults;

    switch (algo)
    {
        case 'F': case 'f':
            pageFaults = FIFO(frameCount, refs, refCount, displayTable);
            break;
        case 'O': case 'o':
            pageFaults = OPT(frameCount, refs, refCount, displayTable);
            break;
        case 'L': case 'l':
            pageFaults = LRU(frameCount, refs, refCount, displayTable);
            break;
    }

    printTable(refs, refCount, frameCount, displayTable, pageFaults);
    
}