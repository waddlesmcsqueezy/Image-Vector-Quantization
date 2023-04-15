// IanHarmon-CS4378-Assign01.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "IanHarmon-CS4378-Assign06.h"

using namespace std;
const int INTENSITY = 255;

int get_quantization_range(int _gray_level)
{
	return (INTENSITY / _gray_level) - 1;
}

// sum of pixels values in a quad divided by the number of pixel in the quad
int pixel_get_quantized_value(int _pixel, int _gray_level)
{
	return _pixel / get_quantization_range(_gray_level);
}

int main()
{
	// determines the variance threshold (info in assignment rubric, user input)
	int gray_level,
		// width and height of the image (retrieved from the file itself)
		image_width,
		image_height;
		// grayscale of the image (retrieved from the file itself)
		//image_intensity;

	// determines the name of the input image for the program (user input)
	string image_name,
	       output_name,
		// string of the current line of the input image
		   line;

	ifstream fin;
	stringstream ss;

	image_name;

	cout << "MAKE SURE TO INCLUDE DOUBLE SLASHES IN THE FILE NAME, EG; C:\\\\Users\\\\name\\\\Downloads\\\\baboon\\\\baboon.pgma" << endl;
	// request user input of image input and output name
	cout << "Please enter the exact path name of the INPUT image: ";
	cin >> image_name;
	cout << "Please enter the exact path name of the desired OUTPUT image: ";
	cin >> output_name;

	// request user input of the desired variance threshold for the image/program
	cout << "Please enter gray levels for quantization: ";
	cin >> gray_level;

	gray_level -= 1;

	// open the requested file
	fin.open(image_name);

	if (fin.fail() == true) {
		cout << "ERROR: File " << image_name << " not found";
		return -1;
	}

	// check if is a P2 image, exit if not
	getline(fin, line);
	if (line.compare("P2")) {
		cout << "ERROR: input file is not of type \"P2\", please fix the image file and try again.";
		return -1;
	}
	// get comment line from input file, do nothing with it
	getline(fin, line);
	// Continue with a stringstream
	ss << fin.rdbuf();
	// Third line : size
	ss >> image_width >> image_height;


	// initialize the vectors we will be using to modify the image
	vector<vector<int>> image_vector(image_width, vector<int>(image_height));
	vector<vector<int>> result_vector(image_width, vector<int>(image_height));

	
	//reading the actual pixel values
	int current_x = 0,
	current_y = 0;

	if (fin.is_open()) {
		while (fin.good()) {

			getline(fin, line);

			string word;
			while (ss >> word) {
				image_vector[current_x][current_y] = stoi(word);
				result_vector[current_x][current_y] = pixel_get_quantized_value(image_vector[current_x][current_y], gray_level);
				current_x += 1;

				if (current_x == result_vector.size()) {
					current_x = 0;
					current_y += 1;
				}
			}
		}
	}

	current_x = 0,
	current_y = 0;


	int linebreak_threshold = 0;

	ofstream fout;
	fout.open(output_name);  // eg; "C:\\Users\\name\\Downloads\\baboon\\output.pgma"

	fout << "P2" << endl
		<< "# output.pgma created by Ian Harmon's program IanHarmon-CS4378-Assign01." << endl
		<< result_vector.size() << "  " << result_vector[0].size() << endl;

	if (fout.is_open()) {
		while (current_y < image_height) {
			fout << result_vector[current_x][current_y] << "  ";

			current_x += 1;
			linebreak_threshold += 1;
			if (linebreak_threshold == 12) {
				fout << "\n";
				linebreak_threshold = 0;
			}
			if (current_x == image_width) {
				current_x = 0;
				current_y += 1;
			}
		}
	}



	fout.close();

	fin.close();

	cout << "Program finished with no errors. Output written to " << output_name << endl;

	return 0;
}