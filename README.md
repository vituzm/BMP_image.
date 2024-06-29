# BMP image in c++
> This project consists in editing a [BMP image](https://en.wikipedia.org/wiki/BMP_file_format) to grayscale and displaying a piece of the image on a [Nokia Display](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf).

## Table of Contents
* [General Info](#general-information)
* [Technologies Used](#technologies-used)
* [Usage](#usage)
* [Fotos](#fotos)
* [Room for Improvement](#room-for-improvement)
* [Contact](#contact)
<!-- [Acknowledgements](#acknowledgements) -->


## General Information
- This project is based on a work for school
- The names of the variables and comments were written in portuguese

## Technologies Used
- C and C++
- STM NUCLEO F446RE
- Nokia Display 5110

## Usage
- The program has 3 user input:
  1. the name of the BMP image
  2. the threshold value for the image (from 0 to 255)
  3. the bottom left x and y coordenates to display the image on the Nokia
     
- And 2 outputs:
  1. the grayscaled bmp image with the same name and _gs suffix 
  2. a vector with the hex values for each 8 pixel of the cropped image to be used on the Nokia Display
     
- The name of the image file has to be the same that was given by the user when executing the program
- After given the name of the image file, there is a table with useful information of the bmp header
  
- Download all files and include your bmp image and its name when executing `image_project.exe` as well

## Room for Improvement 

Room for improvement;
- Naming variabels

## Fotos
- Ford Mustang Shelby gt500 -> RGB
<img src="https://github.com/vituzm/BMP_image./assets/134985122/48714152-004a-4a21-8c04-eca6c08364d2" alt = "Ford Mustang Shelby gt500 4k" width="50%" height="20%"/>

&nbsp; 
- Ford Mustang Shelby gt500 -> GrayScale
<img src="https://github.com/vituzm/BMP_image./assets/134985122/a979f95b-69ba-4bb2-8b9b-0edc73a86df2" alt = "Ford Mustang Shelby gt500 grayscale" width="50%" height="20%"/>

&nbsp;
- Ford Mustang Shelby gt500 -> Mono (*note: the threshold value is set to 90 here)
<img src="https://github.com/vituzm/BMP_image./assets/134985122/8692683d-950c-44ff-b009-ce93d843e356" alt = "Ford Mustang Shelby gt500 mono" width="50%" height="20%"/>

&nbsp;
- Cropped image on a [Nokia Display](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf) (*note: the cropped image is the snake from the car model*)
<img src="https://github.com/vituzm/BMP_image./assets/134985122/ea4aa474-23cb-4204-b058-4570835bb942" alt = "Crop" width="50%" height="20%"/> 


## Contact
Created by [vituzm](https://github.com/vituzm) and [nicao2005](https://github.com/nicao2005) - feel free to contact us!

