#include <cassert>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);


  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}

// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
    int width = Image_width(img);
    int height = Image_height(img);
    Matrix_init(energy, width, height);
    Matrix_fill(energy, 0);

    // Iterate over inner pixels to avoid boundary checks
    for (int r = 1; r < height - 1; r++) {
        for (int c = 1; c < width - 1; c++) {
            Pixel N = Image_get_pixel(img, r - 1, c);
            Pixel S = Image_get_pixel(img, r + 1, c);
            Pixel W = Image_get_pixel(img, r, c - 1);
            Pixel E = Image_get_pixel(img, r, c + 1);
            
            // Calculate energy for the current pixel
            int energyofX = squared_difference(N, S) + squared_difference(W, E);
            *Matrix_at(energy, r, c) = energyofX;
        }
    }
    
    // Find maximum energy within the inner region
    int max_energy = Matrix_max(energy);
    
    // Fill the border of the energy matrix with the maximum energy value
    Matrix_fill_border(energy, max_energy);
}







// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
    // Initialize the cost Matrix with the same size as the energy Matrix.
    int width = Matrix_width(energy);
    int height = Matrix_height(energy);
    Matrix_init(cost, width, height);

    // Fill in costs for the first row (index 0) using energy matrix directly
    for (int c = 0; c < width; ++c) {
        *Matrix_at(cost, 0, c) = *Matrix_at(energy, 0, c);
    }

    for (int r = 1; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            // Computes cost for current pixel
            double minimumPrevCost = 
            Matrix_min_value_in_row(cost, r - 1, max(0, c - 1), min(width, c + 2));
            *Matrix_at(cost, r, c) = *Matrix_at(energy, r, c) + minimumPrevCost;
        }
    }
}



// REQUIRES: cost points to a valid Matrix
//           seam points to an array
//           the size of seam is >= Matrix_height(cost)
// MODIFIES: seam[0]...seam[Matrix_height(cost)-1]
// EFFECTS:  seam serves as an "output parameter".
//           The vertical seam with the minimal cost according to the given
//           cost matrix is found and the seam array is filled with the column
//           numbers for each pixel along the seam, with the pixel for each
//           row r placed at seam[r]. While determining the seam, if any pixels
//           tie for lowest cost, the leftmost one (i.e. with the lowest
//           column number) is used.
//           See the project spec for details on computing the minimal seam.
// NOTE:     You should compute the seam in reverse order, starting
//           with the bottom of the image and proceeding to the top,
//           as described in the project spec.
void find_minimal_vertical_seam(const Matrix* cost, int seam[]) {  
  int bottomRow = Matrix_height(cost) - 1;
  int col = Matrix_column_of_min_value_in_row(cost, bottomRow, 0, Matrix_width(cost));
    while (bottomRow >= 0) {

      if (bottomRow ==Matrix_height(cost) - 1) {
        seam[bottomRow] = col;
        } else {
        col = Matrix_column_of_min_value_in_row(cost, bottomRow, 
        max(0, col - 1), min(Matrix_width(cost) - 1, col + 2));
        seam[bottomRow] = col;
      }
      bottomRow--;
    } 

}


// REQUIRES: img points to a valid Image with width >= 2
//           seam points to an array
//           the size of seam is == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Use the new operator here to create the smaller Image,
//           and then use delete when you are done with it.
void remove_vertical_seam(Image *img, const int seam[]) {
  //create new image to copy in
  Image *copy = new Image;
  int copy_width = Image_width(img) - 1;
  int copy_height = Image_height(img);
  Image_init(copy, copy_width, copy_height);

  for (int row = 0; row < copy_height; row++) {
    // iterate through squares before seam
    for (int col = 0; col < seam[row]; col++) {
      Pixel copy_pixel;
      copy_pixel = Image_get_pixel(img, row, col);
      Image_set_pixel(copy, row, col, copy_pixel);
    }
    
    // iterate through squares after seam
    for (int col = seam[row] + 1; col < copy_width + 1; col++) {
      Pixel copy_pixel;
      copy_pixel = Image_get_pixel(img, row, col);
      Image_set_pixel(copy, row, col - 1, copy_pixel);
    }
  }

  
  
  // clear image
  Pixel black;
  black.r = 0;
  black.g = 0;
  black.b = 0;
  
  Image_fill(img, black);
  
  *img = *copy;

  delete copy;
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use the new operator here to create Matrix objects, and
//           then use delete when you are done with them.
// NOTE:     The size of an array local variable must be a compile-time
//           constant. You can use MAX_MATRIX_HEIGHT as the constant,
//           since an image's height can be no larger than that.
void seam_carve_width(Image *img, int newWidth) {
      int widthDiff= Image_width(img) - newWidth;
      int x = 0; // Initialized x

    while (x < widthDiff) {
      Matrix* energy = new Matrix(); // Corrected memory allocation
      Matrix* cost = new Matrix(); // Corrected memory allocation
      compute_energy_matrix(img, energy);
      compute_vertical_cost_matrix(energy, cost);
      int* seam = new int[Matrix_height(cost)]; // allocate memory for seam array
      find_minimal_vertical_seam(cost, seam); // Removed square brackets
      remove_vertical_seam(img, seam); // Removed square brackets
      x++;
      delete cost; // Deallocated memory
      delete energy; // Deallocated memory
      delete[] seam; // Deallocate memory for seam array
    }


}



// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  rotate_left(img);
  seam_carve_width(img, newHeight);
  rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight);
}