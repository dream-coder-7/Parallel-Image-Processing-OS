#include <bits/stdc++.h>
#include <fstream>
using namespace std;



void grey_convert(int hei, int wid, int max_val, vector<vector<vector<int>>>& image) {
    
    vector<vector<int>> grayscale(hei, vector<int>(wid));

    for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            grayscale[i][j] = (image[i][j][0] + image[i][j][1] + image[i][j][2]) / 3;
        }
    }
     
     //update image
    for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            image[i][j][0] = grayscale[i][j];
            image[i][j][1] = grayscale[i][j];
            image[i][j][2] = grayscale[i][j];
        }
    }
}

void blur_convert(int hei, int wid, int max_val, vector<vector<vector<int>>>& image) {

    vector<vector<vector<int>>> blurred(hei, vector<vector<int>>(wid, vector<int>(3)));

    for (int i = 1; i < hei - 1; i++) {
        for (int j = 1; j < wid - 1; j++) {
            for (int k = 0; k < 3; k++) {
                int sum = image[i - 1][j - 1][k] + image[i - 1][j][k] + image[i - 1][j + 1][k] +
                          image[i][j - 1][k] + image[i][j][k] + image[i][j + 1][k] +
                          image[i + 1][j - 1][k] + image[i + 1][j][k] + image[i + 1][j + 1][k];
                blurred[i][j][k] = sum / 9;
            }
        }
    }

    // update image 
    for (int i = 1; i < hei - 1; i++) {
        for (int j = 1; j < wid - 1; j++) {
            for (int k = 0; k < 3; k++) {
                image[i][j][k] = blurred[i][j][k];
            }
        }
    }
}

void write_image(const string& filename, int wid, int hei, int max_val, vector<vector<vector<int>>>& image) {
    ofstream outfile(filename);
    outfile << "P3" << endl;
    outfile << wid << " " << hei << endl;
    outfile << max_val << endl;

    for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            outfile << image[i][j][0] << " " << image[i][j][1] << " " << image[i][j][2] << " ";
        }
        outfile << endl;
    }

    outfile.close();
}

int main(int argc, char* argv[]) {
    ifstream file(argv[1]);

    if (!file.is_open()) {
        cout << "failed to open" << endl;
        return 1;
    }

    int wid, hei, max_val;
    string format;

    file >> format >> wid >> hei >> max_val;

    vector<vector<vector<int>>> image(hei, vector<vector<int>>(wid, vector<int>(3)));

    for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            for (int k = 0; k < 3; k++) {
                int val;
                file >> val;
                image[i][j][k] = val;
            }
        }
    }

    file.close();

    //cout << "original Pixel Values: " << image[0][0][0] << " " << image[0][0][1] << " " << image[0][0][2] << endl;

    grey_convert(hei, wid, max_val, image);
    cout << "Converted to Grayscale." << endl;

    blur_convert(hei, wid, max_val, image);
    cout << "Converted to Blurscale." << endl;
     
    write_image(argv[2], wid, hei, max_val, image);
    cout << "Output image saved as " << argv[2] << endl;
    
    return 0;
}
