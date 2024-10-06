#include <cassert>
#include "Matrix.hpp"

#include <iostream>


// EFFECTS:  Initializes *mat as a Matrix with the given width and height.
void Matrix_init(Matrix* mat, int width, int height) {
  mat->width = width;
  mat->height = height;
}

// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  os << mat->width << " " << mat->height << "\n";

  for (int i = 0; i < mat->height; i++) {
    for (int j = 0; j < mat->width; j++) {
      os << mat->data[i * mat->width + j] << " "; //NOT SURE ABOUT THIS
    }
    os << "\n";
  }
}


// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  return mat->width;
}

// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  return mat->height;
}

// EFFECTS:  Returns the row of the element pointed to by ptr.
int Matrix_row(const Matrix* mat, const int* ptr) { //PLEASE CHECK THIS!!

  return (ptr - mat->data) / mat->width;
} //


// EFFECTS:  Returns the column of the element pointed to by ptr.
int Matrix_column(const Matrix* mat, const int* ptr) { //AND CHECK THIS!!
  return (ptr - mat->data) % mat->width;
}
//| (• ◡•)| (❍ᴥ❍ʋ


// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  return &mat->data[row * mat->width + column];
}


// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  return &mat->data[row * mat->width + column]; //should be same as above
}


// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  for (int i = 0; i < mat->height * mat->width; i++) {
      mat->data[i] = value;
  }
}


// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  for (int i = 0; i < mat->height; i++) {
    for (int j = 0; j < mat->width; j++) {
      if (i == 0 || i == mat->height - 1 || j == 0 || j == mat->width - 1) {
        mat->data[i * mat->width + j] = value; //check pls
      }
    }
  }
}

// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int maxVal = mat->data[0];
  for (int i = 1; i < mat->height * mat->width; i++) {
    if (mat->data[i] > maxVal) {
      maxVal = mat->data[i];
    }
  }

  return maxVal;
}


// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  int minVal = mat->data[row * mat->width + column_start];
  int minColumn = column_start;

  for (int j = column_start + 1; j < column_end; j++) {
    if (mat->data[row * mat->width + j] < minVal) {
      minVal = mat->data[row * mat->width + j];
      minColumn = j;
 }  
 }

  return minColumn;
}


// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  int minVal = mat->data[row * mat->width + column_start];

  for (int j = column_start + 1; j < column_end; j++) {
    if (mat->data[row * mat->width + j] < minVal) {
      minVal = mat->data[row * mat->width + j];
    }
  }

  return minVal;
}
