#include <bits/stdc++.h>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

struct Pixel {
    unsigned char r,g,b;
};


struct imageheader {
    int height;
    int width;
    int maxcolor;
    string format;
};


void grey_convert(string format,int hei, int wid, int max_val, vector<vector<vector<int>>>& image ,int pipefds[2],int pipefds2[2]) {



    struct imageheader storeHeader[1];

    
    storeHeader[0].format=format;
    storeHeader[0].height = hei;
    storeHeader[0].width = wid;
    storeHeader[0].maxcolor = max_val;
    write(pipefds2[1], storeHeader, sizeof(struct imageheader));
    


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

        for(int y = 0; y<image.size(); y++) {
        for(int x = 0; x<image[y].size(); x++) {
            struct Pixel tempPix[1];
            tempPix[0].r= image[y][x][0];
            tempPix[0].g= image[y][x][1];
            tempPix[0].b= image[y][x][2];
            write(pipefds[1], tempPix, sizeof(tempPix));
    }}

    exit(EXIT_SUCCESS);
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

void blur_convert(const string& output,  int pipefds[2],int pipefds2[2]) {

    
    struct imageheader
 header[1];
    read(pipefds2[0], header, sizeof(header));
    int width = header[0].width;
    int height = header[0].height;
    int colormax = header[0].maxcolor;

    vector<vector<vector<int>>> blurred(height, vector<vector<int>>(width, vector<int>(3)));


    vector<vector<vector<int>>> image_a(height, vector<vector<int>>(width, vector<int>(3)));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            struct Pixel tempPixel[1];
            read(pipefds[0], tempPixel, sizeof(tempPixel));
            
                image_a[i][j][0] = tempPixel[0].r;
                image_a[i][j][1]=tempPixel[0].g;
                image_a[i][j][2]=tempPixel[0].b;
            
        }
    }

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            for (int k = 0; k < 3; k++) {
                int sum = image_a[i - 1][j - 1][k] + image_a[i - 1][j][k] + image_a[i - 1][j + 1][k] +
                          image_a[i][j - 1][k] + image_a[i][j][k] + image_a[i][j + 1][k] +
                          image_a[i + 1][j - 1][k] + image_a[i + 1][j][k] + image_a[i + 1][j + 1][k];
                blurred[i][j][k] = sum / 9;
            }
        }
    }

    // update image 
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            for (int k = 0; k < 3; k++) {
                image_a[i][j][k] = blurred[i][j][k];
            }
        }
    }


   write_image(output, width, height, colormax, image_a);
    exit(EXIT_SUCCESS);
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



        //making a pipe
    
    int pipefds[2]; // 0 -- read , 1 -- write ; pipefds[2] is the file descriptor to the pipe in memory
    int pipeDesc1;

    pipeDesc1 = pipe(pipefds); // pipe creation
    
    if (pipeDesc1 == -1)
        perror("pipe");
    
    int pipefds2[2]; // 0 -- read , 1 -- write ; pipefds[2] is the file descriptor to the pipe in memory
    int pipeDesc2;
    
    pipeDesc2 = pipe(pipefds2); // pipe creation
    
    if (pipeDesc2 == -1)
        perror("pipe");

    //cout << "original Pixel Values: " << image[0][0][0] << " " << image[0][0][1] << " " << image[0][0][2] << endl;
    int pid=fork();
    if(pid == 0){ 
    grey_convert(format,hei, wid, max_val, image ,pipefds,pipefds2);
    // cout << "Converted to Grayscale." << endl;
    }else{ 
    blur_convert(argv[2],pipefds,pipefds2);
    // cout << "Blurred Image Saved as output.ppm." << endl;
    }
    wait(NULL);
    //wait(NULL);
     
    // write_image("output.ppm", wid, hei, max_val, image);
    
    return 0;
}
