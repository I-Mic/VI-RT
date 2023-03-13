//
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include <iostream>
#include <fstream>

#include "image/image_ppm.hpp"

namespace img {

void image_ppm_t::tone_map(){

    image_to_save = new rgb::rgb_t[this->width * this->height];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (size_t j = 0 ; j< this->height ; j++) {
        for (size_t i = 0; i < this->width ; ++i) {
            image_to_save[j*this->width+i].r = (unsigned char)(std::min(1.f, this->image_plane[j*this->width+i].r) * 255);
            image_to_save[j*this->width+i].g = (unsigned char)(std::min(1.f, this->image_plane[j*this->width+i].g) * 255);
            image_to_save[j*this->width+i].b = (unsigned char)(std::min(1.f, this->image_plane[j*this->width+i].b) * 255);
        }
    }

}

bool image_ppm_t::save(std::string const& filename){ 
    
    if(this->height == 0 || this->width == 0){
        std::cerr << "Can't save an empty image\n";
        return false;
    }

    // convert from float to {0,1,..., 255}
    tone_map();
    
    std::ofstream ofs;
    ofs.open(filename, std::ios::out | std::ios::binary);  //need to spec. binary mode for Windows users

    if (ofs.fail()){
        std::cerr << "IO error\n";
        return false;
    }

    ofs << "P3\n" << this->width << " " << this->height << "\n255\n";

    unsigned char r, g, b;
    // loop over each pixel in the image, clamp and convert to byte format
    for (int i = 0; i < this->width * this->height; ++i) {
        r = static_cast<unsigned char>(this->image_to_save[i].r);
        g = static_cast<unsigned char>(this->image_to_save[i].g);
        b = static_cast<unsigned char>(this->image_to_save[i].b);
        ofs << r << g << b;
    }
    ofs.close();
    // write imageToSave to file
    
    // Details and code on PPM files available at:
    // https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/reading-writing-images.html
    
    return true;
}

};
