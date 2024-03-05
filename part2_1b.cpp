#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <semaphore.h>
using namespace std;

sem_t semaphore;

void grey_convert(int hei, int wid, vector<vector<vector<int>>>& image) {
    for (int i = 0; i < hei; i++) {
        sem_wait(&semaphore); // Wait until semaphore is available
        for (int j = 0; j < wid; j++) {
            int val = (image[i][j][0] + image[i][j][1] + image[i][j][2]) / 3;
            image[i][j][0] = val;
            image[i][j][1] = val;
            image[i][j][2] = val;
        }
        sem_post(&semaphore); // Release the semaphore
    }
}

void blur_convert(int hei, int wid, vector<vector<vector<int>>>& image) {
    for (int i = 1; i < hei - 1; i++) {
        sem_wait(&semaphore); // Wait until semaphore is available
        for (int j = 1; j < wid - 1; j++) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int ii = -1; ii <= 1; ii++) {
                for (int jj = -1; jj <= 1; jj++) {
                    sum_r += image[i + ii][j + jj][0];
                    sum_g += image[i + ii][j + jj][1];
                    sum_b += image[i + ii][j + jj][2];
                }
            }
            image[i][j][0] = sum_r / 9;
            image[i][j][1] = sum_g / 9;
            image[i][j][2] = sum_b / 9;
        }
        sem_post(&semaphore); // Release the semaphore
    }
}

void write_image(const string& filename, int wid, int hei,int max_val, vector<vector<vector<int>>>& image) {
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


    sem_init(&semaphore, 0, 1); // Initialize semaphore with value 1

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

    thread grey_thread(grey_convert, hei, wid, ref(image));
    blur_convert(hei, wid, ref(image));
    grey_thread.join();

    write_image(argv[2], wid, hei,max_val, ref(image));
    cout << "Output image saved as " << argv[2] << endl;

    sem_destroy(&semaphore); // Destroy the semaphore

    return 0;
}
