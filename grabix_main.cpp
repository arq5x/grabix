#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "bgzf.h"
#include "grabix.h"

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
            int64_t from_line = atol(argv[3]);
            int64_t to_line = from_line;
            if (argc == 5)
                to_line = atol(argv[4]);

            grab(bgzf_file, from_line, to_line);
        }
        else if (sub_command == "random")
        {
            size_t N = atoi(argv[3]);
            random(bgzf_file, N);
        }
        else if (sub_command == "check")
        {
          cout << ((bgzf_is_bgzf(bgzf_file.c_str()) == 1) ? "yes" : "no") << "\n";
        }
        else if (sub_command == "size")
        {
          cout << size(bgzf_file) << "\n";
        }
    }

    return EXIT_SUCCESS;
}
