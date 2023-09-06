# BMP image in c++
> This project consists in editing a [BMP image](https://en.wikipedia.org/wiki/BMP_file_format) to grayscale and displaying a piece of the image on a [Nokia Display](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf).

## Table of Contents
* [General Info](#general-information)
* [Technologies Used](#technologies-used)
* [Usage](#usage)
* [Project Status](#project-status)
* [Room for Improvement](#room-for-improvement)
* [Contact](#contact)
<!-- [Acknowledgements](#acknowledgements) -->


## General Information
- The program has 3 user input:
  1. the name of the BMP image
  2. the threshold value for the image
  3. the x and y coordenates of the bottom left piece of the image that you want cut
- And 2 outputs:
  1. the grayscaled bmp image with the same name and _gs suffix 
  2. a vector with the hex values for each 8 pixel of the cropped image to be used on the Nokia Display
  

## Technologies Used
- C and C++
- STM NUCLEO F446RE
- Nokia Display 5110


## Usage
Download all files and include your bmp image and its name when executing `image_project.exe` as well

## Project Status
Project is: _in progress_

## Room for Improvement 

Room for improvement:
- Grayscaling
- Naming variabels
- Cropping the image

<!--  
## Acknowledgements
- This project was an ideia of 
- This project was based on [this tutorial](https://www.example.com).
- Many thanks to...
-->

## Contact
Created by [vituzm]([https://www.flynerd.pl/](https://www.instagram.com/vitozm/)) and [nicao2005](https://github.com/nicao2005) - feel free to contact us!

<!--  -->
<!-- ## License -->
<!-- This project is open source and available under the [... License](). -->

<!-- You don't have to include all sections - just the one's relevant to your project -->
