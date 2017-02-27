#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"

using namespace std;


//function prototype
void makeBlur(GWindow &gw , GBufferedImage &img , string imageName, int blurLeve);
static bool     openImageFromFilename(GBufferedImage &img, string filename);
static bool 	saveImageToFilename(const GBufferedImage &img, string filename);
void MakeBlurGridImage(GBufferedImage &img, int blurLevel);
int blurHex(int rows, int cols, int blurLevel);


//global variable
Grid<int > blurredImgGrid;
//int blurLevel;


//main function
int main(){
    GWindow gw;
    gw.setTitle("photoshop");
    gw.setVisible(true);
    GBufferedImage img;



    cout << "Welcome to my PhotoEditor.please,choose file which u want to blur." << endl;
    string imageName = getLine();
    cout << "please enter the blur level from 1 to 15" << endl;
    int blurLevel = getInteger();
    makeBlur(gw,img,imageName , blurLevel);

    return 0;
}

//a function where other functions are called
void makeBlur(GWindow &gw , GBufferedImage &img , string imageName , int blurLeve){

    openImageFromFilename(img, imageName);

    gw.setSize(img.getWidth(),img.getHeight());

    MakeBlurGridImage(img,blurLeve);

    img.fromGrid(blurredImgGrid);

    gw.add(&img);

    cout << "Enter the saved file name:" << endl;
    saveImageToFilename(img,getLine());

}

/*
 *this funtion is void type and it just set the new hexa value into every pixel of an image.
 *
 */
void MakeBlurGridImage(GBufferedImage &img ,int blurLevel){

    blurredImgGrid = img.toGrid();

    for(int i = 0; i < blurredImgGrid.numRows();i++){
        for(int j = 0; j < blurredImgGrid.numCols();j++){
            blurredImgGrid[i][j]=blurHex(i,j , blurLevel);
        }
    }
}

/*
 *this funtion returns the blur value(hexa) of every pixel.
 */
 int blurHex(int rows, int cols , int blurLevel){
    int red,green,blue,sumRed = 0,sumGreen = 0,sumBlue =0;
    for(int x = (rows-blurLevel);x <=(rows+blurLevel);x++){
    if(x < 0 || x > (blurredImgGrid.numRows()-1)) continue;
        for(int y = (cols-blurLevel);y <=(cols+blurLevel);y++){
            if(y < 0 || y > (blurredImgGrid.numCols()-1)) continue;
            GBufferedImage::getRedGreenBlue(blurredImgGrid[x][y],red,green,blue);
            sumRed = sumRed + red;
            sumGreen= sumGreen + green;
            sumBlue = sumBlue + blue;
            }
        }

     int totalPixels = ((2*blurLevel)+1)*((2*blurLevel)+1);
     int averageRedValue = (sumRed / totalPixels);
     int averageGreenValue = (sumGreen / totalPixels);
     int averageBlueValue = (sumBlue / totalPixels);

    return GBufferedImage::createRgbPixel(averageRedValue,averageGreenValue,averageBlueValue);
}

/* This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
static bool openImageFromFilename(GBufferedImage &img, string filename) {
    if(filename.length() == 0){
        cout << "Exit" << endl;
        return false;
    }
    while(true){
    try { img.load(filename); }
    catch (...) {
        cout << "file not found.please,enter the valid file name or just hit enter to exit:"<< endl;
        openImageFromFilename(img, getLine());
        return false; }
    return true;
}
}

/*
 *
 * Attempts to save the image file to 'filename'.
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
static bool saveImageToFilename(const GBufferedImage &img, string filename) {

    if(filename.length() == 0){

        return false;
    }
    try {
        img.save(filename);
        cout << "file saved successfully" << endl;
    } catch (...) {
        return false;
    }
    return true;
}
