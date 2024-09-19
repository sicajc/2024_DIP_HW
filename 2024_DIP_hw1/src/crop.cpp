#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>

#pragma pack(push, 1) // Ensure that structures are byte-aligned
typedef struct
{
    /* type : Magic identifier,一般為BM(0x42,0x4d) */
    unsigned short int type;
    unsigned int file_size;                  /* File size in bytes,全部的檔案大小 */
    unsigned short int reserved1, reserved2; /* 保留欄位 */
    unsigned int offset;                     /* Offset to image data, bytes */
    unsigned int header_size;                /* Info Header size in bytes */
    int width, height;                       /* Width and height of image */
    unsigned short int planes;               /* Number of colour planes */
    unsigned short int bits;                 /* Bits per pixel */
    unsigned int compression;                /* Compression type */
    unsigned int imagesize;                  /* Image size in bytes */
    int xresolution, yresolution;            /* Pixels per meter */
    unsigned int ncolours;                   /* Number of colours */
    unsigned int importantcolours;           /* Important colours */
} IMG_HEADER;
#pragma pack(pop)

int main()
{
    // read in the .bmp file format
    IMG_HEADER fileheader;
    std::string filename;
    std::string outputfilename;

    for (int img_num = 0; img_num < 2; img_num++)
    {
        if (img_num == 0)
            filename = "inputs/input1.bmp";
        else
            filename = "inputs/input2.bmp";

        std::ifstream file(filename, std::ios::binary);
        file.read((char *)&fileheader, sizeof(fileheader));
        // check if the file exists
        if (!file.is_open())
        {
            std::cout << "File not found" << std::endl;
            return 1;
        }
        int channel = fileheader.bits / 8;

        const unsigned int image_size = fileheader.height * fileheader.width * channel;
        std::vector<char> imagedata(image_size);
        file.read(imagedata.data(), image_size);
        file.close();

        // Print out the file header, infoheader and file data
        std::cout << "Image read" << std::endl;

        // flip the image in a horizontal direction every 3 bytes
        int width = fileheader.width;
        int height = fileheader.height;

        const int bytesPerRow = width * channel;

        int x, y, w, h;

        for (;;)
        {
            std::cout << "Enter the x coordinate of the top left corner of the cropped image: ";
            std::cin >> x;
            // check if x is smaller than the width of the image
            if (x < 0 || x >= width)
            {
                std::cout << "Invalid x coordinate, x must be in range of: "<< width << std::endl;
                continue;
            } else {
                break;
            }
        }

        for(;;)
        {
            std::cout << "Enter the y coordinate of the top left corner of the cropped image: ";
            std::cin >> y;
            // check if y is smaller than the height of the image
            if (y < 0 || y >= height)
            {
                std::cout << "Invalid y coordinate, y must be in range of: "<< height << std::endl;
                continue;
            } else {
                break;
            }
        }

        for(;;)
        {
            std::cout << "Enter the width of the cropped image: ";
            std::cin >> w;
            // check if w is smaller than the width of the image
            if (w < 0 || w+x > width)
            {
                std::cout << "Crop width out of range: "<<std::endl;
                std::cout << "The width of the cropped image must be smaller than: "<< width-x << std::endl;
                continue;
            } else {
                break;
            }
        }

        for(;;)
        {
            std::cout << "Enter the height of the cropped image: ";
            std::cin >> h;
            // check if h is smaller than the height of the image
            if (h < 0 || h+y > height)
            {
                std::cout << "Crop height out of range: "<< height << std::endl;
                std::cout << "The height of the cropped image must be smaller than: "<< height-y << std::endl;
                continue;
            } else {
                break;
            }
        }

        const int new_image_size = w * h * channel;

        std::vector<char> cropped_img(new_image_size);

        // crop the image
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                for (int k = 0; k < channel; k++)
                {
                    cropped_img[i * w * channel + j * channel + k] = imagedata[(i + y) * bytesPerRow + (j + x) * channel + k];
                }
            }
        }

        // write the cropped_img image to a new file
        if (img_num == 0)
            outputfilename = "output/output1_crop.bmp";
        else
            outputfilename = "output/output2_crop.bmp";

        std::ofstream outputfile(outputfilename, std::ios::binary);

        // updates the header with the new image size and w,h
        fileheader.width = w;
        fileheader.height = h;
        fileheader.file_size = fileheader.offset + new_image_size;
        fileheader.imagesize = new_image_size;

        outputfile.write((char *)&fileheader, sizeof(fileheader));
        outputfile.write(cropped_img.data(), w * h * channel);
        outputfile.close();

        std::cout << "Image cropped_img and saved" << std::endl;
    }

    return 0;
}