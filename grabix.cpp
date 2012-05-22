#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "bgzf.h"

// we only want to store the offset for every 10000th
// line. otherwise, were we to store the position of every
// line in the file, the index could become very large for
// files with many records.
#define CHUNK_SIZE 10000

struct index_info {
    vector<size_t> chunk_offsets;
    size_t num_lines;
};

int usage()
{
    cout << "usage: grabix index bgzf_file " << endl;
    cout << "       grabix grab bgzf_file line_start [line_end] " << endl;
    cout << endl;
    cout << "examples:" << endl;
    cout << "       # create a grabix index (big.vcf.gz.gbi)" << endl;
    cout << "       grabix index big.vcf.gz" << endl;
    cout << endl;
    cout << "       # extract the 100th line." << endl;
    cout << "       grabix grab big.vcf.gz 100 [line_end] " << endl;
    cout << endl;
    cout << "       # extract the 100th through the 200th lines." << endl;
    cout << "       grabix grab big.vcf.gz 100 200 " << endl;
    cout << endl;
    return EXIT_SUCCESS;
}

// from http://biostars.org/post/show/13595/ \
   random-access-of-lines-in-a-compressed-file-having-a-custom-tabulated-format/#13616
void bgzf_getline (BGZF * stream, string & line)
{
    line.erase();
    int c = -1, i = 0;
    char * p = (char *) malloc (1);
    if (p == NULL)
    {
        fprintf (stderr, "ERROR: can't allocate memory\n");
        exit (1);
    }
    while (true)
    {
        ++i;
        c = bgzf_getc (stream);
        if (c == -2)
        {
            fprintf (stderr, "ERROR: can't read %i-th character\n", i);
            exit (1);
        }
        if (c == -1) // reach end of file
            break;
        else if (c == 10) // \n
            break; 
        sprintf (p, "%c", c);
        line.append(p);
    }
    free (p);
}

int create_grabix_index(string bgzf_file)
{
    BGZF *bgzf_fp = bgzf_open(bgzf_file.c_str(), "r");
    if (bgzf_fp == NULL)
    {
        cerr << "[grabix] could not open file:" << bgzf_file << endl;
        exit (1);
    }
    
    //kstring_t *line = new kstring_t;
    string line;
    size_t offset;
    size_t chunk_count = 0;
    size_t total_lines = 0;
    vector<size_t> chunk_positions;
    chunk_positions.push_back (0);
    while (bgzf_check_EOF(bgzf_fp) == 1)
    {
        // grab the next line and store the offset
        // bgzf_getline(bgzf_fp, '\n', line);
        bgzf_getline(bgzf_fp, line);
        offset = bgzf_tell (bgzf_fp);
        chunk_count++;
        total_lines++;
        // stop if we have encountered an empty line
        if (line.empty())
            break;
        // store the offset of this chunk start
        else if (chunk_count == CHUNK_SIZE) 
        {
            chunk_positions.push_back(offset);
            chunk_count = 0;
        }
    }
    bgzf_close(bgzf_fp);
    
    // write the index
    string index_file_name = bgzf_file + ".gbi";
	ofstream index_file(index_file_name.c_str(), ios::out);
	
    index_file << total_lines << endl;
    for (size_t i = 0; i < chunk_positions.size(); ++i)
    {
        index_file << chunk_positions[i] << endl;
    }
    index_file.close();
    
    return EXIT_SUCCESS;
}


void load_index(string bgzf_file, index_info &index)
{
    string index_file_name = bgzf_file + ".gbi";
	// open the index file for reading
	ifstream index_file(index_file_name.c_str(), ios::in);

	if ( !index_file ) {
		cerr << "[grabix] coould not find index file: " << index_file_name << ". Exiting!" << endl;
		exit (1);
	}
	else {
        string line;
	    getline (index_file, line);
        index.num_lines = atoi(line.c_str());
	    
	    while (index_file >> line)
	    {
            index.chunk_offsets.push_back(atoi(line.c_str()));
	    }
	}
	index_file.close();
}

int grab(string bgzf_file, size_t from_line, size_t to_line)
{
    // load index into vector of offsets
    index_info index;
    load_index(bgzf_file, index);
    
    if ((from_line > index.num_lines) 
         || 
        (to_line > index.num_lines))
    {
        cerr << "[grabix] requested lines exceed the number of lines in the file." << endl;
        exit(1);
    }
    else if (from_line < 0) 
    {
        cerr << "[grabix] requested line exceeds the number of lines in the file." << endl;
        exit(1);
    }
    else if (from_line > to_line) 
    {
        cerr << "[grabix] requested end line is less than the requested begin line." << endl;
        exit(1);
    }
    else {
        // load the BGZF file
        BGZF *bgzf_fp = bgzf_open(bgzf_file.c_str(), "r");
        if (bgzf_fp == NULL)
        {
            cerr << "[grabix] could not open file:" << bgzf_file << endl;
            exit (1);
        }
        // easier to work in 0-based space
        size_t from_line_0  = from_line - 1;
        size_t to_line_0    = to_line - 1;
        // get the chunk index for the requested line
        size_t requested_chunk = from_line_0 / CHUNK_SIZE;
        // derive the first line in that chunk
        size_t chunk_line_start = (requested_chunk * CHUNK_SIZE);

        // jump to the correct offset for the relevant chunk
        // and fast forward until we find the requested line 
        // kstring_t *line = new kstring_t;       
        string line;
        bgzf_seek (bgzf_fp, index.chunk_offsets[requested_chunk], SEEK_SET);        
        while (chunk_line_start <= from_line_0)
        {
            bgzf_getline(bgzf_fp, line);
            chunk_line_start++;
        }
        
        // now, print each line until we reach the end of the requested block
        printf("%s\n", line.c_str());
        while (chunk_line_start <= to_line_0)
        {
            bgzf_getline (bgzf_fp, line);
            printf("%s\n", line.c_str());
            chunk_line_start++;
        }
    }
    return EXIT_SUCCESS;
}

int main (int argc, char **argv)
{
    if (argc == 1) { return usage(); }
    if (argc >= 3) 
    {
        // create input file for the purpose of the example
        string bgzf_file = argv[2];
        
        string sub_command = argv[1];
        if (sub_command == "index")
        {
            create_grabix_index(bgzf_file);
        }
        else if (sub_command == "grab")
        {
            size_t from_line = atoi(argv[3]);
            size_t to_line = from_line;
            if (argc == 5)
                to_line = atoi(argv[4]);
            
            grab(bgzf_file, from_line, to_line);
        }
    }
    
    return EXIT_SUCCESS;
}