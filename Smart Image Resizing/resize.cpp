#include <iostream>
#include <fstream>
#include <string>
#include "processing.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
             << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }

    string ifilename = argv[1];
    string ofilename = argv[2];
    int des_width = stoi(argv[3]);
    int des_height; stoi(argv[4]);

    ifstream is(ifilename);

    if (!is.is_open()) {
        cout << "Error opening file: " << ifilename << endl;
        return 1;
    }

    string p3;
    int img_width, img_height;
    is >> p3 >> img_width >> img_height;
    is.close(); // Close the input file stream

    if (argc == 4) {
        des_height = img_height;
    } else {
        des_height = stoi(argv[4]);
    }

    if (des_width > img_width || des_width <= 0 ||
        des_height > img_height || des_height <= 0) {
        cout << "Error: Desired width and height must be less ";
        cout << "than or equal to original" << endl;
        return 1;
    }

    Image* img = new Image;
    ifstream is2(ifilename); // Reopen the file
    Image_init(img, is2); // Initialize the image from the file
    is2.close(); // Close the input file stream

    ofstream os(ofilename);

    if (!os.is_open()) {
        cout << "Error opening file: " << ofilename << endl;
        delete img; // Deallocate memory before returning
        return 1;
    }

    seam_carve(img, des_width, des_height);

    Image_print(img, os);

    os.close();
    delete img; // Deallocate memory
    return 0;
}