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
        std::vector<char> flipped(image_size);
        int width = fileheader.width;
        int height = fileheader.height;

        const int bytesPerRow = width * channel;

        u_int32_t quantized_bits = 8;

        std::string outputfilename;
        // quantize the image
        for (int l = 0; l < 3; l = l + 1)
        {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    for (int k = 0; k < channel; k++)
                    {
                        flipped[i * bytesPerRow + j * channel + k] = imagedata[i * bytesPerRow + j * channel + k] >> (l + 1) * 2;
                        flipped[i * bytesPerRow + j * channel + k] = flipped[i * bytesPerRow + j * channel + k] << (l + 1) * 2;
                    }


            if(img_num==0)
                outputfilename = "output/output1_" + std::to_string(l + 1) + ".bmp";
            else
                outputfilename = "output/output2_" + std::to_string(l + 1) + ".bmp";
            std::ofstream outputfile(outputfilename, std::ios::binary);
            outputfile.write((char *)&fileheader, sizeof(fileheader));
            outputfile.write(flipped.data(), image_size);
            outputfile.close();

            // display the quantized bit info of the image
            std::cout << "Image quantized to " << l << " bits and saved" << std::endl;
        }
    }

    return 0;
}