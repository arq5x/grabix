#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "bgzf.h"


#define VERSION "0.1.8"
// we only want to store the offset for every 10000th
// line. otherwise, were we to store the position of every
// line in the file, the index could become very large for
// files with many records.
#define CHUNK_SIZE 10000

struct index_info {
    int64_t header_end;
    vector<int64_t> chunk_offsets;
    int64_t num_lines;
};

int usage();

bool bgzf_getline_counting(BGZF * stream);

/*
Create a gbi index for the file to facilitate
random access via the BGZF seek utility
*/
int create_grabix_index(string bgzf_file);

/*
Load an existing gbi index for the file to facilitate
random access via the BGZF seek utility
*/
void load_index(string bgzf_file, index_info &index);

/*
Extract lines [FROM, TO] from file.
*/
int grab(string bgzf_file, int64_t from_line, int64_t to_line);

/*
Extract K random lines from file using reservoir sampling
*/
int random(string bgzf_file, uint64_t K);

/*
Return the total number of records in the index.
*/
int size(string bgzf_file);
